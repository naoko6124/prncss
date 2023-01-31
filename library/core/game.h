#pragma once
#include "core/event.h"
#include "scene/transform.h"
#include "window.h"

#include "scene/scene.h"
#include "utils/assets.h"

namespace prncss
{
    class game
    {
    public:
        void run()
        {
            m_window.init("prncss", 1280, 720);
            m_window.set_event_callback(std::bind(&game::on_event, this, std::placeholders::_1));
            m_scene.load();
            init();
            auto last_time = glfwGetTime();
            auto current_time = glfwGetTime();
            while(m_window.is_running())
            {
                current_time = glfwGetTime();
                auto delta_time = current_time - last_time;
                last_time = current_time;
                m_window.get_events();
                update((float)delta_time);
                m_window.clear();
                draw();
                m_window.swap_buffers();
            }
            m_scene.unload();
        }
        void on_event(event* e)
        {
            switch(e->get_type())
            {
                case event::type::mouse_move:
                {
                    event_mouse_move* ev = reinterpret_cast<event_mouse_move*>(e);
                    auto t = m_scene.get_camera().get_ref<transform>();
                    t->rotation.x += ev->get_offset_y() * 0.1f;
                    t->rotation.y += ev->get_offset_x() * 0.1f;
                    break;
                }
            }
        }
        void init()
        {
            auto e = m_scene.create_entity();
            e.set([](transform& t, mesh& m){
                std::vector<mesh::vertex> vertices = {
                    { { -1,  1,  1 }, { -2,  2,  2 }, { 0, 1 } },
                    { {  1,  1,  1 }, {  2,  2,  2 }, { 1, 1 } },
                    { { -1, -1,  1 }, { -2, -2,  2 }, { 0, 0 } },
                    { {  1, -1,  1 }, {  2, -2,  2 }, { 1, 0 } },
                    { { -1,  1, -1 }, { -2,  2, -2 }, { 1, 1 } },
                    { {  1,  1, -1 }, {  2,  2, -2 }, { 0, 1 } },
                    { { -1, -1, -1 }, { -2, -2, -2 }, { 1, 0 } },
                    { {  1, -1, -1 }, {  2, -2, -2 }, { 0, 0 } }
                };
                m.set_vertices(vertices);

                
                std::vector<uint32_t> indices = {
                    0, 1, 2, 2, 3, 1,
                    4, 5, 6, 6, 7, 5,
                    1, 5, 3, 3, 7, 5,
                    0, 4, 2, 2, 6, 4,
                    4, 5, 0, 0, 1, 5,
                    6, 7, 2, 2, 3, 7
                };
                m.set_indices(indices);

                m.set_texture("builtin/tex.png");

                t.position = { 0, 0, 0 };
                t.rotation = { 0, 0, 0};
            });
        }
        void update(float delta_time)
        {
            auto t = m_scene.get_camera().get_ref<transform>();
            auto c = m_scene.get_camera().get_ref<camera>();
            if (m_window.get_key_down(GLFW_KEY_W))
                t->position += delta_time * c->front;
            if (m_window.get_key_down(GLFW_KEY_S))
                t->position -= delta_time * c->front;
            if (m_window.get_key_down(GLFW_KEY_D))
                t->position += delta_time * c->right;
            if (m_window.get_key_down(GLFW_KEY_A))
                t->position -= delta_time * c->right;
        }
        void draw()
        {
            m_scene.render();
        }
    private:
        window m_window;
        scene m_scene;
    };
}