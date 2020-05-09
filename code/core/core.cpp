
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
#include "glfw/glfw3.h"

static config::opt<f64> STEP_TIME{"core.step_time", "core step time (MS)", 16.0};

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

    config::save();
}

bool FXCore::init() {

    if (!window.create(true))
        return false;

    renderer = std::make_unique<FXRenderer>(window);
    if (!renderer->init()) {
        LOG_ERROR("Failed to create renderer");
        return false;
    }

    editor = std::make_unique<ui::Editor>(window, *renderer);
    if (!editor->create(*renderer)) {
        LOG_ERROR("Failed to create editor");
        return false;
    }

    editor->init();

    window.show();
    return true;
}

void FXCore::onViewportChange(i32 x, i32 y) {
    renderer->resize(x, y);

    // update ui
    editor->resize(x, y);
}

void FXCore::shutdown() {
}

void FXCore::step() {
    std::puts("stepping");
}

i32 FXCore::exec() {
    updateTime = glfwGetTime();

    while (true) {
        // update core timer
        double current = glfwGetTime();
        double elapsed = current - updateTime;

        updateTime = current;
        lagTime += elapsed;

        // always update window to avoid ghosting
        if (!window.update()) {
            shutdown();
            break;
        }

        // issue steps while we catch up
        while (lagTime >= STEP_TIME) {
            step();
            lagTime -= STEP_TIME;
        }

        // TODO: frame time checks
        renderer->draw(*editor);
    }
    return 0;
}