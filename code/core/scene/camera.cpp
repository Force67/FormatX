
/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <glm/gtc/matrix_transform.hpp> 

#include "camera.h"
#include "graphics/gl_shader.h"
#include "glad/gl.h"

namespace scene {

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : 
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    moveSpeed(DEFAULT_SPEED),
    mouseSensitivity(DEFAULT_SENSITIVITY), zoom(DEFAULT_ZOOM) {
    pos = position;
    worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;

    // update to initial state
    //update();
}

void Camera::init(graphics::GLShaderProgram *prog) {
    projHandle = prog->getUniform("projection");
    viewHandle = prog->getUniform("view");
}

void Camera::update(graphics::GLShaderProgram* prog) {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(
        front, worldUp)); // Normalize the vectors, because their length gets closer to 0 the more
                          // you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));

    prog->use();

    glm::mat4 projMatrix = glm::perspective(glm::radians(zoom),
                                            (float)1920.f / (float)1080.f /*HACK*/, 0.1f, 100.0f);
    glUniformMatrix4fv(projHandle, 1, GL_FALSE, &projMatrix[0][0]);
    glUniformMatrix4fv(viewHandle, 1, GL_FALSE, &viewMatrix()[0][0]);
}

glm::mat4 Camera::viewMatrix() {
    return glm::lookAt(pos, pos + front, up);
}
}