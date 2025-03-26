/* 
    This program should be used within /bin dir
    cd bin
    g++ ../debug.cpp -o debug
    [cmd] debug.exe, [powershell] ./debug.exe
*/

#include <iostream>
#include <string>
#include <Windows.h>

#include "./config.h"

#define STR_ARRAY_SIZE(X) sizeof(X) / sizeof(std::string);
#define SIMPLE_LOOP(X) for(int i=0;i<X;i++)

// configs

int customLibDirectoryNum = STR_ARRAY_SIZE(ProjectConfig::customLibDirectory);
int customSystemIncludeDirectoryNum = STR_ARRAY_SIZE(ProjectConfig::customSystemIncludeDirectory);

int predefinedMacroNum = STR_ARRAY_SIZE(ProjectConfig::predefinedMacros);

int cppFileNum = STR_ARRAY_SIZE(ProjectConfig::cppFiles)
int customLibNum = STR_ARRAY_SIZE(ProjectConfig::customLibs);
int mingwLibNum = STR_ARRAY_SIZE(ProjectConfig::mingwLibs);

int main() {
    try {
        std::string cmd = "g++ ";
        SIMPLE_LOOP(cppFileNum) cmd += "\"./src/" + ProjectConfig::cppFiles[i] + "\" ";
        SIMPLE_LOOP(customLibDirectoryNum) cmd += "-L\"" + ProjectConfig::customLibDirectory[i] +"\" ";
        SIMPLE_LOOP(customSystemIncludeDirectoryNum) cmd += "-I\"" + ProjectConfig::customSystemIncludeDirectory[i] + "\" ";
        SIMPLE_LOOP(predefinedMacroNum) cmd += "-D" + ProjectConfig::predefinedMacros[i] + " ";
        SIMPLE_LOOP(customLibNum) cmd += "-l" + ProjectConfig::customLibs[i] + " ";
        SIMPLE_LOOP(mingwLibNum) cmd += "-l" + ProjectConfig::mingwLibs[i] + " ";
        cmd += " -o \"" + ProjectConfig::debugProgram + "\"";
        system(cmd.c_str());
        system("debug.tmp.exe");
        return 0;
    } catch(...) {
        std::cout << "Unexpected debug error..." << std::endl;
    }
}