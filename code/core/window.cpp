
/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "core.h"
#include "window.h"

#include <fmt/format.h>

#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>

FXWindow::FXWindow(FXCore& core) : core(core) {}

FXWindow::~FXWindow() {
    if (window)
        glfwDestroyWindow(window);

    glfwTerminate();
}

void* FXWindow::getHandle() {
#ifdef _WIN32
    return glfwGetWin32Window(window);
#endif
}

bool FXWindow::create(bool glInit) {
    if (glInit) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        requiresSwap = true;
    } else
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // create it hidden, so we can setup renderer
    // and then present the window
    glfwWindowHint(GLFW_VISIBLE, 0);

    const char* name;
    if (glInit)
        name = "OpenGL";
    else
        name = "Unknown";

    auto windowName = fmt::format("FormatX | " GIT_BRANCH "@" GIT_COMMIT " | Renderer: {}", name);

    // get optimal window size
#ifdef _WIN32
    i32 optimalWidth = 1920;// GetSystemMetrics(0);
    i32 optiomalHeight = 1080; // GetSystemMetrics(1);
#endif

    frameSize.x = optimalWidth;
    frameSize.y = optiomalHeight;

    window = glfwCreateWindow(optimalWidth, optiomalHeight, windowName.c_str(), nullptr, nullptr);
    if (!window)
        return false;

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, resizeCallback);

    if (requiresSwap)
        glfwMakeContextCurrent(window);

    return true;
}

void FXWindow::resizeCallback(GLFWwindow* window, i32 w, i32 h) {
    // yea that is the simplest way of doing this honestly
    auto* self = reinterpret_cast<FXWindow*>(glfwGetWindowUserPointer(window));
    self->frameSize.x = w;
    self->frameSize.y = h;

    // notify core subsystems
    self->core.onViewportChange(w, h);
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

float FXWindow::getHDPIScale() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(monitor, &xscale, &yscale);

    return xscale;
}