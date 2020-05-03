#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <array>
#include <vector>

#include "gl_renderer.h"

namespace graphics {

class GLRenderer;

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

// little helper class
template<size_t HowMany>
class GLShaderBuilder {
public:
    inline explicit GLShaderBuilder(GLRenderer& renderer) 
        : factory(*renderer.shaderFactory){};

    inline void add(ShaderType t, const char* src) {
        shaders[pos] = factory.createFromSource(t, src);
        pos++;
    }

    // validate all shaders
    inline bool good() {
        size_t count = 0;

        for (const auto* s : shaders)
            if (s)
                count++;

        return count == HowMany;
    }

    inline GLShaderProgram* finish() {
        // link the prog
        GLShaderProgram *prog = factory.createProgram(reinterpret_cast<const GLShader**>(&shaders), HowMany);
        if (!prog) return nullptr;

        // and release resources
        prog->unlinkAllshaders();

        for (auto* s : shaders)
            factory.deleteShader(s);

        return prog;
    }

private:
    GLShaderFactory &factory;
    size_t pos = 0;
    std::array<GLShader*, HowMany> shaders;
};

} // namespace video_core