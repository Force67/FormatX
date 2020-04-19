
/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "imgui_driver.h"
#include "imgui_internal.h"

#include "glfw/glfw3.h"
#include "video_core.h"
#include "window.h"

#include <shader.h>
#include <texture.h>

namespace ui {

namespace vc = video_core;

ImguiDriver::ImguiDriver(FXWindow& window) : window(window) {}

ImguiDriver::~ImguiDriver() {
    auto &io = ctx->IO;
    io.Fonts->TexID = 0;

    ImGui::DestroyContext(ctx);
}

// these are here temporarily
const char* fragment_shader_glsl_410_core =
    "in vec2 Frag_UV;\n"
    "in vec4 Frag_Color;\n"
    "uniform sampler2D Texture;\n"
    "layout (location = 0) out vec4 Out_Color;\n"
    "void main()\n"
    "{\n"
    "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
    "}\n";

const char* vertex_shader_glsl_410_core =
        "layout (location = 0) in vec2 Position;\n"
        "layout (location = 1) in vec2 UV;\n"
        "layout (location = 2) in vec4 Color;\n"
        "uniform mat4 ProjMtx;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "    Frag_UV = UV;\n"
        "    Frag_Color = Color;\n"
        "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
        "}\n";

void ImguiDriver::setKeybindings(ImGuiIO& io) {

    // todo: remapable keybinds
    // maybe make this an editor function instead?
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
    io.ClipboardUserData = this;

    io.SetClipboardTextFn = [](void* userp, const char* text) {
        glfwSetClipboardString(static_cast<GLFWwindow*>(userp), text);
    };

    io.GetClipboardTextFn = [](void* userp) {
        return glfwGetClipboardString(static_cast<GLFWwindow*>(userp));
    };
}

bool ImguiDriver::create(video_core::renderInterface& renderer) {
    // use video core quick allocator
    ImGui::SetAllocatorFunctions(([](size_t size, void* unused) { 
        return vc::v_malloc(size);
    }),
    ([](void* ptr, void*) {
        return vc::v_free(ptr);   
    }), nullptr);

    // create ImGui 
    ctx = ImGui::CreateContext();

    auto& io = ctx->IO;
    setKeybindings(io);

#if defined(_WIN32)
    io.ImeWindowHandle = window.getHandle();
#endif

    io.BackendRendererName = "VideoCore2";
    io.BackendPlatformName = "GLFW"; 

    io.UserData = static_cast<void*>(window.HACK_getWindow());
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset; // Large meshes
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    // setup style
    ImGui::StyleColorsDark();

    // create font texture
    u8* pixels;
    i32 width, height, bytes_per_pixel;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);

    // no need to cleanup the texture from here
    // it will get free'd once factory gets destroyed
    auto *texture = renderer.texFactory->createTexture(
        static_cast<u16>(width), 
        static_cast<u16>(height), pixels);

    if (!texture) return false;

    io.Fonts->TexID = texture->handle();

    // create vertex and fragment shaders
    auto* vertexShader = renderer.shaderFactory->createFromSource(
        vc::ShaderType::Vertex, 
        vertex_shader_glsl_410_core);

    auto* fragShader = renderer.shaderFactory->createFromSource(
        vc::ShaderType::Fragment,
        fragment_shader_glsl_410_core);

    if (!vertexShader || !fragShader) {
        LOG_ERROR("Failed to create frag/vertex shader for imgui");
        return false;
    }

    const vc::Shader* imguiShaders[2] = {vertexShader, fragShader};
    if (!renderer.shaderFactory->createProgram(imguiShaders, 2)) {
        LOG_ERROR("Failed to create shader program");
        return false;
    }

    // inital flush
    // to intiailize render state
    render();

    return true;
}

void ImguiDriver::setupRenderstate(const ImDrawList* drawList) {

}

void ImguiDriver::render() {

    auto& config = window.getConfig();
    ctx->IO.DisplaySize = ImVec2(
        static_cast<float>(config.width), 
        static_cast<float>(config.height)
    );

    // HACK
    i32 displayW, displayH;
    glfwGetFramebufferSize(window.HACK_getWindow(), &displayW, &displayH);

    if (config.width > 0 && config.height)
        ctx->IO.DisplayFramebufferScale = ImVec2(
            static_cast<float>(displayW) / config.width, 
            static_cast<float>(displayH) / config.height);

    // update imgui timer
    double time = glfwGetTime();
    ctx->IO.DeltaTime = timestamp > 0.0 ? 
        static_cast<float>(time - timestamp) :
        static_cast<float>(1.f / 60.f);
       
    timestamp = time;

    ImDrawData* drawData = &ctx->DrawData;

    // we are minimized, ensure that we don't render anything
    // TODO: nuke this check, check within GLFW instead...
    float fbWidth = drawData->DisplaySize.x * drawData->FramebufferScale.x;
    float fbHeight = drawData->DisplaySize.y * drawData->FramebufferScale.y;
    if (fbWidth <= 0.f || fbHeight <= 0.f)
        return;

    for (i32 n = 0; n < drawData->CmdListsCount; n++) {
        const ImDrawList* drawList = drawData->CmdLists[n];

        // for each draw command, we check
        for (const ImDrawCmd &cmd : drawList->CmdBuffer) {

            if (cmd.UserCallback) {
                // we must reset the render state
                if (cmd.UserCallback == ImDrawCallback_ResetRenderState)
                    setupRenderstate(drawList);
                else
                    cmd.UserCallback(drawList, &cmd);
            }

            // scissor 
            //const u16 xx = std::max<u16>(cmd.ClipRect.x, 0.f);
            //const u16 yy = std::max<u16>(cmd.ClipRect.y, 0.f);

                        /*
        bgfx::setScissor(xx, yy
            , uint16_t(bx::min(cmd->ClipRect.z, 65535.0f)-xx)
            , uint16_t(bx::min(cmd->ClipRect.w, 65535.0f)-yy)
            );
*/
        }

    }
}
}