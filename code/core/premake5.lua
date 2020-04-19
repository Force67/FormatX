
project "core"
    language "C++"
    kind "SharedLib"
    targetname "FormatX"

    includedirs
    {
        ".",
        "../common",
        "../common/utl",
        "../video_core",
        "../vendor/fmtlib/include",
        "../vendor/glfw/include",
        "../vendor/imgui"
    }

    defines "GLFW_EXPOSE_NATIVE_WIN32"

    links {
        "video_core",
        "common",
        "fmtlib",
        "glfw",
        "imgui"
    }

    defines "COMPILING_CORE"

    files {
        "premake5.lua",
        "**.h",
        "**.cpp",
		"**.cc",
        "**.ui",
        "**.inl"
    }