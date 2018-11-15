#include "Shader.h"
#include <math.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
GLFWwindow * configOpenGL();
void loadImg(const char * path,unsigned int * texture,unsigned int uniteLoc);
void configVAO(unsigned int * VAO,unsigned int * VBO,unsigned int * EBO);
void finishiRenderLoop();
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

#define zAngleThreshold (89.0 * M_PI / 180)

float angle = 0;
float zAngle = 0;
float radius = 10.f;

int main()
{
    GLFWwindow * window = configOpenGL();
    
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("Vertex.h","Fragment.h");
    
    unsigned int VAO,VBO,EBO;
    
    ///配置VAO
    configVAO(&VAO,&VBO,&EBO);
    
    ourShader.use();
    
    glm::vec3 postions[] = {
        glm::vec3(0.0,0.0,0.0),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
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
    
    
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)(SCR_WIDTH * 1.0 / SCR_HEIGHT), 0.1f, 100.0f);
    
    ourShader.setMtx4fv("projection", projection);
    
    
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        ///设置清屏颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        ///清屏
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ///绑定定点数组对象
        glBindVertexArray(VAO);
        
        float camX = sin(angle) * cos(zAngle) * radius;
        float camZ = cos(angle) * cos(zAngle) * radius;
        float camY = sin(zAngle) * radius;
        glm::mat4 view = glm::lookAt(glm::vec3(camX,camY,camZ), glm::vec3(0.f,0.f,0.f), glm::vec3(0.f,1.f,0.f));
        ourShader.setMtx4fv("view", view);
        
        float time = glfwGetTime();
        float factor = sin(time) * 0.5 + 0.5;
        float angle = 360 * factor;
        angle = 0;
        for (int i = 0; i < 10; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            
            model = glm::translate(model, postions[i]);
            
            model = glm::rotate(model, glm::radians(angle), rotateAxis[i]);
            ourShader.setMtx4fv("model", model);
            
            ///以索引绘制顶点数据
            //        glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
        }
        
        
        ///交换颜色缓冲
        glfwSwapBuffers(window);
        ///拉取用户事件
        glfwPollEvents();
    }
    
    ///释放对象
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    finishiRenderLoop();
    
    return 0;
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
    
    ///初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    return window;
}

void configVAO(unsigned int * VAO,unsigned int * VBO,unsigned int * EBO) {
    ///顶点数据
    float vertices[] = {
        0.5,0.5,0.5,0.0,0.0,0.0,
        0.5,-0.5,0.5,1.0,0.0,0.0,
        -0.5,-0.5,0.5,0.0,1.0,0.0,
        -0.5,0.5,0.5,0.0,0.0,1.0,
        0.5,0.5,-0.5,1,1,1,
        0.5,-0.5,-0.5,0,1,1,
        -0.5,-0.5,-0.5,1,0,1,
        -0.5,0.5,-0.5,1,1,0
    };

    ///索引数据
    unsigned int indices[] = {
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
    
    ///创建顶点数组对象
    glGenVertexArrays(1, VAO);
    
    ///创建顶点缓冲对象
    glGenBuffers(1, VBO);
    ///创建索引缓冲对象
    glGenBuffers(1, EBO);
    
    ///绑定定点数组对象至上下文
    glBindVertexArray(*VAO);
    
    ///绑定定点缓冲对象至上下文
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    ///把顶点数组复制到顶点缓冲对象中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    ///设置顶点属性并激活属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    ///绑定索引缓冲对象至上下文
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    ///把索引数据复制到索引缓冲对象中
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    ///解除顶点数组对象的绑定
    glBindVertexArray(0);
    ///解除顶点缓冲对象的绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    ///解除索引缓冲对象的绑定
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

void loadImg(const char * path,unsigned int * texture,unsigned int uniteLoc) {
    ///设置图片加载时上下翻转
    stbi_set_flip_vertically_on_load(true);
    
    ///加载图片
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
    float speed = 0.01;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        angle -= speed;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        angle += speed;
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        zAngle += speed * 0.1;
        if (zAngle >= zAngleThreshold) {
            zAngle = zAngleThreshold;
        }
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        zAngle -= speed * 0.1;
        if (zAngle <= -zAngleThreshold) {
            zAngle = -zAngleThreshold;
        }
    }
    
    if (glfwGetKey(window, GLFW_KEY_Q)) {
        radius -= speed;
        if (radius < 0.1) {
            radius = 0.1;
        }
    } else if (glfwGetKey(window, GLFW_KEY_E)) {
        radius += speed;
        if (radius > 100) {
            radius = 100;
        }
    }
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        angle = 0;
        zAngle = 0;
        radius = 10;
    }
    
}

///窗口事件更新回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ///设置视口大小
    glViewport(0, 0, width, height);
}
