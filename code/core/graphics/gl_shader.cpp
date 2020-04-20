
/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "gl_shader.h"
#include "glad/gl.h"

#include <logger/logger.h>

namespace graphics {

// 410 is our lowest supported GL version for now
// TODO: auto convert GL version!
constexpr const char* GLSL_VERSION = "#version 410 core\n";

GLShader::~GLShader() {
    if (GL_handle)
        glDeleteShader(GL_handle);
}
GLShaderProgram::~GLShaderProgram() {
    // unlink all shaders to ensure
    // that the program will be free'd
    unlinkAllshaders();

    if (GL_handle)
        glDeleteProgram(GL_handle);
}

i32 GLShaderProgram::getAttrib(const char* name) {
    return glGetAttribLocation(GL_handle, name);
}

i32 GLShaderProgram::getUniform(const char* name) {
    return glGetUniformLocation(GL_handle, name);
}

void GLShaderProgram::unlinkAllshaders() {
    // number of shaders
    GLsizei length;
    glGetProgramiv(GL_handle, GL_ATTACHED_SHADERS, &length);

    std::vector<GLuint> handles(length);
    glGetAttachedShaders(GL_handle, length, nullptr, handles.data());

    for (GLuint handle : handles)
        glDetachShader(GL_handle, handle);
}

void GLShaderProgram::use() {
    glUseProgram(GL_handle);
}

GLShaderFactory::GLShaderFactory() {}

GLShaderFactory::~GLShaderFactory() {
    for (auto* p : programs)
        delete p;

    for (auto* s : shaders)
        delete s;
}

// to native type
GLuint GLShaderFactory::translateType(ShaderType type) {
    switch (type) { 
    case ShaderType::Fragment:
        return GL_FRAGMENT_SHADER;
    case ShaderType::Vertex:
        return GL_VERTEX_SHADER;
    default:
        return GL_INVALID_ENUM;
    }
}

GLShader* GLShaderFactory::createFromSource(ShaderType type, const char* src) {
    auto glType = translateType(type);
    if (glType == GL_INVALID_ENUM)
        return nullptr;

    auto handle = glCreateShader(glType);
    if (!handle)
        return nullptr;

    // compile the shader
    const GLchar* with_version[2] = {GLSL_VERSION, src};
    glShaderSource(handle, 2, with_version, nullptr);
    glCompileShader(handle);

    // error reporting
    GLint compileResult = GL_FALSE, logLength = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &compileResult);
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 1) {
        std::string error(logLength, ' ');
        glGetShaderInfoLog(handle, logLength, NULL, &error[0]);

        if (compileResult == GL_TRUE)
            LOG_INFO("GL compile {}", error);
        else {
            LOG_ERROR("Error compiling shader {}", error);
            return nullptr;
        }

    }

    GLShader* shader = new GLShader;
    shader->GL_handle = handle;

    shaders.push_back(shader);
    return shader;
}

GLShaderProgram* GLShaderFactory::createProgram(const GLShader** shaderList, size_t count) {
    if (!shaderList || !count) return nullptr;

    auto handle = glCreateProgram();

    auto* prog = new GLShaderProgram;
    prog->GL_handle = handle;

    for (size_t i = 0; i < count; i++) {
        const GLShader* shader = reinterpret_cast<const GLShader*>(shaderList[i]);

        if (shader)
            glAttachShader(handle, shader->GL_handle);
    }

    glLinkProgram(handle);

    programs.push_back(prog);
    return prog;
}

void GLShaderFactory::deleteShader(GLShader* shader) {
    auto shaderIt =
        std::find_if(shaders.begin(), shaders.end(), [&shader](const auto* e) { return e == shader; });

    if (shaderIt != shaders.end()) {
        delete *shaderIt;
        shaders.erase(shaderIt);
    }
}
}