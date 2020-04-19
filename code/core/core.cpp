
/*
 * FormatX 
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#ifdef _WIN32
#include <Windows.h>
#endif

#include "core.h"
#include "video_core.h"

constexpr video_core::RenderApi BACKEND_TYPE = video_core::RenderApi::opengl;

FXCore::FXCore(int argc, char** argv) {
    if (argc > 1) {
        std::vector<std::string> xargv;

        for (int i = 1; i < argc; i++)
            xargv.emplace_back(argv[i]);

        this->argv = std::move(xargv);
    }
}

FXCore::~FXCore() {
    if (renderer)
        renderer->shutdown();
}

bool FXCore::init() {
    if (!createViewport()) {
        return false;
    }

    editor = std::make_unique<ui::FXEditor>(window);
    if (!editor->create(*renderer))
        __debugbreak();

    return true;
}

bool FXCore::createViewport() {
    auto apiType = BACKEND_TYPE;

    if (window.create(apiType)) {

        video_core::RenderInstanceDesc desc{};
        desc.flags = video_core::enable_validation;
        desc.appName = "FormatX";
        desc.apiType = apiType;

        renderer = video_core::createRenderer(window, desc);
        if (!renderer) {
            LOG_ERROR("Failed to create renderer. Error code: {}", static_cast<i32>(apiType));
            return false;
        }

        if (renderer->init()) {
            LOG_INFO("Render init success");
            window.show();
            return true;
        }
    }

    return false;
}

i32 FXCore::exec() {
    while (true) {
        // exit requested
        if (!window.update()) break;
    
        editor->update();
        renderer->present();
    }
    return 0;
}