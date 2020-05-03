
// Copyright (C) 2019-2020 Force67

#include "core.h"
#include <logger/logger.h>

#ifdef _WIN32

#include <shellscalingapi.h>
#include <VersionHelpers.h>

#ifdef COMPILING_CORE
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
#endif

#include <glfw/glfw3.h>

static void applyDPIScaling() {
#ifdef _WIN32
    // windows 10 exclusive new scaling api
    auto hUser32 = ::LoadLibraryW(L"user32.dll");
    if (auto* pfunc = (decltype(&::SetThreadDpiAwarenessContext))::GetProcAddress(
            hUser32, "SetThreadDpiAwarenessContext")) {

        pfunc(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        return;
    }

    // w8 scaling
    if (IsWindows8Point1OrGreater()) {
        auto hShcore = ::LoadLibraryW(L"shcore.dll");
        if (auto* pfunc = (decltype(&::SetProcessDpiAwareness))::GetProcAddress(
                hShcore, "SetProcessDpiAwareness")) {

            pfunc(PROCESS_PER_MONITOR_DPI_AWARE);
            return;
        }
    }

    // legacy scaling
    SetProcessDPIAware();
#endif
}

int CORE_API core_main(int argc, char** argv) {
    // logger with own console
    utl::createLogger(true);
    config::load();

    applyDPIScaling();

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