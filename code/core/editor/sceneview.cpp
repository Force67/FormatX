
/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "sceneview.h"

namespace editor {

SceneView::SceneView() {}

void SceneView::update() {

    // due to the docking system the window will allready get asigned a size; we render at that size.
    ImGui::Begin("Scene View");
    ImGui::Image((void*)ImGui::GetIO().Fonts->TexID, ImGui::GetWindowSize(), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
    ImGui::End();
}
}