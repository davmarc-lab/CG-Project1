workspace "Project1"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "App"

    OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

    libdirs = {
        "bin/" .. OutputDir .. "/",
    }

    group "App"
        include "App/BuildApp.lua"

    group "Opengl-Core"
        include "Opengl-Core/BuildCore.lua"


