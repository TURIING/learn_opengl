//
// Created by TURIING on 2023/11/25.
//

#include "MaterialLight.h"

MaterialLight::MaterialLight(const char *_title): GLObject(_title) {
    this->enableDeepTest();
}

MaterialLight::~MaterialLight() {
    /* 清理并退出 */
    glDeleteVertexArrays(1, &m_objVAO);
    glDeleteVertexArrays(1, &m_lightVAO);
    glDeleteBuffers(1, &m_VBO);

    delete m_objShader;
    delete m_lightShader;

    glfwTerminate();
}

void MaterialLight::init() {
    glfwSetWindowUserPointer(m_window, this);

    // 绑定鼠标移动回调事件
    auto pMouseCallBack = [](GLFWwindow *_window, double _xPosIn, double _yPosIn) {
        static_cast<MaterialLight *>(glfwGetWindowUserPointer(_window))->mouseCallBack(_window, _xPosIn, _yPosIn);
    };
    glfwSetCursorPosCallback(m_window, pMouseCallBack);

    // 绑定鼠标滚动回调事件
    auto pScrollCallBack = [](GLFWwindow *_window, double _xOffset, double _yOffset) {
        static_cast<MaterialLight *>(glfwGetWindowUserPointer(_window))->scrollCallBack(_window, _xOffset, _yOffset);
    };
    glfwSetScrollCallback(m_window, pScrollCallBack);

    // 捕获鼠标
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLObject::init();
}

void MaterialLight::initVaoAndVbo() {
    assert(!m_vertices.empty());

    //region 初始化物体VAO
    glGenVertexArrays(1, &m_objVAO);
    glBindVertexArray(m_objVAO);
    //endregion

    //region 初始化物体、光源VBO
    // 生成VBO对象
    glGenBuffers(1, &m_VBO);

    // 绑定缓冲类型
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // 将顶点数据复制到缓冲内存中
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

    // 解释如何解析顶点位置数据并启用位置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_verticesStride * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, m_verticesStride * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //endregion

    //region 初始化光源VAO
    glGenVertexArrays(1, &m_lightVAO);
    glBindVertexArray(m_lightVAO);
    //endregion

    //region 再次绑定VBO到光源
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_verticesStride * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //endregion
}

void MaterialLight::paint() {
    m_objShader->use();
    m_objShader->setVec3("viewPos", m_camera.getPosition());

    m_objShader->setVec3("light.position", m_lightPos);
    m_objShader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    m_objShader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    m_objShader->setVec3("light.specular",  1.0f, 1.0f, 1.0f);

    m_objShader->setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    m_objShader->setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
    m_objShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    m_objShader->setFloat("material.shininess", 32.0f);

    glm::mat4 projection = glm::perspective(glm::radians(m_camera.getFov()), (float)getWidth() / (float)getHeight(), 0.1f, 100.0f);
    glm::mat4 view = m_camera.getViewMatrix();
    m_objShader->setMat4("projection", glm::value_ptr(projection));
    m_objShader->setMat4("view", glm::value_ptr(view));

    glm::mat4 model = glm::mat4(1.0f);
    m_objShader->setMat4("model", glm::value_ptr(model));

    glBindVertexArray(m_objVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_lightShader->use();
    m_lightShader->setMat4("projection", glm::value_ptr(projection));
    m_lightShader->setMat4("view", glm::value_ptr(view));
    model = glm::mat4(1.0f);
    model = glm::translate(model, m_lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    m_lightShader->setMat4("model", glm::value_ptr(model));

    glBindVertexArray(m_lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void MaterialLight::initShader() {
    m_objShader = new GLShader("../../chapter3-3/obj_vs.glsl", "../../chapter3-3/obj_fs.glsl");
    m_lightShader = new GLShader("../../chapter3-3/light_vs.glsl", "../../chapter3-3/light_fs.glsl");
}


