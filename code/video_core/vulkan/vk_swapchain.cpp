
/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "vk_swapchain.h"

namespace video_core {

static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> formats, bool srgb) {
    if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED) {
        VkSurfaceFormatKHR format;
        format.format = VK_FORMAT_B8G8R8A8_UNORM;
        format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        return format;
    }
    const auto& found = std::find_if(formats.begin(), formats.end(), [srgb](const auto& format) {
        const auto request_format = srgb ? VK_FORMAT_B8G8R8A8_SRGB : VK_FORMAT_B8G8R8A8_UNORM;
        return format.format == request_format &&
               format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    });
    return found != formats.end() ? *found : formats[0];
}

static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &modes) {
    // Mailbox doesn't lock the application like fifo (vsync), prefer it
    const auto found = std::find(modes.begin(), modes.end(), VK_PRESENT_MODE_MAILBOX_KHR);
    return found != modes.end() ? *found : VK_PRESENT_MODE_FIFO_KHR;
}

// choose resolution of swapchain images
static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& caps, u32 width, u32 height) {
    constexpr auto undefined_size{std::numeric_limits<u32>::max()};
    if (caps.currentExtent.width != undefined_size) {
        return caps.currentExtent;
    }

    const u32 extWidth = std::clamp(width, caps.minImageExtent.width, caps.maxImageExtent.width);
    const u32 extHeight = std::clamp(height, caps.minImageExtent.height, caps.maxImageExtent.height);

    return {extWidth, extHeight};
}

VKSwapchain::VKSwapchain(VKDevice& dev, VkSurfaceKHR surface) : device(dev), surface(surface) {}

VKSwapchain::~VKSwapchain() {
    for (auto imageView : imageViews)
        vkDestroyImageView(device.getLogicalDevice(), imageView, nullptr);

    if (swapChain != VK_NULL_HANDLE)
        vkDestroySwapchainKHR(device.getLogicalDevice(), swapChain, nullptr);
}

bool VKSwapchain::createImageViews() {

    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swapChainImageFormat;
    createInfo.components = {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
                     VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY};
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.layerCount = 1;

    imageViews.resize(images.size());
    for (size_t i = 0; i < images.size(); i++) {
        createInfo.image = images[i];
        
        if (vkCreateImageView(device.getLogicalDevice(), &createInfo, nullptr, &imageViews[i]) !=
            VK_SUCCESS) {
            LOG_ERROR("Failed to create image view {}", i);
            return false;
        }
    }

    return true;
}

bool VKSwapchain::create(i32 width, i32 height, bool srgb) {
    VkSurfaceCapabilitiesKHR caps;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.getPhysicalDevice(), surface, &caps);
    if (caps.maxImageExtent.width == 0 || caps.maxImageExtent.height == 0) {
        return false;
    }

    u32 imageCount = caps.minImageCount + 1;
    if (caps.maxImageCount > 0 &&
        imageCount > caps.maxImageCount) {
        imageCount = caps.maxImageCount;
    }

    helpers::DeviceSurfaceFormatsCheck surfaceFormats(device.getPhysicalDevice(), surface);
    helpers::DevicePresentModesCheck presentModes(device.getPhysicalDevice(), surface);

    if (surfaceFormats.formats.empty() || presentModes.modes.empty())
        return false;

    auto surfaceFormat = ChooseSwapSurfaceFormat(surfaceFormats.formats, srgb);
    auto presentMode = ChooseSwapPresentMode(presentModes.modes);
    swapChainExtent = ChooseSwapExtent(caps, static_cast<u32>(width), static_cast<u32>(height));

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = swapChainExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.preTransform = caps.currentTransform;                // < disable transform
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;  // < ignore alpha channel
    createInfo.presentMode = presentMode;                           // < mailbox or vsync
    createInfo.clipped = VK_TRUE;                                   // < ignore obscured pixels
    createInfo.oldSwapchain = VK_NULL_HANDLE;   // HACK: assume only one swapchain for now

    // use concurrent mode to avoid issues with queues
    const auto indices = device.getFamilies();
    u32 qxxx[] = {indices.first, indices.second};

    // TODO: abandon this concept
    // use only EXCLUSIVE mode
    if (indices.first != indices.second) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = qxxx;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    if (vkCreateSwapchainKHR(device.getLogicalDevice(), &createInfo, nullptr, &swapChain) !=
        VK_SUCCESS) {
        LOG_ERROR("Failed to create swapchain");
        return false;
    }

    swapChainImageFormat = surfaceFormat.format;

    vkGetSwapchainImagesKHR(device.getLogicalDevice(), swapChain, &imageCount, nullptr);
    images.resize(imageCount);
    vkGetSwapchainImagesKHR(device.getLogicalDevice(), swapChain, &imageCount, images.data());

    return createImageViews();
}
}