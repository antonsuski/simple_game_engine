#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "engine.hxx"
#include "shader.hxx"
#include "texture2d.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    std::unique_ptr<engine::core, void (*)(engine::core*)> engine(
        engine::create_engine(), engine::destroy_engine);

    if (!engine->init(1365, 767))
    {
        std::cerr << "init failed" << std::endl;
        return EXIT_FAILURE;
    }

    engine::shader tr_sh("../../res/shaders/default_shader.vs",
                         "../../res/shaders/default_shader.fs");

    engine::texture2d tank_texture("../../res/images/tank.png", 0, &tr_sh);
    engine::texture2d tank_1("../../res/images/tank_1.png", 1, &tr_sh);

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

        std::ifstream file("../../res/vertexes.txt");
        assert(!!file);

        engine::triangle tr1, tr2, tr3, tr4;
        file >> tr1 >> tr2 >> tr3 >> tr4;

        engine->render_my_triangle(tr1, tr_sh, tank_texture);
        engine->render_my_triangle(tr2, tr_sh, tank_texture);

        engine->render_my_triangle(tr3, tr_sh, tank_1);
        engine->render_my_triangle(tr4, tr_sh, tank_1);

        engine->swap_buffers();
    }
    return EXIT_SUCCESS;
}
