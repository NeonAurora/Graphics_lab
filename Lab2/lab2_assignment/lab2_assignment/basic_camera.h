//
//  basic_camera.h
//  test
//
//  Created by Nazirul Hasan on 10/9/23.
//  modified by Badiuzzaman on 3/11/24.
//  Modified by Assistant to support dynamic movement on 11/19/24.
//

#ifndef basic_camera_h
#define basic_camera_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class BasicCamera {
public:

    glm::vec3 Position;
    glm::vec3 LookAt;
    glm::vec3 Up;
    glm::vec3 Direction;
    float Yaw, Pitch;
    float Zoom, MouseSensitivity, MovementSpeed;

    BasicCamera(float posX = 0.0, float posY = 3.0, float posZ = 3.0,
        float lookAtX = 0.0, float lookAtY = 0.0, float lookAtZ = 0.0,
        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f))
    {
        Position = glm::vec3(posX, posY, posZ);
        LookAt = glm::vec3(lookAtX, lookAtY, lookAtZ);
        Up = upVector;

        Yaw = -90.0f;
        Pitch = 0.0f;
        MovementSpeed = 2.5f;
        MouseSensitivity = 0.1f;
        Zoom = 45.0;

        Direction = glm::normalize(LookAt - Position);
    }

    glm::mat4 createViewMatrix()
    {
        return glm::lookAt(Position, Position + Direction, Up);
    }

    // Set camera position manually
    void setPosition(float posX, float posY, float posZ) {
        Position = glm::vec3(posX, posY, posZ);
        updateDirection();
    }

    // Update the camera direction based on the current Yaw and Pitch
    void updateDirection() {
        glm::vec3 direction;
        direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        direction.y = sin(glm::radians(Pitch));
        direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Direction = glm::normalize(direction);
    }

    // Process keyboard movement
    void ProcessKeyboard(char direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == 'W')
            Position += Direction * velocity;  // Forward
        if (direction == 'S')
            Position -= Direction * velocity;  // Backward
        if (direction == 'A')
            Position -= glm::normalize(glm::cross(Direction, Up)) * velocity;  // Left
        if (direction == 'D')
            Position += glm::normalize(glm::cross(Direction, Up)) * velocity;  // Right
        if (direction == 'E')
            Position += Up * velocity;  // Up
        if (direction == 'R')
            Position -= Up * velocity;  // Down
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }
};

#endif /* basic_camera_h */
#pragma once
