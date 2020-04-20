#pragma once

/*
 * VideoCore - Render framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "video_core.h"
#include "gl_helpers.h"

namespace video_core {

class GLFramebuffer {
public:

private:
    u32 GL_fbo = 0;
};

class GLRenderer final : public renderInterface {
public:
    explicit GLRenderer(renderWindow& window);

    // controller
    bool init() override;
    void shutdown() override;
    void present() override;

private:
    helpers::GLFramebuffer presentFbo;
    helpers::GLFramebuffer renderFbo;

    void logDeviceInfo();

    static void debugCallback(GLenum, GLenum, GLuint, GLenum,
                               GLsizei, const GLchar*, const void*); 
};
}