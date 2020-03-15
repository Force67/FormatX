
// Copyright (C) 2019-2020 Force67

#define CORE_API __declspec(dllimport)

CORE_API int core_main(int argc, char** argv);

int main(int argc, char** argv) {
    // TODO: crash dump init

    return core_main(argc, argv);
}