
/*
 * FormatX : Video core
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "video_core.h"

#include "dx12/dx12_backend.h"
namespace video_core {
UniquePtr<renderInterface> createRenderer(renderWindow* parent, backendKind kind) {
    if (!parent) {
        LOG_ERROR("please supply a renderWindow");
        return nullptr;
    }

    switch (kind) {
    case backendKind::vulkan:
        return nullptr;
    case backendKind::dx12:
        return std::make_unique<dx12Backend>(parent);
    case backendKind::null:
    case backendKind::opengl: {
        __debugbreak();
        return nullptr;
    }
    }

    return nullptr;
}
}