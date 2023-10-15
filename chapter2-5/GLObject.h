//
// Created by TURIING on 2023/10/14.
//

#ifndef LEARN_OPENGL_GLOBJECT_H
#define LEARN_OPENGL_GLOBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <assert.h>

class GLObject {
public:
    GLObject(int _scrWidth, int _scrHeight, const char *_title): m_width(_scrWidth), m_height(_scrHeight), m_windowTitle(_title) {
        initGL();
    }

    virtual ~GLObject(){};

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
        initVertexShader();
        initFragmentShader();
        initShaderProgram();

        initVAO();
        initVBO();
        initEBO();
    }

    void setVertices(std::vector<float> &_vertices, int _stride) {
        m_vertices = std::move(_vertices);
        m_verticesStride = _stride;
    }
    void setIndices(std::vector<unsigned int> &_indices) { m_indices = std::move(_indices); }
    void setVertexShaderSource(const char *_shader) { m_vertexShaderSource = _shader; }
    void setFragmentShaderSource(const char *_shader) { m_fragmentShaderSource = _shader; }
    void setClearColor(float _r, float _g, float _b, float _a) {
        m_clearColor[0] = _r;
        m_clearColor[1] = _g;
        m_clearColor[2] = _b;
        m_clearColor[3] = _a;
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

    const char *m_vertexShaderSource = nullptr;                                                   // 顶点着色器源码
    unsigned int m_vertexShader = 0;                                                              // 顶点着色器对象

    const char *m_fragmentShaderSource = nullptr;                                                 // 顶点着色器源码
    unsigned int m_fragmentShader = 0;                                                            // 顶点着色器对象

    unsigned int m_shaderProgram = 0;                                                             // 着色器程序

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
     * 初始化顶点着色器
     */
    void initVertexShader() {
        assert(m_vertexShaderSource != nullptr);

        m_vertexShader = glCreateShader(GL_VERTEX_SHADER);                                         // 创建顶点着色器对象
        glShaderSource(m_vertexShader, 1, &m_vertexShaderSource, nullptr);                         // 将源码绑定到着色器上
        glCompileShader(m_vertexShader);                                                           // 编译顶点着色器

        int success;
        char info[512];
        glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &success);                                 // 获取编译结果
        if(!success) {
            glGetShaderInfoLog(m_vertexShader, 512, nullptr, info);
            throw info;
        }
    }

    /*
     * 初始化片段着色器
     */
    void initFragmentShader() {
        assert(m_fragmentShaderSource != nullptr);

        m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);                                       // 创建顶点着色器对象
        glShaderSource(m_fragmentShader, 1, &m_fragmentShaderSource, nullptr);                       // 将源码绑定到着色器上
        glCompileShader(m_fragmentShader);                                                           // 编译顶点着色器

        int success;
        char info[512];
        glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &success);                                 // 获取编译结果
        if(!success) {
            glGetShaderInfoLog(m_fragmentShader, 512, nullptr, info);
            throw info;
        }
    }

    /*
     * 初始化着色器程序
     */
    void initShaderProgram() {
        // 创建着色器程序
        m_shaderProgram = glCreateProgram();

        // 附加着色器到着色器程序上
        glAttachShader(m_shaderProgram, m_vertexShader);
        glAttachShader(m_shaderProgram, m_fragmentShader);

        // 链接着色器程序
        glLinkProgram(m_shaderProgram);

        // 检查链接错误
        int success;
        char info[512];
        glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(m_shaderProgram, 512, nullptr, info);
            throw info;
        }

        // 删除着色器对象
        glDeleteShader(m_vertexShader);
        glDeleteShader(m_fragmentShader);
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
