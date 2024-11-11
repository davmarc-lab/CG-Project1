project "Bolt-Kart"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"

    files { "include/**.hpp", "src/**.cpp" }

    includedirs {
        "include",
        "../Bolt-Core/include/"
    }

    links {
        "Bolt-Core",
        "glfw",
        "freetype",
        "Bolt-imgui",
    }

    dependson { "Bolt-Core", "Bolt-imgui" }

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


