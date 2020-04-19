
/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "window.h"

#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>

FXWindow::~FXWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void* FXWindow::getHandle() {
#ifdef _WIN32
    return glfwGetWin32Window(window);
#endif
}

void FXWindow::setPlatformDefaultRes() {
#ifdef _WIN32
    config.width = GetSystemMetrics(0);
    config.height = GetSystemMetrics(1);
    // temporary:
    config.fullScreen = false;
#else
#error fix me
#endif
}

bool FXWindow::create(video_core::RenderApi api) {
    // setup the proper context for the video api
    switch (api) {
    case video_core::RenderApi::dx11:
    case video_core::RenderApi::dx12:
    case video_core::RenderApi::vulkan:
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        break;
    case video_core::RenderApi::opengl: {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        requiresSwap = true;
        break;
    default: // unsupported
        LOG_ERROR("Unsupported graphics api requested ({})", static_cast<i32>(api));
        return false;
    }
    }

    // create it hidden, so we can setup renderer
    // and then present the window
    glfwWindowHint(GLFW_VISIBLE, 0);

    const char* name;
    switch (api) {
    case video_core::RenderApi::opengl:
        name = "OpenGL";
        break;
    case video_core::RenderApi::vulkan:
        name = "Vulkan";
        break;
    case video_core::RenderApi::null:
    default:
        name = "Null";
        break;
    }

    setPlatformDefaultRes();

    auto windowName = fmt::format("FormatX | " GIT_BRANCH "@" GIT_COMMIT " | Renderer: {}", name);

    window = glfwCreateWindow(config.width, config.height, windowName.c_str(), nullptr, nullptr);
    if (!window) {
        LOG_ERROR("Failed to create window");
        return false;
    }

    glfwSetWindowUserPointer(window, this);

    if (requiresSwap)
        glfwMakeContextCurrent(window);

    return true;
}

void FXWindow::show() {
    glfwShowWindow(window);
}

bool FXWindow::update() {
    // user requested window close
    if (glfwWindowShouldClose(window))
        return false;

    // update input, check for window resize, etc
    glfwPollEvents();

    // OpenGL requires swapping of front and back buffer
    if (requiresSwap)
        glfwSwapBuffers(window);

    return true;
}