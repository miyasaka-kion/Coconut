workspace "SDL_Game"
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
		-- SDL2
		"%{prj.name}/vendor/SDL2/include",
		"%{prj.name}/vendor/SDL2",

		-- SDL2_image
		"%{prj.name}/vendor/SDL2_image/include",
		"%{prj.name}/vendor/SDL2_image",
		-- others
		"%{prj.name}/src"
	}

	libdirs {
		-- SDL2
		"%{prj.name}/vendor/SDL2/lib/x64",

		-- SDL2_image
		"%{prj.name}/vendor/SDL2_image/lib/x64",
		"%{prj.name}/vendor/SDL2_image/lib/x64/optional"

	}

	links {
		-- SDL2
		"SDL2",
		"SDL2main",

		--SDL2_image
		"SDL2_image",
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



project "CoconutEngine"
	location "CoconutEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputDirName .."/%{prj.name}")
	objdir ("bin-int/" .. outputDirName .."/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		-- SDL2
		"%{prj.name}/vendor/SDL2/include".

		-- SDL2_image
		"%{prj.name}/vendor/SDL2_image/include",

		-- spdlog
		-- "%{prj.name}/vendor/spdlog/include",

		-- others
		"%{prj.name}/src"
	}

	libdirs {
		-- SDL2
		"%{prj.name}/vendor/SDL2/lib/x64",

		-- SDL2_image
		"%{prj.name}/vendor/SDL2_image/lib/x64",
		"%{prj.name}/vendor/SDL2_image/lib/x64/optional"

		-- spdlog
		-- 
	}

	links {
		-- SDL2
		"SDL2",
		"SDL2main",

		--SDL2_image
		"SDL2_image",
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
