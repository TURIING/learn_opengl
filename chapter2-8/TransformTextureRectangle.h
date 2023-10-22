//
// Created by TURIING on 2023/10/14.
//

#ifndef LEARN_OPENGL_MutiTextureRectangle_H
#define LEARN_OPENGL_MutiTextureRectangle_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cassert>
#include "GLObject.h"
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class InclinedPlane: public GLObject{
public:
    InclinedPlane(int _scrWidth, int _scrHeight, const char *_title): GLObject(_scrWidth, _scrHeight, _title) {

    }

    ~InclinedPlane() override {
        /* 清理并退出 */
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);

        glfwTerminate();
    }

    void setTexturePath(const std::string &_texturePath1, const std::string &_texturePath2) {
        m_texturePath1 = _texturePath1;
        m_texturePath2 = _texturePath2;
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

    void initEBO() override {
        assert(!m_indices.empty());

        // 创建EBO对象
        glGenBuffers(1, &m_EBO);

        // 绑定缓冲类型
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

        // 传送数据到缓冲内存
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
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

    /*
     * 绘制图形
     */
    void paint() override {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.5f, 0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        m_shader->setMat4("transform", glm::value_ptr(transform));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    }
private:
    std::string m_texturePath1;
    std::string m_texturePath2;
    // 纹理对象
    unsigned int m_texture1 = 0;
    unsigned int m_texture2 = 0;
};

#endif //LEARN_OPENGL_MutiTextureRectangle_H
