#pragma once
#include "pch.h"

namespace editor
{
    class window
    {
    public:
        window(const char* title, int width, int height)
        {
            glfwInit();
            m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
            glfwSetWindowSizeCallback(m_window, [](GLFWwindow* p_window, int width, int height){
                glViewport(0, 0, width, height);
            });
            glfwMakeContextCurrent(m_window);
            gladLoaderLoadGL();
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.IniFilename = "prncss.ini";
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

            ImGui_ImplGlfw_InitForOpenGL(m_window, true);
            ImGui_ImplOpenGL3_Init("#version 450");
        }
        ~window()
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            glfwDestroyWindow(m_window);
            glfwTerminate();
        }
        bool running()
        {
            return !glfwWindowShouldClose(m_window);
        }
        void clear()
        {
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
        }
        void draw()
        {
			ImGui::Render();
			ImGui::EndFrame();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(m_window);
        }
    private:
        GLFWwindow* m_window;
    };
}