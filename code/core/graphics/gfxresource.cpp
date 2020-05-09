
/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "gfxresource.h"

#include "gl_renderer.h"
#include "gl_texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

namespace gfx {

Texture* loadTexture(std::string_view path_Utf8) {

    i32 width, height, nrComponents;
    u8* data = stbi_load(path_Utf8.data(), &width, &height, &nrComponents, 0);
    if (!data) {
        LOG_ERROR("Failed to load texture from path {}", path_Utf8);

        stbi_image_free(data);
        return nullptr;
    }

    ColorFormat format;
    switch (nrComponents) { 
    case 1:
        format = ColorFormat::RED;
        break;
    case 3:
        format = ColorFormat::RGB;
        break;
    case 4:
        format = ColorFormat::RGBA;
        break;
    default:
        BUGCHECK();
        return nullptr;
    }

    TextureDesc desc;
    desc.colorFormat = format;
    desc.height = static_cast<u16>(height);
    desc.width = static_cast<u16>(width);

    auto* texture = GetRenderer()->textureFactory->createTexture(desc, data);
    if (texture) {
        stbi_image_free(data);
        return texture;
    }

    return nullptr;
}
}