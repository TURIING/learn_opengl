//
// Created by TURIING on 2023/10/14.
//

#ifndef LEARN_OPENGL_AngleShift_H
#define LEARN_OPENGL_AngleShift_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cassert>
#include "GLObject.h"
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class AngleShift: public GLObject{
public:
    AngleShift(int _scrWidth, int _scrHeight, const char *_title): GLObject(_scrWidth, _scrHeight, _title) {
        this->enableDeepTest();
    }

    ~AngleShift() override {
        /* 清理并退出 */
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);

        glfwTerminate();
    }

    void setTexturePath(const std::string &_texturePath1, const std::string &_texturePath2) {
        m_texturePath1 = _texturePath1;
        m_texturePath2 = _texturePath2;
    }

    void setCubePos(const std::vector<glm::vec3> &_cubePos) {
        m_cubePos = _cubePos;
    }

    void init() override {
        glfwSetWindowUserPointer(m_window, this);

        // 绑定鼠标移动回调事件
        auto pMouseCallBack = [](GLFWwindow *_window, double _xPosIn, double _yPosIn) {
            static_cast<AngleShift *>(glfwGetWindowUserPointer(_window))->mouseCallBack(_window, _xPosIn, _yPosIn);
        };
        glfwSetCursorPosCallback(m_window, pMouseCallBack);

        // 绑定鼠标滚动回调事件
        auto pScrollCallBack = [](GLFWwindow *_window, double _xOffset, double _yOffset) {
            static_cast<AngleShift *>(glfwGetWindowUserPointer(_window))->scrollCallBack(_window, _xOffset, _yOffset);
        };
        glfwSetScrollCallback(m_window, pScrollCallBack);

        // 捕获鼠标
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        GLObject::init();
    }
protected:
    void initVAO() override {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
    }

    /*
     * 初始化VBO
     */
    void initVBO() override {
        assert(!m_vertices.empty());

        // 生成VBO对象
        glGenBuffers(1, &m_VBO);

        // 绑定缓冲类型
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        // 将顶点数据复制到缓冲内存中
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

        // 解释如何解析顶点位置数据并启用位置顶点属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_verticesStride * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // 解释如何解析顶点纹理数据并启用纹理顶点属性
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, m_verticesStride * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void initTexture() override {
        /* 生成纹理1 */
        // 创建纹理对象
        glGenTextures(1, &m_texture1);

        // 绑定纹理到纹理目标
        glBindTexture(GL_TEXTURE_2D, m_texture1);

        // 设置环绕方式和应用的纹理轴
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // 设置放大/缩小对应的纹理过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        generateTexture(m_texturePath1, GL_TEXTURE_2D, GL_RGB);


        /* 生成纹理2 */
        glGenTextures(1, &m_texture2);

        // 绑定纹理到纹理目标
        glBindTexture(GL_TEXTURE_2D, m_texture2);

        // 设置环绕方式和应用的纹理轴
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // 设置放大/缩小对应的纹理过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        generateTexture(m_texturePath2, GL_TEXTURE_2D, GL_RGBA);

        /* 将着色器采样器(sampler2D)绑定到纹理单元 */
        m_shader->setInt("texture1", 0);
        m_shader->setInt("texture2", 1);

        /* 激活并绑定纹理 */
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_texture2);
    }

    void preLoop() override {

    }

    /*
     * 绘制图形
     */
    void paint() override {
        assert(!m_cubePos.empty());

        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 projection = glm::perspective(glm::radians(m_fov), (float)getWidth() / (float)getHeight(), 0.1f, 100.0f);
        m_shader->setMat4("projection", glm::value_ptr(projection));

        glm::mat4 view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
        m_shader->setMat4("view", glm::value_ptr(view));

        for(auto i = 0; i < m_cubePos.size(); i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_cubePos[i]);
            float angle = 20.0f * static_cast<float>(i);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            m_shader->setMat4("model", glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    void processInput(GLFWwindow *_window) override {
        float cameraSpeed = static_cast<float>(2.5 * deltaTime);
        if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
            m_cameraPos += cameraSpeed * m_cameraFront;
        if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
            m_cameraPos -= cameraSpeed * m_cameraFront;
        if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
            m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
        if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
            m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;

        GLObject::processInput(_window);
    }
private:
    /*
     * 鼠标事件回调函数
     */
    void mouseCallBack(GLFWwindow *_window, double _xPosIn, double _yPosIn) override {
        auto xpos = static_cast<float>(_xPosIn);
        auto ypos = static_cast<float>(_yPosIn);
        if(m_bFirstMouse) {
            m_fLastX = xpos;
            m_fLastY = ypos;
            m_bFirstMouse = false;
        }

        float xOffset = xpos - m_fLastX;
        float yOffset = m_fLastY - ypos;
        m_fLastX = xpos;
        m_fLastY = ypos;

        float sensitivity = 0.1f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        m_fYaw += xOffset;
        m_fPitch += yOffset;

        if(m_fPitch > 89.0f) m_fPitch = 89.0f;
        if(m_fPitch < -89.0f) m_fPitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
        front.y = sin(glm::radians(m_fPitch));
        front.z = sin(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
        m_cameraFront = glm::normalize(front);
    }

    void scrollCallBack(GLFWwindow *_window, double _xOffset, double _yOffset) override {
        m_fov -= static_cast<float>(_yOffset);
        if(m_fov < 1.0f)
            m_fov = 1.0f;
        if(m_fov > 45.0f)
            m_fov = 45.0f;
    }
private:
    std::string m_texturePath1;
    std::string m_texturePath2;
    // 纹理对象
    unsigned int m_texture1 = 0;
    unsigned int m_texture2 = 0;
    // 正方体的位置
    std::vector<glm::vec3> m_cubePos;
    // 摄像机的位置
    glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    // 摄像机指向的向量
    glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    // 摄像机的上向量
    glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    // 上一帧跟当前帧的时间间隔
    float deltaTime = 0.0f;
    // 记录当前帧的时间
    float lastFrame = 0.0f;
    // 鼠标是否刚被捕获
    bool m_bFirstMouse = true;
    float m_fLastX = 800.0f / 2.0;
    float m_fLastY = 600.0f / 2.0;
    // 偏航角
    float m_fYaw = -90.0f;
    // 俯仰角
    float m_fPitch = 0.0f;
    // 视野（Field of View），定义了我们可以看到场景中多大的范围
    float m_fov = 45.0f;
};



#endif //LEARN_OPENGL_AngleShift_H
