
project "zlib"
    language "C"
    kind "StaticLib"

    vpaths
    {
	    ["Code/*"] = { "*.c", "**.h"},
        ["*"] = "premake5.lua"
    }

    includedirs
    {
        "."
    }

    files
    {
        "premake5.lua",
        "*.h",
        "*.c"
    }