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

namespace graphics {
class GLRenderer;
class GLShaderProgram;
}

namespace scene {

class Scene {
public:
    Scene();

    bool create(graphics::GLRenderer&);
    void draw();

private:
    Camera cam;
    graphics::GLShaderProgram* projProgram;
};
} // namespace scene