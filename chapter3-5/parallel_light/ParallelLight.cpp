//
// Created by TURIING on 2023/11/25.
//

#include "ParallelLight.h"

ParallelLight::ParallelLight(const char *_title): GLObject(_title) {
    this->enableDeepTest();
}

ParallelLight::~ParallelLight() {
    /* 清理并退出 */
    glDeleteVertexArrays(1, &m_objVAO);
    glDeleteBuffers(1, &m_VBO);

    delete m_objShader;

    glfwTerminate();
}

void ParallelLight::init() {
    glfwSetWindowUserPointer(m_window, this);

    // 绑定鼠标移动回调事件
    auto pMouseCallBack = [](GLFWwindow *_window, double _xPosIn, double _yPosIn) {
        static_cast<ParallelLight *>(glfwGetWindowUserPointer(_window))->mouseCallBack(_window, _xPosIn, _yPosIn);
    };
    glfwSetCursorPosCallback(m_window, pMouseCallBack);

    // 绑定鼠标滚动回调事件
    auto pScrollCallBack = [](GLFWwindow *_window, double _xOffset, double _yOffset) {
        static_cast<ParallelLight *>(glfwGetWindowUserPointer(_window))->scrollCallBack(_window, _xOffset, _yOffset);
    };
    glfwSetScrollCallback(m_window, pScrollCallBack);

    // 捕获鼠标
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLObject::init();
}

void ParallelLight::initVaoAndVbo() {
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

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, m_verticesStride * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //endregion
}

void ParallelLight::paint() {
    assert(!m_cubePos.empty());

    //region 绘制物体
    m_objShader->use();
    m_objShader->setVec3("viewPos", m_camera.getPosition());

    m_objShader->setVec3("light.direction", -0.2f, -1.0f, -0.3f);
    m_objShader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    m_objShader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    m_objShader->setVec3("light.specular",  1.0f, 1.0f, 1.0f);

    m_objShader->setInt("material.diffuse", 0);
    m_objShader->setInt("material.specular", 1);
    m_objShader->setFloat("material.shininess", 32.0f);

    glm::mat4 projection = glm::perspective(glm::radians(m_camera.getFov()), (float)getWidth() / (float)getHeight(), 0.1f, 100.0f);
    glm::mat4 view = m_camera.getViewMatrix();
    m_objShader->setMat4("projection", glm::value_ptr(projection));
    m_objShader->setMat4("view", glm::value_ptr(view));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture_diffuse);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture_specular);

    glBindVertexArray(m_objVAO);
    for(auto i = 0; i < 10; i++) {
        float angle = 20.0f * i;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_cubePos[i]);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        m_objShader->setMat4("model", glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    //endregion
}

void ParallelLight::initShader() {
    m_objShader = new GLShader("../../chapter3-5/parallel_light/obj_vs.glsl", "../../chapter3-5/parallel_light/obj_fs.glsl");
}

void ParallelLight::initTexture() {
    /* 生成纹理1 */
    // 创建纹理对象
    glGenTextures(1, &m_texture_diffuse);

    // 绑定纹理到纹理目标
    glBindTexture(GL_TEXTURE_2D, m_texture_diffuse);

    // 设置环绕方式和应用的纹理轴
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 设置放大/缩小对应的纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    generateTexture(m_texturePath_diffuse);

    /* 生成纹理2 */
    // 创建纹理对象
    glGenTextures(1, &m_texture_specular);

    // 绑定纹理到纹理目标
    glBindTexture(GL_TEXTURE_2D, m_texture_specular);

    // 设置环绕方式和应用的纹理轴
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 设置放大/缩小对应的纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    generateTexture(m_texturePath_specular);
}

void ParallelLight::setTexturePath(const std::string &_pathDiffuse, const std::string &_pathSpecular) {
    m_texturePath_diffuse = std::move(_pathDiffuse);
    m_texturePath_specular = std::move(_pathSpecular);
}

void ParallelLight::setCubePos(const std::vector<glm::vec3> &_cubePos) {
    m_cubePos = _cubePos;
}


