#pragma once

/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <glm/glm.hpp>
#include <base.h>
#include <glm/ext/matrix_transform.hpp>

namespace graphics {
class GLRenderer;
class GLShaderProgram;
} // namespace graphics

namespace scene {

constexpr float DEFAULT_YAW = -90.0f;
//constexpr float DEFAULT_PITCH = 0.0f;
constexpr float DEFAULT_SPEED = 2.5f;
constexpr float DEFAULT_SENSITIVITY = 0.1f;
constexpr float DEFAULT_ZOOM = 45.0f;

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = DEFAULT_YAW, float pitch = 0.f);

    void update(graphics::GLShaderProgram*);

    glm::mat4 viewMatrix();

    void init(graphics::GLShaderProgram*);
private:

    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Euler Angles
    float yaw;
    float pitch;

    // Camera options
    float moveSpeed;
    float mouseSensitivity;
    float zoom;

private:
    i32 projHandle;
    i32 viewHandle;

    //graphics::GLShaderProgram* sceneProg;
};
} // namespace scene