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
    
    ///构造方法
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
    
    ///朝指定方向移动
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
    
    ///设置移动速度
    void setSpeed(float speed = SPEED) {
        MovementSpeed = speed;
    }
    
    ///设置鼠标灵敏度
    void setSensityvity(float sensityvity = SENSITYVITY) {
        MouseSensityvity = sensityvity;
    }
    
    ///设置俯仰角
    void setPitch(float pitch) {
        Pitch = pitch;
    }
    
    ///设置偏航角
    void setYaw(float yaw) {
        Yaw = yaw;
    }
    
    ///设置世界坐标系中的上向量
    void setWorldUp (glm::vec3 worldUp) {
        WorldUp = worldUp;
    }
    
    ///改变俯仰角、偏航角或世界坐标上向量后需要更新摄像机的方向向量
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
        front.y = sin(glm::radians(Pitch));
        front.z = -cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
        Front = front;
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
    
    ///设置默认的俯仰角
    void setDefaultPicth(float pitch) {
        DefaultPitch = pitch;
    }
    
    ///设置默认的偏航角
    void setDefaultYaw(float yaw) {
        DefaultYaw = yaw;
    }
    
    ///设置默认的摄像机位置
    void setDefaultOrigin(glm::vec3 camera) {
        DefaultOrigin = camera;
    }
    
    ///设置默认的世界坐标上向量
    void setDefaultWorldUp(glm::vec3 worldUp) {
        DefaultWorldUp = worldUp;
    }
    
    ///获取当前的观察矩阵
    glm::mat4 getViewMatrix() {
        return glm::lookAt(Position, Position + Front, WorldUp);
    }
    
    ///重置摄像机至默认状态
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
};

#endif /* Camera_h */
