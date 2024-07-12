#pragma once

#include <vprof.h>

#define VPROF_BUDGETGROUP_GLUA_SCRIPT		_T("GLua (Script)")

/*
class CVEvilProfile : public CVProfile
{
public:
	void SetCurrentNode(CVProfNode *pNode) { this->m_pCurNode = pNode; }
	void SetAtRoot(bool bAtRoot)           { this->m_fAtRoot = bAtRoot; }
};
static_assert(sizeof(CVEvilProfile) == sizeof(CVProfile));
*/

class CVEvilVProfile : public CVProfile
{
public:
	void FreeNodes_R( CVProfNode *pNode ) { CVProfile::FreeNodes_R( pNode ); }
};

// Different from CVProfScope, holds it's name reference etc.
class CVProfLuaScope
{
public:
	CVProfLuaScope( const tchar *pszName );
	~CVProfLuaScope();

	// Returns the pointer to the objects name.
	const tchar *GetName() { return m_pszName; }

	void SetEnabled( bool bEnable );
	void EnterScope();
	void ExitScope();
	bool IsEnabled() { return m_bEnabled; }

protected:
	const tchar *m_pszName;
	bool m_bEnabled;
	bool m_bInScope = false;
};

// Construct a Lua script VProf scope and automatically enable, code handles scope entry.
inline CVProfLuaScope::CVProfLuaScope( const tchar *pszName )
	: m_bEnabled( g_VProfCurrentProfile.IsEnabled() ),
	  m_pszName( pszName )
{
}

// Sanity check
inline CVProfLuaScope::~CVProfLuaScope()		
{ 
	if ( m_bEnabled )
	{
		g_VProfCurrentProfile.ExitScope();
	}
}

// Set enable for Lua VProf scopes.
// NOTICE: This will always set to false when the VProf profiler is disabled.
inline void CVProfLuaScope::SetEnabled( bool bEnable )
{
	m_bEnabled = (bEnable && g_VProfCurrentProfile.IsEnabled());
}

// Enter the scope with our defined name. Hardcoded to "GLua (Script)"
inline void CVProfLuaScope::EnterScope()
{ 
	if ( m_bEnabled )
	{
		g_VProfCurrentProfile.EnterScope( m_pszName, 1, VPROF_BUDGETGROUP_GLUA_SCRIPT, false, 0 ); 
	}
}

inline void CVProfLuaScope::ExitScope()					
{ 
	if ( m_bEnabled )
	{
		g_VProfCurrentProfile.ExitScope(); 
	}
}

// A must do evil to dismiss nodes.
class CEvilVProfNode : public CVProfNode
{
public:
	CVProfNode *m_pParent;
	CVProfNode *m_pChild;
	CVProfNode *m_pSibling;
};