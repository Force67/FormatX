
/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <glfw/glfw3.h>
#include "editor.h"

#include "imgui_internal.h"

namespace editor {

FXEditor::FXEditor(FXWindow& window) : ImguiDriver(window) {
    renderView = std::make_unique<SceneView>();
}

void FXEditor::drawStatsOverlay() {
    auto& io = ImGui::GetIO();

    ImGui::SetNextWindowPos({io.DisplaySize.x - 10.f, 10.f}, ImGuiCond_Always, {1.f, 0.f});
    ImGui::SetNextWindowBgAlpha(0.35f); // transparent
    ImGui::Begin("window_stats", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration |
                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | 
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavFocus);

    ImGui::Text("Render Stats");
    ImGui::Separator();

    const float fps = io.Framerate;

    // MAYBE TODO: color gradient?
    ImVec4 col;
    if (fps < 60.f)
        col = {0.921f, 0.603f, 0.f, 1.f};
    else if (fps < 30.f)
        col = {1.f, 0.019f, 0.133f, 1.f};
    else
        col = {0.133f, 0.921f, 0.f, 1.f};

    ImGui::TextColored(col, "Frame time %.2f\nFPS %.1f", 1000.f / fps, fps);
    ImGui::End();
}

void FXEditor::drawLogtab() {
    ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("Log", nullptr);
    if (ImGui::SmallButton("[Debug] Add 5 entries")) {
        
    }
    ImGui::End();
}

void FXEditor::applyDefaultLayout(ImGuiID dockId) {
    //__debugbreak();
    //ImGui::DockSpace(dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    // setup root node
    ImGui::DockBuilderRemoveNode(dockId);
    ImGui::DockBuilderAddNode(dockId);

    // dockspace tracker
    ImGuiID mainId = dockId;

    ImGuiID leftSlot =
        ImGui::DockBuilderSplitNode(mainId, ImGuiDir_Up, 0.20f, nullptr, &mainId);
    ImGuiID downSlotId =
        ImGui::DockBuilderSplitNode(mainId, ImGuiDir_Down, 0.20f, nullptr, &mainId);

    ImGui::DockBuilderDockWindow("Scene View", leftSlot);
    ImGui::DockBuilderDockWindow("Log", downSlotId);
    ImGui::DockBuilderFinish(dockId);
}

void FXEditor::update() {
    // handle resizing
    ImguiDriver::poll();
    ImGui::NewFrame();

    static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (dockFullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and
    // handle the pass-thru hole, so we ask Begin() to not render a background.
    if (opt_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", (bool*)0, window_flags);
    ImGui::PopStyleVar();

    if (dockFullscreen)
        ImGui::PopStyleVar(2);

    // Dockspace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockId = ImGui::GetID("RootDockspace");

        if (!ImGui::DockBuilderGetNode(dockId))
            applyDefaultLayout(dockId);

        ImGui::DockSpace(dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    }
    else {
        // ShowDockingDisabledMessage();
    }

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File")) {
        ImGui::MenuItem("Open File", "CTRL+O");
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Tools")) {
        ImGui::MenuItem("Converter", "CTRL+T");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Options")) {
        ImGui::MenuItem("Style");
        ImGui::Separator();
        ImGui::MenuItem("Plugins");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("About")) {
        ImGui::MenuItem("Check for Updates");
        ImGui::Separator();
        ImGui::MenuItem("About FormatX");
        ImGui::EndMenu();
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::TextColored({0.615f, 0.631f, 0.705f, 1.f}, GIT_BRANCH "@" GIT_COMMIT);
    ImGui::EndMainMenuBar();

    //static bool open = true;
    //ImGui::ShowDemoWindow(&open);
 
    if (showStats)
        drawStatsOverlay();

    drawLogtab();

    ImGui::End();

    renderView->update();

    ImGui::Render();
    ImguiDriver::render();
}

void FXEditor::resize(i32 x, i32 y) {
    // notify driver
    ImguiDriver::resize(x, y);
}

void FXEditor::setupKeybinds() {
    auto& io = ImGui::GetIO();

    // setup keymap
    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
}

void FXEditor::init() {
    setupKeybinds();

    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

    // yea this is kinda hacky
    window.bindInput();

    // init style
    ImGuiStyle* style = &ImGui::GetStyle();
    style->ScaleAllSizes(FXWindow::getHDPIScale());

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