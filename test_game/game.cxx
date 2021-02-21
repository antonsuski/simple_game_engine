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

    engine->init(h, w);

    bool continue_loop = true;
    while (continue_loop)
    {
        using namespace engine;

        event game_event;
        engine->handl_imput(game_event);
        switch (game_event.type)
        {
            case event::turn_off:
            {
                std::clog << game_event;
                continue_loop = false;
            }
            break;
            case event::left:
            {
                std::clog << game_event;
            }
            break;
            case event::right:
            {
                std::clog << game_event;
            }
            break;
            case event::up:
            {
                std::clog << game_event;
            }
            break;
            case event::down:
            {
                std::clog << game_event;
            }
            break;
        }
    }

    return 0;
}
