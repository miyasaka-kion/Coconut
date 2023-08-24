workspace "SDL_test"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

myProjectName = "SDL_test"
outputDirName = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "SDL_test"
	location "SDL_test"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputDirName .."/%{prj.name}")
	objdir ("bin-int/" .. outputDirName .."/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/vendor/SDL2/include",
		"%{prj.name}/src"
	}

	libdirs {
		"%{prj.name}/vendor/SDL2/lib/x64"
	}

	links {
		"SDL2",
		"SDL2main"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"SDL_PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines "SDL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SDL_RELEASE"
		symbols "On"

	filter "configurations:Dist"
		defines "SDL_DIST"
		symbols "On"

