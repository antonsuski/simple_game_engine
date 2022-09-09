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
    uint32_t w = 1000;

    engine->init(w, h);

    engine::object2d new_obj;
    engine::object2d obj{ "../../res/rgba_square.txt",
                          "../../res/shaders/shader_transform_v_8.vs",
                          "../../res/shaders/shader_v_8.fs",
                          "../../res/images/awesomeface.png",
                          engine::texture::RGBA };

    imput_handler event_handler;

    bool continue_loop = true;
    while (continue_loop)
    {
        command* current_command =
            event_handler.new_handle_imput(continue_loop);
        if (current_command)
        {
            current_command->execute(&obj);
        }

        v_2 win_size = engine->get_windonw_size();
        obj.get_shader()->set_uniform_2f("resolution", win_size);
        engine->render(new_obj);
        engine->render(obj);
        engine->swap_buffers();
    }

    return 0;
}
