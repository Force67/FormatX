#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "vk_config.h"
#include "video_core.h"
#include "vk_helpers.h"

namespace video_core {

class VKDevice {
public:
    static bool isSuitable(helpers::Queue&, VkPhysicalDevice, VkSurfaceKHR);

    explicit VKDevice(VkInstance, VkPhysicalDevice, VkSurfaceKHR);
    ~VKDevice();

    bool create(helpers::Queue&);

    inline auto getPhysicalDevice() const { return physDev; }
    inline auto getLogicalDevice() const { return logicDevice; }

    inline auto getFamilies() {
        return std::pair(graphicsFamily, presentFamily);
    }

private:
    void setupFamilies(helpers::Queue&);

    VkInstance parentInstance;
    VkPhysicalDevice physDev;
    VkDevice logicDevice = VK_NULL_HANDLE;
    VkSurfaceKHR surface;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    u32 graphicsFamily = 0;
    u32 presentFamily = 0;
};
}