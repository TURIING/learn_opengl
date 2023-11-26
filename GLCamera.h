//
// Created by TURIING on 2023/11/5.
//

#ifndef LEARN_OPENGL_GLCAMERA_H
#define LEARN_OPENGL_GLCAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * 摄像机移动的方向
 */
enum CameraMovement {
    FORWARD = 0,
    BAKCWARD,
    LEFT,
    RIGHT
};

constexpr float YAW         = -90.0f;
constexpr float PITCH       =  0.0f;
constexpr float SPEED       =  2.5f;
constexpr float SENSITIVITY =  0.1f;
constexpr float FOV        =  45.0f;

class GLCamera {
public:

    GLCamera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), float _yaw = YAW, float _pitch = PITCH) {
        m_position = _position;
        m_worldUp = _up;
        m_yaw = _yaw;
        m_pitch = _pitch;

        updateCameraVector();
    }
    GLCamera(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch) {
        m_front = glm::vec3(0.0f, 0.0f, -1.0f);
        m_worldUp = glm::vec3(_upX, _upY, _upZ);
        m_position = glm::vec3(_posX, _posY, _posZ);
        m_yaw = _yaw;
        m_pitch = _pitch;

        updateCameraVector();
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    glm::vec3 getPosition() { return m_position; }

    /*
     * 处理键盘事件
     */
    void processKeyboard(CameraMovement _direction, float _deltaTime) {
        float velocity = m_movementSpeed * _deltaTime;

        if (_direction == FORWARD)
            m_position += velocity * m_front;
        if (_direction == BAKCWARD)
            m_position -= velocity * m_front;
        if (_direction == LEFT)
            m_position -= m_right * velocity;
        if (_direction == RIGHT)
            m_position += m_right * velocity;
    }

    /*
     * 处理鼠标移动事件
     */
    void processMouseMovement(float _xOffset, float _yOffset, GLboolean _constrainPitch = true) {
        _xOffset *= m_mouseSensitivity;
        _yOffset *= m_mouseSensitivity;

        m_yaw += _xOffset;
        m_pitch += _yOffset;

        if (_constrainPitch) {
            if (m_pitch > 89.0f) m_pitch = 89.0f;
            if (m_pitch < -89.0f) m_pitch = -89.0f;
        }

        updateCameraVector();
    }

    /*
     * 处理鼠标滚动事件
     */
    void processMouseScroll(float _yOffset) {
        m_fov -= static_cast<float>(_yOffset);
        if(m_fov < 1.0f)
            m_fov = 1.0f;
        if(m_fov > 45.0f)
            m_fov = 45.0f;
    }

    float getFov() const {
        return m_fov;
    }

private:
    void updateCameraVector() {
        glm::vec3 front;
        front.x = static_cast<float>(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
        front.y = static_cast<float>(sin(glm::radians(m_pitch)));
        front.z = static_cast<float>(sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
        m_front = glm::normalize(front);

        m_right = glm::normalize(glm::cross(m_front, m_worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }

private:
    glm::vec3 m_position;
    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);                     // 摄像机指向的向量
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_yaw;                                                          // 偏航角
    float m_pitch;                                                        // 俯仰角

    float m_movementSpeed = SPEED;
    float m_mouseSensitivity = SENSITIVITY;
    float m_fov = FOV;                                                    // 视野（Field of View），定义了我们可以看到场景中多大的范围
};

#endif //LEARN_OPENGL_GLCAMERA_H
