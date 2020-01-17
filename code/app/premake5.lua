
project "app"
    language "C++"
    kind "WindowedApp"
    targetname "formatx"

    filter "system:windows"
        linkoptions "/ENTRY:mainCRTStartup"
    filter {}

    links
    {
        "shared"
    }

    includedirs
    {
        ".",
        "../shared"
    }

    files
    {
        "premake5.lua",
        "**.h",
        "**.cpp",
        "**.ui"
    }
    
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