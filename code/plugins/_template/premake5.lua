
project "Template"
    language "C++"
    kind "ConsoleApp"
	flags "NoManifest"

    vpaths
    {
        ["*"] = { "**.hpp", "**.h" },
        ["*"] = "**.cpp",
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