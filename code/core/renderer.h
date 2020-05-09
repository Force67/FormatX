#pragma once

/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <glm/vec2.hpp>

#include "scene/scene.h"

#include "graphics/gl_renderer.h"
#include "graphics/gl_helpers.h"

class FXWindow;

namespace ui {
class Editor;
}

namespace gfx {
class GLRenderTexture;
}

class FXRenderer final : public gfx::GLRenderer {
public:
    FXRenderer(FXWindow&);
    ~FXRenderer();

    bool init();
    void shutdown();

    void draw(ui::Editor&);
private:
    UniquePtr<gfx::GLRenderTexture> sceneTarget;
    UniquePtr<scene::Scene> scene;

    bool renderScene = false;
};