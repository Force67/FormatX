
/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <vector>
#include <map>

#include "vk_helpers.h"

namespace video_core::helpers {

ExtensionCheck::ExtensionCheck() {
    u32 propCount = 0;
    VkResult res;
    do {
        // count available extensions
        res = vkEnumerateInstanceExtensionProperties(nullptr, &propCount, nullptr);
        if (res == VK_SUCCESS && propCount) {
            props.resize(propCount);
            res = vkEnumerateInstanceExtensionProperties(
                       nullptr, &propCount,
                       reinterpret_cast<VkExtensionProperties*>(props.data()));
        }
    } while (res == VK_INCOMPLETE);
    if (res == VK_SUCCESS) {
        props.resize(propCount);
    }
}

bool ExtensionCheck::supports(const char* ext) {
    return std::any_of(props.cbegin(), props.cend(), [&](const VkExtensionProperties& p) {
        return std::strcmp(p.extensionName, ext) == 0;
    });
}

InstanceLayerCheck::InstanceLayerCheck() {
    u32 propCount = 0;
    VkResult res;
    do {
        // count available extensions
        res = vkEnumerateInstanceLayerProperties(&propCount, nullptr);
        if (res == VK_SUCCESS && propCount) {
            props.resize(propCount);
            res = vkEnumerateInstanceLayerProperties(
                &propCount, reinterpret_cast<VkLayerProperties*>(props.data()));
        }
    } while (res == VK_INCOMPLETE);
    if (res == VK_SUCCESS) {
        props.resize(propCount);
    }
}

bool InstanceLayerCheck::supports(const char* layer) {
    return std::any_of(props.cbegin(), props.cend(), [&](const VkLayerProperties& p) {
        return std::strcmp(p.layerName, layer) == 0;
    });
}

PhysicalDeviceCheck::PhysicalDeviceCheck(VkInstance instance) {
    u32 physDeviceCount = 0;
    VkResult res;
    do {
        // count available extensions
        res = vkEnumeratePhysicalDevices(instance, &physDeviceCount, nullptr);
        if (res == VK_SUCCESS && physDeviceCount) {
            devices.resize(physDeviceCount);
            res = vkEnumeratePhysicalDevices(instance,
                &physDeviceCount, reinterpret_cast<VkPhysicalDevice*>(devices.data()));
        }
    } while (res == VK_INCOMPLETE);
    if (res == VK_SUCCESS) {
        devices.resize(physDeviceCount);
    }
}

bool PhysicalDeviceCheck::chooseBest(VkPhysicalDevice& devOut) {
    std::multimap<i32, VkPhysicalDevice> canidates;

    for (const auto& dev : devices) {
        // rate the gpu (choose dedicated gpu over integrated,
        // but allow fallback to worse gpu)
        VkPhysicalDeviceProperties props{};
        vkGetPhysicalDeviceProperties(dev, &props);

        VkPhysicalDeviceFeatures features{};
        vkGetPhysicalDeviceFeatures(dev, &features);

        if (!features.geometryShader)
            continue;

        i32 score = 0;

        // should be dedicated
        if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            score += 1000;
    
        // bonus for greater cache size
        score += props.limits.maxImageDimension2D;

        // insert in ordered map
        canidates.insert({score, dev});
    }

    if (canidates.size() > 0) {
        // can it be used at all?
        if (canidates.rbegin()->first > 0) {
            devOut = canidates.rbegin()->second;
            return true;
        }
    }

    devOut = VK_NULL_HANDLE;
    return false;
}

Queue::Queue(VkPhysicalDevice dev) {
    u32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueFamilyCount, nullptr);
    props.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        dev, &queueFamilyCount, reinterpret_cast<VkQueueFamilyProperties*>(props.data()));
}

bool Queue::supports(VkQueueFlags flag) {
    return std::any_of(props.cbegin(), props.cend(), [&](const VkQueueFamilyProperties& p) {
        return p.queueFlags & flag;
    });
}

DeviceExtensionCheck::DeviceExtensionCheck(VkPhysicalDevice dev) {
    u32 propCount = 0;
    VkResult res;
    do {
        // count available extensions
        res = vkEnumerateDeviceExtensionProperties(dev, nullptr, &propCount, nullptr);
        if (res == VK_SUCCESS && propCount) {
            props.resize(propCount);
            res = vkEnumerateDeviceExtensionProperties(
                dev, nullptr, &propCount, reinterpret_cast<VkExtensionProperties*>(props.data()));
        }
    } while (res == VK_INCOMPLETE);
}

bool DeviceExtensionCheck::supports(const char* ext) {
    return std::any_of(props.cbegin(), props.cend(), [&](const VkExtensionProperties& p) {
        return std::strcmp(p.extensionName, ext) == 0;
    });
}

DeviceSurfaceFormatsCheck::DeviceSurfaceFormatsCheck(VkPhysicalDevice dev, VkSurfaceKHR surface) {
    u32 formatCount = 0;
    VkResult res;
    do {
        // count available extensions
        res = vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &formatCount, nullptr);
        if (res == VK_SUCCESS && formatCount) {
            formats.resize(formatCount);
            res = vkGetPhysicalDeviceSurfaceFormatsKHR(
                dev, surface, &formatCount, reinterpret_cast<VkSurfaceFormatKHR*>(formats.data()));
        }
    } while (res == VK_INCOMPLETE);
    if (res == VK_SUCCESS) {
        formats.resize(formatCount);
    }
}

DevicePresentModesCheck::DevicePresentModesCheck(VkPhysicalDevice dev, VkSurfaceKHR surface) {
    u32 modeCount = 0;
    VkResult res;
    do {
        // count available extensions
        res = vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &modeCount, nullptr);
        if (res == VK_SUCCESS && modeCount) {
            modes.resize(modeCount);
            res = vkGetPhysicalDeviceSurfacePresentModesKHR(
                dev, surface, &modeCount, reinterpret_cast<VkPresentModeKHR*>(modes.data()));
        }
    } while (res == VK_INCOMPLETE);
    if (res == VK_SUCCESS) {
        modes.resize(modeCount);
    }
}
}