//
// Created by TURIING on 2023/10/14.
//

#ifndef LEARN_OPENGL_TextureRectangle_H
#define LEARN_OPENGL_TextureRectangle_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cassert>
#include "GLObject.h"
#include <string>

class TextureRectangle: public GLObject{
public:
    TextureRectangle(int _scrWidth, int _scrHeight, const char *_title): GLObject(_scrWidth, _scrHeight, _title) {

    }

    ~TextureRectangle() override {
        /* 清理并退出 */
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);

        glfwTerminate();
    }

    void setTexturePath(const std::string &_texturePath) {
        m_texturePath = _texturePath;
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

        // 解释如何解析顶点颜色数据并启用颜色顶点属性
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, m_verticesStride * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // 解释如何解析顶点纹理数据并启用纹理顶点属性
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, m_verticesStride * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
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

        // 创建纹理对象
        glGenTextures(1, &m_texture);

        // 绑定纹理到纹理目标
        glBindTexture(GL_TEXTURE_2D, m_texture);

        // 设置环绕方式和应用的纹理轴
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        // 设置放大/缩小对应的纹理过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 加载纹理图片
        int width, height, nrChannels;
        unsigned char *data = stbi_load(m_texturePath.c_str(), &width, &height, &nrChannels, 0);
        assert(data != nullptr);

        // 生成纹理
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        // 生成多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);

        // 释放图像
        stbi_image_free(data);
    }

    /*
     * 绘制图形
     */
    void paint() override {
        // 使用着色器程序
        m_shader->use();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
private:
    std::string m_texturePath;

    unsigned int m_texture = 0;
};

#endif //LEARN_OPENGL_TextureRectangle_H
