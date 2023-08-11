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

    uint32_t h = 640;
    uint32_t w = 640;

    engine->init(w, h);

    engine::object2d obj{ "../../res/rgba_square.txt",
                          "../../res/shaders/shader_v_8",
                          { "../../res/images/tank.png",
                            engine::texture::RGBA } };

    bool continue_loop = true;
    while (continue_loop)
    {
        using namespace engine;

        event system_event;
        while (engine->handl_imput(system_event))
        {
            if(system_event.type == event::turn_off)
            {
                continue_loop = false;
                std::clog << system_event << "\n";
            }
        }

        if(engine->is_button_pushed(button::space_button))
        {
            std::clog << "space_button\n";
        }

        v_2 win_size = engine->get_windonw_size();
        obj.get_shader()->set_uniform_2f("resolution", win_size);
        engine->render(obj);
        engine->swap_buffers();
    }

    return 0;
}
