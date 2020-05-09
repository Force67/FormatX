
/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "scene.h"
#include "glad/gl.h"

#include "graphics/gl_renderer.h"
#include "graphics/gl_texture.h"
#include "graphics/gl_shader.h"
#include "graphics/gl_helpers.h"

#include "graphics/gfxresource.h"

namespace scene {

constexpr char FRAG_SHADER[] = R"(
    out vec4 FragColor;
    in vec2 TexCoords;

    uniform sampler2D texture_diffuse1;

    void main() {    
        FragColor = texture(texture_diffuse1, TexCoords);
    }
)";

constexpr char VERTEX_SHADER[] = R"(
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec2 aTexCoords;

    out vec2 TexCoords;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        TexCoords = aTexCoords;    
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)";

static Scene* g_Scene{nullptr};

Scene::Scene() {
    g_Scene = this;
}

Scene::~Scene() {
    g_Scene = nullptr;
}

Scene* ActiveScene() {
    return g_Scene;
}

static gfx::GLTexture* TEST_tex;

bool Scene::create(gfx::GLRenderer& renderer) {

    gfx::GLShaderBuilder<2> builder(renderer);
    builder.add(gfx::ShaderType::Fragment, FRAG_SHADER);
    builder.add(gfx::ShaderType::Vertex, VERTEX_SHADER);

    if (!builder.good()) {
        LOG_ERROR("Failed to compile scene shaders");
        return false;
    }

    projProgram = builder.finish();
    if (!projProgram)
        __debugbreak();

    cam.init(projProgram);

    TEST_tex = reinterpret_cast<gfx::GLTexture*>(gfx::loadTexture(
        utl::make_app_path(utl::app_path::self, "missing.png")));

    return true;
}

void Scene::draw(gfx::GLRenderTexture &target) {
    // activate target
    glBindFramebuffer(GL_FRAMEBUFFER, target.HACK_fbohandle());

    // TODO: move update back texture
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           target.texture()->GL_handle, 0);

#if 0
        static bool TEST = false;
    if (!TEST) {
        if (auto* tex = gfx::loadTexture(utl::make_app_path(utl::app_path::self, "missing.png"))) {
            glFramebufferTexture2D(
                GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                reinterpret_cast<u32>(reinterpret_cast<gfx::GLTexture*>(tex)->nativeHandle()), 0);

            auto tSZ = target.size();
            glBlitFramebuffer(0, 0, tex->desc.width, tex->desc.height, 0, 0, tSZ.x, tSZ.y,
                              GL_COLOR_BUFFER_BIT, GL_LINEAR);

            target.HACK_SetInternalTexture(reinterpret_cast<gfx::GLTexture*>(tex));
        }

        TEST = true;
    }
    #endif
    
    glEnable(GL_DEPTH_TEST);  

    projProgram->use();
    cam.update(projProgram);

    // draw model
    glm::mat4 model = glm::mat4(1.0f);

    // put the model in the center and scale it
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(glGetUniformLocation(projProgram->GL_handle, "model"), 1, GL_FALSE, &model[0][0]);

    glDisable(GL_DEPTH_TEST);

    // disable target
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}