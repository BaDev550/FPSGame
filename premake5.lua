workspace "FPSGame"
    architecture "x64"
    startproject "Game"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] =  "Engine/vendor/GLFW/include"
IncludeDir["GLAD"] =  "Engine/vendor/GLAD/include"
IncludeDir["Assimp"] ="Engine/vendor/assimp/include"
IncludeDir["ImGuiGizmo"] = "Engine/vendor/ImGuizmo/"
IncludeDir["ImGui"] = "Engine/vendor/ImGui/"
IncludeDir["glm"] =   "Engine/vendor/glm/"
IncludeDir["stb"] =   "Engine/vendor/stb/"
IncludeDir["entt"] =  "Engine/vendor/entt/include"
IncludeDir["yaml"] =  "Engine/vendor/yaml/include"

group "Dependencies"
    include "Engine/vendor/GLFW"
    include "Engine/vendor/GLAD"
    include "Engine/vendor/imgui"
    include "Engine/vendor/assimp/"
    include "Engine/vendor/ImGuizmo/"
    include "Engine/vendor/yaml/"
group ""

project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "epch.h"
    pchsource "Engine/src/epch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb/**.h",
        "%{prj.name}/vendor/stb/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    defines 
    {
        "_CRT_SECURE_NO_WARNINGS"
    }
    
    includedirs 
    {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGuiGizmo}",
        "%{IncludeDir.Assimp}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.yaml}",
        "%{IncludeDir.entt}"
    }

    links
    {
        "GLFW",
        "GLAD",
        "ImGui",
        "Assimp",
        "yaml-cpp",
        "ImGuiGizmo",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"
        buildoptions "/utf-8"

        defines 
        {
            "E_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"
        }
        
    filter "configurations:Debug"
        defines "E_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "E_RELEASE"
        runtime "Release"
        optimize "on"

project "Game"
location "Game"
kind "ConsoleApp"
language "C++"
cppdialect "C++17"
staticruntime "on"

targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

files
{
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp"
}

includedirs
{
    "Engine/src",
    "Engine/vendor",
    "Game/src",
    "%{IncludeDir.glm}",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.Assimp}",
    "%{IncludeDir.yaml}",
    "%{IncludeDir.entt}"
}

links 
{
    "Engine"
}

filter "system:windows"
    systemversion "latest"
    buildoptions "/utf-8"

    defines 
    {
        "E_PLATFORM_WINDOWS"
    }
    
filter "configurations:Debug"
    defines "E_DEBUG"
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    defines "E_RELEASE"
    runtime "Release"
    optimize "on"

project "LevelEditor"
    location "LevelEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Engine/src",
        "Engine/vendor",
        "LevelEditor/src",
        "%{IncludeDir.glm}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Assimp}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.yaml}",
        "%{IncludeDir.entt}"
    }

    links
    {
        "Engine",
        "Game",
        "GLFW",
        "GLAD",
        "ImGui",
        "Assimp",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"
        buildoptions "/utf-8"

        defines 
        {
            "E_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "E_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "E_RELEASE"
        runtime "Release"
        optimize "on"