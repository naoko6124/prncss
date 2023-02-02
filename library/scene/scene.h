#pragma once
#include "pch.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "camera.h"
#include "transform.h"
#include "utils/assets.h"

namespace prncss
{
    class scene
    {
    public:
        void load()
        {
            m_shader = new shader();
            m_shader->set_vertex(assets::load_file("builtin/default.vsh"));
            m_shader->set_fragment(assets::load_file("builtin/default.fsh"));
            world.system<transform, mesh>("render").each([this](transform& t, mesh& m){
                static int once = 0;
                if (once < 6)
                {
                    once++;
                    printf("%s\n", m.mesh_data.name.c_str());
                    printf(" -vertices: %d\n", m.mesh_data.vertices.size());
                    printf(" -texture: %s\n", m.mesh_data.texture.c_str());
                }
                m_shader->set_mat4("model", t.get_matrix());
                m.draw();
            });
            main_camera = world.entity().set([](transform& t, camera& c) {
                t.position = { 0, 0, 2 };
            });
        }
        void unload()
        {
            delete m_shader;
        }
        flecs::entity create_entity()
        {
            return world.entity();
        }
        flecs::entity& get_camera()
        {
            return main_camera;
        }
        void render()
        {
            m_shader->use();
            auto t = main_camera.get_ref<transform>();
            auto viewport = main_camera.get_ref<camera>()->get_matrix(t.get());
            m_shader->set_mat4("camera", viewport);
            world.progress();
        }
    private:
        flecs::world world;
        shader* m_shader;
        flecs::entity main_camera;
    };
}