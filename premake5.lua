workspace "Coma2D"
	location ( "Projects/%{_ACTION}" )
	configurations { "Debug", "Release" }
	platforms { "x86", "x64" }

	configuration "vs*"
		defines { "_CRT_SECURE_NO_WARNINGS" }	

	filter "configurations:Debug"
		targetdir ( "Bin/%{_ACTION}/Debug" )
		objdir ( "Bin/%{_ACTION}/Debug/Obj" )
	 	defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		targetdir ( "Bin/%{_ACTION}/Release" )
		objdir ( "Bin/%{_ACTION}/Release/Obj" )
		defines { "NDEBUG" }
		optimize "On"

project "Coma2D"
	kind "StaticLib"
	language "C++"
	files { "Coma2D/Coma2D/**.h", "Coma2D/Coma2D/**.cpp" }
	includedirs { "Coma2D" }
	links { "d2d1", "Box2D", "Box2D.lib" }
	-- links { "d2d1", "xinput", "windowscodecs", "Box2D", "Box2D.lib", "dwrite" }
	libdirs { "Lib" }

project "Box2D"
	kind "StaticLib"
	language "C++"
	files { "Coma2D/Box2D/**.h", "Coma2D/Box2D/**.cpp" }
	includedirs { "Coma2D" }
	targetdir ( "Lib" )
	objdir ( "Lib/Box2D_Obj" )