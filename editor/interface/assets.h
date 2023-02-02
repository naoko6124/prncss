#pragma once
#include "imgui.h"
#include "pch.h"
#include <filesystem>
#include <fstream>

namespace editor
{
    class assets
    {
    public:
        void draw(std::string assets_path = std::string())
        {
		    bool open = ImGui::Begin("assets");
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("new folder"))
                {
                    int i = 1;
                    while (true)
                    {
                        if (!std::filesystem::exists(assets_path + "/new folder " + std::to_string(i)))
                        {
                            std::filesystem::create_directory(assets_path + "/new folder " + std::to_string(i));
                            break;
                        }
                        i++;
                    }
                }
                ImGui::EndPopup();
            }
            if (open)
            {
                if (!assets_path.empty())
                    search_dir(assets_path);
                ImGui::End();
            }
            for (auto& editor : editors)
            {
                static bool w_open = true;
                ImGuiWindowFlags w_flags = ImGuiWindowFlags_None;
                if (editor.second.CanUndo())
                {
                    w_flags |= ImGuiWindowFlags_UnsavedDocument;
                }
                std::string w_name = editor.first.filename().string() + "##" + editor.first.string();
                ImGui::Begin(w_name.c_str(), &w_open, w_flags);
                if (ImGui::Shortcut(ImGuiMod_Ctrl + ImGuiKey_S))
                {
                    std::ofstream file(editor.first.string());
                    file << editor.second.GetText().substr(0, editor.second.GetText().size() - 1);
                    file.close();
                    editor.second.SetText(editor.second.GetText().substr(0, editor.second.GetText().size() - 1));
                }
                editor.second.Render("TextEditor");
                ImGui::End();
                if(!w_open)
                {
                    w_open = true;
                    editors.erase(editor.first);
                    break;
                }
            }
        }
    public:
        std::filesystem::path current_file;
        std::unordered_map<std::filesystem::path, ImTextEditor> editors;
    private:
        void search_dir(std::string path)
        {
            static std::filesystem::path edit_path;
            static std::string edit_name;
            for (auto& p : std::filesystem::directory_iterator(path))
            {
                std::string current_path = p.path().string();
                std::string current_path_name = p.path().filename().string();
                if (p.is_directory())
                {
                    std::string node_name = current_path_name;
                    bool open = false;
                    if (edit_path == current_path)
                    {
                        bool modified = ImGui::InputText("##edit_input", &edit_name);
                        if (ImGui::IsKeyPressed(ImGuiKey_Enter))
                        {
                            std::filesystem::rename(edit_path, edit_path.parent_path().string() + "\\" + edit_name);
                            edit_path.clear();
                            edit_name.clear();
                        }
                    }
                    else
                    {
                        open = ImGui::TreeNode(node_name.c_str());
                        if (ImGui::BeginPopupContextItem())
                        {
                            if (ImGui::MenuItem("new folder"))
                            {
                                int i = 1;
                                while (true)
                                {
                                    if (!std::filesystem::exists(current_path + "/new folder " + std::to_string(i)))
                                    {
                                        std::filesystem::create_directory(current_path + "/new folder " + std::to_string(i));
                                        break;
                                    }
                                    i++;
                                }
                            }
                            ImGui::Separator();
                            if (ImGui::MenuItem("new scene"))
                            {
                                int i = 1;
                                while (true)
                                {
                                    if (!std::filesystem::exists(current_path + "/new scene " + std::to_string(i) + ".lua"))
                                    {
                                        std::ofstream file(current_path + "/new scene " + std::to_string(i) + ".lua");
                                        file << "-- run once\n";
                                        file << "function on_init():\n";
                                        file << "    -- code here\n";
                                        file << "end\n";
                                        file << "-- run every frame\n";
                                        file << "function on_update():\n";
                                        file << "    -- code here\n";
                                        file << "end";
                                        file.close();
                                        break;
                                    }
                                    i++;
                                }
                            }
                            if (ImGui::MenuItem("new script"))
                            {
                                int i = 1;
                                while (true)
                                {
                                    if (!std::filesystem::exists(current_path + "/new script " + std::to_string(i) + ".lua"))
                                    {
                                        std::ofstream file(current_path + "/new script " + std::to_string(i) + ".lua");
                                        file << "-- run once\n";
                                        file << "function on_init():\n";
                                        file << "    -- code here\n";
                                        file << "end\n";
                                        file << "-- run every frame\n";
                                        file << "function on_update():\n";
                                        file << "    -- code here\n";
                                        file << "end";
                                        file.close();
                                        break;
                                    }
                                    i++;
                                }
                            }
                            ImGui::Separator();
                            if (ImGui::MenuItem("rename"))
                            {
                                edit_name = current_path_name;
                                edit_path = current_path;
                            }
                            if (ImGui::MenuItem("delete"))
                            {
                                std::filesystem::remove_all(current_path);
                            }

                            ImGui::EndPopup();
                        }
                        if (open)
                        {
                            if (std::filesystem::exists(current_path))
                                search_dir(current_path.c_str());
                            ImGui::TreePop();
                        }
                    }
                }
            }
            for (auto& p : std::filesystem::directory_iterator(path))
            {
                std::string current_path = p.path().string();
                std::string current_path_name = p.path().filename().string();
                if (!p.is_directory())
                {
                    if (edit_path == current_path)
                    {
                        bool modified = ImGui::InputText("##edit_input", &edit_name);
                        if (ImGui::IsKeyPressed(ImGuiKey_Enter))
                        {
                            std::filesystem::rename(edit_path, edit_path.parent_path().string() + "\\" + edit_name);
                            edit_path.clear();
                            edit_name.clear();
                        }
                    }
                    else
                    {
                        bool open = ImGui::Selectable(current_path_name.c_str(), current_file == p.path());
                        if (ImGui::BeginPopupContextItem())
                        {
                            if (ImGui::MenuItem("rename"))
                            {
                                edit_name = current_path_name;
                                edit_path = current_path;
                            }
                            if (ImGui::MenuItem("delete"))
                            {
                                open = false;
                                std::filesystem::remove(current_path);
                            }

                            ImGui::EndPopup();
                        }
                        if (open)
                        {
                            current_file = p.path();
                        }
                        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                        {
                            std::ifstream t(p.path().c_str());
                            if (t.good())
                            {
                                std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
                                ImTextEditor editor;
                                editor.SetLanguageDefinition(ImTextEditor::LanguageDefinition::Lua());
                                editor.SetText(str.c_str());
                                if (editors.find(p.path()) == editors.end())
                                    editors[p.path()] = editor;
                                else
                                    ImGui::SetWindowFocus(p.path().filename().string().c_str());
                            }
                        }
                    }
                }
            }
        }
    };
}