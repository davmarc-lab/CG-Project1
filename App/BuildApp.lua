project "App"
    kind "ConsoleApp"
    language "C++"
    -- current project cpp version
    -- cppdialect "C++20"

    targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("../bin/int/" .. OutputDir .. "/%{prj.name}")

    files { "include/**.hpp", "src/**.cpp" }

    includedirs {
        "include",
        "../Opengl-Core/include/"
    }

    links {
        "Opengl-Core",
        "glfw",
        "freetype",
    }

    dependson { "Opengl-Core" }

    filter "configurations:Debug"
        defines({ "DEBUG" })
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines({ "NDEBUG" })
        runtime "Release"
        optimize "On"
        symbols "On"

