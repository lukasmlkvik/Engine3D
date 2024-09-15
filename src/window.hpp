#pragma once

#include <GLFW/glfw3.h>
#include "types.hpp"

class Window{
    public:
    static bool Init(){
        return glfwInit();
    }
    static void Destroy(){
        glfwTerminate();
    } 
    static void PullEvents(){
        glfwPollEvents();
    }
    
    GLFWwindow* window;
    
    Window(ui32 width, ui32 height){

        window = glfwCreateWindow(width, height, "Engine3D", NULL, NULL);
        glfwMakeContextCurrent(window);

    }

    bool shouldClose() {
        return glfwWindowShouldClose(window);
    }

    void setTitle(const std::string& title) {
        glfwSetWindowTitle(window, title.c_str());
    }

    void clear() {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void draw() {
        //glfwSwapInterval(0);
        glfwSwapBuffers(window);
    }
    ~Window(){
        glfwDestroyWindow(window);
    }

    
};
