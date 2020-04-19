#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "video_core.h"

#include "vk_device.h"
#include "vk_swapchain.h"

namespace video_core {

class VKRenderer final : public renderInterface {
public:
    VKRenderer(renderWindow&);

    // controller
    bool init() override;
    void shutdown() override;
    void present() override;

private:
    bool createInstance();
    bool setupDebugLogger();
    bool createDevice();
    bool createSurface();

    void logDeviceInfo(VkPhysicalDevice);

    VkInstance instance = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    UniquePtr<VKDevice> device;
    UniquePtr<VKSwapchain> swapChain;

    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
    void setupDebugInstance();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT, 
            VkDebugUtilsMessageTypeFlagsEXT,
            const VkDebugUtilsMessengerCallbackDataEXT*, void*);
};
}