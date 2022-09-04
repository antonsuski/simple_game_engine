#include <iostream>
#include <memory>
#include <string_view>

#include "engine.hxx"
#include "pattern_command.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    using namespace engine;

    std::string_view lol{ "lol? Hello world" };
    std::cout << lol << std::endl;

    std::unique_ptr<engine::engine, void (*)(engine::engine*)> engine(
        engine::create_engine(), engine::destroy_engine);

    uint32_t h = 640;
    uint32_t w = 640;

    engine->init(w, h);

    engine::object2d new_obj;
    engine::object2d obj{ "../../res/rgba_square.txt",
                          "../../res/shaders/shader_v_8",
                          { "../../res/images/awesomeface.png",
                            engine::texture::RGBA } };

    imput_handler event_handler;

    bool continue_loop = true;
    while (continue_loop)
    {
        event_handler.handle_imput(engine.get(), continue_loop);

        v_2 win_size = engine->get_windonw_size();
        obj.get_shader()->set_uniform_2f("resolution", win_size);
        engine->render(new_obj);
        engine->render(obj);
        engine->swap_buffers();
    }

    return 0;
}
