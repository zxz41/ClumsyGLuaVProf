PROJECT_GENERATOR_VERSION = 2

newoption({
    trigger = "gmcommon",
    description = "Sets the path to the garrysmod_common (https://github.com/danielga/garrysmod_common) directory",
    value = "path to garrysmod_common directory"
})

local gmcommon = assert(_OPTIONS.gmcommon or os.getenv("GARRYSMOD_COMMON"),
    "you didn't provide a path to your garrysmod_common (https://github.com/danielga/garrysmod_common) directory")
include(gmcommon)

CreateWorkspace({name = "luavprof", abi_compatible = false, path="src/"})
    CreateProject({serverside = true, source_path = "src", manual_files = false})
        IncludeLuaShared()
        IncludeSDKCommon()
		IncludeSDKTier0()
		IncludeSDKTier1()

    CreateProject({serverside = false, source_path = "src", manual_files = false})
        IncludeLuaShared()
        IncludeSDKCommon()
		IncludeSDKTier0()
		IncludeSDKTier1()