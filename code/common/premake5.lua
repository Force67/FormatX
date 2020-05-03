
project "common"
    language "C++"
    kind "StaticLib"

    vpaths
    {
	    ["Code/*"] = { "**.cpp", "**.hpp", "**.h" },
        ["*"] = "premake5.lua"
    }

    includedirs
    {
        ".",
        "../vendor/fmtlib/include",
        "../vendor/yaml-cpp/include",
    }

    files
    {
        "premake5.lua",
        "**.h",
        "**.cpp"
    }