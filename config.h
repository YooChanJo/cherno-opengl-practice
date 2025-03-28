#ifndef CONFIG_H
#define CONFIG_H

#include <string>

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
    };
    
    std::string cppFiles[] = { // with extension
        "Main.cpp",
        "VertexBuffer.cpp",
        "IndexBuffer.cpp",
        "VertexBufferLayout.cpp",
        "VertexArray.cpp",
        "Shader.cpp",
        "Renderer.cpp",
        "stb_image_define.cpp",
        "Texture.cpp",
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