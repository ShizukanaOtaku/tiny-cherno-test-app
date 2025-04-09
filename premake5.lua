workspace "TestApp"
    configurations { "Debug", "Release" }
    location "build"

    include "lib/tiny-cherno-lib/Include.lua"

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
