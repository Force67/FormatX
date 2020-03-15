
project "app"
    language "C++"
    kind "WindowedApp"
    targetname "FormatX"

    filter "system:windows"
        linkoptions "/ENTRY:mainCRTStartup"
    filter {}

        -- win/project resource description
    defines { "rsc_company=\"Dev-Force\"",
              "rsc_copyright=\"(C) Force67 2019-2020. All rights reserved\"",
              "rsc_fileversion=\"1.0.0.0\"", 
              "rsc_productversion=\"1.0.0.0\"",
              "rsc_internalname=\"%{prj.name}\"",
              "rsc_productname=\"FormatX\"",
              "rsc_originalname=\"%{prj.name}%{prj.targetextension}\"",
              "rsc_description=\"Asset Editing Utility\"" }

    links
    {
        "FormatX"
    }

    includedirs
    {
        ".",
        "../core",
    }

    files
    {
        "premake5.lua",
        "_res/app.rc",
        "**.h",
        "**.cpp",
    }
