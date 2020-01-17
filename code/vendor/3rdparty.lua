
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
    