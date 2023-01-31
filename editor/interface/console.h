#pragma once
#include "pch.h"

namespace editor
{
    class console
    {
    public:
        void draw()
        {
		    ImGui::Begin("console");
            ImGui::Text("framerate: %.1f", ImGui::GetIO().Framerate);
		    ImGui::End();
        }
    };
}