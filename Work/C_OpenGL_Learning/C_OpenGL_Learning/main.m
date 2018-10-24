//
//  main.m
//  C_OpenGL_Learning
//
//  Created by Wicky on 2018/10/24.
//  Copyright © 2018 Wicky. All rights reserved.
//

#include <OpenGL/OpenGL.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

int main(int argc, const char * argv[]) {
    if (!glfwInit()) {
        return -1;
    }
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello OpenGL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
//    cout << "OpenGL Vendor:" << glGetString(GL_VENDOR) << endl;
//    cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << endl;
//    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
//    cout << "GLSL Version:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
