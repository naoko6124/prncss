#pragma once
#include "pch.h"
#include "window.h"
#include "styles.h"
#include "dockspace.h"
//
#include "interface/assets.h"
#include "interface/console.h"
#include "interface/entities.h"
#include "interface/info.h"
#include "interface/scene.h"

namespace editor
{
    class app
    {
    public:
        app()
         : m_window("prncss editor", 1280, 720), m_scene(&my_scene)
        { }
        ~app() {}
        void run()
        {
            styles::dark_theme();
            while (m_window.running())
            {
                m_window.clear();
                m_dockspace.draw(project_path, project_name);
                if (!project_path.empty())
                {
                    m_assets.draw(project_path + "/assets");
                    m_console.draw();
                    m_entities.draw();
                    m_info.draw();
                    m_scene.draw();
                }
                m_window.draw();
            }
        }
    private:
        std::string project_path;
        std::string project_name;
        prncss::scene my_scene;
        window m_window;
        dockspace m_dockspace;
        assets m_assets;
        console m_console;
        entities m_entities;
        info m_info;
        scene m_scene;
    };
}