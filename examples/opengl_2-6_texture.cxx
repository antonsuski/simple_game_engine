#include <iostream>
#include <memory>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "engine.hxx"

int main(int /*argc*/, char** /*argv*/)
{
    std::cout << "Hello opengl_2-6_texture" << std::endl;

    std::unique_ptr<engine::engine, void (*)(engine::engine*)> engine(
        engine::create_engine(), engine::destroy_engine);

    uint32_t h = 480;
    uint32_t w = 640;
    uint32_t tex_id;
    int      tex_h;
    int      tex_w;
    int      tex_channals;

    unsigned char* tex_data;

    float tex_color[] = { 1.0f, 1.0f, 0.0f, 1.0f };

    engine->init(w, h);

    engine::vbo_6        lol_buffer("../../res/rgb_triangle_v_6.txt");
    engine::shader_es_32 lol_shader("../../res/shaders/shader_v_6.vs",
                                    "../../res/shaders/shader_v_6.fs");

    engine::vbo_8        v8_buffer("../../res/rgb_triangle.txt");
    engine::shader_es_32 v8_shader("../../res/shaders/shader_v_8.vs",
                                   "../../res/shaders/shader_v_8.fs");
    engine::shader_es_32 v8_double_sh(
        "../../res/shaders/shader_v_8.vs",
        "../../res/shaders/shader_double_tex_v_8.fs");
    std::vector<std::pair<engine::texture, engine::tex_unit>> tex_buffer{
        std::make_pair(engine::texture{ "../../res/images/wall.jpg",
                                        engine::texture::RGB },
                       engine::unit_0),
        std::make_pair(engine::texture{ "../../res/images/awesomeface.png",
                                        engine::texture::RGBA },
                       engine::unit_1)
    };

    v8_double_sh.set_uniform_1i("first_texture", 0);
    v8_double_sh.set_uniform_1i("second_texture", 1);

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
        engine::v_2 win_res = engine->get_windonw_size();
        v8_double_sh.set_uniform_2f("resolution", win_res);
        engine->render(v8_buffer, v8_double_sh, tex_buffer);
        engine->swap_buffers();
    }
    return EXIT_SUCCESS;
}
