/* 
    This program should be used within /bin dir
    cd bin
    g++ ../debug.cpp -o debug
    [cmd] debug.exe, [powershell] ./debug.exe
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

#include <Windows.h>

#include "./config.h"

#define STR_ARRAY_SIZE(X) sizeof(X) / sizeof(std::string);
#define SL(X, Y) for(int Y=0; Y<X; Y++)

// configs
namespace fs = std::filesystem;
static std::vector<std::string> getCppFilesOfDir(std::string path) {
    std::vector<std::string> result;
    for (const auto & entry : fs::directory_iterator(path))
        if(entry.path().extension() == ".cpp")
            result.push_back(entry.path().filename().generic_string()); // with extension
    return result;
}

int customLibDirectoryNum = STR_ARRAY_SIZE(ProjectConfig::customLibDirectory);
int customSystemIncludeDirectoryNum = STR_ARRAY_SIZE(ProjectConfig::customSystemIncludeDirectory);

int predefinedMacroNum = STR_ARRAY_SIZE(ProjectConfig::predefinedMacros);

int customLibNum = STR_ARRAY_SIZE(ProjectConfig::customLibs);
int mingwLibNum = STR_ARRAY_SIZE(ProjectConfig::mingwLibs);

int main() {
    try {
        std::string cmd = "g++ ";

        int srcDirctoryNum = sizeof(ProjectConfig::srcDirectory) / sizeof(ProjectConfig::SrcDir);
        SL(srcDirctoryNum, i) {
            std::vector<std::string> files = getCppFilesOfDir(ProjectConfig::srcDirectory[i].dir);
            const std::vector<std::string>& exceptions = ProjectConfig::srcDirectory[i].excludedFileNames;
            for(const auto file: files) {
                bool used = true;
                for(const auto except: exceptions) {
                    if(except == file) used = false;
                }
                if(used) cmd += "\"" + ProjectConfig::srcDirectory[i].dir + "/" + file + "\" ";
            }
        }

        //SL(cppFileNum) cmd += "\"./src/" + ProjectConfig::cppFiles[i] + "\" ";
        SL(customLibDirectoryNum, i) cmd += "-L\"" + ProjectConfig::customLibDirectory[i] +"\" ";
        SL(customSystemIncludeDirectoryNum, i) cmd += "-I\"" + ProjectConfig::customSystemIncludeDirectory[i] + "\" ";
        SL(predefinedMacroNum, i) cmd += "-D" + ProjectConfig::predefinedMacros[i] + " ";
        SL(customLibNum, i) cmd += "-l" + ProjectConfig::customLibs[i] + " ";
        SL(mingwLibNum, i) cmd += "-l" + ProjectConfig::mingwLibs[i] + " ";
        cmd += " -o \"" + ProjectConfig::debugProgram + "\"";
        system(cmd.c_str());
        system("debug.tmp.exe");
        return 0;
    } catch(...) {
        std::cout << "Unexpected debug error..." << std::endl;
    }
}