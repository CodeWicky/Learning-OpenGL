#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
    if (NULL == window) {
        return -1;
    }
    
    ///Render loop
    while (!glfwWindowShouldClose(window))
    {
        
        processInput(window);
        
        ///设置清屏颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        ///清屏
        glClear(GL_COLOR_BUFFER_BIT);
        
        ///交换颜色缓冲
        glfwSwapBuffers(window);
        ///拉取用户事件
        glfwPollEvents();
    }
    
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
