#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "video_core.h"
#include "glad/gl.h"

namespace video_core::helpers {

class GLFramebuffer {
public:
    GLFramebuffer() = default;

    void create();

    u32 handle() {
        return GL_handle;
    }

private:
    u32 GL_handle = 0;
};

}