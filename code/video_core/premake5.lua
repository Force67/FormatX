
project "video_core"
    language "C++"
    kind "SharedLib"

    filter "kind:SharedLib"
        links { "fmtlib", "common", "glad", "imgui" }

    defines "BUILDING_VCORE"
    defines "VK_USE_PLATFORM_WIN32_KHR"

    includedirs
    {
        ".",
        "../common",
        "../vendor/fmtlib/include",

        "../vendor/glad/include",
        "../vendor/glad/include/glad",
        "../vendor/imgui"
    }

    files {
        "premake5.lua",
        "**.h",
        "**.cpp",
        "**.c"
    }