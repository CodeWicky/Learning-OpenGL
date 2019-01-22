//
//  Mesh.h
//  OpenGL_Template
//
//  Created by Wicky on 2019/1/21.
//  Copyright © 2019 Wicky. All rights reserved.
//

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

struct Mesh_Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

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
    
private:
    unsigned int VBO,EBO;
    
    void setupMesh(){
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glGenBuffers(1,&EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Mesh_Vertex),&vertices[0],GL_STATIC_DRAW);
    }
};


