#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include <math.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

///函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
GLFWwindow * configOpenGL();
void loadImg(const char * path,unsigned int * texture,unsigned int uniteLoc);
void configBoxMesh (Mesh * mesh);
void configLightMesh(Mesh * mesh);
void finishiRenderLoop();
float timeIntervalToLastFrame();

///常量
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

///属性
float g = 0.05;
float jumpSpeed = 10;
float currentSpeed = 0;
bool jumping = false;
float lastFrameTS = 0;
bool firstCursor = true;
float lastCursorX = 0;
float lastCursorY = 0;
float fov = 45.f;
Camera camera;
Light moon;
Light lamp;
Light torch;
Shader shader;

int main()
{
    ///配置GL的window及视口
    GLFWwindow * window = configOpenGL();
    
    glEnable(GL_DEPTH_TEST);
    ///模型着色器程序
    Shader ourShader("Vertex.h","Fragment.h");
    shader = ourShader;
    ///光源着色器程序
    Shader lightShader("Vertex.h","LightFragment.h");
    
    ///加载光照贴图
    unsigned int texture,tex_specular;
    loadImg("container2.png", &texture,0);
    loadImg("container2_specular.png", &tex_specular, 1);
    ///摄像机
    camera = Camera();
    camera.setDefaultOrigin(glm::vec3(0.f,0.f,3.f));
    camera.resetCamera();
    
    ///光源属性
    moon = Light(glm::vec3(1.0,0.965,0.56),glm::vec3(10.f,10.f,10.f),glm::vec3(-1.0,-1.0,-1.0));
    lamp = Light(glm::vec3(0.961f,1.f,0.98f),glm::vec3(0.f,0.f,-2.f));
    torch = Light(glm::vec3(1.f,0.412f,0.706f));
    
    ///配置盒子网格
    Mesh box;
    configBoxMesh(&box);
    
    Mesh light;
    configLightMesh(&light);
    
    ///设置模型着色器程序相关参数
    ourShader.use();
    
    ///设置光的属性
    ///设置定向光源初始属性（模拟月光）
    ourShader.setVec3f("directionLight.direction", moon.Direction);
    ourShader.setVec3f("directionLight.ambient", moon.AmbientColor);
    ourShader.setVec3f("directionLight.diffuse", moon.DiffuseColor);
    ourShader.setVec3f("directionLight.specular", moon.SpecularColor);
    ourShader.setFloat("directionLight.constant", 1.0f);
    ourShader.setFloat("directionLight.linear", 0.0f);
    ourShader.setFloat("directionLight.quadratic", 0.0f);

    ///设置点光源初始属性（模拟灯泡）
    ourShader.setVec3f("pointLight.position", lamp.Position);
    ourShader.setVec3f("pointLight.ambient", lamp.AmbientColor);
    ourShader.setVec3f("pointLight.diffuse", lamp.DiffuseColor);
    ourShader.setVec3f("pointLight.specular", lamp.SpecularColor);
    ourShader.setFloat("pointLight.constant", 1.0f);
    ourShader.setFloat("pointLight.linear", 0.09f);
    ourShader.setFloat("pointLight.quadratic", 0.032f);
    
    ///torch Positions
    glm::vec3 torch_positions[] = {
        glm::vec3(-6.f,0.f,-2.f),
        glm::vec3(6.f,0.f,-2.f),
        glm::vec3(0.f,-6.f,-2.f),
        glm::vec3(0.f,6.f,-2.f),
        glm::vec3(0.f,0.f,2.f),
        glm::vec3(0.f,0.f,-6.f),
    };
    ///torch Direction
    glm::vec3 torch_directions[] = {
        glm::vec3(1.f,0.f,0.f),
        glm::vec3(-1.f,0.f,0.f),
        glm::vec3(0.f,1.f,0.f),
        glm::vec3(0.f,-1.f,0.f),
        glm::vec3(0.f,0.f,-1.f),
        glm::vec3(0.f,0.f,1.f),
    };

    ///设置聚光光源初始属性（模拟手电筒）
    for (int i = 0; i < 6; ++i) {
        std::string str = "spotLights[";
        std::string res = str + std::to_string(i) + "].";

        ourShader.setFloat(res + "cutOff", glm::cos(glm::radians(12.5f)));
        ourShader.setFloat(res + "outerCutOff", glm::cos(glm::radians(17.5f)));
        ourShader.setVec3f(res + "ambient", torch.AmbientColor);
        ourShader.setVec3f(res + "diffuse", torch.DiffuseColor);
        ourShader.setVec3f(res + "specular", torch.SpecularColor);
        ourShader.setFloat(res + "constant", 1.0f);
        ourShader.setFloat(res + "linear", 0.09f);
        ourShader.setFloat(res + "quadratic", 0.032f);

        ourShader.setVec3f(res + "position", torch_positions[i]);
        ourShader.setVec3f(res + "direction", torch_directions[i]);
    }
    
    ///设置材质
    ourShader.setInt("material.diffuse", 0);
    ourShader.setInt("material.specular", 1);
    ourShader.setFloat("material.shininess", 32.0f);
    
    ///模型偏移量数组
    glm::vec3 postions[] = {
        glm::vec3(0.0,0.0,0.0),
        glm::vec3( -2.0f,  -2.0f, -2.0f),
        glm::vec3(-2.0f, 2.0f, -2.0f),
        glm::vec3(2.0f, -2.0f, -2.0f),
        glm::vec3( 2.0f, 2.0f, -2.0f),
        glm::vec3(0.0f,  -4.0f, -2.0f),
        glm::vec3( 0.0f, 4.0f, -2.0f),
        glm::vec3( -4.0f,  0.0f, -2.0f),
        glm::vec3( 4.0f,  0.0f, -2.0f),
        glm::vec3(0.0f,  0.0f, -4.0f)
    };
    
    ///模型旋转轴数组
    glm::vec3 rotateAxis[] = {
        glm::vec3(1.0,0.3,-0.7),
        glm::vec3( 0.2f,  0.5f, -0.15f),
        glm::vec3(-0.15f, -0.22f, -0.25f),
        glm::vec3(-0.38f, -0.2f, -0.23f),
        glm::vec3( 0.24f, -0.4f, -0.35f),
        glm::vec3(-0.17f,  0.3f, -0.75f),
        glm::vec3( 0.13f, -0.2f, -0.25f),
        glm::vec3( 0.15f,  0.2f, -0.25f),
        glm::vec3( 0.15f,  0.2f, -0.15f),
        glm::vec3(-0.13f,  1.0f, -0.15f)
    };
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        ///设置清屏颜色
        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        ///清屏
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ///开始绘制光源
        lightShader.use();
        ///设置光源展示颜色
        lightShader.setVec3f("lightColor", moon.LightColor);
        

        ///绘制顶部太阳光源（定向光源）
        glm::mat4 lightView = camera.getViewMatrix();
        lightShader.setMtx4fv("view", lightView);
        glm::mat4 lightProjection = glm::perspective(glm::radians(fov), (float)(SCR_WIDTH * 1.0 / SCR_HEIGHT), 0.1f, 100.0f);
        lightShader.setMtx4fv("projection", lightProjection);
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, moon.Position);
        lightShader.setMtx4fv("model", lightModel);
        light.DrawWithoutConfigImage();

        ///设置灯泡光源（点光源）
        lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, glm::vec3(0.0,0.0,-2.0));
        lightModel = glm::scale(lightModel, glm::vec3(0.5,0.5,0.5));
        lightShader.setVec3f("lightColor", lamp.LightColor);
        lightShader.setMtx4fv("model", lightModel);
        light.DrawWithoutConfigImage();

        ///设置手电筒光源（聚光光源）
        for (int i = 0; i < 6; ++i) {
            lightModel = glm::mat4(1.0f);
            lightModel = glm::translate(lightModel, torch_positions[i]);
            lightModel = glm::scale(lightModel, glm::vec3(0.25,0.25,0.25));
            lightShader.setVec3f("lightColor", torch.LightColor);
            lightShader.setMtx4fv("model", lightModel);
            light.DrawWithoutConfigImage();
        }

        ///判断是否正在跳跃，来改变摄像头位置模拟跳跃行为
        if (jumping) {
            float factor = timeIntervalToLastFrame();
            camera.Position += camera.WorldUp * (float)(currentSpeed * factor);
            currentSpeed -= g;
            if (currentSpeed <= -jumpSpeed) {
                currentSpeed = 0;
                jumping = false;
//                camera.Position *= glm::vec3(1.f,0.f,1.f);
            }
        }

        ///开始绘制模型
        ourShader.use();

        ///设置观察点位置
        glm::vec3 viewPos = camera.Position;
        ourShader.setVec3f("viewPosition", viewPos);

        ///设置转换矩阵
        glm::mat4 view = camera.getViewMatrix();
        ourShader.setMtx4fv("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)(SCR_WIDTH * 1.0 / SCR_HEIGHT), 0.1f, 100.0f);
        ourShader.setMtx4fv("projection", projection);

        lastFrameTS = glfwGetTime();

        bool changeLightColor = false;
        if (changeLightColor) {
            glm::vec3 tmpLightColor = glm::vec3(0.5 * sin(lastFrameTS * 2.0f) + 0.5,0.5 * sin(lastFrameTS * 0.7) + 0.5,0.5 * sin(lastFrameTS * 1.3) + 0.5);
            moon.updateLightColor(tmpLightColor);

            ///设置光的属性
            ourShader.setVec3f("directionLight.position", moon.Position);
            ourShader.setVec3f("directionLight.ambient", moon.AmbientColor);
            ourShader.setVec3f("directionLight.diffuse", moon.DiffuseColor);
            ourShader.setVec3f("directionLight.specular", moon.SpecularColor);
        }


        bool changeModelAngle = false;
        float factor = sin(lastFrameTS) * 0.5 + 0.5;
        float angle = 0;
        if (changeModelAngle) {
            angle = 360 * factor;
        }
        for (int i = 0; i < 10; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, postions[i]);
            model = glm::rotate(model, glm::radians(angle), rotateAxis[i]);
            ourShader.setMtx4fv("model", model);
            box.DrawWithoutConfigImage();
        }
        
        
        ///交换颜色缓冲
        glfwSwapBuffers(window);
        ///拉取用户事件
        glfwPollEvents();
    }
    
    ///释放Box网格
    box.ReleaseMesh();
    ///释放Light网格
    light.ReleaseMesh();
    
    finishiRenderLoop();
    
    return 0;
}

float timeIntervalToLastFrame () {
    if (lastFrameTS <= 0) {
        return 0;
    }
    return glfwGetTime() - lastFrameTS;
}

GLFWwindow* configOpenGL() {
    ///初始化glfw
    glfwInit();
    
    ///设置版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    ///设置核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    ///设置对Mac OS X的兼容
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    ///创建window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    ///将window设置成当前上下文
    glfwMakeContextCurrent(window);
    ///设置窗口事件更新触发的回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    ///设置鼠标事件回调
    glfwSetCursorPosCallback(window, mouse_callback);
    ///设置不显示鼠标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    ///初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    return window;
}

void configBoxMesh (Mesh * mesh) {
    ///顶点数据
    float vertices[] = {
        ///vertex-3 ///normal-3 ///textureCord-2
        0.5,0.5,0.5,0.f,0.f,1.f,1.0,1.0,//1
        0.5,-0.5,0.5,0.f,0.f,1.f,1.0,0.0,//2
        -0.5,-0.5,0.5,0.f,0.f,1.f,0.0,0.0,//3
        -0.5,0.5,0.5,0.f,0.f,1.f,0.0,1.0,//4
        0.5,-0.5,0.5,1.f,0.f,0.f,0.0,0.0,//5
        0.5,0.5,-0.5,1.f,0.f,0.f,1.0,1.0,//6
        0.5,-0.5,-0.5,1.f,0.f,0.f,1.0,0.0,//7
        0.5,0.5,0.5,1.f,0.f,0.f,0.0,1.0,//8
        0.5,-0.5,-0.5,0.f,0.f,-1.f,0.0,0.0,//9
        -0.5,-0.5,-0.5,0.f,0.f,-1.f,1.0,0.0,//10
        -0.5,0.5,-0.5,0.f,0.f,-1.f,1.0,1.0,//11
        0.5,0.5,-0.5,0.f,0.f,-1.f,0.0,1.0,//12
        -0.5,-0.5,0.5,-1.f,0.f,0.f,1.0,0.0,//13
        -0.5,0.5,0.5,-1.f,0.f,0.f,1.0,1.0,//14
        -0.5,-0.5,-0.5,-1.f,0.f,0.f,0.0,0.0,//15
        -0.5,0.5,-0.5,-1.f,0.f,0.f,0.0,1.0,//16
        0.5,0.5,0.5,0.f,1.f,0.f,1.0,0.0,//17
        -0.5,0.5,0.5,0.f,1.f,0.f,0.0,0.0,//18
        0.5,0.5,-0.5,0.f,1.f,0.f,1.0,1.0,//19
        -0.5,0.5,-0.5,0.f,1.f,0.f,0.0,1.0,//20
        0.5,-0.5,0.5,0.f,-1.f,0.f,1.0,1.0,//21
        0.5,-0.5,-0.5,0.f,-1.f,0.f,1.0,0.0,//22
        -0.5,-0.5,-0.5,0.f,-1.f,0.f,0.0,0.0,//23
        -0.5,-0.5,0.5,0.f,-1.f,0.f,0.0,1.0,//24
    };
    
    ///索引数据
    vector<unsigned int> indices = {
        0,1,2,
        0,2,3,
        4,5,6,
        7,4,5,
        8,9,10,
        11,8,10,
        12,13,14,
        13,14,15,
        16,17,18,
        17,18,19,
        20,21,22,
        20,23,22,
    };
    
    vector<Mesh_Vertex> v;
    for (int i = 0; i < 24; ++i) {
        Mesh_Vertex tmp;
        int offset = 8 * i;
        tmp.Position = glm::vec3(vertices[offset],vertices[offset + 1],vertices[offset + 2]);
        tmp.Normal = glm::vec3(vertices[offset + 3],vertices[offset + 4],vertices[offset + 5]);
        tmp.TexCoords = glm::vec2(vertices[offset + 6],vertices[offset + 7]);
        v.push_back(tmp);
    }
    
    *mesh = Mesh(v, indices);
}

void configLightMesh(Mesh * mesh) {
    ///顶点数据
    float vertices[] = {
        0.5,0.5,0.5,
        0.5,-0.5,0.5,
        -0.5,-0.5,0.5,
        -0.5,0.5,0.5,
        0.5,0.5,-0.5,
        0.5,-0.5,-0.5,
        -0.5,-0.5,-0.5,
        -0.5,0.5,-0.5,
    };
    
    ///索引数据
    vector<unsigned int> indices = {
        0,1,2,
        0,2,3,
        1,4,5,
        0,1,4,
        5,6,7,
        4,5,7,
        2,3,6,
        3,6,7,
        0,3,4,
        3,4,7,
        1,5,6,
        1,2,6,
    };
    
    vector<Mesh_Vertex> v;
    for (int i = 0; i < 8; ++i) {
        Mesh_Vertex tmp;
        int offset = 3 * i;
        tmp.Position = glm::vec3(vertices[offset],vertices[offset + 1],vertices[offset + 2]);
        v.push_back(tmp);
    }
    
    *mesh = Mesh(v, indices);
}

void loadImg(const char * path,unsigned int * texture,unsigned int uniteLoc) {
    ///设置图片加载时上下翻转
    stbi_set_flip_vertically_on_load(true);
    
    ///加载图片
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    
    GLenum format = 0;
    if (nrChannels == 0) {
        format = GL_RED;
    } else if (nrChannels == 1) {
        format = GL_RGB;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
    }
    
    ///生成纹理对象并绑定至上下文中的2D纹理
    glGenTextures(1, texture);
    glActiveTexture(GL_TEXTURE0 + uniteLoc);
    glBindTexture(GL_TEXTURE_2D, *texture);
    
    ///设置纹理环绕及过滤模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    ///加载纹理数据并设置多级渐远纹理
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    ///释放图像数据
    stbi_image_free(data);
}

void finishiRenderLoop() {
    ///释放窗口资源
    glfwTerminate();
}

///处理输入
void processInput(GLFWwindow *window)
{
    float speed = 10 * timeIntervalToLastFrame();
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.move(LEFT, timeIntervalToLastFrame());
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.move(RIGHT, timeIntervalToLastFrame());
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.move(FORWARD, timeIntervalToLastFrame());
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.move(BACKWARD, timeIntervalToLastFrame());
    }
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.move(UPWARD, timeIntervalToLastFrame());
    } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.move(DOWNWARD, timeIntervalToLastFrame());
    }
    
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        fov -= speed;
        if (fov < 1.0) {
            fov = 1.0;
        }
    } else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        fov += speed;
        if (fov > 45.0) {
            fov = 45.0;
        }
    }
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!jumping) {
            currentSpeed = jumpSpeed;
            jumping = true;
        }
    }
    
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.resetCamera();
        fov = 45.0;
        jumping = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstCursor) {
        lastCursorX = xpos;
        lastCursorY = ypos;
        firstCursor = false;
        return;
    }
    
    float xoffset = xpos - lastCursorX;
    float yoffset = lastCursorY - ypos;
    lastCursorX = xpos;
    lastCursorY = ypos;
    
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    float yaw = camera.Yaw;
    float pitch = camera.Pitch;
    yaw += xoffset;
    pitch += yoffset;
    
    if (pitch > 89.0f) {
        pitch = 89.0f;
    } else if (pitch < -89.0f) {
        pitch = -89.0f;
    }
    camera.setPitch(pitch);
    camera.setYaw(yaw);
    camera.updateCameraVectors();
}

///窗口事件更新回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ///设置视口大小
    glViewport(0, 0, width, height);
}
