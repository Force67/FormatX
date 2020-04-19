#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "gl_renderer.h"
#include "shader.h"

namespace video_core {

class GLShader final : public Shader {
    friend class GLShaderFactory;

public:
    ~GLShader();

private:
    GLuint GL_handle = 0;
};

class GLShaderProgram final : public ShaderProgram {
    friend class GLShaderFactory;

public:
    ~GLShaderProgram();

    void unlinkAllshaders();

private:
    GLuint GL_handle = 0;
};

class GLShaderFactory final : public ShaderFactory {
public:
    GLShaderFactory();
    ~GLShaderFactory();

    Shader* createFromSource(ShaderType, const char* src) override;
    ShaderProgram* createProgram(const Shader**, size_t count) override;

    // Shader type to GL
    static GLuint translateType(ShaderType);

private:
    std::vector<GLShader*> shaders;
    std::vector<GLShaderProgram*> programs;
};
} // namespace video_core