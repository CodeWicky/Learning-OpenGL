#include "Shader.h"
#include <iostream>
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
GLFWwindow * configOpenGL ();
void finishiRenderLoop ();
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    
    GLFWwindow * window = configOpenGL();
    
    Shader ourShader("/Users/momo/Desktop/Wicky/Learn\ OpenGL/入门/Demos/5.着色器/5-3.着色器\(Shader Class\)/OpenGL_Template/Vertex.h","/Users/momo/Desktop/Wicky/Learn\ OpenGL/入门/Demos/5.着色器/5-3.着色器\(Shader Class\)/OpenGL_Template/Fragment.h");
    
    
    ///顶点数据
    float vertices[] = {
        0.5f, 0.5f,    // 右上角
        0.5f, -0.5f,  // 右下角
        -0.5f, -0.5f,  // 左下角
        -0.5f, 0.5f,    // 左上角
    };
    
    ///索引数据
    unsigned int indices[] = {
        0,1,3,
        1,2,3,
    };
    
    unsigned int VBO,EBO,VAO;
    
    ///创建顶点数组对象
    glGenVertexArrays(1, &VAO);
    
    ///创建顶点缓冲对象
    glGenBuffers(1, &VBO);
    ///创建索引缓冲对象
    glGenBuffers(1, &EBO);
    
    ///绑定定点数组对象至上下文
    glBindVertexArray(VAO);
    
    ///绑定定点缓冲对象至上下文
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    ///把顶点数组复制到顶点缓冲对象中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    ///设置顶点属性
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    ///激活顶点属性
    glEnableVertexAttribArray(0);
    ///绑定索引缓冲对象至上下文
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    ///把索引数据复制到索引缓冲对象中
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    ///解除顶点数组对象的绑定
    glBindVertexArray(0);
    ///解除顶点缓冲对象的绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    ///解除索引缓冲对象的绑定
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        ///设置清屏颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        ///清屏
        glClear(GL_COLOR_BUFFER_BIT);
        
        ourShader.use();
        ///更新uniform颜色
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        ourShader.setVec4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
        ///绑定定点数组对象
        glBindVertexArray(VAO);
        ///以索引绘制顶点数据
        //        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
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

GLFWwindow* configOpenGL () {
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

void finishiRenderLoop () {
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
