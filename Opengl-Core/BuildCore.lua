project "Opengl-Core"
    kind "StaticLib"
    language "C++"
    -- current project cpp version
    -- cppdialect "C++20"

    targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("../bin/int/" .. OutputDir .. "/%{prj.name}")

    files { "include/**.hpp", "src/**.cpp", "**.hpp", "**.cpp", "**.h", "**.c" }

    includedirs {
        "include/",
        "vendor/include/"
    }

    links {
        "GL",
        "glfw",
        "X11",
        "Xrandr",
        "Xi",
        "dl"
    }

    filter "configurations:Debug"
        defines({ "DEBUG" })
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines({ "NDEBUG" })
        runtime "Release"
        optimize "On"
        symbols "On"

