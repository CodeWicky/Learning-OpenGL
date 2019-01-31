//
//  Model.h
//  OpenGL_Template
//
//  Created by Wicky on 2019/1/30.
//  Copyright © 2019 Wicky. All rights reserved.
//

#ifndef Model_h
#define Model_h

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Model {
public:
    Model(string modelPath = "") {
        if (modelPath.size() == 0) {
            return;
        }
        loadModel(modelPath);
    }
    
    void Draw(Shader shader) {
        
    }
    
private:
    
    vector<Mesh> meshes;
    string directory;
    
    
    void loadModel(string modelPath) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
            return;
        }
        directory = modelPath.substr(0, modelPath.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    }
    
    void processNode (aiNode * node,const aiScene * scene) {
        // 处理节点所有的网格（如果有的话）
        for(unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // 接下来对它的子节点重复这一过程
        for(unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }
    
    Mesh processMesh (aiMesh * mesh,const aiScene * scene) {
        vector<Mesh_Vertex> vertices;
        vector<unsigned int> indices;
        vector<Mesh_Texture> textures;
        
        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Mesh_Vertex vertex;
            ///处理顶点位置、法线和纹理坐标
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            
            if(mesh->mTextureCoords[0]) {// 网格是否有纹理坐标？
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            } else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
            
            vertices.push_back(vertex);
        }
        ///处理索引
        for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }
        
        ///处理材质
        if(mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Mesh_Texture> diffuseMaps = loadMaterialTextures(material,
                                                               aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            vector<Mesh_Texture> specularMaps = loadMaterialTextures(material,
                                                                aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
        
        return Mesh(vertices, indices, textures);
    }
    
    vector<Mesh_Texture> loadMaterialTextures(aiMaterial * mat,aiTextureType type,string typeName) {
        vector<Mesh_Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            Mesh_Texture texture;
            texture.t_id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            textures.push_back(texture);
        }
        return textures;
    }
    
    unsigned int TextureFromFile(const char *path, const string &directory) {
        string filename = string(path);
        filename = directory + '/' + filename;
        
        unsigned int textureID;
        glGenTextures(1, &textureID);
        
        int width, height, nrComponents;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format = 0;
            if (nrComponents == 1) {
                format = GL_RED;
            } else if (nrComponents == 3) {
                format = GL_RGB;
            } else if (nrComponents == 4) {
                format = GL_RGBA;
            }
            
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            stbi_image_free(data);
        } else {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }
        
        return textureID;
    }
    
};

#endif /* Model_h */
