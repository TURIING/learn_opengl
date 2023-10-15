//
// Created by TURIING on 2023/10/14.
//


#include <iostream>
#include "TimerTriangle.h"
#include "GradientTriangle.h"
#include <vector>

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT= 600;

int main() {

#if 1   // 绘制定时换色三角形

    std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.0f,  0.5f, 0.0f  // top
    };

    auto triangle = TimerTriangle(SCR_WIDTH, SCR_HEIGHT, "chapter2-6");

    triangle.setVertices(vertices, 3);
    triangle.setShader("../../chapter2-6/timer_triangle_shader.vs", "../../chapter2-6/timer_triangle_shader.fs");

    triangle.init();
    triangle.loop();

#else   // 绘制颜色渐变三角形

    std::vector<float> vertices = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };

    auto triangle = GradientTriangle(SCR_WIDTH, SCR_HEIGHT, "chapter2-6");

    triangle.setVertices(vertices, 6);
    triangle.setShader("../../chapter2-6/gradient_triangle_shader.vs", "../../chapter2-6/gradient_triangle_shader.fs");

    triangle.init();
    triangle.loop();

#endif
}



