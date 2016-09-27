#include "Extension.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//#define WSA_CONSOLE
#ifndef WSA_CONSOLE

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        wsa::extension::initialize();
        break;

    case DLL_PROCESS_DETACH:
        wsa::extension::finalize();
        break;
    }
    return true;
}

extern "C" {
    __declspec(dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
};

void __stdcall RVExtension(char *output, int outputSize, const char *function) {
    outputSize -= 1;
    wsa::extension::call(output, outputSize, function);
};

#else

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string line = "";
    const int outputSize = 10000;
    char *output = new char[outputSize];
    
    wsa::extension::initialize();
    while (line != "exit") {
        std::getline(std::cin, line);
        wsa::extension::call(output, outputSize, line.c_str());
        std::cout << "WSA: " << output << std::endl;
    }
    wsa::extension::finalize();
    return 0;
}

#endif