
/*
 * FormatX : Video core
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "core.h"
#include "video_core.h"

namespace core {
fxcore::fxcore() {
}

fxcore::~fxcore() {
}

result fxcore::init(video_core::renderWindow& renderTo) {
    // TODO: determine backend based on config
    const auto backendType = video_core::backendKind::dx12;

    renderer = video_core::createRenderer(renderTo, backendType);

    const char* name;
    switch (backendType) {
    case video_core::backendKind::dx12:
        name = "DirectX 12";
        break;
    case video_core::backendKind::opengl:
        name = "OpenGL";
        break;
    case video_core::backendKind::vulkan:
        name = "Vulkan";
        break;
    case video_core::backendKind::null:
    default:
        name = "Null";
        break;
    }

    // init the renderer
    if (renderer->create()) {
        LOG_INFO("Started with {} renderer", name);
    } else
        __debugbreak();

    return result::success;
}
}