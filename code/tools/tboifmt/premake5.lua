
project "TboiFmt"
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
		"zlib"
	}

    includedirs
    {
        ".",
		"../../shared",
		"../../ext/zlib"
    }

    files
    {
        "premake5.lua",
        "**.h",
        "**.cpp"
    }