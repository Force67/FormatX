#pragma once

/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "video_core.h"

struct GLFWwindow;

class FXWindow final : public video_core::renderWindow {
public:
    ~FXWindow();

    void* getHandle() override;

    bool create(video_core::RenderApi);
    bool update();

    void show();

    inline GLFWwindow* HACK_getWindow() {
        return window;
    }

private:
    void setPlatformDefaultRes();

    bool requiresSwap = false;
    GLFWwindow* window;
};