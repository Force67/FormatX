
project "core"
    language "C++"
    kind "SharedLib"
    targetname "FormatX"

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
		"**.cc",
        "**.ui",
        "**.inl"
    }

    filter "system:not windows"
        removefiles {"**_win.cpp", "**_win.h"}
    filter {}

        -- fetch vk dir from path
    local vkd = os.getenv("VULKAN_SDK")
    if vkd then
        libdirs(vkd .. "\\Lib")
        includedirs(vkd .. "\\Include")

        links 
        {
            "vulkan-1",
            "VkLayer_utils"
        }

        defines "QT_FEATURE_vulkan"
    end
    
    -- enable qt for this project
    qt.enable()
    
    qtmodules {
        "core",
        "gui",
        "widgets",
        "opengl"
    }

    filter "system:windows"
        qtmodules {
            "winextras",
            "qml"
        }
    filter {}
    
    qtprefix "Qt5"
    qtgenerateddir "qtgen"

    -- for ci
    local qtd = os.getenv("Qt5_Dir")
    if qtd then
        qtpath(qtd)
    end
    
    -- use debug versions of qt
    -- libs in debug builds
    configuration { "Debug" }
            qtsuffix "d"
    configuration {}