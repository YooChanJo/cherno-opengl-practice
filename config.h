#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

namespace ProjectConfig {
    std::string projectDir = "./"; // depends on where the exe is

    std::string customLibDirectory[] = {
        "./glfw-3.4.bin.WIN64/lib-mingw-w64",
        "./glew-2.2.0/lib",
    };
    std::string customSystemIncludeDirectory[] = {
        "./glfw-3.4.bin.WIN64/include",
        "./glew-2.2.0/include",
        "./vendor",
    };
    
    std::string predefinedMacros[] = {
        "GLEW_STATIC",
        "STB_IMAGE_IMPLEMENTATION",
    };

    struct SrcDir {
        std::string dir;
        std::vector<std::string> excludedFileNames;
    };

    SrcDir srcDirectory[] = {
        { "./src", {} },
        { "./vendor/imgui", { "main.cpp" } },
    };

    std::string customLibs[] = {
        "glfw3",
        "glew32",
    };
    std::string mingwLibs[] = {
        "gdi32",
        "user32",
        "opengl32"
    };
    std::string debugProgram = "debug.tmp.exe";
}


#endif