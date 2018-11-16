//
//  Camera.h
//  OpenGL_Template
//
//  Created by Wicky on 2018/11/16.
//  Copyright © 2018 Wicky. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float PITCH = 0.f;
const float YAW = -90.f;
const float SPEED = 10.f;
const float SENSITYVITY = 0.1f;

class Camera {

public:
    glm::vec3 Position;
    
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    
    float Pitch = -90.f;
    float Yaw = 0.f;
    
    float MovementSpeed = 10.f;
    float MouseSensityvity = 0.1f;
    
    Camera(glm::vec3 position = glm::vec3(0.f,0.f,0.f),glm::vec3 up = glm::vec3(0.f,1.f,0.f),float pitch = PITCH,float yaw = YAW) : MovementSpeed(SPEED),MouseSensityvity(SENSITYVITY) {
        Position = position;
        WorldUp = up;
        Pitch = pitch;
        Yaw = yaw;
        
    }
    
private:
    void updateCameraVectors() {
        glm::vec3 front = glm::vec3(1.0f);
        front.y = cos(glm::radians(Yaw));
        front.x = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * sin(glm::radians(Pitch));
    }
};

#endif /* Camera_h */
