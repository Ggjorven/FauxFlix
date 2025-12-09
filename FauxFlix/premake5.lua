local Dependencies = local_require("../Dependencies.lua")
local MacOSVersion = MacOSVersion or "14.5"
local OutputDir = OutputDir or "%{cfg.buildcfg}-%{cfg.system}"

project "FauxFlix"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "On"

	architecture "x86_64"

	warnings "Extra"

	targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")

	-- Note: VS2022/Make only need the pchheader filename
	pchheader "ffpch.h"
	pchsource "Source/Faux/ffpch.cpp"

	files
	{
		"Source/Faux/**.h",
		"Source/Faux/**.hpp",
		"Source/Faux/**.inl",
		"Source/Faux/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS",
	}

	includedirs
	{
		"Source",
		"Source/Faux",
	}

	includedirs(Dependencies.Faux.IncludeDir)
	libdirs(Dependencies.Faux.LibDir)
	links(remove_from_table(Dependencies.Faux.LibName, "Faux"))
	--postbuildcommands(Dependencies.Faux.PostBuildCommands)
	defines(Dependencies.Faux.Defines)
	
	buildoptions(Dependencies.Faux.BuildOptions)
	linkoptions(Dependencies.Faux.LinkOptions)

	filter "system:windows"
		systemversion "latest"
		staticruntime "on"
		editandcontinue "off"

        defines
        {
            "NOMINMAX"
        }

	filter "system:linux"
		systemversion "latest"
		staticruntime "on"

		-- Note: For some reason gmake, now also needs full pchheader path
		pchheader "Source/Faux/ffpch.h"

    filter "system:macosx"
		systemversion(MacOSVersion)
		staticruntime "on"

	filter "action:vs*"
		buildoptions { "/Zc:preprocessor" }

	filter "action:xcode*"
		-- Note: XCode only needs the full pchheader path
		pchheader "Source/Faux/ffpch.h"

		-- Note: If we don't add the header files to the externalincludedirs
		-- we can't use <angled> brackets to include files.
		externalincludedirs(includedirs())

	filter "configurations:Debug"
		defines "FF_CONFIG_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "FF_CONFIG_RELEASE"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Distribution"
		defines "FF_CONFIG_DISTRIBUTION"
		runtime "Release"
		optimize "Full"
		linktimeoptimization "on"
