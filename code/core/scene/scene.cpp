
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

Scene::Scene() {
}

bool Scene::create(graphics::GLRenderer& renderer) {

    graphics::GLShaderBuilder<2> builder(renderer);
    builder.add(graphics::ShaderType::Fragment, FRAG_SHADER);
    builder.add(graphics::ShaderType::Vertex, VERTEX_SHADER);

    if (!builder.good()) {
        LOG_ERROR("Failed to compile scene shaders");
        return false;
    }

    projProgram = builder.finish();
    if (!projProgram)
        __debugbreak();

    cam.init(projProgram);
    return true;
}

void Scene::draw() {
    // enable state
    glEnable(GL_DEPTH_TEST);

    projProgram->use();

    cam.update(projProgram);

    glm::mat4 model = glm::mat4(1.0f);

    // put the model in the center and scale it
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

   // #error handle bug! HANDLE SHOULD BE SHADER MEMBER
    glUniformMatrix4fv(glGetUniformLocation(projProgram->GL_handle, "model"), 1, GL_FALSE, &model[0][0]);

    glDisable(GL_DEPTH_TEST);
}
}