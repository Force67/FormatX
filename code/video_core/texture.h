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

// texture base type
class Texture {
public:
    virtual void* handle() = 0;

public:
    u16 width;
    u16 height;
    u16 depth;
};

// objects that are not explicitly destroyed
// will be released on program exit
class TextureFactory {
public:
    virtual Texture* createTexture(u16 width, u16 height, u8* pixels) = 0;
    virtual void destroyTexture(Texture*) = 0;
};
}