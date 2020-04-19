
/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

// For OpenGL and Vulkan, configure glad like this:
// http://gen.glad.sh/#profile=gl%3Dcore%2Cgles1%3Dcommon&api=gl%3D4.6%2Cvulkan%3D1.2&extensions=GL_AMD_debug_output%2CGL_ARB_debug_output%2CGL_EXT_debug_label%2CGL_EXT_debug_marker%2CGL_KHR_debug%2CVK_EXT_debug_marker%2CVK_EXT_debug_report%2CVK_EXT_debug_utils%2CVK_EXT_swapchain_colorspace%2CVK_KHR_display_swapchain%2CVK_KHR_surface%2CVK_KHR_swapchain%2CVK_KHR_swapchain_mutable_format%2CVK_KHR_win32_surface&options=ALIAS%2CALIAS%2CLOADER&generator=c

#include "video_core.h"

#include "opengl/gl_renderer.h"
#include "vulkan/vk_renderer.h"

#include <utl/logger/logger.h>

namespace video_core 
{
static RenderCallbacks g_callbacks{ malloc, free };

UniquePtr<renderInterface> createRenderer(renderWindow& parent, const RenderInstanceDesc& desc) {

    // TEMP LOG HACK
    utl::createLogger(true);
    LOG_WARNING("LOG HACK FOR VIDEO CORE ACTIVE");

    // custom allocator support
    if (desc.callbacks.malloc)
        g_callbacks.malloc = desc.callbacks.malloc;
    if (desc.callbacks.free)
        g_callbacks.free = desc.callbacks.free;

    // create renderer
    switch (desc.apiType) {
    case RenderApi::dx11:
    case RenderApi::dx12:
        LOG_ERROR("Support for DX11/12 has been removed in the 04/2020 vcore update.");
        return nullptr;
    case RenderApi::vulkan:
        return std::make_unique<VKRenderer>(parent);
    case RenderApi::opengl: {
        return std::make_unique<GLRenderer>(parent);
    }
    default:
        __debugbreak();
        return nullptr;
    }

    return nullptr;
}

void* v_malloc(size_t size) {
    return g_callbacks.malloc(size);
}

void v_free(void* ptr) {
    return g_callbacks.free(ptr);
}
}