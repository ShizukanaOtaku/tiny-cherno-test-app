workspace "TestApp"
    configurations { "Debug", "Release" }
    location "build"

group "Core"
    include "lib/tiny-cherno-lib/Include.lua"
group ""

project "TestApp"
    kind "ConsoleApp"
    language "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    links { "TinyChernoLib" }

    files { "src/**" }
    includedirs { "lib/tiny-cherno-lib/src" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "on"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "on"
