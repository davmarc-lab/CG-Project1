project "Bolt-Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"

    files { "include/**.hpp", "src/**.cpp", "../Bolt-Graphics/**.h", "../Bolt-Graphics/**.c" }

    includedirs {
        "../Bolt-Graphics/include/",
    }

    links {
        "Bolt-Graphics",
        "Bolt-imgui",
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
