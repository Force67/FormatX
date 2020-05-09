
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
#include <imgui_markdown.h>

#include "builtin.h"
#include "elements.h"

#include "graphics/gl_texture.h"
#include "graphics/gl_helpers.h"

namespace ui {

// constants
constexpr auto kMaxUrlSize = 128;

static ImGui::MarkdownConfig md_conf;

Editor::Editor(FXWindow& window, gfx::GLRenderer& renderer) : ImguiDriver(window), rend(renderer) {
    logwidget = std::make_unique<LogWidget>();
}

void Editor::updateMenu() {
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
        ImGui::MenuItem("About " PRJ_NAME, nullptr, &showAbout);
        ImGui::EndMenu();
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::TextColored({0.615f, 0.631f, 0.705f, 1.f}, GIT_BRANCH "@" GIT_COMMIT);
    ImGui::EndMainMenuBar();
}

void Editor::helpAbout() {
    constexpr float width = 600.f;
    constexpr float height = 420.f;
    constexpr float padding = 40.0f;

    ImGui::SetNextWindowSize({width, height}, ImGuiCond_Once);
    ImGui::SetNextWindowPos({200.f, 200.f}, ImGuiCond_Once, {0.5f, 0.5f}); // TODO: FIX THIS; center it
    if (ImGui::Begin("About", &showAbout, ImGuiWindowFlags_NoResize)) {
        ImGui::BeginChild("##about", {width - 2 * padding, -40.0f}, false);
        ImGui::Markdown(text::kaboutInfo, sizeof(text::kaboutInfo) - 1, md_conf);
        ImGui::EndChild();
    }
    ImGui::End();
}

void Editor::updateStats() {
    auto& io = ctx->IO;

    ImGui::SetNextWindowPos({io.DisplaySize.x - 10.f, 10.f}, ImGuiCond_Always, {1.f, 0.f});
    ImGui::SetNextWindowBgAlpha(0.35f); // transparency
    ImGui::Begin("window_stats", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration |
                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
                 ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDocking);

    ImGui::Text("Render Stats");
    ImGui::Separator();

    const float fps = io.Framerate;

    ImVec4 col;
    if (fps < 60.f) col = {0.921f, 0.603f, 0.f, 1.f};
    else if (fps < 30.f) col = {1.f, 0.019f, 0.133f, 1.f};
    else col = {0.133f, 0.921f, 0.f, 1.f};

    ImGui::TextColored(col, "Frame time %.2f\nFPS %.1f", 1000.f / fps, fps);
    ImGui::End();
}

void Editor::updateViewer(gfx::GLRenderTexture& renderTarget) {
    ImGui::Begin("Scene View" /*Do not rename-> docking system!*/);

    //TODO: this aint right, as this counts the window border...
    auto curSize = ImGui::GetWindowSize();

    // *cba to implement overloads*
    if (curSize.x != lastSize.x || curSize.y != lastSize.y) {
        renderTarget.resize({static_cast<u16>(curSize.x), static_cast<u16>(curSize.y)});
    }

    lastSize = curSize;

    void* target = reinterpret_cast<void*>(renderTarget.texture());
    ImGui::Image(target, curSize, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));

#if 0
        static MessageBox* g_box = nullptr;
    if (!g_box) {
        g_box = new MessageBox;
        static const char* buttons[] = {"Abort", "Cancel", "Please No", NULL};
        g_box->Init("XOXO?", nullptr,
                    "Are you really really sure you want to delete the entire log history and "
                    "loose all that information forever?",
                    buttons, true);
    }

    // Drawing
    if (ImGui::Button("Clear log")) {
        g_box->Open();
    }

    int selected = g_box->Draw();
#endif

    ImGui::End();
}

void Editor::applyDefaultLayout(ImGuiID dockId) {
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

    LOG_INFO("Rebuilding dockspace");
}

void Editor::updateDockLayout() {
    static const ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    // disable docking into parent window, as this would be awkward
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (dockFullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (opt_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", (bool*)0, window_flags);
    ImGui::PopStyleVar();

    if (dockFullscreen)
        ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockId = ImGui::GetID("RootDockspace");

        if (!ImGui::DockBuilderGetNode(dockId))
            applyDefaultLayout(dockId);

        ImGui::DockSpace(dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    } else {
        // ShowDockingDisabledMessage();
    }
}

void Editor::frame(gfx::GLRenderTexture& target) {
    ImguiDriver::poll();
    ImGui::NewFrame();
    updateDockLayout();

    updateMenu();

    if (showLog) logwidget->draw("Log", showLog);
    if (showViewer) updateViewer(target);
    if (showStats) updateStats();
    if (showAbout) helpAbout();

    ImGui::End(); // end dock space
    ImGui::Render();
    ImguiDriver::render();
}

void Editor::resize(i32 x, i32 y) {
    // notify driver
    // this is kinda hacky...
    ImguiDriver::resize(x, y);
}

void Editor::init() {
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

    // yea this is kinda hacky
    window.bindInput();

    // init style
    ImGuiStyle* style = &ImGui::GetStyle();
    style->ScaleAllSizes(FXWindow::getHDPIScale());

    // apply our default theme
    theme::applyStyle(style);

    md_conf.headingFormats[0].separator = true;

    ImFontConfig h1Conf;
    h1Conf.SizePixels = 26.0f;
    md_conf.headingFormats[0].font = io.Fonts->Fonts[0];
    md_conf.linkCallback = [](ImGui::MarkdownLinkCallbackData data) {

    };
   // md_conf.linkIcon = ICON_FA_LINK;
}
}