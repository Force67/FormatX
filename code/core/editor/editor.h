#pragma once

/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <imgui.h>

#include "window.h"
#include "graphics/imgui_driver.h"

#include "sceneview.h"

namespace editor {

class FXEditor final : public graphics::ImguiDriver {
public:
    explicit FXEditor(FXWindow&);

    void init();
    void update();

    void resize(i32, i32);
private:
    void applyDefaultLayout(ImGuiID);
    void drawStatsOverlay();
    void drawLogtab();

    static void setupKeybinds();

private:
    bool showStats = true;
    bool dockFullscreen = true;

    UniquePtr<SceneView> renderView;
};
}