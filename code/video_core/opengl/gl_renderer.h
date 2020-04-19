#pragma once

/*
 * VideoCore - Render framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "video_core.h"
#include "glad/gl.h"

namespace video_core {

class GLRenderer final : public renderInterface {
public:
    explicit GLRenderer(renderWindow& window);

    // controller
    bool init() override;
    void shutdown() override;
    void present() override;

private:
    void logDeviceInfo();

    static void debugCallback(GLenum, GLenum, GLuint, GLenum,
                               GLsizei, const GLchar*, const void*); 
};
}