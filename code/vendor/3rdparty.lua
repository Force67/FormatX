
project "pugixml"
    language "C++"
    kind "StaticLib"

    includedirs
    {
        "."
    }

    files
    {
        "pugixml/src/pugiconfig.hpp",
        "pugixml/src/pugixml.hpp",
        "pugixml/src/pugixml.cpp"
    }
    
project "zlib"
    language "C"
    kind "StaticLib"

    includedirs
    {
        "."
    }

    files
    {
        "zlib/premake5.lua",
        "zlib/*.h",
        "zlib/*.c"
    }
    
project "fmtlib"
    language "C++"
    kind "StaticLib"
    
    includedirs
    {
        "fmtlib/include"
    }
    
    files
    {
        "fmtlib/include/fmt/*.h",
        "fmtlib/src/*.cc"
    }
    
project "imgui"
    language "C++"
    kind "StaticLib"

    includedirs
    {
        "./imgui"
    }

    files
    {
        "./imgui/imconfig.h",
        "./imgui/imgui.cpp",
        "./imgui/imgui.h",
        "./imgui/imgui_truetype.h",
        "./imgui/imgui_draw.cpp",
        "./imgui/imgui_internal.h",
        "./imgui/imgui_widgets.cpp",
        "./imgui/imgui_demo.cpp"
    }

project "glad"
    language "C++"
    kind "StaticLib"

    filter "system:windows"
        defines "VK_USE_PLATFORM_WIN32_KHR"

    defines "VK_ENABLE_BETA_EXTENSIONS"

    includedirs
    {
        "glad/include",
        "glad/include/glad"
    }

    files
    {
        "glad/src/gl.c",
        "glad/src/vulkan.c",

        "glad/include/**.h"
    }

project "glfw"
    language "C++"
    kind "StaticLib"
    
    includedirs
    {
        "glfw/include",
        "glfw/src"
    }
    
    files
    {   
        "glfw/include/*.h",
        "glfw/src/context.c",
        "glfw/src/init.c",
        "glfw/src/input.c",
        "glfw/src/monitor.c",
        "glfw/src/vulkan.c",
        "glfw/src/window.c"
    }
    
    filter "system:windows"
		defines { "GLFW_EXPOSE_NATIVE_WIN32", "_GLFW_WIN32" }
            files
            {
                "glfw/src/win32_platform.h",
                "glfw/src/win32_joystick.h",
                "glfw/src/wgl_context.h",
                "glfw/src/egl_context.h",
                "glfw/src/osmesa_context.h",
                "glfw/src/win32_init.c",
                "glfw/src/win32_joystick.c",
                "glfw/src/win32_monitor.c",
                "glfw/src/win32_time.c",
                "glfw/src/win32_thread.c",
                "glfw/src/win32_window.c",
                "glfw/src/wgl_context.c",
                "glfw/src/egl_context.c",
                "glfw/src/osmesa_context.c",
        
            }
    filter{}

project "assimp"
    language "C++"
    kind "StaticLib"

    defines
    {
        "ASSIMP_BUILD_NO_IFC_IMPORTER",
        "ASSIMP_BUILD_NO_GLTF_IMPORTER",
        "ASSIMP_BUILD_NO_OPENGEX_IMPORTER",
        "ASSIMP_BUILD_NO_C4D_IMPORTER",
    }
    
    includedirs
    {
        "assimp/include",
        "assimp/code",
        "_ship/assimp",
        "_ship",

        -- thirdparty
        "./assimp",
        "assimp/contrib",
        "assimp/contrib/zlib",
        "assimp/contrib/zlib/contrib/minizip",
        "assimp/contrib/irrXML",
    }
    
    files
    {   
        "assimp/include/**.h",
        "assimp/code/**.cpp",
        "assimp/code/**.h",
        "_ship/assimp/config.h",

        -- thirdparty
        "assimp/contrib/zlib/*.h",
        "assimp/contrib/zlib/*.c",
        "assimp/contrib/zlib/contrib/minizip/*.c",
        "assimp/contrib/zlib/contrib/minizip/*.h",

        "assimp/contrib/irrXML/*.h",
        "assimp/contrib/irrXML/*.cpp",

        "assimp/contrib/utf8cpp/source/utf8/**.h",
    }

    removefiles
    {
        "assimp/code/Importer/StepFile/**",
        "assimp/code/C4D/**",
        "assimp/code/glTF/**",
        "assimp/code/glTF2/**"
    }
    
project "yaml-cpp"
    language "C++"
    kind "StaticLib"
    
    includedirs
    {
        "yaml-cpp/include",
        "yaml-cpp/src"
    }
    
    files
    {   
        "yaml-cpp/src/**.cpp",
        "yaml-cpp/include/**.h",
    }