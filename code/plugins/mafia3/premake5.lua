
project "M3Format"
    language "C++"
    kind "ConsoleApp"
	flags "NoManifest"

    vpaths
    {
		-- shorten virtual include paths
        ["Code/*"] = { "**.cpp", "**.hpp", "**.h" },
        ["*"] = "premake5.lua"
    }
	
	links
	{
		"shared",
		"pugixml"
	}

    includedirs
    {
        ".",
		"../../shared",
		"../../ext/pugixml"
    }

    files
    {
        "premake5.lua",
        "**.h",
        "**.cpp"
    }