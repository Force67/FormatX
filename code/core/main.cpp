
// Copyright (C) 2019-2020 Force67

#include "core.h"
#include <logger/logger.h>

#ifdef _WIN32

#include <shellscalingapi.h>

#ifdef COMPILING_CORE
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
#endif

#include <glfw/glfw3.h>

static void applyDPIScaling() {
#ifdef _WIN32
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

    EnumDisplayMonitors(
        nullptr, nullptr, [](HMONITOR monitor, HDC hdcMonitor, LPRECT rectMonitor, LPARAM data) -> BOOL CALLBACK {
            uint32_t dpi_x, dpi_y;
            GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpi_x, &dpi_y);
        
            float dpi_scale_factor = (float)dpi_x / (float)USER_DEFAULT_SCREEN_DPI;


        }, 0);
#endif
}

int CORE_API core_main(int argc, char** argv) {
    utl::createLogger(true);
    //applyDPIScaling();

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