
project "CrystalFMT"
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
		"shared"
	}

    includedirs
    {
        ".",
		"../../shared"
    }

    files
    {
        "premake5.lua",
        "**.h",
        "**.cpp"
    }