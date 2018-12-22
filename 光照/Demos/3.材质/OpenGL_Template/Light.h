//
//  Light.h
//  OpenGL_Template
//
//  Created by Wicky on 2018/11/22.
//  Copyright © 2018 Wicky. All rights reserved.
//

#ifndef Light_h
#define Light_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float AMBIENTSTRENGTH = 0.1f;
const float SPECULARSTRENGTH = 0.5f;

class Light {
    
public :
    
    glm::vec3 Color;
    glm::vec3 Position;
    
    float AmbientStrength;
    float SpecularStrength;
    
    Light(glm::vec3 lightColor = glm::vec3(1.0f),glm::vec3 worldPosition = glm::vec3(0.0f)) : AmbientStrength(AMBIENTSTRENGTH),SpecularStrength(SPECULARSTRENGTH) {
        Position = worldPosition;
        Color = lightColor;
    }
    
};

#endif /* Light_h */
