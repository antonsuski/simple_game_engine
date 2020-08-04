#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "engine.hxx"
#include "shader.hxx"
#include "stuff.hxx"
#include "texture2d.hxx"
#include "vbo.hxx"

void load_txt(std::string_view path, GLuint& texture_id);

int main(int /*argc*/, char* /*argv*/[])
{
    std::unique_ptr<engine::core, void (*)(engine::core*)> engine(
        engine::create_engine(), engine::destroy_engine);

    if (!engine->init(1364, 766))
    {
        std::cerr << "init failed" << std::endl;
        return EXIT_FAILURE;
    }

    // shaders
    engine::shader_es_32 default_shader(
        "../../../res/shaders/default_shader_v_8.vs",
        "../../../res/shaders/default_shader_v_8.fs");
    engine::shader_es_32 def_txt_sh(
        "../../../res/shaders/default_texture_shader_v_8.vs",
        "../../../res/shaders/default_texture_shader_v_8.fs");
    engine::shader_es_32 r_txt_sh("../../../res/shaders/r_txt.vs",
                                  "../../../res/shaders/r_txt.fs");
    // buffers
    engine::vbo_v_8 triangle("../../../res/rgb_triangle.txt");
    engine::vbo_v_8 triangle_2("../../../res/rgb_triangle_1.txt");
    // uniforms
    engine::uniform  u{ 0, 0, 0, 0 };
    std::string_view u_name{ "my_color" };
    GLuint           txt{ 0 };
    load_txt("../../../res/images/tank.png", txt);
    // engine
    bool continue_loop = true;
    while (continue_loop)
    {
        engine::event event;

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

        u.u0 = sin(engine->get_time_for_init());
        r_txt_sh.set_uniform_4f(u_name, u);
        engine->render_(triangle, r_txt_sh, txt);
        engine->render_(triangle_2, r_txt_sh, txt);

        engine->swap_buffers();
    }
    return EXIT_SUCCESS;
}

void load_txt(std::string_view path, GLuint& texture_id)
{
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D,
                  texture_id); // все последующие GL_TEXTURE_2D-операции теперь
                               // будут влиять на данный текстурный объект
    // установка параметров наложения текстуры
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_BORDER); // установка метода наложения текстуры GL_REPEAT
                             // (стандартный метод наложения)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // установка параметров фильтрации текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // загрузка изображения, создание текстуры и генерирование mipmap-уровней
    stbi_set_flip_vertically_on_load(true);
    int            width, height, nrChannels;
    unsigned char* data =
        stbi_load(path.data(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}
