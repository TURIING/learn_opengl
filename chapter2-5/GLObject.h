//
// Created by TURIING on 2023/10/14.
//

#ifndef LEARN_OPENGL_GLOBJECT_H
#define LEARN_OPENGL_GLOBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <assert.h>
#include <string>
#include <GLShader.h>

class GLObject {
public:
    GLObject(int _scrWidth, int _scrHeight, const char *_title): m_width(_scrWidth), m_height(_scrHeight), m_windowTitle(_title) {
        initGL();
    }

    virtual ~GLObject(){
        delete m_shader;
    };

    void loop() {
        /* 循环渲染 */
        while (!glfwWindowShouldClose(m_window)) {
            processInput(m_window);

            glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);       // 设置清空屏幕所用的颜色
            glClear(GL_COLOR_BUFFER_BIT);                                                           // 清空屏幕的颜色缓冲

            paint();

            glfwSwapBuffers(m_window);                                                              // 交换颜色缓冲
            glfwPollEvents();                                                                       // 检查是否触发事件
        }
    }

    void init() {
        initVAO();
        initVBO();
        initEBO();

        initShader();
    }

    void setVertices(std::vector<float> &_vertices, int _stride) {
        m_vertices = std::move(_vertices);
        m_verticesStride = _stride;
    }
    void setIndices(std::vector<unsigned int> &_indices) { m_indices = std::move(_indices); }
    void setClearColor(float _r, float _g, float _b, float _a) {
        m_clearColor[0] = _r;
        m_clearColor[1] = _g;
        m_clearColor[2] = _b;
        m_clearColor[3] = _a;
    }
    void setShader(std::string _vertexPath, std::string _fragmentPath) {
        m_vertexPath = std::move(_vertexPath);
        m_fragmentPath = std::move(_fragmentPath);
    }
private:
    GLFWwindow *m_window = nullptr;
    const char *m_windowTitle = nullptr;
    int m_width;
    int m_height;

    float m_clearColor[4] = { 0.26f, 0.30f, 0.31f, 1.0f };                                        // 清屏颜色

protected:
    unsigned int m_VAO = 0;

    std::vector<float> m_vertices;                                                                // 顶点坐标数组
    int m_verticesStride = 0;
    unsigned int m_VBO = 0;

    unsigned int m_EBO = 0;
    std::vector<unsigned int> m_indices;                                                          // 顶点索引数组

    /* 着色器 */
    // 着色器文件路径
    std::string m_vertexPath;
    std::string m_fragmentPath;
    // 着色器对象
    GLShader *m_shader;

private:
    void initGL() {
        /* 初始化GLFW */
        glfwInit();                                                                               // 初始化GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                                            // 设置主版本号和次版本号
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);                            // 设置使用核心模式

        /* 创建窗口 */
        m_window = glfwCreateWindow(m_width, m_height, m_windowTitle, nullptr, nullptr);
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
     * 初始化着色器
     */
    void initShader() {
        assert(!m_vertexPath.empty());
        assert(!m_fragmentPath.empty());

        m_shader = new GLShader(m_vertexPath.c_str(), m_fragmentPath.c_str());
    }

    /*
     * 窗口尺寸改变，视口回调函数
     */
    static void framebuffer_size_callback(GLFWwindow *_window, int _width, int _height) {
        // 告诉OpenGL渲染窗口的尺寸大小
        glViewport(0, 0, _width, _height);
    }

    /*
     * 检测按键是否被按下，以及处理按键事件
     */
    static void processInput(GLFWwindow *_window) {
        if(glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(_window, true);
        }
    }

protected:
    virtual void initVAO() = 0;

    /*
     * 初始化VBO
     */
    virtual void initVBO() = 0;

    /*
     * 初始化EBO
     */
    virtual void initEBO() {};

    /*
     * 绘制图形
     */
    virtual void paint() = 0;
};

#endif //LEARN_OPENGL_GLOBJECT_H
