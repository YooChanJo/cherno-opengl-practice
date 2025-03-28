#include <Windows.h>
#include <iostream>

int main() {
    std::cout << "Starting reconfiguration with build.cpp run.cpp debug.cpp config.h...\n" << std::endl;

    std::cout << "Building build.cpp..." << std::endl;
    system("g++ build.cpp -o build.exe");
    std::cout << "Finished building build.cpp" << std::endl;

    std::cout << "\nBuilding run.cpp..." << std::endl;
    system("g++ run.cpp -o run.exe");
    std::cout << "Finished building run.cpp" << std::endl;

    std::cout << "\nBuilding debug.cpp..." << std::endl;
    system("g++ debug.cpp -o debug.exe");
    std::cout << "Finished building debug.cpp" << std::endl;

    return 0;
}