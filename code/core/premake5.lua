
project "core"
    language "C++"
    kind "SharedLib"

    includedirs
    {
        ".",
        "../common",
        "../video_core",
        "../vendor/fmtlib/include"
    }

    links {
        "video_core",
        "common",
        "fmtlib"
    }

    defines "COMPILING_CORE"

    files {
        "premake5.lua",
        "**.h",
        "**.cpp",
		"**.cc"
    }

    filter "system:not windows"
        removefiles {"**_win.cpp", "**_win.h"}
    filter {}
