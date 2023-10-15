//
// Created by TURIING on 2023/10/14.
//

#ifndef LEARN_OPENGL_GradientTriangle_H
#define LEARN_OPENGL_GradientTriangle_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cassert>
#include "GLObject.h"
#include <cmath>

class GradientTriangle: public GLObject{
public:
    GradientTriangle(int _scrWidth, int _scrHeight, const char *_title): GLObject(_scrWidth, _scrHeight, _title) {

    }

    ~GradientTriangle() override {
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

        // 解释如何解析顶点数据并启用顶点属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_verticesStride * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


    }

    /*
     * 绘制图形
     */
    void paint() override {
        // 使用着色器程序
        m_shader->use();

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};

#endif //LEARN_OPENGL_GradientTriangle_H
