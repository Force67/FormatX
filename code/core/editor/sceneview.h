#pragma once

/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <imgui.h>

#include "graphics/imgui_driver.h"
#include "window.h"

namespace editor {

class SceneView {
public:
    SceneView();

    void update();

private:
    bool allowShaders = true;
    bool allowLightning = true;
};
} // namespace ui