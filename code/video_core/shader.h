#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>

namespace video_core {

enum class ShaderType {
    Error,
    Fragment,
    Vertex
};

class Shader {
public:
};

class ShaderProgram {

};

// objects that are not explicitly destroyed
// will be released on program exit
class ShaderFactory {
public:
    virtual Shader* createFromSource(ShaderType, const char *src) = 0;
    virtual ShaderProgram* createProgram(const Shader** /*list of shaders*/, size_t count) = 0;
};

} // namespace video_core