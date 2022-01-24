#include <iostream>
#include <memory>
#include <string_view>

#include "engine.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    std::string_view lol{ "lol" };
    std::cout << lol << std::endl;

    std::unique_ptr<engine::engine, void (*)(engine::engine*)> engine(
        engine::create_engine(), engine::destroy_engine);

    uint32_t h = 480;
    uint32_t w = 640;

    engine->init(w, h);

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
                engine->tmp_test_method();
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
            {
            }
            break;
        }
        engine->swap_buffers();
    }

    return 0;
}
