#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    ///初始化GLFW
    glfwInit();
    
    ///设置指定版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    ///指定核心数
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    ///Mac OS X 需要指定兼容
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    ///创建一个窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    ///将窗口设置为OpenGL当前的上下文
    glfwMakeContextCurrent(window);
    
    ///设置窗口尺寸改变的回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    ///持续接收输入事件
    while (!glfwWindowShouldClose(window))
    {
        ///处理用户输入
        processInput(window);
        
        ///设置清屏颜色
        glClearColor(1.f, 0.f, 0.f, 1.0f);
        ///清屏
        glClear(GL_COLOR_BUFFER_BIT);

        ///交换颜色缓冲
        glfwSwapBuffers(window);
        ///检测事件（如键盘输入、鼠标移动、窗口更新等）
        glfwPollEvents();
    }
    ///清理资源
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    ///检测用户是否点击了esc，如果是，则将shouldClose置为true
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ///设置视口大小
    glViewport(0, 0, width, height);
}
