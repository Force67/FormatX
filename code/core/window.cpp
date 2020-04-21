
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

static GLFWcursor* g_MouseCursors[ImGuiMouseCursor_COUNT] = {};


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

    auto windowName = fmt::format("FormatX | Renderer: {}", name);

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

    if (requiresSwap) {
        glfwMakeContextCurrent(window);

        // vsync
        glfwSwapInterval(1);
    }

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

    // update mouse buttons
    ImGuiIO& io = ImGui::GetIO();
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) {
        io.MouseDown[i] = mouseJustPressed[i] || glfwGetMouseButton(window, i) != 0;
        mouseJustPressed[i] = false;
    }

    // update mouse pos
    const auto posBackup = io.MousePos;
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

    if (glfwGetWindowAttrib(window, GLFW_FOCUSED) != 0) {
        if (io.WantSetMousePos)
            glfwSetCursorPos(window, 
                static_cast<double>(posBackup.x),
                static_cast<double>(posBackup.y));
        else {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            io.MousePos = ImVec2(
                static_cast<float>(mouseX),
                static_cast<float>(mouseY));
        }
    
    }

    ImGuiMouseCursor imCursor = ImGui::GetMouseCursor();
    if (imCursor == ImGuiMouseCursor_None || io.MouseDrawCursor) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    } else {
        glfwSetCursor(window, g_MouseCursors[imCursor] ? g_MouseCursors[imCursor]
                                                       : g_MouseCursors[ImGuiMouseCursor_Arrow]);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

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

void FXWindow::resizeCallback(GLFWwindow* window, i32 w, i32 h) {
    // yea that is the simplest way of doing this honestly
    auto* self = reinterpret_cast<FXWindow*>(glfwGetWindowUserPointer(window));
    self->frameSize.x = w;
    self->frameSize.y = h;

    // notify core subsystems
    self->core.onViewportChange(w, h);
}

void FXWindow::bindInput() {

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, i32 button, i32 action, i32 mods) {
        auto* self = reinterpret_cast<FXWindow*>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS && button >= 0 && button < sizeof(bool) * 5)
            self->mouseJustPressed[button] = true;
    });

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += static_cast<float>(xoffset);
        io.MouseWheel += static_cast<float>(yoffset);
    });

    glfwSetKeyCallback(window, [](GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
        ImGuiIO& io = ImGui::GetIO();
        if (action == GLFW_PRESS)
            io.KeysDown[key] = true;
        if (action == GLFW_RELEASE)
            io.KeysDown[key] = false;

        // Modifiers are not reliable across systems
        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
#ifdef _WIN32
        io.KeySuper = false;
#else
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
#endif
    });

    // todo: make these tied into the window context
    glfwSetCharCallback(window, [](GLFWwindow*, u32 c){ ImGui::GetIO().AddInputCharacter(c);
    });
}