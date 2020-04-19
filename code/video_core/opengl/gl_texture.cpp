
/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "gl_texture.h"

namespace video_core {

GLTexture::~GLTexture() {
    if (GL_handle)
        glDeleteTextures(1, &GL_handle);
}

void* GLTexture::handle() {
    // avoid pointer size warning
    // by static_cast'ing
    return reinterpret_cast<void*>(static_cast<size_t>(GL_handle));
}

GLTextureFactory::GLTextureFactory() {
    // TODO: reserve a huge pool beforehand
    // so we don't have to do any allocations
}

GLTextureFactory::~GLTextureFactory() {
    // ensure that all texture memory
    // is released
    for (auto* tex : textures) {
        VFree(tex);
    }
}

Texture* GLTextureFactory::createTexture(u16 width, u16 height, u8* pixels) {

    GLTexture* tex = VAlloc<GLTexture>();
    tex->width = width;
    tex->height = height;

    // backup the last texture
    GLint lastTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);

    // upload new texture pixels
    glGenTextures(1, &tex->GL_handle);
    glBindTexture(GL_TEXTURE_2D, tex->GL_handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // restore texture
    glBindTexture(GL_TEXTURE_2D, lastTexture);
    
    textures.push_back(tex);
    return tex;
}

void GLTextureFactory::destroyTexture(Texture* tex) {
    auto texIt = std::find_if(textures.begin(), textures.end(), [&tex](const auto* e) { 
        return e == tex; 
    });

    if (texIt != textures.end()) {
        VFree((*texIt));
        textures.erase(texIt);
    }
}
}