#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <vector>

#include "gfxresource.h"

namespace gfx {

class GLTexture : public Texture {
    friend class GLTextureFactory;
public:
    ~GLTexture();

    void* nativeHandle();

public:
    u32 GL_handle = 0;
};

class GLTextureFactory  {
public:
    GLTextureFactory();
    ~GLTextureFactory();

    GLTexture* createTexture(const TextureDesc&, u8* pixels);
    void destroyTexture(GLTexture*);

private:
    std::vector<GLTexture*> textures;
};
}