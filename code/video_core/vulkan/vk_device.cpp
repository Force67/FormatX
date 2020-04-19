
/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <array>
#include <optional>
#include <unordered_set>

#include "vk_device.h"
#include "vk_helpers.h"

namespace video_core {

static constexpr float QUEUE_PRIORITY = 1.0f;

static constexpr std::array REQUIRED_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME,
    VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME,
};

bool VKDevice::isSuitable(helpers::Queue& qc, VkPhysicalDevice physDev, VkSurfaceKHR surface) {
    // determine extension support
    helpers::DeviceExtensionCheck extCheck(physDev);

    u32 supportCount = 0;
    for (const char* ext : REQUIRED_EXTENSIONS) {
        if (extCheck.supports(ext))
            supportCount++;
        else
            LOG_ERROR("Unsupported extension {}", ext);
    }

    if (supportCount != REQUIRED_EXTENSIONS.size())
        return false;

    // determine queue support
    bool supportsGraphics = false;
    bool supportsPresent = false;

    u32 i = 0;
    for (auto& p : qc.props) {
        if (p.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            supportsGraphics = true;

        VkBool32 supportsSurface = VK_FALSE;
        if (vkGetPhysicalDeviceSurfaceSupportKHR(physDev, i, surface, &supportsSurface) ==
                VK_SUCCESS &&
            supportsSurface == VK_TRUE)
            supportsPresent = true;

        i++;
    }

    if (!supportsGraphics || !supportsPresent)
        return false;

    // TODO: viewport check
    return true;
}

VKDevice::VKDevice(VkInstance instance, VkPhysicalDevice physDev, VkSurfaceKHR surface)
    : parentInstance(instance), physDev(physDev), surface(surface) {}

VKDevice::~VKDevice() {
    if (logicDevice)
        vkDestroyDevice(logicDevice, nullptr);
}

void VKDevice::setupFamilies(helpers::Queue& queueinfo) {
    std::optional<u32> optgraphicsFamily, optpresentFamily;

    // collect indices of required graphics families
    for (u32 i = 0; i < static_cast<u32>(queueinfo.props.size()); i++) {
        const auto& p = queueinfo.props[i];

        if (p.queueCount == 0)
            continue;

        if (p.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            optgraphicsFamily = i;

        VkBool32 supportsSurface = VK_FALSE;
        if (vkGetPhysicalDeviceSurfaceSupportKHR(physDev, i, surface, &supportsSurface) ==
                VK_SUCCESS &&
            supportsSurface == VK_TRUE)
            optpresentFamily = i;
    }

    // these must be tested for already..
    LOG_ASSERT(optgraphicsFamily && optpresentFamily);

    graphicsFamily = *optgraphicsFamily;
    presentFamily = *optpresentFamily;
}

bool VKDevice::create(helpers::Queue& queueinfo) {
    setupFamilies(queueinfo);

    std::vector<VkDeviceQueueCreateInfo> queueInfos;

    // populate queue info with required families
    std::unordered_set<u32> uniqueFamilies = {graphicsFamily, presentFamily};
    for (const u32 index : uniqueFamilies) {
        auto& info = queueInfos.emplace_back();
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.pNext = nullptr;
        info.flags = 0;
        info.queueFamilyIndex = index;
        info.queueCount = 1;
        info.pQueuePriorities = &QUEUE_PRIORITY;
    }

    // create device
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.queueCreateInfoCount = static_cast<u32>(queueInfos.size());
    createInfo.pQueueCreateInfos = queueInfos.data();
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
    createInfo.enabledExtensionCount = static_cast<u32>(REQUIRED_EXTENSIONS.size());
    createInfo.ppEnabledExtensionNames = REQUIRED_EXTENSIONS.data();
    createInfo.pEnabledFeatures = nullptr;

    if (vkCreateDevice(physDev, &createInfo, nullptr, &logicDevice) == VK_SUCCESS) {
        vkGetDeviceQueue(logicDevice, graphicsFamily, 0, &graphicsQueue);
        vkGetDeviceQueue(logicDevice, presentFamily, 0, &presentQueue);
        return true;
    }

    return false;
}
}