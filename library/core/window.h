#pragma once
#include "pch.h"
#include "event.h"

namespace prncss
{
    class window
    {
    public:
        void init(std::string title, int width, int height)
        {
            glfwInit();
            m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
            mouse_x = (int)(width/2.0f);
            mouse_y = (int)(height/2.0f);
            glfwSetWindowUserPointer(m_window, (void*)this);
            glfwSetWindowSizeCallback(m_window, [](GLFWwindow* p_window, int width, int height){
                window* handler = (window*)glfwGetWindowUserPointer(p_window);
                handler->resize_framebuffer(width, height);
                event_window_resize e(width, height);
                handler->event_callback(&e);
            });
            glfwSetKeyCallback(m_window, [](GLFWwindow* p_window, int key, int scancode, int action, int mods){
                window* handler = (window*)glfwGetWindowUserPointer(p_window);
                if (action == GLFW_PRESS)
                {
                    event_key_down e(key);
                    handler->event_callback(&e);
                }
                else if (action == GLFW_RELEASE)
                {
                    event_key_up e(key);
                    handler->event_callback(&e);
                }
            });
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(m_window, [](GLFWwindow* p_window, double xpos, double ypos){
                window* handler = (window*)glfwGetWindowUserPointer(p_window);
                event_mouse_move e((int)xpos, (int)ypos, (int)xpos - handler->mouse_x, handler->mouse_y - (int)ypos);
                handler->mouse_x = (int)xpos;
                handler->mouse_y = (int)ypos;
                handler->event_callback(&e);
            });
            glfwMakeContextCurrent(m_window);
            gladLoaderLoadGL();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }
        ~window()
        {
            glfwDestroyWindow(m_window);
            glfwTerminate();
        }
        void get_events()
        {
            glfwPollEvents();
        }
        void clear()
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        void swap_buffers()
        {
            glfwSwapBuffers(m_window);
        }
        bool is_running()
        {
            return !glfwWindowShouldClose(m_window);
        }
        void set_event_callback(std::function<void(event*)> fn) { event_callback = fn; }
        bool get_key_down(int keycode) { return glfwGetKey(m_window, keycode) == GLFW_PRESS; }
        bool get_key_up(int keycode) { return glfwGetKey(m_window, keycode) == GLFW_RELEASE; }
    private:
        std::function<void(event*)> event_callback;
        void resize_framebuffer(int w, int h)
        {
            glViewport(0, 0, w, h);
        }
        GLFWwindow* m_window;
        int mouse_x, mouse_y;
    };
}