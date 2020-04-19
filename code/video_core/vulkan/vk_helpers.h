#pragma once

/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <vector>
#include "vk_config.h"
#include "video_core.h"

namespace video_core::helpers {

// name aliases
constexpr const char* VK_KHR_LAYER_VALIDATION = "VK_LAYER_KHRONOS_validation";

struct ExtensionCheck {
    ExtensionCheck();

    bool supports(const char* ext);
private:
    std::vector<VkExtensionProperties> props;
};

struct InstanceLayerCheck {
    InstanceLayerCheck();

    bool supports(const char* layer);
private:
    std::vector<VkLayerProperties> props;
};

struct PhysicalDeviceCheck {
    PhysicalDeviceCheck(VkInstance);

    bool chooseBest(VkPhysicalDevice &);
private:
    std::vector<VkPhysicalDevice> devices;
};

struct Queue {
    Queue(VkPhysicalDevice);

    bool supports(VkQueueFlags);

public:
    std::vector<VkQueueFamilyProperties> props;
};

struct DeviceExtensionCheck {
    DeviceExtensionCheck(VkPhysicalDevice);

    bool supports(const char* ext);

    std::vector<VkExtensionProperties> props;
};

struct DeviceSurfaceFormatsCheck {
    DeviceSurfaceFormatsCheck(VkPhysicalDevice, VkSurfaceKHR);

    std::vector<VkSurfaceFormatKHR> formats;
};

struct DevicePresentModesCheck {
    DevicePresentModesCheck(VkPhysicalDevice, VkSurfaceKHR);
    
    std::vector<VkPresentModeKHR> modes;
};
} // namespace video_core