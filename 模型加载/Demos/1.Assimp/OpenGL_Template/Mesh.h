//
//  Mesh.h
//  OpenGL_Template
//
//  Created by Wicky on 2019/1/21.
//  Copyright © 2019 Wicky. All rights reserved.
//

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
    
    void Draw(Shader shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (int i = 0; i < textures.size(); ++i) {
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
            shader.setInt((type + number), i);
            glBindTexture(GL_TEXTURE_2D,textures[i].t_id);
        }
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
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


