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
IncludeDir["json"] =  "Engine/vendor/json/"
IncludeDir["entt"] =  "Engine/vendor/entt/include"
IncludeDir["JoltPhysics"] =  "Engine/vendor/JoltPhysics/include"

group "Dependencies"
    include "Engine/vendor/GLFW"
    include "Engine/vendor/GLAD"
    include "Engine/vendor/imgui"
    include "Engine/vendor/assimp/"
    include "Engine/vendor/ImGuizmo/"
    include "Engine/vendor/JoltPhysics/"
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
        "%{IncludeDir.json}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.JoltPhysics}"
    }

    links
    {
        "GLFW",
        "GLAD",
        "ImGui",
        "Assimp",
        "ImGuiGizmo",
        "JoltPhysics",
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
    "LevelEditor/src",
    "%{IncludeDir.glm}",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.Assimp}",
    "%{IncludeDir.json}",
    "%{IncludeDir.entt}",
    "%{IncludeDir.JoltPhysics}"
}

links 
{
    "Engine",
    "LevelEditor",
    "JoltPhysics"
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
    kind "StaticLib"
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
        "%{IncludeDir.json}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.JoltPhysics}"
    }

    links
    {
        "Engine",
        "GLFW",
        "GLAD",
        "ImGui",
        "Assimp",
        "JoltPhysics",
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