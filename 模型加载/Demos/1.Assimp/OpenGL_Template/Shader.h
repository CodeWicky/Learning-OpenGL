//
//  Shader.hpp
//  OpenGL_Template
//
//  Created by Wicky on 2018/10/28.
//  Copyright © 2018 Wicky. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
    public:
    ///着色器程序ID
    unsigned int ID;
    ///构造器读取着色器
    Shader(const GLchar * vertexPath = "",const GLchar * fragmentPath = "") {
        if (strcmp(vertexPath, "") == 0 || strcmp(fragmentPath, "") == 0) {
            return;
        }
        
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            ///打开文件
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            ///读取文件
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            ///关闭文件
            vShaderFile.close();
            fShaderFile.close();
            ///获取源码
            vertexCode   = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        ///获取c字符串
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        ///编译
        unsigned int vertex, fragment;
        ///顶点着色器
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        ///片段着色器
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        ///着色器程序
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        ///释放着色器
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    
    ///激活主色器程序
    void use() {
        glUseProgram(ID);
    }
    
    ///uniform setter
    void setBool(const std::string &name ,GLboolean value) const {
        glUniform1i(glGetUniformLocation(ID,name.c_str()),(GLint)value);
    }
    void setInt(const std::string &name ,GLint value) const {
        glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
    }
    void setFloat(const std::string &name ,GLfloat value) const {
        glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
    }
    void setVec3f(const std::string &name ,glm::vec3 vec) const {
        glUniform3f(glGetUniformLocation(ID,name.c_str()),vec.x,vec.y,vec.z);
    }
    void setVec4f(const std::string &name ,glm::vec4 vec) const {
        glUniform4f(glGetUniformLocation(ID,name.c_str()),vec.x,vec.y,vec.z,vec.w);
    }
    void setMtx4fv(const std::string &name ,glm::mat4 mat) {
        glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()),1,GL_FALSE,glm::value_ptr(mat));
    }
    
private:
    ///检查编译错误
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

#endif /* Shader_hpp */
