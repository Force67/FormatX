
/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <glfw/glfw3.h>
#include <utl/path.h>

#include "editor.h"

#include "imgui_internal.h"
#include <imgui_markdown.h>

#include "builtin.h"
#include "elements.h"

#include "graphics/gl_texture.h"
#include "graphics/gl_helpers.h"

#ifdef _WIN32
#include <Windows.h>
#include <shellapi.h> // for ShellExecuteA
#endif

namespace ui {

// constants
constexpr auto kMaxUrlSize = 128;

static ImGui::MarkdownConfig mdConf;

Editor::Editor(FXWindow& window, gfx::GLRenderer& renderer) : ImguiDriver(window), rend(renderer) {
    logwidget = std::make_unique<LogWidget>();
}

ImVec2 Editor::center() {
    auto dispSize = ctx->IO.DisplaySize;
    return ImVec2(dispSize.x * 0.5f, dispSize.y * 0.5f);
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
        if (ImGui::MenuItem("About " PRJ_NAME))
            showAbout = !showAbout;
        ImGui::EndMenu();
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::TextColored({0.615f, 0.631f, 0.705f, 1.f},
                       ICON_FA_CODE ": unknown " ICON_FA_GITHUB ": " GIT_BRANCH "@" GIT_COMMIT);
    ImGui::EndMainMenuBar();
}

void Editor::helpAbout() {
    constexpr float k_width = 600.f;
    constexpr float k_height = 420.f;
    constexpr char k_appname[] = "About " PRJ_NAME;

    // this is due to the way popups work in imgui
    // see: https://github.com/ocornut/imgui/issues/331#issuecomment-140055181
    ImGui::OpenPopup(k_appname);

    ImGui::SetNextWindowSize({k_width, k_height}, ImGuiCond_Once);
    ImGui::SetNextWindowPos(center(), ImGuiCond_Once, {0.5f, 0.5f});
    if (ImGui::BeginPopupModal(k_appname, &showAbout,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::Markdown(text::kaboutInfo, sizeof(text::kaboutInfo) - 1, mdConf);
        ImGui::EndPopup();
    }
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
    const float fps = io.Framerate;

    ImVec4 col;
    if (fps < 60.f) col = {0.921f, 0.603f, 0.f, 1.f};
    else if (fps < 30.f) col = {1.f, 0.019f, 0.133f, 1.f};
    else col = {0.133f, 0.921f, 0.f, 1.f};

    ImGui::TextColored(col, "FPS %.1f\nms %.2f", fps, 1000.f / fps);
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

    //void* target = reinterpret_cast<void*>(renderTarget.texture());
    void* target = ImGui::GetIO().Fonts->TexID;
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

bool Editor::init() {
    // create ImGui instance
    ctx = ImGui::CreateContext();
    float dpiScale = FXWindow::getHDPIScale();

    auto& io = ctx->IO;
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard | 
                     ImGuiConfigFlags_NavEnableGamepad |
                     ImGuiConfigFlags_DockingEnable;

    auto& style = ctx->Style;
    style.ScaleAllSizes(dpiScale);
    theme::applyStyle(&style);

    constexpr float k_textscale = 18.f;
    constexpr float k_iconscale = 14.5f;

    const float iconPixelSize = dpiScale * k_iconscale;

    // TODO: add bold/italic roboto variants for markdown?
    // TODO: move these?
    auto path1 = utl::make_app_path(utl::app_path::self, "Roboto-Regular.ttf");
    auto path2 = utl::make_app_path(utl::app_path::self, "fa-solid-900.ttf");
    auto path3 = utl::make_app_path(utl::app_path::self, "fa-brands-400.ttf");

    static const ImWchar ranges_icons[] = {ICON_MIN_FA, ICON_MAX_FA, 0};

    // TODO: to avoid loading useless stuff, we specify only a few
    static const ImWchar ranges_brands[] = {ICON_MIN_FAB, ICON_MAX_FAB, 0};

    ImFontConfig config;
    config.MergeMode = true;
    config.PixelSnapH = true;
    config.GlyphMinAdvanceX = iconPixelSize;

    ImFont* textFont = io.Fonts->AddFontFromFileTTF(path1.c_str(), dpiScale * k_textscale);
    ImFont* iconFont = io.Fonts->AddFontFromFileTTF(path2.c_str(), iconPixelSize, &config, ranges_icons);
    ImFont* brandFont = io.Fonts->AddFontFromFileTTF(path3.c_str(), iconPixelSize, &config, ranges_brands);

    if (!textFont || !iconFont || !brandFont) {
        LOG_ERROR("Failed to load required fonts");
        return false;
    }

    if (!ImguiDriver::create(rend)) {
        return false;
    }

    window.bindInput();

    ImFontConfig h1Conf;
    h1Conf.SizePixels = 26.0f;

    // configure markdown engine
    mdConf.headingFormats[0].separator = true;
    mdConf.headingFormats[0].font = textFont;
    mdConf.linkIcon = ICON_FA_LINK;
    mdConf.linkCallback = [](ImGui::MarkdownLinkCallbackData data) {
        std::string url(data.link, data.linkLength);
#ifdef _WIN32
        if (!data.isImage) {
            ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
        }
#endif
    };

    return true;
}
}