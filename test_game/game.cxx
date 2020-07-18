#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "engine.hxx"
#include "shader.hxx"
#include "stuff.hxx"
#include "texture2d.hxx"
#include "vbo.hxx"

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
    engine::shader_es_32 sh("../../../res/shaders/shader_v_3_no_version.vs",
                            "../../../res/shaders/shader_v_3_no_version.fs");
    engine::vbo_v_8      sampl("../../../res/vertexes.txt");

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

        std::ifstream    file("../../../res/vertexes_v_3.txt");
        engine::triangle tr;
        file >> tr;
        // engine->render_my_triangle(tr, sh);
        engine->render_(sampl, sample_sh);
        // engine->render_(sampl, sample_sh);
        engine->swap_buffers();
    }
    return EXIT_SUCCESS;
}
