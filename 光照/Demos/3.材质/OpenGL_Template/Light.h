//
//  Header.h
//  OpenGL_Template
//
//  Created by Wicky on 2018/12/23.
//  Copyright © 2018 Wicky. All rights reserved.
//

#ifndef Light_h
#define Light_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const glm::vec3 AMBIENT = glm::vec3(0.1);
const glm::vec3 DIFFUSE = glm::vec3(0.5);
const glm::vec3 SPECULAR = glm::vec3(1.0);

class Light {

public:
    glm::vec3 Position;
    glm::vec3 LightColor;
    glm::vec3 Ambient;
    glm::vec3 AmbientColor;
    glm::vec3 Diffuse;
    glm::vec3 DiffuseColor;
    glm::vec3 Specular;
    glm::vec3 SpecularColor;
    
    Light(glm::vec3 lightColor = glm::vec3(1.0f),glm::vec3 worldPosition = glm::vec3(0.0f)) : Ambient(AMBIENT),Diffuse(DIFFUSE),Specular(SPECULAR) {
        Position = worldPosition;
        updateLightColor(lightColor);
    }
    
    void updateAmbient(glm::vec3 ambient) {
        Ambient = ambient;
        AmbientColor = LightColor * ambient;
    }
    
    void updateDiffuse(glm::vec3 diffuse) {
        Diffuse = diffuse;
        DiffuseColor = LightColor * diffuse;
    }
    
    void updateSpecular(glm::vec3 specular) {
        Specular = specular;
        SpecularColor = LightColor * specular;
    }
    
    void updateLightColor(glm::vec3 lightColor) {
        LightColor = lightColor;
        AmbientColor = lightColor * Ambient;
        DiffuseColor = lightColor * Diffuse;
        SpecularColor = lightColor * Specular;
    }
};



#endif /* Header_h */

