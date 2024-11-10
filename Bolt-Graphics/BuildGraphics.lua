project "Bolt-Graphics"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"

    files { "include/**.hpp", "src/**.cpp" }

    includedirs {
        "include",
    }

    links { "glm", "GL", "glfw", "X11", "Xrandr", "Xi", "dl"}

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
