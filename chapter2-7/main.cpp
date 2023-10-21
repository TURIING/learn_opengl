//
// Created by TURIING on 2023/10/14.
//


#include <iostream>
#include "TextureRectangle.h"
#include "MutiTextureRectangle.h"
#include <vector>

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT= 600;

int main() {

#if 0   // 绘制纹理贴图矩形

    std::vector<float> vertices = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    std::vector<unsigned int> indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    auto rectangle = TextureRectangle(SCR_WIDTH, SCR_HEIGHT, "chapter2-7");

    rectangle.setVertices(vertices, 8);
    rectangle.setIndices(indices);
    rectangle.setTexturePath("../../Resources/texture/container.jpg");
    rectangle.setShader("../../chapter2-7/texture_rectangle.vs", "../../chapter2-7/texture_rectangle.fs");

    rectangle.init();
    rectangle.loop();

#else   // 绘制

    std::vector<float> vertices = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    std::vector<unsigned int> indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    auto rectangle = MutiTextureRectangle(SCR_WIDTH, SCR_HEIGHT, "chapter2-7");

    rectangle.setVertices(vertices, 8);
    rectangle.setIndices(indices);
    rectangle.setTexturePath("../../Resources/texture/container.jpg", "../../Resources/texture/awesomeface.png");
    rectangle.setShader("../../chapter2-7/muti_texture_rectangle.vs", "../../chapter2-7/muti_texture_rectangle.fs");

    rectangle.init();
    rectangle.loop();

#endif
}



