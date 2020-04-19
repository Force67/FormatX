
/*
 * VideoCore2 - Render Framework
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <vector>

#include "vk_renderer.h"
#include "vk_device.h"
#include "vk_helpers.h"

namespace video_core {

VKRenderer::VKRenderer(renderWindow& window) : renderInterface(window) {}

// gets only called when validation layers are enabled
VkBool32 VKRenderer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* data, void*) {

    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        LOG_ERROR("VK Error : {}", data->pMessage);
    else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        LOG_WARNING("VK Warning: {}", data->pMessage);
    else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        LOG_INFO("VK Info: {}", data->pMessage);
    else
        LOG_WARNING("VK Verbose: {}", data->pMessage);

    return VK_FALSE;
}

void VKRenderer::setupDebugInstance() {

}

static void test() {
    u32 deviceCount = 0;

}

bool VKRenderer::init() {

    // load VK function pointers
    if (!gladLoaderLoadVulkan(nullptr, nullptr, nullptr)) {
        LOG_ERROR("Failed to resolve VK function pointers");
        return false;
    }

    if (!GLAD_VK_VERSION_1_1) {
        LOG_ERROR("Vulkan 1.1 not available");
        shutdown();
        return false;
    }

    if (!createInstance() || !createSurface() || !createDevice()) {
        // shutdown releases all allocated objects by vulkan
        shutdown();
        return false;
    }

    swapChain = std::make_unique<VKSwapchain>(*device, surface);

    // init frame buffer
    const auto& config = window.getConfig();

    // TODO: make swapchain tests at device suitability check...
    if (!swapChain->create(config.width, config.height, true)) {
        shutdown();
        return false;
    }

    return true;
}

bool VKRenderer::setupDebugLogger() {
    return true;
}

bool VKRenderer::createInstance() {
    // apparently some systems don't have VK 1.2 libs yet...
    if (!GLAD_VK_VERSION_1_2) {
        LOG_INFO("Vulkan 1.2 not available. Fallback to Vulkan 1.1");
    }

    const u32 apiVersion = (!GLAD_VK_VERSION_1_2) ? VCORE_MIN_VK_VERSION : VK_API_VERSION_1_2;

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "PSDelta";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "VideoCore2";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = apiVersion;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    std::vector<const char*> extensions;
    std::vector<const char*> layers;

    // query essential extensions
    helpers::ExtensionCheck extcheck;
    if (!extcheck.supports(VK_KHR_SURFACE_EXTENSION_NAME) ||
#ifdef _WIN32
        !extcheck.supports(VK_KHR_WIN32_SURFACE_EXTENSION_NAME))
#elif defined(__APPLE__)
        !extcheck.supports(VK_MVK_MACOS_SURFACE_EXTENSION_NAME))
#endif
    {
        LOG_ERROR("Failed to query required VK extensions");
        return false;
    }

    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef _WIN32
    extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(__APPLE__)
    extensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#endif

    helpers::InstanceLayerCheck layercheck;

    // do we support the debug validation layer?
    if (layercheck.supports(helpers::VK_KHR_LAYER_VALIDATION)) {
        layers.push_back(helpers::VK_KHR_LAYER_VALIDATION);
        LOG_WARNING("VK Validation layer active");

        // setup the VK log callback
        // TODO: VK debug massager
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    // debug information
    VkDebugUtilsMessengerCreateInfoEXT messengerInfo{};
    messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    messengerInfo.pfnUserCallback = debugCallback;
    messengerInfo.pUserData = this;

    if (layers.size()) {
        createInfo.enabledLayerCount = static_cast<u32>(layers.size());
        createInfo.ppEnabledLayerNames = layers.data();
        createInfo.pNext = reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&messengerInfo);
    }

    createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        LOG_ERROR("Failed to create VKInstance");
        return false;
    }

    // load instance extensions
    if (!gladLoaderLoadVulkan(instance, nullptr, nullptr)) {
        LOG_ERROR("Failed to load instance extensions");
        return false;
    }

    if (GLAD_VK_EXT_debug_utils) {
        if (vkCreateDebugUtilsMessengerEXT(instance, &messengerInfo, nullptr, &debugMessenger) !=
            VK_SUCCESS) {
            LOG_ERROR("Failed to create VK Debug utils messenger");
            return false;
        }
    }

    return true;
}

void VKRenderer::logDeviceInfo(VkPhysicalDevice dev) {

    VkPhysicalDeviceProperties props{};
    vkGetPhysicalDeviceProperties(dev, &props);

    const char* vendorName;
    switch (props.vendorID) { 
    case 0x1002:
        vendorName = "AMD";
        break;
    case 0x10DE:
        vendorName = "NVIDIA";
        break;
    case 0x8086:
        vendorName = "INTEL";
        break;
    case 0x13B5:
        vendorName = "ARM";
        break;
    default:
        vendorName = "Unknown";
        break;
    }

    LOG_INFO("Selected GPU {} {}", vendorName, props.deviceName);
}

bool VKRenderer::createDevice() {
    helpers::PhysicalDeviceCheck deviceCheck(instance);

    // create the physical device
    VkPhysicalDevice physDevice = VK_NULL_HANDLE;
    deviceCheck.chooseBest(physDevice);

    if (physDevice == VK_NULL_HANDLE) {
        LOG_ERROR("Unable to find a physical adapter");
        return false;
    }

    helpers::Queue familyQueue(physDevice);
    if (!VKDevice::isSuitable(familyQueue, physDevice, surface)) {
        LOG_ERROR("Device not suitable");
        return false;
    }

    logDeviceInfo(physDevice);

    // create the logical device
    device = std::make_unique<VKDevice>(instance, physDevice, surface);
    if (device->create(familyQueue)) {
        // load device extensions (swapchain etc)
        if (!gladLoaderLoadVulkan(instance, physDevice, device->getLogicalDevice())) {
            LOG_ERROR("Failed to load device extensions");
            return false;
        }

        return true;
    }

    return false;
}

bool VKRenderer::createSurface() {
#ifdef _WIN32
    HWND windowHandle = static_cast<HWND>(window.getHandle());

    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = windowHandle;

    if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
        LOG_ERROR("Failed to create a win32 surface");
        return false;
    }

#else
#error TODO
#endif

    return true;
}

void VKRenderer::shutdown() {
    if (swapChain)
        swapChain.reset();
    if (device)
        device.reset();
    if (surface) {
        vkDestroySurfaceKHR(instance, surface, nullptr);
        surface = VK_NULL_HANDLE;
    }
    if (debugMessenger) {
        vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        debugMessenger = VK_NULL_HANDLE;
    }
    if (instance) {
        vkDestroyInstance(instance, nullptr);
        instance = VK_NULL_HANDLE;
    }

    // unload VK dll and function pointers
    gladLoaderUnloadVulkan();
}

void VKRenderer::present() {

}

}