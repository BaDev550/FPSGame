project "JoltPhysics"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "include/Jolt/**.cpp",
        "include/Jolt/**.h"
    }

    includedirs {
        "include"
    }