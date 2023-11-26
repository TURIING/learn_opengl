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
#include "GLCamera.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class GLObject {
public:
    GLObject(const char *_title, int _scrWidth = 1000, int _scrHeight = 800): m_width(_scrWidth), m_height(_scrHeight), m_windowTitle(_title) {
        initGL();

        m_fLastX = m_width / 2.0f;
        m_fLastY = m_height / 2.0f;
    }

    virtual ~GLObject() = default;

    void loop() {
        preLoop();

        /* 循环渲染 */
        while (!glfwWindowShouldClose(m_window)) {
            auto currentFrame = static_cast<float>(glfwGetTime());
            m_deltaTime = currentFrame - m_lastFrame;
            m_lastFrame = currentFrame;

            processInput(m_window);

            clear();

            paint();

            glfwSwapBuffers(m_window);                                                              // 交换颜色缓冲
            glfwPollEvents();                                                                       // 检查是否触发事件
        }
    }

    /**
     * 设置顶点数组
     * @param _vertices
     * @param _stride 步长
     */
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

    int getWidth() const {
        return m_width;
    }

    int getHeight() const {
        return m_height;
    }

    /*
     * 开启深度测试
     */
    void enableDeepTest() {
        glEnable(GL_DEPTH_TEST);
        m_bDeepTest = true;
    }

    virtual void init() {
        initVaoAndVbo();
        initEBO();

        initShader();
        initTexture();
    }
private:
    const char *m_windowTitle = nullptr;

    float m_clearColor[4] = { 0.26f, 0.30f, 0.31f, 1.0f };                                        // 清屏颜色
    // 是否启用深度测试
    bool m_bDeepTest = false;

protected:
    GLFWwindow *m_window = nullptr;

    int m_width;
    int m_height;

    std::vector<float> m_vertices;                                                                // 顶点坐标数组
    int m_verticesStride = 0;
    unsigned int m_VBO = 0;

    unsigned int m_EBO = 0;
    std::vector<unsigned int> m_indices;                                                          // 顶点索引数组

    // 摄像机对象
    GLCamera m_camera = GLCamera(glm::vec3(0.0f, 0.0f, 4.0f));
    // 上一帧跟当前帧的时间间隔
    float m_deltaTime = 0.0f;
    // 记录当前帧的时间
    float m_lastFrame = 0.0f;
    // 鼠标是否刚被捕获
    bool m_bFirstMouse = true;
    float m_fLastX = 0.0f;
    float m_fLastY = 0.0f;

private:
    /*
     * 初始化环境配置
     */
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
     * 窗口尺寸改变，视口回调函数
     */
    static void framebuffer_size_callback(GLFWwindow *_window, int _width, int _height) {
        // 告诉OpenGL渲染窗口的尺寸大小
        glViewport(0, 0, _width, _height);
    }

    void clear() {
        glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);       // 设置清空屏幕所用的颜色

        if(m_bDeepTest) {
            // 清空屏幕的颜色缓冲及深度缓冲
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }else {
            // 清空屏幕的颜色缓冲
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }

protected:
    /*
     * 初始化着色器
     */
    virtual void initShader() {}

    virtual void initVaoAndVbo() = 0;

    /*
     * 初始化EBO
     */
    virtual void initEBO() {};

    /*
     * 初始化纹理
     */
    virtual void  initTexture() {}

    /*
     * 绘制图形
     */
    virtual void paint() = 0;

    /**
     * 生成纹理
     * @param _resPath 图片资源路径
     * @param _target 纹理目标
     */
    static void generateTexture(const std::string &_resPath) {
        assert(!_resPath.empty());

        // 加载纹理图片
        int width, height;
        int nrChannels;                 // 通道数，表示图像中每个像素包含的颜色通道的数量（灰度图像：1个，RGB 彩色图像：3个，RGBA彩色图像：4个）
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(_resPath.c_str(), &width, &height, &nrChannels, 0);
        if(data == nullptr) {
            std::cout << "纹理图片加载失败" << std::endl;
            exit(-1);
        }
        GLenum format;
        switch (nrChannels) {
            case 1: format = GL_RED;
            case 3: format = GL_RGB;
            case 4: format = GL_RGBA;
        }

        // 生成纹理
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // 生成多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);

        // 释放图像
        stbi_image_free(data);
    }

    /*
     * 检测按键是否被按下，以及处理按键事件
     */
    virtual void processInput(GLFWwindow *_window) {
        if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
            m_camera.processKeyboard(CameraMovement::FORWARD, m_deltaTime);
        if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
            m_camera.processKeyboard(CameraMovement::BAKCWARD, m_deltaTime);
        if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
            m_camera.processKeyboard(CameraMovement::LEFT, m_deltaTime);
        if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
            m_camera.processKeyboard(CameraMovement::RIGHT, m_deltaTime);

        if(glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(_window, true);
        }
    }

    /*
     * 循环渲染前的初始化工作
     */
    virtual void preLoop() {}

    /*
     * 鼠标移动事件回调函数
     */
    virtual void mouseCallBack(GLFWwindow *_window, double _xPosIn, double _yPosIn) {
        auto xpos = static_cast<float>(_xPosIn);
        auto ypos = static_cast<float>(_yPosIn);
        if(m_bFirstMouse) {
            m_fLastX = xpos;
            m_fLastY = ypos;
            m_bFirstMouse = false;
        }

        float xOffset = xpos - m_fLastX;
        float yOffset = m_fLastY - ypos;

        m_fLastX = xpos;
        m_fLastY = ypos;

        m_camera.processMouseMovement(xOffset, yOffset);
    }

    /*
     * 鼠标滚动事件回调函数
     */
    virtual void scrollCallBack(GLFWwindow *_window, double _xOffset, double _yOffset) {
        m_camera.processMouseScroll(static_cast<float>(_yOffset));
    }
};

#endif //LEARN_OPENGL_GLOBJECT_H
