
/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "ui/editor.h"

#include "config.h"
#include "renderer.h"
#include "graphics/gl_texture.h"

static config::opt<bool> g_wireFrameMode{"render.wireframe", "Toggle wire frame mode", false};

FXRenderer::FXRenderer(FXWindow &window) : 
    gfx::GLRenderer(window) {
}

FXRenderer::~FXRenderer() {
}

bool FXRenderer::init() {
    bool res = gfx::GLRenderer::init();
    if (!res)
        return false;

    // TODO: create render thread here.
    sceneTarget = std::make_unique<gfx::GLRenderTexture>(*this);
    scene = std::make_unique<scene::Scene>();
    scene->create(*this);

    return true;
}

void FXRenderer::shutdown() {


    gfx::GLRenderer::shutdown();
}

void FXRenderer::draw(ui::Editor &editor) {
    // the order is reversed on purpose
    editor.frame(*sceneTarget);

    // capture scene 
    scene->draw(*sceneTarget);
}