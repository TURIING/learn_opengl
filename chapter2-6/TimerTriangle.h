//
// Created by TURIING on 2023/10/14.
//

#ifndef LEARN_OPENGL_Triangle_H
#define LEARN_OPENGL_Triangle_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cassert>
#include "GLObject.h"
#include <cmath>

class TimerTriangle: public GLObject{
public:
    TimerTriangle(int _scrWidth, int _scrHeight, const char *_title): GLObject(_scrWidth, _scrHeight, _title) {

    }

    ~TimerTriangle() override {
        /* 清理并退出 */
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);

        glfwTerminate();
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

        // 解释如何解析顶点数据
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_verticesStride * sizeof(float), (void *)0);

        // 启用顶点属性
        glEnableVertexAttribArray(0);
    }

    /*
     * 绘制图形
     */
    void paint() override {
        // 使用着色器程序
        m_shader->use();

        double timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        m_shader->setFloat("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};

#endif //LEARN_OPENGL_Triangle_H
