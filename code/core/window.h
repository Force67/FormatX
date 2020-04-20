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

class FXWindow {
public:
    ~FXWindow();

    void* getHandle();

    bool create(bool glInit);
    bool update();

    void show();

    inline GLFWwindow* HACK_getWindow() {
        return window;
    }

    inline glm::i32vec2& size() {
        return frameSize;
    }

private:
    glm::i32vec2 frameSize;

    static void resizeCallback(GLFWwindow* window, i32, i32);

    bool requiresSwap = false;
    GLFWwindow* window;
};