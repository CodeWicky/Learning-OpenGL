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

class Model {
public:
    Model(const GLchar * modelPath = "") {
        if (strcmp(modelPath, "") == 0) {
            return;
        }
        loadModel(modelPath);
    }
    
    void Draw(Shader shader) {
        
    }
    
private:
    
    vector<Mesh> meshes;
    string directory;
    
    
    void loadModel(const GLchar * modelPath) {
        
    }
    
    void processNode (aiNode * node,const aiScene * scene) {
        
    }
    
    Mesh processMesh (aiMesh * mesh,const aiScene * scene) {
        return Mesh();
    }
    
    vector<Mesh_Texture> loadMaterialTextures(aiMaterial * mat,aiTextureType type,string typeName) {
        return vector<Mesh_Texture>();
    }
};

#endif /* Model_h */
