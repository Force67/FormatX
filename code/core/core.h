#pragma once

/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <vector>
#include <base.h>
#include <config.h>
#include <plugintraits.h>
#include <utl/logger/logger.h>

#include <base.h>
#include <core.h>

#include "window.h"

#include "editor/editor.h"
#include "scene/scene.h"

namespace graphics {
class GLRenderer;
}

class FXCore {
public:
    FXCore(int, char**);
    ~FXCore();

    bool init();
    i32 exec();

    void onViewportChange(i32, i32);
private:
    void step();
    void shutdown();

    double updateTime;
    double lagTime;

    FXWindow window;

    UniquePtr<graphics::GLRenderer> renderer;
    UniquePtr<editor::FXEditor> editor;
    UniquePtr<scene::Scene> scene;

    std::vector<std::string> argv;
    std::vector<pluginLoader*> plugins;
};