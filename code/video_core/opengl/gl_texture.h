#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "texture.h"
#include "gl_renderer.h"

namespace video_core {

class GLTexture final : public Texture {
    friend class GLTextureFactory;
public:
    ~GLTexture();

    void* handle() override;

private:
    GLuint GL_handle = 0;
};

class GLTextureFactory final : public TextureFactory {
public:
    GLTextureFactory();
    ~GLTextureFactory();

    Texture* createTexture(u16 width, u16 height, u8* pixels) override;
    void destroyTexture(Texture*);

private:
    std::vector<Texture*> textures;
};
}