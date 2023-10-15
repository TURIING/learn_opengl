//
// Created by TURIING on 2023/10/15.
//

#ifndef LEARN_OPENGL_GLSHADER_H
#define LEARN_OPENGL_GLSHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include "glad/glad.h"

class GLShader {
public:
    GLShader(const char *_vertexPath, const char *_fragmentPath) {
        /* 从文件读取着色器代码 */
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile, fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vShaderFile.open(_vertexPath);
            std::stringstream vShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            vShaderFile.close();
            vertexCode = vShaderStream.str();

            fShaderFile.open(_fragmentPath);
            std::stringstream fShaderStream;
            fShaderStream << fShaderFile.rdbuf();
            fShaderFile.close();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure &_e) {
            throw "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ";
        }

        const char *vertexShaderCode = vertexCode.c_str();
        const char *fragmentShaderCode = fragmentCode.c_str();

        /* 创建着色器对象 */
        // 创建顶点着色器
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader, VERTEX);

        // 创建片段着色器
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader, FRAGMENT);

        // 创建着色器程序
        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);
        checkCompileErrors(ID, PROGRAM);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ~GLShader() {
        glDeleteProgram(ID);
    }

    void use() const { glUseProgram(ID); }

    void setBool(const std::string &_name, bool _value) const {
        glUniform1i(glGetUniformLocation(ID, _name.c_str()), (int)_value);
    }

    void setInt(const std::string &_name, int _value) const {
        glUniform1i(glGetUniformLocation(ID, _name.c_str()), _value);
    }

    void setFloat(const std::string &_name, float _value) const {
        glUniform1f(glGetUniformLocation(ID, _name.c_str()), _value);
    }

public:
    unsigned int ID = 0;
private:
    enum CHECK_TYPE {
        PROGRAM = 0,
        VERTEX,
        FRAGMENT
    };

private:
    static void checkCompileErrors(unsigned int shader, CHECK_TYPE type)
    {
        int success;
        char infoLog[1024];
        if (type != PROGRAM)
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                throw "ERROR::SHADER_COMPILATION_ERROR of type";
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                throw "ERROR::PROGRAM_LINKING_ERROR of type";
            }
        }
    }
};

#endif //LEARN_OPENGL_GLSHADER_H
