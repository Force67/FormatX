
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
        "../vendor/glad/include",
        "../vendor/imgui",
        "../vendor/glm/"
    }

    filter "system:windows"
        links "Shcore"

    defines "GLFW_EXPOSE_NATIVE_WIN32"
    defines "VK_USE_PLATFORM_WIN32_KHR"

    links {
        "common",
        "fmtlib",
        "glfw",
        "imgui",
        "glad"
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