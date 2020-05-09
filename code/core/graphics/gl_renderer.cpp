
/*
 * VideoCore - Render framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <array>

#include "gl_renderer.h"
#include "gl_texture.h"
#include "gl_shader.h"

#include "window.h"

namespace gfx {

static GLRenderer* g_Renderer{nullptr};

GLRenderer::GLRenderer(FXWindow& window) : window(window) {
    textureFactory = std::make_unique<GLTextureFactory>();
    shaderFactory = std::make_unique<GLShaderFactory>();

    g_Renderer = this;
}

GLRenderer::~GLRenderer() {
    g_Renderer = nullptr;
}

GLRenderer* GetRenderer() {
    if (!g_Renderer)
        __debugbreak();

    return g_Renderer;
}

void GLRenderer::debugCallback(GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar* msg,
    const void*) {
    switch (severity) { 
    case GL_DEBUG_SEVERITY_HIGH:
        LOG_ERROR("GL Error : {}", msg);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        LOG_WARNING("GL Warning : {}", msg);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        LOG_INFO("GL Info : {}", msg);
        break;
    default:
        // TODO: maybe debug log verbose stuff?
        break;
    }
}

void GLRenderer::logDeviceInfo() {
    const char* const gpuVendor{reinterpret_cast<char const*>(glGetString(GL_VENDOR))};
    const char* const gpuModel{reinterpret_cast<char const*>(glGetString(GL_RENDERER))};

    // OpenGL doesn't let us choose the GPU
    // please supply optimus GPU hints from your main executable
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

    auto& info = window.size();
    glViewport(0, 0, info.x, info.y);

    //presentFbo.create();

    return true;
}

void GLRenderer::resize(i32 x, i32 y) {
    glViewport(0, 0, static_cast<size_t>(x), static_cast<size_t>(y));
}

void GLRenderer::shutdown() {
    gladLoaderUnloadGL();
}
}