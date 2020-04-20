
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

namespace graphics {

static std::array<GLfloat, 3 * 2> MakeOrthographicMatrix(float width, float height) {
    std::array<GLfloat, 3 * 2> matrix; // Laid out in column-major order

    // clang-format off
    matrix[0] = 2.f / width; matrix[2] =  0.f;          matrix[4] = -1.f;
    matrix[1] = 0.f;         matrix[3] = -2.f / height; matrix[5] =  1.f;
    // Last matrix row is implicitly assumed to be [0, 0, 1].
    // clang-format on

    return matrix;
}

GLRenderer::GLRenderer(FXWindow& window) : window(window) {
    textureFactory = std::make_unique<GLTextureFactory>();
    shaderFactory = std::make_unique<GLShaderFactory>();
}

GLRenderer::~GLRenderer() {
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

    auto& info = window.size();
    glViewport(0, 0, info.x, info.y);

    //presentFbo.create();

    return true;
}

void GLRenderer::shutdown() {
    gladLoaderUnloadGL();
}

void GLRenderer::present() {
    return;

    // clear the screen black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // screen space projection
    const auto& size = window.size();
    auto matrix = MakeOrthographicMatrix(
        static_cast<float>(size.x),
        static_cast<float>(size.y));

    glClear(GL_COLOR_BUFFER_BIT);

   // glBindFramebuffer(GL_READ_FRAMEBUFFER, presentFbo.handle());
}
}