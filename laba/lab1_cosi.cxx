#include <cassert>
#include <cmath>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <numbers>
#include <vector>

#include "engine.hxx"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "shader.hxx"
#include "stuff.hxx"
#include "texture2d.hxx"
#include "vbo.hxx"

#include <SDL2/SDL.h>

int main (int /*argc*/, char* /*argv*/[])
{
    // create engine
    std::unique_ptr<engine::core, void (*)(engine::core*)> engine(
        engine::create_engine(), engine::destroy_engine);

    // resolution
    int width{ 640 }, height{ 480 };

    // buffer
//    engine::vbo_v_8 square("../../res/square.txt");

//    // shaders
//    engine::shader_es_32 default_shader(
//        "../res/shaders/default_shader_v_8.vs",
//        "../res/shaders/default_shader_v_8.fs");
//    engine::shader_es_32 def_txt_sh(
//        "../res/shaders/default_texture_shader_v_8.vs",
//        "../res/shaders/default_texture_shader_v_8.fs");

//    // texture
//     engine::texture_2d_es_32 tank_1_txt("../res/images/tank_1.png");

    // init engine and create window
    if (!engine->init(width, height))
    {
        std::cerr << "init failed" << std::endl;
        return EXIT_FAILURE;
    }

    engine::v_3 clear_color = {0.45f, 0.55f, 0.60f};
    engine::vbo_v_8 square("../../res/square.txt");

    //main proc loop
    bool continue_loop = true;
    while (continue_loop)
    {
        engine::event event;
        SDL_Event     sdl_event;
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
        //engine->render(square, def_txt_sh, tank_1_txt);
        engine->swap_buffers(clear_color);
    }
    return 0;
}
