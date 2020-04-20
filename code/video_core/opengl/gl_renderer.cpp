
/*
 * VideoCore - Render framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "gl_renderer.h"

#include "gl_texture.h"
#include "gl_shader.h"

namespace video_core {

GLRenderer::GLRenderer(renderWindow& window) : renderInterface(window) {}

void GLRenderer::debugCallback(GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar* msg,
    const void*) {
    switch (severity) { 
    case GL_DEBUG_SEVERITY_HIGH:
        LOG_ERROR("GL Error : {}", msg);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        LOG_WARNING("GL Warning : {}", msg);
        break;
    default:
        LOG_INFO("GL Info : {}", msg);
        break;
    }
}

void GLRenderer::logDeviceInfo() {
    const char* const gpuVendor{reinterpret_cast<char const*>(glGetString(GL_VENDOR))};
    const char* const gpuModel{reinterpret_cast<char const*>(glGetString(GL_RENDERER))};

    // OpenGL doesn't let us choose the GPU
    // please supply optimus GPU in your main executable
    LOG_INFO("Selected GPU {} {}", gpuVendor, gpuModel);
}

bool GLRenderer::init() {
    // load GL function pointers
    if (!gladLoaderLoadGL()) {
        LOG_ERROR("Failed to resolve OpenGL function pointers");
        return false;
    }

    if (!GLAD_GL_VERSION_4_3) {
        LOG_ERROR("OpenGL 4.3 not available");
        shutdown();
        return false;
    }

    if (GLAD_GL_KHR_debug) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(debugCallback, this);
    }

    logDeviceInfo();

    // init viewport
    const auto& config = window.getConfig();
    glViewport(0, 0, config.width, config.height);

    //presentFbo.create();

    texFactory = VAlloc<GLTextureFactory>();
    shaderFactory = VAlloc<GLShaderFactory>();
    return true;
}

void GLRenderer::shutdown() {
    if (texFactory)
        VFree(texFactory);
    if (shaderFactory)
        VFree(shaderFactory);

    gladLoaderUnloadGL();
}

void GLRenderer::present() {
    // clear the screen black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

   // glBindFramebuffer(GL_READ_FRAMEBUFFER, presentFbo.handle());
}
}