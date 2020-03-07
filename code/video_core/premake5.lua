
project "video_core"
    language "C++"
    kind "StaticLib"

    includedirs
    {
        ".",
        "../common",
        "../vendor/fmtlib/include"
    }

    files {
        "premake5.lua",
        "**.h",
        "**.cpp",
        "**.cc"
    }
    
    filter "system:windows"
        files {
            "dxgi.h",
            "dxgi.cpp",
            "dx12/*.cpp",
            "dx12/*.h"
		}
    filter {}