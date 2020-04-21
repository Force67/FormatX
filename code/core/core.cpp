
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
#include "graphics/gl_renderer.h"

FXCore::FXCore(int argc, char** argv) : window(*this) {
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

    editor->init();

    return true;
}

bool FXCore::createViewport() {
    if (window.create(true)) {

        renderer = std::make_unique<graphics::GLRenderer>(window);
        if (!renderer->init()) {
            LOG_ERROR("Failed to create renderer");
            return false;
        }

        LOG_INFO("Renderer OK");
        window.show();
        return true;
    }

    return false;
}

void FXCore::onViewportChange(i32 x, i32 y) {
    renderer->resize(x, y);

    // update ui
    editor->resize(x, y);
}

i32 FXCore::exec() {
    while (true) {
        // exit requested
        if (!window.update()) break;
    
        editor->update();
        renderer->present();

        // yield
        Sleep(1);
    }
    return 0;
}