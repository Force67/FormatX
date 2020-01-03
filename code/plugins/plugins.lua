
project "plugin-tombraider"
    language "C++"
    kind "SharedLib"
	targetdir '../../bin/%{cfg.buildcfg}/plugins'
	
	links
	{
		"shared",
		"zlib"
	}

    includedirs
    {
        ".",
		"../shared",
		"../vendor/zlib"
    }

    files
    {
        "tombraider/**.h",
        "tombraider/**.cpp"
    }
    
project "plugin-tboi"
    language "C++"
    kind "SharedLib"
	targetdir '../../bin/%{cfg.buildcfg}/plugins'
	
	links
	{
		"shared",
		"zlib"
	}

    includedirs
    {
        ".",
		"../shared",
		"../ext/zlib"
    }

    files
    {
        "tboi/**.h",
        "tboi/**.cpp"
    }
	
project "plugin-control"
    language "C++"
    kind "SharedLib"
	targetdir '../../bin/%{cfg.buildcfg}/plugins'

	links
	{
		"shared",
		"zlib",
		"pugixml"
	}

    includedirs
    {
        ".",
		"../shared",
		"../vendor/pugixml/src"
    }

    files
    {
        "control/**.h",
        "control/**.cpp"
    }
    