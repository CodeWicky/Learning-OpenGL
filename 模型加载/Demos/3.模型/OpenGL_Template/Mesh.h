//
//  Mesh.h
//  OpenGL_Template
//
//  Created by Wicky on 2019/1/21.
//  Copyright © 2019 Wicky. All rights reserved.
//

#ifndef Mesh_h
#define Mesh_h

///system framework
#include <vector>

///third party framework
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

///custom framework
#include "Shader.h"

using namespace std;

struct Mesh_Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

///纹理结构体（标明已经加载的纹理的纹理ID及纹理对应类型）
struct Mesh_Texture {
    unsigned int t_id;
    string type;
    string path;
};

class Mesh {
public:
    
    vector<Mesh_Vertex> vertices;
    vector<unsigned int> indices;
    vector<Mesh_Texture> textures;
    unsigned int VAO;
    
    Mesh(vector<Mesh_Vertex> aVertices, vector<unsigned int> aIndices, vector<Mesh_Texture> aTextures) {
        vertices = aVertices;
        indices = aIndices;
        textures = aTextures;
        setupMesh();
    }
    
    Mesh() {
        
    }
    
    void Draw(Shader shader) {
        ///用四个变量来记录不同纹理类型已经存在的个数，这样做的好处是可以在循环中为不同类型的问题自动赋值给片段着色器，故片段着色器中不同纹理的命名应遵循如下命名规则，如第一个漫反射纹理应为：texture_diffuse1
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (int i = 0; i < textures.size(); ++i) {
            ///首先激活指定位置的纹理单元
            glActiveTexture(GL_TEXTURE0 + i);
            string number;
            string type = textures[i].type;
            if (type == "texture_diffuse") {
                number = to_string(diffuseNr++);
            } else if (type == "texture_specular") {
                number = to_string(specularNr++);
            } else if (type == "texture_normal") {
                number = to_string(normalNr++);
            } else if (type == "texture_height") {
                number = to_string(heightNr++);
            }
            ///将指定位置的纹理单元指定给片段着色器
            shader.setInt((type + number), i);
            ///数组中为纹理结构体，纹理结构体中均为已经加载到gl中的纹理对应的t_id及本纹理的文理类型。故我们在当前激活的纹理单元上绑定之前加载的纹理t_id后，当前激活的纹理单元中的数据即指向先前提交到gl的纹理数据，故而片段着色器中对应纹理也指向该纹理数据
            glBindTexture(GL_TEXTURE_2D,textures[i].t_id);
        }
        ///循环结束后，即将所有传入的纹理数据都绑定在不同的纹理单元内，同时片段着色器中各纹理字段均已对应指定纹理单元，可以开始绘制当前传入纹理，绑定当前顶点数组对象并按索引绘制
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
        ///结束顶点数组对象的绑定
        glBindVertexArray(0);
        ///恢复当前激活的纹理当前元默认的纹理单元（GL_TEXTURE0）。当另一个Mesh开始绘制时，新的Mesh将重新从GL_TEXTURE0开始绑定纹理数据，即会覆盖本次纹理数据。这样做是因为gl中可用的纹理单元是有限的，故而要反复使用纹理单元，在每次使用前均应重新绑定纹理单元中的纹理数据。（当然如果你仅仅使用了不足16个纹理单元，那么你完全可以像之前的教程中所示不必每次重新绑定）
        glActiveTexture(GL_TEXTURE0);
    }
    
private:
    unsigned int VBO,EBO;
    
    void setupMesh(){
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glGenBuffers(1,&EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Mesh_Vertex),&vertices[0],GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Mesh_Vertex),(void *)(offsetof(Mesh_Vertex, Position)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Mesh_Vertex),(void *)(offsetof(Mesh_Vertex, Normal)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Mesh_Vertex),(void *)(offsetof(Mesh_Vertex, TexCoords)));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }
};

#endif


