
// Copyright (C) 2019-2020 Force67

#include "core.h"
#include <logger/logger.h>

#ifdef _WIN32
#ifdef COMPILING_CORE
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
#endif

#include <glfw/glfw3.h>

int CORE_API core_main(int argc, char** argv) {
    utl::createLogger(true);

    glfwSetErrorCallback([](int errc, const char* desc) { 
        LOG_ERROR("glfw error ({}) : {}", errc, desc); 
    });

    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize glfw");
        return -1;
    }

    UniquePtr<FXCore> app(new FXCore(argc, argv));
    if (!app->init()) {
        return 0;
    }

    return app->exec();
}