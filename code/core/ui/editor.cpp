
/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "editor.h"

namespace ui {

FXEditor::FXEditor(FXWindow& window) : ImguiDriver(window) {
}

void FXEditor::update() {

    ImGui::NewFrame();

    static bool show_another_window = false;
    ImGui::Begin("Another Window",
                 &show_another_window); // Pass a pointer to our bool variable (the window will have
                                        // a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
        show_another_window = false;
    ImGui::End();

    ImGui::Render();

    //submit data
    ImguiDriver::render();
}
}