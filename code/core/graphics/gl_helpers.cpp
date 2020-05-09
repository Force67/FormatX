
/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "gl_helpers.h"
#include "glad/gl.h"

#include "graphics/gl_renderer.h"

namespace gfx {

void GLRenderTarget::create() {
    glGenFramebuffers(1, &handle);
}

void GLRenderTarget::release() {
    glDeleteFramebuffers(1, &handle);
}

GLRenderTexture::GLRenderTexture(GLRenderer &render) {
    glGenFramebuffers(1, &fbohandle);
    //glBindFramebuffer(GL_FRAMEBUFFER, fbohandle);

    // create an empty texture.
    TextureDesc desc{};
    desc.colorFormat = ColorFormat::RGBA;

    tex = render.textureFactory->createTexture(desc, nullptr);

   // glFramebufferTexture2D(GL_FRAMEBUFFER, );
}

GLRenderTexture::GLRenderTexture(GLTexture* tex) : tex(tex) {
    glGenFramebuffers(1, &fbohandle);
}

GLRenderTexture::~GLRenderTexture() {
    glDeleteFramebuffers(1, &fbohandle);
}

void GLRenderTexture::resize(glm::u16vec2 newSz) {
    tex->desc.height = newSz.x;
    tex->desc.width = newSz.y;
}
}