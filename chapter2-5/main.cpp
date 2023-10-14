//
// Created by TURIING on 2023/10/14.
//


#include <iostream>
#include "GLObject.h"
#include <vector>

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT= 600;

int main() {
    std::vector<float> vertices = {-0.5f, -0.5f, 0.0f,
                                   0.5f, -0.5f, 0.0f,
                                   0.0f,  0.5f, 0.0f };

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

    auto gl = GLObject(SCR_WIDTH, SCR_HEIGHT, "chapter2-5");

    gl.setVertices(vertices, 3);
    gl.setVertexShaderSource(vertexShaderSource);
    gl.setFragmentShaderSource(fragmentShaderSource);

    gl.init();
    gl.loop();
}



