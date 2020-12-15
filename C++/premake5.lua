--Main Project
workspace "Gravity Dynamics"
    architecture "x64"
    startproject "Gravity Dynamics"
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

--How The Files are organized
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"



project "Gravity Dynamics"
    location "Gravity Dynamics"
    kind "ConsoleApp"
    language "C++"
    staticruntime "On"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    defines{
        "_CRT_SECURE_NO_WARNINGS"
    }
    includedirs
    {
        "%{prj.name}/src"
    }
    links{

    }
    filter "system:windows"
        
        
        systemversion "latest"


        filter "configurations:Debug"
            symbols "On"

        filter "configurations:Release"
            optimize "On"

        filter "configurations:Dist"
            optimize "On"
