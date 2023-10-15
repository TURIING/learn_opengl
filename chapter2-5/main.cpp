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
    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\n\0";

#if 0   // 绘制三角形
    std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.0f,  0.5f, 0.0f  // top
    };

    auto triangle = Triangle(SCR_WIDTH, SCR_HEIGHT, "chapter2-5");

    triangle.setVertices(vertices, 3);
    triangle.setVertexShaderSource(vertexShaderSource);
    triangle.setFragmentShaderSource(fragmentShaderSource);

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
    rectangle.setVertexShaderSource(vertexShaderSource);
    rectangle.setFragmentShaderSource(fragmentShaderSource);

    rectangle.init();
    rectangle.loop();
#endif
}



