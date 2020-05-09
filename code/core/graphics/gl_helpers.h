#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <glm/glm.hpp>

#include "gl_texture.h"

namespace gfx {

class GLRenderer;

struct GLRenderTarget {
    void create();
    void release();

    u32 handle;
};

class GLRenderTexture {
public:
    // new texture?
    GLRenderTexture(GLRenderer &);

    // from existing texture
    GLRenderTexture(GLTexture*);

    ~GLRenderTexture();

    u32 HACK_fbohandle() {
        return fbohandle;
    }

    void HACK_SetInternalTexture(GLTexture* newTx) {
        tex = newTx;
    }

    void resize(glm::u16vec2);
    auto size() {
        return glm::u16vec2(tex->desc.height, tex->desc.width);
    }

    inline auto* texture() { return tex; }

private:
    GLTexture* tex = nullptr;
    u32 fbohandle;
};

}