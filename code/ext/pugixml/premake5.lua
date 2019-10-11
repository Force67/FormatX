
project "pugixml"
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
        "pugiconfig.hpp",
		"pugixml.hpp",
		"pugixml.cpp"
    }