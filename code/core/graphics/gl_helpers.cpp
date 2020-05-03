
/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "gl_helpers.h"
#include "glad/gl.h"

namespace graphics::helpers {

GLRenderTarget::GLRenderTarget() {
    glGenFramebuffers(1, &GL_handle);
}

GLRenderTarget::~GLRenderTarget() {
    glDeleteFramebuffers(1, &GL_handle);
}

void GLRenderTarget::use() {
    glBindFramebuffer(GL_FRAMEBUFFER, GL_handle);

}
}