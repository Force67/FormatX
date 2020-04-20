
/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "gl_helpers.h"

namespace video_core::helpers {

void GLFramebuffer::create() {
    glGenFramebuffers(1, &GL_handle);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_handle);
}
}