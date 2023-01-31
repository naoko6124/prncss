#pragma once
#include "pch.h"

namespace editor
{
    class assets
    {
    public:
        void draw(std::string assets_path = std::string())
        {
		    ImGui::Begin("assets");
            if (!assets_path.empty())
                search_dir(assets_path);
            ImGui::End();
            for (auto& editor : editors)
            {
                static bool w_open = true;
                ImGui::Begin(editor.first.filename().string().c_str(), &w_open);
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
            for (auto& p : std::filesystem::directory_iterator(path))
            {
                std::string current_path = p.path().string();
                std::string current_path_name = p.path().filename().string();
                if (p.is_directory())
                {
                    if (ImGui::TreeNode(current_path_name.c_str()))
                    {
                        search_dir(current_path.c_str());
                        ImGui::TreePop();
                    }
                }
            }
            for (auto& p : std::filesystem::directory_iterator(path))
            {
                std::string current_path = p.path().string();
                std::string current_path_name = p.path().filename().string();
                if (!p.is_directory())
                {
                    if (ImGui::Selectable(current_path_name.c_str(), current_file == p.path()))
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
    };
}