//
// Created by TURIING on 2023/11/25.
//

#ifndef LEARN_OPENGL_MATERIALLIGHT_H
#define LEARN_OPENGL_MATERIALLIGHT_H

#include "GLObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MaterialLight: public GLObject {
public:
    MaterialLight(const char *_title);
    ~MaterialLight() override;

    void init() override;

protected:
    void initVaoAndVbo() override;
    void paint() override;
    void initShader() override;
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
};


#endif //LEARN_OPENGL_MATERIALLIGHT_H
