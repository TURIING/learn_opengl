//
// Created by TURIING on 2023/10/14.
//


#include <iostream>
#include "TransformTextureRectangle.h"
#include <vector>

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT= 600;

int main() {
    /* 随时间转动的贴图矩形 */
    std::vector<float> vertices = {
        //     ---- 位置 ----  - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // 左上
    };

    std::vector<unsigned int> indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    auto rectangle = InclinedPlane(SCR_WIDTH, SCR_HEIGHT, "chapter2-8");

    rectangle.setVertices(vertices, 5);
    rectangle.setIndices(indices);
    rectangle.setTexturePath("../../Resources/texture/container.jpg", "../../Resources/texture/awesomeface.png");
    rectangle.setShader("../../chapter2-8/trans_texture_rectangle.vs", "../../chapter2-8/trans_texture_rectangle.fs");

    rectangle.init();
    rectangle.loop();


}



