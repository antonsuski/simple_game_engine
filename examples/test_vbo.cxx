#include <iostream>
#include <memory>

#include "engine.hxx"

int main(int, char const**)
{
    std::cout << "Hello, world!\n";

    std::unique_ptr<engine::engine, void (*)(engine::engine*)> engine(
        engine::create_engine(), engine::destroy_engine);

    uint32_t h = 640;
    uint32_t w = 640;

    engine->init(w, h);

    // resources

    engine::vbo_8        default_vbo;
    engine::shader_es_32 v8_shader("../../res/shaders/default_shader_v_8.vs",
                                   "../../res/shaders/default_shader_v_8.fs");

    bool continue_loop = true;
    while (continue_loop)
    {
        using namespace engine;

        event system_event;
        while (engine->handl_imput(system_event))
        {
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
                default:
                    break;
            }
        }
        engine->render(default_vbo, v8_shader);
        engine->swap_buffers();
    }

    return 0;
}
