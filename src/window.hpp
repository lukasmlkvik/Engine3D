#pragma once

#include <GLFW/glfw3.h>
#include "types.hpp"

#pragma once

#include <map>

class IEvent
{
public:
	virtual void notify() = 0;
};

template<class T>
class EventController : public IEvent
{
public:

	EventController(T* t, void (*fun)(T*)) :t(t), fun(fun) { }

	void notify() {
		(*fun)(t);
	}

private:
	T* t;
	void (*fun)(T*);

};


class IEventMouseMove : public IEvent
{
public:
	float x, y;

};

template<class T>
class EventMouseMove : public IEventMouseMove
{
public:

	EventMouseMove(T* t, void (*fun)(T*,float, float)) :t(t), fun(fun) { }

	virtual void notify() {
		(*fun)(t, x, y);
	}
private:
	T* t;
	void (*fun)(T*, float x, float y);
};

class KeyboardController
{
public:

    void registerEvent(int key, int action, IEvent* e) {
        events[key][action] = e;
    }

    void sendCallback(int key, int action)
    {
        IEvent* e = events[key][action];
        if (e) {
            e->notify();
        }
    }

    virtual ~KeyboardController() {
        clear();
    }

    void clear() {
        for (auto a : events)
        {
            for (auto b : a.second)
            {
                delete b.second;
            }
        }
        events.clear();
    }


private:

    std::map<int, std::map<int, IEvent*>> events;
};

class MouseController : public KeyboardController
{
public:
    void registerEvent(IEventMouseMove* e) {
        mouseMoveEvent = e;
    }

    void sendMuseMoveCallback(float x, float y)
    {
        if (mouseMoveEvent) {
            mouseMoveEvent->x = x;
            mouseMoveEvent->y = y;
            mouseMoveEvent->notify();
        }
    }

    virtual ~MouseController() {
        if (mouseMoveEvent) {
            delete mouseMoveEvent;
        }
    }


private:

    IEventMouseMove* mouseMoveEvent = nullptr;
};


class Window{
    public:

    GLFWwindow* window;
    MouseController mc;
    KeyboardController kc;

    static bool Init(){
        return glfwInit();
    }
    static void Destroy(){
        glfwTerminate();
    } 
    static void PullEvents(){
        glfwPollEvents();
    }
    
    Window(ui32 width, ui32 height){

        window = glfwCreateWindow(width, height, "Engine3D", NULL, NULL);
        glfwMakeContextCurrent(window);

        glfwSetWindowUserPointer(window, this);

        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetKeyCallback(window, key_callback);

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

    void lockCursor() {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    void unLockCursor() {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

private:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->kc.sendCallback(key,action);
    }
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->mc.sendMuseMoveCallback((float)xpos, (float)ypos);
    }
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->mc.sendCallback(button, action);
    }
    
};
