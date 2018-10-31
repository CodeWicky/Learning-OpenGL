#include "Shader.h"
#include <math.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
GLFWwindow * configOpenGL();
void loadImg(const char * path,unsigned int * texture,unsigned int uniteLoc);
void configVAO(unsigned int * VAO,unsigned int * VBO,unsigned int * EBO);
void finishiRenderLoop();
// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    GLFWwindow * window = configOpenGL();
    
    Shader ourShader("Vertex.h","Fragment.h");
    
    unsigned int VAO,VBO,EBO;
    
    ///配置VAO
    configVAO(&VAO,&VBO,&EBO);
    
    ///设置纹理单元的位置（想要设置着色器程序的值，必先激活着色器程序）
    ourShader.use();
    ourShader.setInt("ourTexture", 0);
    ourShader.setInt("avatarTexture", 1);
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        ///设置清屏颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        ///清屏
        glClear(GL_COLOR_BUFFER_BIT);
        
        float time = glfwGetTime();
        
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, time, glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        
        
        float factor = sin(time) * 0.5 + 0.5;
        ourShader.setFloat("factor", factor);
        
        ///绑定定点数组对象
        glBindVertexArray(VAO);
        ///以索引绘制顶点数据
        //        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES,30,GL_UNSIGNED_INT,0);
        
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
        //顶点坐标-2 //颜色-3 //是否绘制图片-1 //纹理坐标-2
        0.5f, 0.5f,1.0f,1.0f,0.0f,1.0f,1.5f,1.5f,    // 右上角
        0.5f, -0.5f,0.0f,1.0f,1.0f,1.0f,1.5f,-0.5f,  // 右下角
        -0.5f, -0.5f,1.0f,0.0f,1.0f,1.0f,-0.5f,-0.5f,  // 左下角
        -0.5f, 0.5f,1.0f,1.0f,1.0f,1.0f,-0.5f,1.5f,    // 左上角
        1.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,
        1.0f,-1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
        -1.0f,-1.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,
        -1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
    };

    ///索引数据
    unsigned int indices[] = {
        0,1,3,
        1,2,3,
        0,4,5,
        0,1,5,
        1,5,6,
        1,2,6,
        2,6,7,
        2,3,7,
        3,7,4,
        3,0,4,
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    ///绑定索引缓冲对象至上下文
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    ///把索引数据复制到索引缓冲对象中
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    ///加载图片
    unsigned int texture,avatar;
    loadImg("container.jpg", &texture,0);
    loadImg("avatar.jpeg", &avatar, 1);
    
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
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

///窗口事件更新回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ///设置视口大小
    glViewport(0, 0, width, height);
}
