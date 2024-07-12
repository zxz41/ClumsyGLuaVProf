#include "GarrysMod/Lua/Interface.h"
#include <Color.h>
#include <evilvprof.h>
#include <utldict.h>
#include <utlstack.h>
#include <convar.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace GarrysMod::Lua;

// This may look stupid, but VProf uses pointers to uniquely identify strings.
// And we currently don't have a decent UUID system...
#define MAX_RECURSIVE_CALLS 512

int g_recursiveScopes = 0;
CUtlDict<CVProfLuaScope *> GLuaVProfEntries( k_eDictCompareTypeCaseInsensitive );
CUtlStack<int> GLuaVProfEntryNum;
CVEvilVProfile& g_EvilVProfCurrentProfile = *static_cast< CVEvilVProfile * >( &g_VProfCurrentProfile );

LUA_FUNCTION( Lua_VProfEnter )
{
	if ( g_VProfCurrentProfile.IsEnabled() )
	{
		if ( g_recursiveScopes >= MAX_RECURSIVE_CALLS )
		{
			LUA->ThrowError( "Too many recursive VProf calls!" );
		}
		LUA->CheckType( 1, GarrysMod::Lua::Type::String );

		unsigned int strlen = 0;
		const char* pszluaStr = LUA->GetString( -1, &strlen );
		int iEntryNum = GLuaVProfEntries.Find( pszluaStr );

		if ( iEntryNum == GLuaVProfEntries.InvalidIndex() )
		{
			// No entry found, copy lua string into persistent memory.
			// TODO: clump together memory allocations for speed.
			char* pszEntryName = new char[strlen+1];
			V_strncpy( pszEntryName, pszluaStr, strlen+1);

			iEntryNum = GLuaVProfEntries.Insert( pszEntryName, new CVProfLuaScope( pszEntryName ) );

			ConMsg( "Added entry %s (%d) at address %x\n",
					GLuaVProfEntries.Element(iEntryNum)->GetName(),
					iEntryNum,
					(unsigned int)GLuaVProfEntries.Element(iEntryNum)
			);
		}

		GLuaVProfEntryNum.Push( iEntryNum );
		GLuaVProfEntries.Element( iEntryNum )->EnterScope();
		g_recursiveScopes++;
	}

	return 0;
}

LUA_FUNCTION( Lua_VProfExit )
{
	if ( g_VProfCurrentProfile.IsEnabled() )
	{
		if ( !GLuaVProfEntryNum.Count() )
		{
			LUA->ThrowError( "Tried to exit scope with no scopes left!" );
		}

		//CVEvilVProfile &prof = g_EvilVProfCurrentProfile;
		CVProfLuaScope &scope = *GLuaVProfEntries.Element( GLuaVProfEntryNum.Top() );
		//CVProfNode *nodeForDeletion = prof.FindNode( prof.GetRoot(), scope.GetName() );

		scope.ExitScope();
		GLuaVProfEntryNum.Pop();

		g_recursiveScopes--;
	}

	return 0;
}

GMOD_MODULE_OPEN()
{
	LUA->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );
	LUA->PushString( "VProfStart" );
	LUA->PushCFunction( Lua_VProfEnter );
	LUA->SetTable( -3 ); // `_G.TestFunction = MyExampleFunction`

	LUA->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );
	LUA->PushString( "VProfEnd" );
	LUA->PushCFunction( Lua_VProfExit );
	LUA->SetTable( -3 ); // `_G.TestFunction = MyExampleFunction`

	Color WarnColor = Color( 255, 40, 40, 255 );
	Msg( "\n\n\n\n" );
	ConColorMsg(WarnColor, "******************************\n" );
	ConColorMsg(WarnColor, "***        READ THIS       ***\n" );
	ConColorMsg(WarnColor, "******************************\n" );
	Msg( "Clumsy GLua VProf is NOT a stable module, there WILL likely be bugs, crashes and heap corruptions.\n" );
	Msg( "You should NEVER run this unless you're activily profiling, or know what you're doing.\n" );
	Msg( "\n\n\n\n" );

	return 0;
}

GMOD_MODULE_CLOSE()
{
	// TODO detour vprof panel and reset it so it's not trying to read invalid heap.
	// VProfPanel construction func signature: 55 8B EC 83 EC 18 83 7D 10 00

	// Dirty workaround for now
	//engine->ClientCmd_Unrestricted( "vprof_reset" );
	return 0;
}