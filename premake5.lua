workspace "TestApp"
    configurations { "Debug", "Release" }
    location "build"
    architecture "x64"
    cppdialect "C++20"

    include "lib/cherrypink-lib/Include.lua"

project "TestApp"
    kind "WindowedApp"
    language "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    links { "CherryPink", "glfw3" }

    files { "src/**" }
    includedirs { "lib/cherrypink-lib/src" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "on"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "on"

    filter "system:windows"
        entrypoint "mainCRTStartup" -- Make sure that we use the main function instead of WinMain for cross-platform compatibility
