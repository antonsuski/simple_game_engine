#include <iostream>
#include <memory>
#include <string_view>

#include "engine.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    std::string_view lol{ "lol? Hello world" };
    std::cout << lol << std::endl;

    std::unique_ptr<engine::engine, void (*)(engine::engine*)> engine(
        engine::create_engine(), engine::destroy_engine);

    uint32_t h = 480;
    uint32_t w = 640;

    engine->init(w, h);
    // engine->tmp_test_method(sh_prog, vao_id);

    engine::vbo_v_3      v_buffer("../../res/vertexes_v_3.txt");
    engine::shader_es_32 v_shader("../../res/shaders/shader_v_3.vs",
                                  "../../res/shaders/shader_v_3.fs");

    engine::vbo_v_8      lol_buffer("../../res/rgb_triangle.txt");
    engine::shader_es_32 lol_shader("../../res/shaders/shader_v_8.vs",
                                    "../../res/shaders/shader_v_8.fs");

    bool continue_loop = true;
    while (continue_loop)
    {
        using namespace engine;

        event system_event;
        engine->handl_imput(system_event);

        switch (system_event.type)
        {
            case event::turn_off:
            {
                continue_loop = false;
                std::clog << system_event << std::endl;
            }
            break;
            case event::left:
            {
                std::clog << system_event << std::endl;
            }
            break;
            case event::right:
            {
                std::clog << system_event << std::endl;
            }
            break;
            case event::up:
            {
                std::clog << system_event << std::endl;
            }
            break;
            case event::down:
            {
                std::clog << system_event << std::endl;
            }
            break;
        }
        engine->render(lol_buffer, lol_shader);
        //        engine->render(v_buffer, v_shader);
        // engine->tmp_test_method2(sh_prog, vao_id);
        engine->swap_buffers();
    }

    return 0;
}
