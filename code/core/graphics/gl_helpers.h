#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>

namespace graphics::helpers {

// you have to render onto the render target
// in order to display in sceneview
class GLRenderTarget {
public:
    GLRenderTarget();
    ~GLRenderTarget();

    void use();

private:
    u32 GL_handle;
};

}