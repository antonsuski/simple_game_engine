#include <iostream>
#include <memory>
#include <string_view>

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

    // engine::vbo_6        lol_buffer("../../res/rgb_triangle_v_6.txt");
    // engine::shader_es_32 lol_shader("../../res/shaders/shader_v_6.vs",
    //                                 "../../res/shaders/shader_v_6.fs");

    engine::vbo_8        v8_buffer("../../res/rgb_triangle.txt");
    engine::shader_es_32 v8_shader("../../res/shaders/shader_v_8.vs",
                                   "../../res/shaders/shader_v_8.fs");

    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, tex_color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    tex_data = stbi_load("../../res/images/wall.jpg", &tex_w, &tex_h,
                         &tex_channals, 0);
    if (tex_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_w, tex_h, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, tex_data);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Cant load image" << std::endl;
        stbi_image_free(tex_data);
    }

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
        glBindTexture(GL_TEXTURE_2D, tex_id);
        // engine->render(lol_buffer, lol_shader);
        engine->render(v8_buffer, v8_shader);
        engine->swap_buffers();
    }

    return EXIT_SUCCESS;
}
