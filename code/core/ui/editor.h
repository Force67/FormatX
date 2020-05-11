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
#include "graphics/gl_helpers.h"

#include "logs.h"

namespace ui {

class Editor final : public gfx::ImguiDriver {
public:
    explicit Editor(FXWindow&, /*HACK*/ gfx::GLRenderer&);

    bool init();
    void frame(gfx::GLRenderTexture&);

    void resize(i32, i32);
private:
    void applyDefaultLayout(ImGuiID);
    void updateDockLayout();

    void updateMenu();
    void helpAbout();

    void updateStats();
    void updateViewer(gfx::GLRenderTexture&);

private:
    ImVec2 center();

    gfx::GLRenderer& rend;

    bool showLog = true;
    bool showStats = true;
    bool showViewer = true;
    bool showAbout = false;

    bool dockFullscreen = true;

    ImVec2 lastSize{};
    bool allowShaders = true;
    bool allowLightning = true;

    UniquePtr<LogWidget> logwidget;
};
}