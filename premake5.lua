workspace "bolt-engine"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Bolt-Test"

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

libdirs = {
    "bin/" .. OutputDir .. "/",
}

group "Bolt-Graphics"
include "Bolt-Graphics/BuildGraphics.lua"

group "Bolt-imgui"
include "Bolt-imgui/BuildImGui.lua"

group "Bolt-Core"
include "Bolt-Core/BuildCore.lua"

group "Bolt-Kart"
    include "Bolt-Kart/BuildKart.lua"

