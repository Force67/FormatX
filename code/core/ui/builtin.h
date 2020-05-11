#pragma once

/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

// This file contains "built in" data
// for the ui

#include <imgui.h>

#include "_res/brand_icons_fontawesome_5.h"
#include "_res/icons_fontawesome_5.h"

namespace ui::text {

constexpr char kaboutInfo[] = u8R"(
                                        
          Source: [github](https://github.com/Force67/FormatX)

# Implemented with:

* Dear ImGui: [https://github.com/ocornut/imgui](https://github.com/ocornut/imgui)
* ImGuiColorTextEdit: [https://github.com/BalazsJako/ImGuiColorTextEdit](https://github.com/BalazsJako/ImGuiColorTextEdit)
* ImGuiMarkdown: [https://github.com/juliettef/imgui_markdown](https://github.com/juliettef/imgui_markdown)
* IconFontCppHeaders: [https://github.com/juliettef/IconFontCppHeaders](https://github.com/juliettef/IconFontCppHeaders)
* Font Awesome 4: [https://github.com/FortAwesome/Font-Awesome/tree/fa-4](https://github.com/FortAwesome/Font-Awesome/tree/fa-4)
)";
}

namespace ui::theme {

inline void applyStyle(ImGuiStyle *style) {
    ImVec4* colors = style->Colors;

    style->WindowRounding =
        2.0f; // Radius of window corners rounding. Set to 0.0f to have rectangular windows
    style->ScrollbarRounding = 3.0f; // Radius of grab corners rounding for scrollbar
    style->GrabRounding =
        2.0f; // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    style->AntiAliasedLines = true;
    style->AntiAliasedFill = true;
    style->WindowRounding = 2;
    style->ChildRounding = 2;
    style->ScrollbarSize = 16;
    style->ScrollbarRounding = 3;
    style->GrabRounding = 2;
    style->ItemSpacing.x = 10;
    style->ItemSpacing.y = 4;
    style->IndentSpacing = 22;
    style->FramePadding.x = 6;
    style->FramePadding.y = 4;
    style->Alpha = 1.0f;
    style->FrameRounding = 3.0f;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.27f, 0.27f, 0.28f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.27f, 0.27f, 0.28f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.27f, 0.27f, 0.28f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.61f, 0.35f, 0.71f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.61f, 0.35f, 0.71f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.27f, 0.27f, 0.28f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.27f, 0.27f, 0.28f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.27f, 0.27f, 0.28f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.27f, 0.27f, 0.28f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.27f, 0.27f, 0.28f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.36f, 0.38f, 0.39f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.36f, 0.38f, 0.39f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.56f, 0.27f, 0.68f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.56f, 0.27f, 0.68f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.27f, 0.68f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.36f, 0.38f, 0.39f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.61f, 0.35f, 0.71f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.27f, 0.68f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.27f, 0.27f, 0.28f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.27f, 0.27f, 0.28f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.27f, 0.27f, 0.28f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.61f, 0.35f, 0.71f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.27f, 0.68f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f, 0.38f, 0.39f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.61f, 0.35f, 0.71f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.56f, 0.27f, 0.68f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.60f, 0.36f, 0.76f, 0.40f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.61f, 0.35f, 0.71f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.56f, 0.27f, 0.68f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);

    style->FrameRounding = 3;
}
}