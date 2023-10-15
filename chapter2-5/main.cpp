//
// Created by TURIING on 2023/10/14.
//


#include <iostream>
#include "Triangle.h"
#include "Rectangle.h"
#include <vector>

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT= 600;

int main() {
#if 1   // 绘制三角形
    std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.0f,  0.5f, 0.0f  // top
    };

    auto triangle = Triangle(SCR_WIDTH, SCR_HEIGHT, "chapter2-5");

    triangle.setVertices(vertices, 3);
    triangle.setShader("../../chapter2-5/shader.vs", "../../chapter2-5/shader.fs");

    triangle.init();
    triangle.loop();

#else   // 绘制矩形
    std::vector<float> vertices = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    std::vector<unsigned int> indices = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    auto rectangle = Rectangle(SCR_WIDTH, SCR_HEIGHT, "chapter2-5");

    rectangle.setVertices(vertices, 3);
    rectangle.setIndices(indices);
    rectangle.setShader("../../chapter2-5/shader.vs", "../../chapter2-5/shader.fs");

    rectangle.init();
    rectangle.loop();
#endif
}



