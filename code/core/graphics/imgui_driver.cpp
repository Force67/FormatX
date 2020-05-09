
/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "imgui.h"
#include "imgui_internal.h"

#include "imgui_driver.h"

#include "glad/gl.h"
#include "glfw/glfw3.h"

#include "window.h"

#include "gl_renderer.h"
#include "gl_shader.h"
#include "gl_texture.h"

#include <glm/glm.hpp>

namespace gfx {

constexpr char FRAG_SHADER[] = R"(
    in vec2 Frag_UV;
    in vec4 Frag_Color;
    uniform sampler2D Texture;
    layout (location = 0) out vec4 Out_Color;
    void main() {
        Out_Color = Frag_Color * texture(Texture, Frag_UV.st);
    };
)";

constexpr char VERTEX_SHADER[] = R"(
    layout (location = 0) in vec2 Position;
    layout (location = 1) in vec2 UV;
    layout (location = 2) in vec4 Color;
    uniform mat4 ProjMtx;
    out vec2 Frag_UV;
    out vec4 Frag_Color;
    void main() {
        Frag_UV = UV;
        Frag_Color = Color;
        gl_Position = ProjMtx * vec4(Position.xy,0,1);
    };
)";

ImguiDriver::ImguiDriver(FXWindow& window) : window(window) {}

ImguiDriver::~ImguiDriver() {
    if (vertexBuffer) {
        glDeleteBuffers(1, &vertexBuffer);
        vertexBuffer = 0;
    }
    if (arrayList) {
        glDeleteBuffers(1, &arrayList);
        arrayList = 0;
    }

    auto& io = ctx->IO;
    io.Fonts->TexID = 0;

    ImGui::DestroyContext(ctx);
}

void ImguiDriver::setupRenderstate(ImDrawData* drawData, i32 fbWidth, i32 fbHeight, u32 vertexArrayHandle) {
    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor
    // enabled, polygon fill
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

    glViewport(0, 0, (GLsizei)fbWidth, (GLsizei)fbHeight);

    // TODO: make this nice
    float L = drawData->DisplayPos.x;
    float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
    float T = drawData->DisplayPos.y;
    float B = drawData->DisplayPos.y + drawData->DisplaySize.y;
    const float ortho_projection[4][4] = {
        {2.0f / (R - L), 0.0f, 0.0f, 0.0f},
        {0.0f, 2.0f / (T - B), 0.0f, 0.0f},
        {0.0f, 0.0f, -1.0f, 0.0f},
        {(R + L) / (L - R), (T + B) / (B - T), 0.0f, 1.0f},
    };

    glUseProgram(imageProgram->GL_handle);
    glUniform1i(g_AttribLocationTex, 0);
    glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
    glBindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set
                         // that otherwise.
    glBindVertexArray(vertexArrayHandle);

    // Bind vertex/index buffers and setup attributes for ImDrawVert
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrayList);

    glEnableVertexAttribArray(g_AttribLocationVtxPos);
    glEnableVertexAttribArray(g_AttribLocationVtxUV);
    glEnableVertexAttribArray(g_AttribLocationVtxColor);
    glVertexAttribPointer(g_AttribLocationVtxPos, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
                          (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(g_AttribLocationVtxUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
                          (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(g_AttribLocationVtxColor, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                          sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));
}

// TODO: do we really need to backup EVERYTHING?
void ImguiDriver::renderDrawData(ImDrawData* drawData) {
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates !=
    // framebuffer coordinates)
    int fb_width = (int)(drawData->DisplaySize.x * drawData->FramebufferScale.x);
    int fb_height = (int)(drawData->DisplaySize.y * drawData->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    // Backup GL state
    GLenum last_active_texture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
    glActiveTexture(GL_TEXTURE0);
    GLint last_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_sampler;
    glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
    GLint last_array_buffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    GLint last_vertex_array_object;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array_object);
    GLint last_polygon_mode[2];
    glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
    GLint last_viewport[4];
    glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLint last_scissor_box[4];
    glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    GLenum last_blend_src_rgb;
    glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
    GLenum last_blend_dst_rgb;
    glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
    GLenum last_blend_src_alpha;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
    GLenum last_blend_dst_alpha;
    glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
    GLenum last_blend_equation_rgb;
    glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
    GLenum last_blend_equation_alpha;
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
    bool clip_origin_lower_left = true;
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__)
    GLenum last_clip_origin = 0;
    glGetIntegerv(GL_CLIP_ORIGIN,
                  (GLint*)&last_clip_origin); // Support for GL 4.5's glClipControl(GL_UPPER_LEFT)
    if (last_clip_origin == GL_UPPER_LEFT)
        clip_origin_lower_left = false;
#endif

    // Setup desired GL state
    // Recreate the VAO every time (this is to easily allow multiple GL contexts to be rendered to.
    // VAO are not shared among GL contexts) The renderer would actually work without any VAO bound,
    // but then our VertexAttrib calls would overwrite the default one currently bound.
    GLuint vertex_array_object = 0;
#ifndef IMGUI_IMPL_OPENGL_ES2
    glGenVertexArrays(1, &vertex_array_object);
#endif
    setupRenderstate(drawData, fb_width, fb_height, vertex_array_object);

    // Will project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_off = drawData->DisplayPos; // (0,0) unless using multi-viewports
    ImVec2 clip_scale =
        drawData->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    // Render command lists
    for (int n = 0; n < drawData->CmdListsCount; n++) {
        const ImDrawList* cmd_list = drawData->CmdLists[n];

        // Upload vertex/index buffers
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert),
                     (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx),
                     (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != NULL) {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to
                // request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    setupRenderstate(drawData, fb_width, fb_height,
                                                       vertex_array_object);
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            } else {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f &&
                    clip_rect.w >= 0.0f) {
                    // Apply scissor/clipping rectangle
                    if (clip_origin_lower_left)
                        glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w),
                                  (int)(clip_rect.z - clip_rect.x),
                                  (int)(clip_rect.w - clip_rect.y));
                    else
                        glScissor(
                            (int)clip_rect.x, (int)clip_rect.y, (int)clip_rect.z,
                            (int)clip_rect
                                .w); // Support for GL 4.5 rarely used glClipControl(GL_UPPER_LEFT)

                    // Bind texture, Draw
                    u32 glTex = reinterpret_cast<gfx::GLTexture*>(pcmd->TextureId)->GL_handle;

                    glBindTexture(GL_TEXTURE_2D, glTex);
                    glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
                                             sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT
                                                                    : GL_UNSIGNED_INT,
                                             (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)),
                                             (GLint)pcmd->VtxOffset);
                }
            }
        }
    }

    // Destroy the temporary VAO
    glDeleteVertexArrays(1, &vertex_array_object);

    // Restore modified GL state
    glUseProgram(last_program);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindSampler(0, last_sampler);
    glActiveTexture(last_active_texture);
    glBindVertexArray(last_vertex_array_object);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha,
                        last_blend_dst_alpha);
    if (last_enable_blend)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
    if (last_enable_cull_face)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    if (last_enable_depth_test)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    if (last_enable_scissor_test)
        glEnable(GL_SCISSOR_TEST);
    else
        glDisable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
    glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
#endif
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2],
               (GLsizei)last_viewport[3]);
    glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2],
              (GLsizei)last_scissor_box[3]);
}

bool ImguiDriver::create(GLRenderer& renderer) {
    // create ImGui
    ctx = ImGui::CreateContext();
    ctx->IO.BackendRendererName = "ImguiDriver";
    ctx->IO.IniFilename = nullptr;
    ctx->IO.LogFilename = nullptr;
    ctx->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    ctx->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    ctx->IO.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset; // Large meshes
    ctx->IO.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    ctx->IO.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    gfx::GLShaderBuilder<2> builder(renderer);
    builder.add(gfx::ShaderType::Fragment, FRAG_SHADER);
    builder.add(gfx::ShaderType::Vertex, VERTEX_SHADER);

    if (!builder.good()) {
        LOG_ERROR("Failed to compile imgui shaders");
        return false;
    }

    imageProgram = builder.finish();
    if (!imageProgram) {
        LOG_ERROR("Failed to link shaders");
        return false;
    }

    g_AttribLocationTex = imageProgram->getUniform("Texture");
    g_AttribLocationProjMtx = imageProgram->getUniform("ProjMtx");
    g_AttribLocationVtxPos = imageProgram->getAttrib("Position");
    g_AttribLocationVtxUV = imageProgram->getAttrib("UV");
    g_AttribLocationVtxColor = imageProgram->getAttrib("Color");

    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &arrayList);

    //ImFontConfig config;
    //config.SizePixels = 20.f;
    //config.

    auto* font = ctx->IO.Fonts->AddFontFromFileTTF(R"(C:\\Windows\\Fonts\\Verdana.ttf)",
                                                   FXWindow::getHDPIScale() * 18.f);
    if (!font) {
        LOG_ERROR("Unable to load font");
        return false;
    }

    // create font atlas
    u8* pixels;
    int width, height;
    ctx->IO.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    TextureDesc desc;
    desc.colorFormat = ColorFormat::RGBA;
    desc.height = static_cast<u16>(width);
    desc.width = static_cast<u16>(width);

    auto* fontAtlas = renderer.textureFactory->createTexture(desc, pixels);

    if (!fontAtlas) {
        LOG_ERROR("Failed to create font altas");
        return false;
    }

    ctx->IO.Fonts->TexID = static_cast<void*>(fontAtlas);
    return true;
}

void ImguiDriver::resize(i32 x, i32 y) {

    if (!x || !y)
        return;
}

void ImguiDriver::poll() {
    auto& config = window.size();
    ctx->IO.DisplaySize = ImVec2(static_cast<float>(config.x), static_cast<float>(config.y));

    // HACK
    i32 displayW, displayH;
    glfwGetFramebufferSize(window.getWindowImp(), &displayW, &displayH);

    if (config.x > 0 && config.y)
        ctx->IO.DisplayFramebufferScale = ImVec2(static_cast<float>(displayW) / config.x,
                                                 static_cast<float>(displayH) / config.y);

    // update imgui timer
    double time = glfwGetTime();
    ctx->IO.DeltaTime =
        timestamp > 0.0 ? static_cast<float>(time - timestamp) : static_cast<float>(1.f / 60.f);

    timestamp = time;
}

void ImguiDriver::render() {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window.getWindowImp(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    renderDrawData(ImGui::GetDrawData());
}
}