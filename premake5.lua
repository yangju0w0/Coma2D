workspace "Coma2D"
	location ( "Projects/%{_ACTION}" )
	configurations { "Debug", "Release" }
	platforms { "x86", "x64" }
	targetdir ( "Bin/%{_ACTION}/$(ProjectName)/$(Configuration)" )
	objdir ( "Bin/%{_ACTION}/$(ProjectName)/$(Configuration)/Obj" )
	callingconvention ("StdCall")

	configuration "vs*"
		defines { "_CRT_SECURE_NO_WARNINGS" }	

	filter "configurations:Debug"
	 	defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

project "Coma2D"
	kind "StaticLib"
	language "C++"
	files { "Coma2D/Coma2D/**.h", "Coma2D/Coma2D/**.cpp" }
	includedirs { "Coma2D" }
	-- links { "d2d1", "Box2D", "Box2D.lib" }
	links { "d2d1", "xinput", "windowscodecs", "Box2D", "Box2D.lib", "dwrite" }
	libdirs { "Bin/%{_ACTION}/Box2D/$(Configuration)" }

project "Box2D"
	kind "StaticLib"
	language "C++"
	files { "Coma2D/Box2D/**.h", "Coma2D/Box2D/**.cpp" }
	includedirs { "Coma2D" }

project "SampleHelloWorld"
	kind "WindowedApp"
	language "C++"
	files { "Coma2D/SampleHelloWorld/**.h", "Coma2D/SampleHelloWorld/**.cpp" }
	entrypoint ( "" )
	includedirs { "Coma2D" }
	links { "Coma2D" }
