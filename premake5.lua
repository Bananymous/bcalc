workspace "bcalc"
    configurations { "Debug", "Release" }

project "bcalc"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    files {
		"src/Lexer.cpp",
        "src/main.cpp",
		"src/Parser.cpp",
		"src/TokenNode.cpp",
    }

    includedirs "src"

    filter "configurations:Debug"  
        symbols "On"

    filter "configurations:Release"
        optimize "On"
