
project "core"
    language "C++"
    kind "SharedLib"
    targetname "FormatX"

    includedirs
    {
        ".",
        "../common",
        "../common/utl",
        "../vendor/fmtlib/include",
        "../vendor/glfw/include",
        "../vendor/glad/include",
        "../vendor/imgui",
        "../vendor/glm/",
        "../vendor/assimp/include",
        "../vendor/yaml-cpp/include",

        -- library config overrides
        "../vendor/_ship"
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
        "glad",
        "assimp",
        "yaml-cpp"
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