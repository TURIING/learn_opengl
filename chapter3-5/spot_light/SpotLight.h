//
// Created by TURIING on 2023/11/25.
//

#ifndef LEARN_OPENGL_SPOTLIGHT_H
#define LEARN_OPENGL_SPOTLIGHT_H

#include "GLObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SpotLight: public GLObject {
public:
    SpotLight(const char *_title);
    ~SpotLight() override;

    void init() override;

    void setTexturePath(const std::string &_pathDiffuse, const std::string &_pathSpecular);

    void setCubePos(const std::vector<glm::vec3> &_cubePos);

protected:
    void initVaoAndVbo() override;
    void paint() override;
    void initShader() override;
    void initTexture() override;
private:
    // 物体VAO
    unsigned int m_objVAO;
    // 物体shader
    GLShader *m_objShader;
    // 光源位置
    glm::vec3 m_lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    // 纹理对象
    unsigned int m_texture_diffuse = 0;
    unsigned int m_texture_specular = 0;
    // 纹理资源路径
    std::string m_texturePath_diffuse;
    std::string m_texturePath_specular;
    // 正方体的位置
    std::vector<glm::vec3> m_cubePos;
};


#endif //LEARN_OPENGL_SpotLight_H
