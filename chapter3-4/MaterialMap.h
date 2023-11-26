//
// Created by TURIING on 2023/11/25.
//

#ifndef LEARN_OPENGL_MATERIALMAP_H
#define LEARN_OPENGL_MATERIALMAP_H

#include "GLObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MaterialMap: public GLObject {
public:
    MaterialMap(const char *_title);
    ~MaterialMap() override;

    void init() override;

    void setTexturePath(const std::string &_pathDiffuse, const std::string &_pathSpecular) {
        m_texturePath_diffuse = std::move(_pathDiffuse);
        m_texturePath_specular = std::move(_pathSpecular);
    }

protected:
    void initVaoAndVbo() override;
    void paint() override;
    void initShader() override;
    void initTexture() override;
private:
    // 物体VAO
    unsigned int m_objVAO;
    // 光源VAO
    unsigned int m_lightVAO;
    // 物体shader
    GLShader *m_objShader;
    //光源shader
    GLShader *m_lightShader;
    // 光源位置
    glm::vec3 m_lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    // 纹理对象
    unsigned int m_texture_diffuse = 0;
    unsigned int m_texture_specular = 0;
    // 纹理资源路径
    std::string m_texturePath_diffuse;
    std::string m_texturePath_specular;
};


#endif //LEARN_OPENGL_MATERIALMAP_H
