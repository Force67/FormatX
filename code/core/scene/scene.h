#pragma once

/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <string>
#include "camera.h"

namespace gfx {
class GLRenderer;
class GLShaderProgram;
class GLRenderTexture;
}

namespace scene {

class Scene {
public:
    Scene();
    ~Scene();

    bool create(gfx::GLRenderer&);

    void draw(gfx::GLRenderTexture&);
private:
    Camera cam;
    gfx::GLShaderProgram* projProgram;
};

Scene* ActiveScene();
} // namespace scene