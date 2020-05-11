
/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "gl_texture.h"
#include "glad/gl.h"

#include <logger/logger.h>

namespace gfx {

GLTexture::~GLTexture() {
    if (GL_handle)
        glDeleteTextures(1, &GL_handle);
}

void* GLTexture::nativeHandle() {
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
        delete tex;
    }
}

GLTexture* GLTextureFactory::createTexture(const TextureDesc& desc, u8* pixels /*its LEGAL for "pixels" to be null*/) {

    i32 glFormat = 0;
    switch (desc.colorFormat) {
    case ColorFormat::RED:
        glFormat = GL_RED;
        break;
    case ColorFormat::RGB:
        glFormat = GL_RGB;
        break;
    case ColorFormat::RGBA:
        glFormat = GL_RGBA;
        break;
    case ColorFormat::SRGB:
        glFormat = GL_SRGB;
        break;
    default:
        BUGCHECK();
        return nullptr;
    }

    GLTexture* tex = new GLTexture;
    tex->desc = desc;

    // backup the last texture
    GLint lastTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);

    // upload new texture pixels
    glGenTextures(1, &tex->GL_handle);
    glBindTexture(GL_TEXTURE_2D, tex->GL_handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, glFormat, 
        static_cast<GLsizei>(desc.width), 
        static_cast<GLsizei>(desc.height), 0,
                 glFormat, GL_UNSIGNED_BYTE,
                 pixels);

    // restore texture
    glBindTexture(GL_TEXTURE_2D, lastTexture);
    textures.push_back(tex);
    return tex;
}

void GLTextureFactory::destroyTexture(GLTexture* tex) {
    auto texIt = std::find_if(textures.begin(), textures.end(), [&tex](const auto* e) { 
        return e == tex; 
    });

    if (texIt != textures.end()) {
        delete *texIt;
        textures.erase(texIt);
    }
}
}