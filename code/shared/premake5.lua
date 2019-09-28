
project "shared"
    language "C++"
    kind "StaticLib"

    vpaths
    {
	    ["Code/*"] = { "**.cpp", "**.hpp", "**.h" },
        ["*"] = "premake5.lua"
    }

    includedirs
    {
        "."
    }

    files
    {
        "premake5.lua",
        "**.h",
        "**.cpp"
    }