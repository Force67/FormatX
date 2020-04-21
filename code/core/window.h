#pragma once

/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <glm/vec2.hpp>

struct GLFWwindow;
class FXCore;

class FXWindow {
public:
    FXWindow(FXCore&);
    ~FXWindow();

    void* getHandle();

    bool create(bool glInit);
    bool update();

    void show();

    inline GLFWwindow* getWindowImp() {
        return window;
    }

    inline glm::i32vec2& size() {
        return frameSize;
    }

    static float getHDPIScale();

    void bindInput();
private:
    glm::i32vec2 frameSize;
    bool mouseJustPressed[5] = {false, false, false, false, false};

private:
    static void resizeCallback(GLFWwindow*, i32, i32);

    bool requiresSwap = false;
    GLFWwindow* window;
    FXCore& core;
};