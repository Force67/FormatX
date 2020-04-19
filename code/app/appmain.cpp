
// Copyright (C) 2019-2020 Force67

// request to be run on a dedicated GPU
/*extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}*/

#define CORE_API __declspec(dllimport)

CORE_API int core_main(int argc, char** argv);

int main(int argc, char** argv) {
    // TODO: crash dump init

    return core_main(argc, argv);
}