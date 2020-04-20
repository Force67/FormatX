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

class GLTexture {
    friend class GLTextureFactory;
public:
    ~GLTexture();

    void* nativeHandle();

public:
    i32 width;
    i32 height;

private:
    u32 GL_handle = 0;
};

class GLTextureFactory  {
public:
    GLTextureFactory();
    ~GLTextureFactory();

    GLTexture* createTexture(u16 width, u16 height, u8* pixels);
    void destroyTexture(GLTexture*);

private:
    std::vector<GLTexture*> textures;
};
}