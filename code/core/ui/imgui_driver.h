#pragma once

/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>

#include <imgui.h>
#include <imgui_internal.h>

#include "window.h"

namespace video_core {
class renderInterface;
class ShaderProgram;
}

namespace ui {

class ImguiDriver {
public:
    ImguiDriver(FXWindow&);
    ~ImguiDriver();

    bool create(video_core::renderInterface&);

protected:
    void render();

private:
    void setKeybindings(ImGuiIO&);
    void setupRenderstate(const ImDrawList*);

    ImGuiContext* ctx = nullptr;
    double timestamp = 0.0;

    // transforms
    video_core::ShaderProgram* imageProgram = nullptr;

protected:
    FXWindow& window;
};
}