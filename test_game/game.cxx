#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "engine.hxx"
#include "shader.hxx"
#include "stuff.hxx"
#include "texture2d.hxx"
#include "vbo.hxx"

void change_color(engine::uniform& u);

int main(int /*argc*/, char* /*argv*/[])
{
    std::unique_ptr<engine::core, void (*)(engine::core*)> engine(
        engine::create_engine(), engine::destroy_engine);

    if (!engine->init(1364, 766))
    {
        std::cerr << "init failed" << std::endl;
        return EXIT_FAILURE;
    }

    engine::shader_es_32 sample_sh("../../../res/shaders/shader_v_3.vs",
                                   "../../../res/shaders/shader_v_3.fs");
    engine::shader_es_32 default_shader(
        "../../../res/shaders/default_shader.vs",
        "../../../res/shaders/default_shader.fs");

    engine::vbo_v_8 sampl("../../../res/vertexes.txt");
    engine::vbo_v_8 triangle("../../../res/rgb_triangle.txt");

    engine::uniform               u{ 0, 0, 0, 0 };
    std::vector<std::string_view> u_name{ "my_color" };

    bool continue_loop = true;
    while (continue_loop)
    {
        engine::event event;

        while (engine->read_event(event))
        {
            std::cout << event << std::endl;
            switch (event.key)
            {
                case engine::event::turn_off:
                    continue_loop = false;
                    break;
                default:
                    break;
            }
        }

        float f = (sin(engine->get_time_for_init()) + 1.0f) / 2.0f;
        // default_shader.set_uniform_4f(u_name, u);
        // std::cerr << u.u0 << std::endl;
        triangle.morf_color(f);
        triangle.print_buffer();
        engine->render_(triangle, default_shader);
        engine->render_(sampl, sample_sh);
        engine->swap_buffers();
    }
    return EXIT_SUCCESS;
}
