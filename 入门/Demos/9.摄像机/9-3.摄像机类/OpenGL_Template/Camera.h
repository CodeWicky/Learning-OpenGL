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
const float YAW = 0.f;
const float SPEED = 10.f;
const float SENSITYVITY = 0.1f;

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UPWARD,
    DOWNWARD
};

class Camera {

public:
    glm::vec3 Position;
    
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    
    float Pitch;
    float Yaw;
    
    float MovementSpeed;
    float MouseSensityvity;
    
    Camera(glm::vec3 origin = glm::vec3(0.f,0.f,0.f),glm::vec3 up = glm::vec3(0.f,1.f,0.f),float pitch = PITCH,float yaw = YAW,float speed = SPEED,float sensityvity = SENSITYVITY) {
        Position = origin;
        setDefaultOrigin(origin);
        WorldUp = up;
        setDefaultWorldUp(up);
        Pitch = pitch;
        setDefaultPicth(pitch);
        Yaw = yaw;
        setDefaultPicth(yaw);
        MovementSpeed = speed;
        MouseSensityvity = sensityvity;
        updateCameraVectors();
    }
    
    void move(Camera_Movement movement,float deltaTime) {
        switch (movement) {
            case FORWARD:
            {
                Position += (Front * MovementSpeed * deltaTime);
            }
                break;
            case BACKWARD:
            {
                Position -= (Front * MovementSpeed * deltaTime);
            }
                break;
            case LEFT:
            {
                Position -= (Right * MovementSpeed * deltaTime);
            }
                break;
            case RIGHT:
            {
                Position += (Right * MovementSpeed * deltaTime);
            }
                break;
            case UPWARD:
            {
                Position += (Up * MovementSpeed * deltaTime);
            }
                break;
            case DOWNWARD:
            {
                Position -= (Up * MovementSpeed * deltaTime);
            }
                break;
            default:
                break;
        }
    }
    
    void setSpeed(float speed = SPEED) {
        MovementSpeed = speed;
    }
    
    void setSensityvity(float sensityvity = SENSITYVITY) {
        MouseSensityvity = sensityvity;
    }
    
    void setPitch(float pitch) {
        Pitch = pitch;
        updateCameraVectors();
    }
    
    void setYaw(float yaw) {
        Yaw = yaw;
        updateCameraVectors();
    }
    
    void setWorldUp (glm::vec3 worldUp) {
        WorldUp = worldUp;
        updateCameraVectors();
    }
    
    void setDefaultPicth(float pitch) {
        DefaultPitch = pitch;
    }
    
    void setDefaultYaw(float yaw) {
        DefaultYaw = yaw;
    }
    
    void setDefaultOrigin(glm::vec3 camera) {
        DefaultOrigin = camera;
    }
    
    void setDefaultWorldUp(glm::vec3 worldUp) {
        DefaultWorldUp = worldUp;
    }
    
    glm::mat4 getViewMatrix() {
        return glm::lookAt(Position, Position + Front, WorldUp);
    }
    
    void resetCamera() {
        Position = DefaultOrigin;
        WorldUp = DefaultWorldUp;
        Pitch = DefaultPitch;
        Yaw = DefaultYaw;
        updateCameraVectors();
    }
    
private:
    glm::vec3 DefaultOrigin;
    glm::vec3 DefaultWorldUp;
    float DefaultPitch;
    float DefaultYaw;
    
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
        front.y = sin(glm::radians(Pitch));
        front.z = -cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
        Front = front;
        Right = glm::normalize(glm::cross(WorldUp, Front));
        Up = glm::normalize(glm::cross(Front, Right));
    }
};

#endif /* Camera_h */
