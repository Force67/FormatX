
// Copyright (C) 2019-2020 Force67

#include "rend.h"

#include "dx12/dx12_backend.h"
#include "vk/vk_rend.h"

namespace rend {
std::unique_ptr<renderInterface> createRenderer(QWindow* parent, backendKind kind) {
    switch (kind) {
    case backendKind::vulkan:
        return std::make_unique<vkBackend>(parent);
        break;
    case backendKind::dx12:
        return std::make_unique<dx12Backend>(parent);
        break;
    case backendKind::null:
    case backendKind::opengl: {
        __debugbreak();
        return nullptr;
    }
    }

    return nullptr;
}
}