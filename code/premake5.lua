
git_branch = ""
git_commit = ""

local function git_info()
    local f = io.popen('git symbolic-ref --short -q HEAD', 'r')
    local temp = f:read("*a")
    f:close()

    -- sanitize
    git_branch = string.gsub(temp, '\n$', '')

    f = io.popen('git rev-parse --short HEAD', 'r')
    temp = f:read("*a")
    f:close()

    git_commit = string.gsub(temp, '\n$', '')
end

workspace "FormatX"
    configurations { "Debug", "Release" }
    
    architecture "x86_64"
    vectorextensions "AVX"

    -- build output
    location "../build"
    os.mkdir"../build/symbols"
    targetdir '../bin/%{cfg.buildcfg}'
    
    flags "MultiProcessorCompile"
    buildoptions "/std:c++17"
    symbols "On"
    characterset "Unicode"
    
    git_info()
    defines { ("PRJ_NAME=\"%{wks.name}\""), 
              ("PRJ_NAME_WIDE=L\"%{wks.name}\""),
              ('GIT_BRANCH="' .. git_branch .. '"'),
              ('GIT_COMMIT="' .. git_commit .. '"'),
              ('PRJ_CANARY=') .. (git_branch == "master" and 0 or 1) }

    libdirs "./shared/Lib"

    filter "platforms:x64"
         architecture "x86_64"

    filter "configurations:Debug"
        defines { "DELTA_DBG" }

    filter "configurations:Release"
        optimize "Speed"

    filter {"system:windows", "kind:not StaticLib"}
         linkoptions { "/PDB:\"$(SolutionDir)\\symbols\\$(ProjectName)_%{cfg.buildcfg}.pdb\"" }

    filter { "system:windows", "kind:not StaticLib" }
        linkoptions "/manifestdependency:\"type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\""

    -- Disable deprecation warnings and errors
    filter "action:vs*"
        defines
        {
            "NOMINMAX",
            "WIN32_LEAN_AND_MEAN",
            "_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING",
            "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING",
            "_CRT_SECURE_NO_WARNINGS",
            "_CRT_SECURE_NO_DEPRECATE",
            "_CRT_NONSTDC_NO_WARNINGS",
            "_CRT_NONSTDC_NO_DEPRECATE",
            "_SCL_SECURE_NO_WARNINGS",
            "_SCL_SECURE_NO_DEPRECATE"
        }
        
    startproject "app"
    group "plugin"
    include "plugins/plugins.lua"
    
    group "app"
    include "./app"
	include "./core"
    include "./video_core"
	include "./common"

    group "vendor"
    include "./vendor/3rdparty.lua"

-- Cleanup
if _ACTION == "clean" then
    os.rmdir("../bin");
    os.rmdir("../build");
end
