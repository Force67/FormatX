#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <utl/path.h>

namespace gfx {

enum class ColorFormat {
    RED,
    RGB,
    RGBA,
    SRGB,
    RGBA8,
};

enum class ShaderType {
    Fragment,
    Vertex,
    Pixel
};

struct TextureDesc {
    ColorFormat colorFormat;
    u16 height, 
        width;
};

struct Texture {
    TextureDesc desc;
};

Texture* loadTexture(std::string_view path_Utf8);
}