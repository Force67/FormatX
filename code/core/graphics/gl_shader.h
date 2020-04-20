#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <vector>

namespace graphics {

enum class ShaderType { Error, Fragment, Vertex, Pixel };


class GLShader {
    friend class GLShaderFactory;

public:
    ~GLShader();

public:
    u32 GL_handle = 0;
};

class GLShaderProgram final  {
    friend class GLShaderFactory;

public:
    ~GLShaderProgram();

    void use();
    void unlinkAllshaders();

    i32 getUniform(const char* name);
    i32 getAttrib(const char* name);

public:
    u32 GL_handle = 0;
};

class GLShaderFactory {
public:
    GLShaderFactory();
    ~GLShaderFactory();

    GLShader* createFromSource(ShaderType, const char* src);
    GLShaderProgram* createProgram(const GLShader**, size_t count);

    void deleteShader(GLShader*);

    // Shader type to GL
    static u32 translateType(ShaderType);

private:
    std::vector<GLShader*> shaders;
    std::vector<GLShaderProgram*> programs;
};
} // namespace video_core