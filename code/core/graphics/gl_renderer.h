#pragma once

/*
 * VideoCore - Render framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "glad/gl.h"

#include <base.h>
#include <logger/logger.h>

class FXWindow;

namespace gfx {

class GLTextureFactory;
class GLShaderFactory;

class GLRenderer {
public:
    explicit GLRenderer(FXWindow& window);
    ~GLRenderer();

protected:
    // controller
    bool init();
    void shutdown();

public:
    void resize(i32, i32);
public:
    UniquePtr<GLTextureFactory> textureFactory;
    UniquePtr<GLShaderFactory> shaderFactory;

private:
    FXWindow& window;


    void logDeviceInfo();

    static void debugCallback(GLenum, GLenum, GLuint, GLenum,
                               GLsizei, const GLchar*, const void*); 
};

GLRenderer* GetRenderer();
}