project "Bolt-imgui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"

    files { "include/**.hpp", "src/**.cpp" }

    includedirs {
        "./include/imgui/",
        "../Bolt-Vendor/include",
    }

    targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("../bin/int/" .. OutputDir .. "/%{prj.name}")

    filter "configurations:Debug"
        defines {
            "BT_ENABLE_DEBUG",
        }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        symbols "On"
