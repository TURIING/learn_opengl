//
// Created by TURIING on 2023/10/14.
//

#ifndef LEARN_OPENGL_GLOBJECT_H
#define LEARN_OPENGL_GLOBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLObject {
public:
    GLObject(int _scrWidth, int _scrHeight): m_width(_scrWidth), m_height(_scrHeight) {
        init();
    }

    ~GLObject() {
        /* 清理并退出 */
        glfwTerminate();
    }

    void loop() {
        /* 循环渲染 */
        while (!glfwWindowShouldClose(m_window)) {
            processInput(m_window);

            glClearColor(0.26f, 0.30f, 0.31f, 1.0f);                                                // 设置清空屏幕所用的颜色
            glClear(GL_COLOR_BUFFER_BIT);                                                           // 清空屏幕的颜色缓冲

            glfwSwapBuffers(m_window);                                                              // 交换颜色缓冲
            glfwPollEvents();                                                                       // 检查是否触发事件
        }
    }
private:
    GLFWwindow *m_window = nullptr;
    int m_width;
    int m_height;

private:
     void init() {
        /* 初始化GLFW */
        glfwInit();                                                                               // 初始化GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                                            // 设置主版本号和次版本号
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);                            // 设置使用核心模式

        /* 创建窗口 */
        m_window = glfwCreateWindow(m_width, m_height, "chapter2-4", nullptr, nullptr);
        if(!m_window) {
            glfwTerminate();
            throw "Failed to create GLFW window";
        }
        glfwMakeContextCurrent(m_window);                                                            // 将窗口的上下文设置为当前线程的主上下文
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

        /* 初始化GLAD */
        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            throw "Failed to initialize GLAD";
        }
    }

    /*
     * 窗口尺寸改变，视口回调函数
     */
    static void framebuffer_size_callback(GLFWwindow *_window, int _width, int _height) {
        glViewport(0, 0, _width, _height);                                                                 // 告诉OpenGL渲染窗口的尺寸大小
    }

    /*
     * 检测按键是否被按下，以及处理按键事件
     */
    static void processInput(GLFWwindow *_window) {
        if(glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(_window, true);
        }
    }
};

#endif //LEARN_OPENGL_GLOBJECT_H
