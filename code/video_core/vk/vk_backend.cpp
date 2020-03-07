
// Copyright (C) 2019-2020 Force67

#if 0
#include "vk_backend.h"

namespace video_core {
vkBackend::vkBackend(QWindow* parent) {

}

bool vkBackend::create() {
#ifdef _DEBUG
    // debug validation layers
    vkInst.setLayers(QByteArrayList() << "VK_LAYER_GOOGLE_threading"
                                      << "VK_LAYER_LUNARG_parameter_validation"
                                      << "VK_LAYER_LUNARG_object_tracker"
                                      << "VK_LAYER_LUNARG_core_validation"
                                      << "VK_LAYER_LUNARG_image"
                                      << "VK_LAYER_LUNARG_swapchain"
                                      << "VK_LAYER_GOOGLE_unique_objects");
#endif

    if (!vkInst.create())
        return false;

    return true;
}

void vkBackend::shutdown() {

}

void vkBackend::renderFrame() {

}
}
#endif