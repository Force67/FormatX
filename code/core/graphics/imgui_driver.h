#pragma once

/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>

class FXWindow;

struct ImGuiContext;
struct ImDrawData;

namespace graphics {

class GLRenderer;
class GLShaderProgram;

class ImguiDriver {
public:
    explicit ImguiDriver(FXWindow&);
    ~ImguiDriver();

    bool create(GLRenderer&);

protected:
    void poll();
    void render();

    void resize(i32, i32);

private:
    i32 lastX = 0;
    i32 lastY = 0;

    void renderDrawData(ImDrawData*);
    void setupRenderstate(ImDrawData*, i32, i32, u32);

    ImGuiContext* ctx = nullptr;
    double timestamp = 0.0;

    u32 vertexBuffer = 0, arrayList = 0;
    i32    g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0; // Uniforms location
    i32 g_AttribLocationVtxPos = 0, g_AttribLocationVtxUV = 0,
               g_AttribLocationVtxColor = 0; // Vertex attributes location

    // transforms
    GLShaderProgram* imageProgram = nullptr;

protected:
    FXWindow& window;
};
}