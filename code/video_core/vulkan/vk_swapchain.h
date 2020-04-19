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

#include "vk_device.h"

namespace video_core {

class VKSwapchain {
public:
    VKSwapchain(VKDevice&, VkSurfaceKHR surface);
    ~VKSwapchain();

    bool create(i32 width, i32 height, bool srgb);

private:
    bool createImageViews();

    VKDevice& device;
    VkSurfaceKHR surface;

    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
};
}