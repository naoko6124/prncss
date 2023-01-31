#pragma once
#include "imgui.h"
#include "pch.h"
#include <filesystem>
#include <fstream>

namespace editor
{
    class dockspace
    {
    public:
        void draw(std::string& glob_project_path, std::string& glob_project_name)
        {
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("dockspace", nullptr, window_flags);
			ImGui::PopStyleVar(3);
            
            static bool new_project_popup = false;
            static bool open_project_popup = false;

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("project"))
                {
                    if (ImGui::MenuItem("new project"))
                    {
                        new_project_popup = true;
                    }
                    if (ImGui::MenuItem("open project"))
                    {
                        open_project_popup = true;
                    }
                    if (ImGui::MenuItem("close project"))
                    {
                        glob_project_path.clear();
                    }
                    ImGui::EndMenu();
                }
                
                if (!glob_project_path.empty())
                {
                    if (ImGui::BeginMenu("scene"))
                    {
                        if (ImGui::MenuItem("new scene"))
                        {
                            
                        }
                        if (ImGui::MenuItem("open scene"))
                        {

                        }
                        if (ImGui::MenuItem("save scene"))
                        {

                        }
                        if (ImGui::MenuItem("save scene as"))
                        {

                        }
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("game"))
                    {
                        if (ImGui::MenuItem("start"))
                        {
                            std::string command = glob_project_path + "\\" + glob_project_name + ".exe";
                            system(command.c_str());
                        }
                        ImGui::EndMenu();
                    }
                }
                ImGui::EndMenuBar();
            }

			ImGuiID dockspace_id = ImGui::GetID("dockspace_id");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

            reset();
            
			ImGui::End();

            if (new_project_popup)
            {
                static bool error_folder_not_empty = false;
                ImGui::Begin("new project");
                static std::string project_name = "project1";
                ImGui::InputText("name", &project_name);
                std::string project_path = std::filesystem::current_path().string() + "\\projects\\" + project_name + "\\";
                ImGui::Text("path: %s", project_path.c_str());
                if (ImGui::Button("create"))
                {
                    std::filesystem::create_directories(project_path);
                    if (!std::filesystem::is_empty(project_path))
                        error_folder_not_empty = true;
                    else
                    {
                        new_project_popup = false;
                        std::ofstream project_file(project_path + "\\project.scr");
                        project_file << "project:\n";
                        project_file << "  name: " << project_name;
                        project_file.close();
                        std::filesystem::create_directories(project_path + "\\assets\\");
                        std::filesystem::create_directories(project_path + "\\assets\\textures\\");
                        std::filesystem::create_directories(project_path + "\\assets\\scripts\\");
                        std::filesystem::create_directories(project_path + "\\assets\\scenes\\");
                        std::filesystem::create_directories(project_path + "\\assets\\prefabs\\");
                        std::filesystem::create_directories(project_path + "\\assets\\audios\\");
                        std::filesystem::copy(std::filesystem::current_path().string() + "\\builtin", project_path + "\\builtin\\");
                        std::filesystem::copy_file(std::filesystem::current_path().string() + "\\game.exe", project_path + "\\" + project_name + ".exe");
                        glob_project_path = project_path;
                        glob_project_name = project_name;
                    }
                }
                if (error_folder_not_empty)
                {
                    ImGui::Text("error: folder not empty");
                }
                if (ImGui::Button("close"))
                {
                    new_project_popup = false;
                }
                ImGui::End();
            }
            if (open_project_popup)
            {
                static bool error_there_is_not_project = false;
                ImGui::Begin("open project");
                static std::string project_name = "";
                ImGui::InputText("name", &project_name);
                if (ImGui::Button("open"))
                {
                    std::string project_path = std::filesystem::current_path().string() + "\\projects\\" + project_name + "\\";
                    if (std::filesystem::exists(project_path + "\\project.scr"))
                    {
                        open_project_popup = false;
                        glob_project_path = project_path;
                        glob_project_name = project_name;
                    }
                    else
                        error_there_is_not_project = true;
                }
                if (error_there_is_not_project)
                {
                    ImGui::Text("error: there is no project with this name");
                }
                if (ImGui::Button("close"))
                {
                    open_project_popup = false;
                }
                ImGui::End();
            }
        }
    private:
        void reset()
        {
            static auto first_time = true;
            if (first_time)
            {
                first_time = false;
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGuiID dockspace_id = ImGui::GetID("dockspace_id");

                static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

                ImGui::DockBuilderRemoveNode(dockspace_id);
                ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

                auto entities_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.5f, nullptr, &dockspace_id);
                auto info_id = ImGui::DockBuilderSplitNode(entities_id, ImGuiDir_Right, 0.6f, nullptr, &entities_id);
                auto assets_id = ImGui::DockBuilderSplitNode(entities_id, ImGuiDir_Down, 0.5f, nullptr, &entities_id);
                auto console_id = ImGui::DockBuilderSplitNode(info_id, ImGuiDir_Down, 0.3f, nullptr, &info_id);

                ImGui::DockBuilderDockWindow("scene", dockspace_id);
                ImGui::DockBuilderDockWindow("assets", assets_id);
                ImGui::DockBuilderDockWindow("entities", entities_id);
                ImGui::DockBuilderDockWindow("info", info_id);
                ImGui::DockBuilderDockWindow("console", console_id);

                ImGui::DockBuilderFinish(dockspace_id);
            }
        }
    };
}