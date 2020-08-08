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
void scale_up(engine::v_4& vect);
void scale_down(engine::v_4& vect);
void move_vertical(engine::v_4& vect, const bool& direction);
void move_horizontal(engine::v_4& vect, const bool& direction);
void rotation(engine::v_4& vect, const bool& direction);

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
    r_txt_sh.use();
    GLint lol = 0;
    r_txt_sh.set_uniform_1i("texture1", lol);
    lol++;
    r_txt_sh.set_uniform_1i("texture2", lol);
    // buffers
    engine::vbo_v_8 triangle("../../../res/rgb_triangle.txt");
    engine::vbo_v_8 triangle_2("../../../res/rgb_triangle_1.txt");

    engine::trans_mat_4x4 mat;
    mat.scale(4);
    // uniforms
    engine::uniform  u{ 0, 0, 0, 0 };
    std::string_view u_name{ "my_color" };
    GLuint           txt{ 0 };
    GLuint           txt1{ 0 };
    load_txt("../../../res/images/tank.png", txt);
    load_txt("../../../res/images/tank_1.png", txt1);
    r_txt_sh.use();
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
                case engine::event::button_1:
                {
                    engine::v_4 vec_4;
                    if (event.is_running)
                    {
                        for (size_t iterator{ 0 };
                             iterator < triangle.vbo_data.size(); iterator++)
                        {
                            vec_4.x = triangle.vbo_data[iterator].x;
                            vec_4.y = triangle.vbo_data[iterator].y;
                            vec_4.z = triangle.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            scale_up(vec_4);

                            triangle.vbo_data[iterator].x = vec_4.x;
                            triangle.vbo_data[iterator].y = vec_4.y;
                            triangle.vbo_data[iterator].z = vec_4.z;

                            vec_4.x = triangle_2.vbo_data[iterator].x;
                            vec_4.y = triangle_2.vbo_data[iterator].y;
                            vec_4.z = triangle_2.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            scale_up(vec_4);
                            triangle_2.vbo_data[iterator].x = vec_4.x;
                            triangle_2.vbo_data[iterator].y = vec_4.y;
                            triangle_2.vbo_data[iterator].z = vec_4.z;
                        }
                    }
                }
                break;
                case engine::event::button_2:
                {
                    engine::v_4 vec_4;
                    if (event.is_running)
                    {
                        for (size_t iterator{ 0 };
                             iterator < triangle.vbo_data.size(); iterator++)
                        {
                            vec_4.x = triangle.vbo_data[iterator].x;
                            vec_4.y = triangle.vbo_data[iterator].y;
                            vec_4.z = triangle.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            scale_down(vec_4);

                            triangle.vbo_data[iterator].x = vec_4.x;
                            triangle.vbo_data[iterator].y = vec_4.y;
                            triangle.vbo_data[iterator].z = vec_4.z;

                            vec_4.x = triangle_2.vbo_data[iterator].x;
                            vec_4.y = triangle_2.vbo_data[iterator].y;
                            vec_4.z = triangle_2.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            scale_down(vec_4);
                            triangle_2.vbo_data[iterator].x = vec_4.x;
                            triangle_2.vbo_data[iterator].y = vec_4.y;
                            triangle_2.vbo_data[iterator].z = vec_4.z;
                        }
                    }
                }
                break;
                case engine::event::up:
                {
                    engine::v_4 vec_4;
                    if (event.is_running)
                    {
                        for (size_t iterator{ 0 };
                             iterator < triangle.vbo_data.size(); iterator++)
                        {
                            vec_4.x = triangle.vbo_data[iterator].x;
                            vec_4.y = triangle.vbo_data[iterator].y;
                            vec_4.z = triangle.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            move_horizontal(vec_4, true);

                            triangle.vbo_data[iterator].x = vec_4.x;
                            triangle.vbo_data[iterator].y = vec_4.y;
                            triangle.vbo_data[iterator].z = vec_4.z;

                            vec_4.x = triangle_2.vbo_data[iterator].x;
                            vec_4.y = triangle_2.vbo_data[iterator].y;
                            vec_4.z = triangle_2.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            move_horizontal(vec_4, true);
                            triangle_2.vbo_data[iterator].x = vec_4.x;
                            triangle_2.vbo_data[iterator].y = vec_4.y;
                            triangle_2.vbo_data[iterator].z = vec_4.z;
                        }
                    }
                }
                break;
                case engine::event::down:
                {
                    engine::v_4 vec_4;
                    if (event.is_running)
                    {
                        for (size_t iterator{ 0 };
                             iterator < triangle.vbo_data.size(); iterator++)
                        {
                            vec_4.x = triangle.vbo_data[iterator].x;
                            vec_4.y = triangle.vbo_data[iterator].y;
                            vec_4.z = triangle.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            move_horizontal(vec_4, false);

                            triangle.vbo_data[iterator].x = vec_4.x;
                            triangle.vbo_data[iterator].y = vec_4.y;
                            triangle.vbo_data[iterator].z = vec_4.z;

                            vec_4.x = triangle_2.vbo_data[iterator].x;
                            vec_4.y = triangle_2.vbo_data[iterator].y;
                            vec_4.z = triangle_2.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            move_horizontal(vec_4, false);
                            triangle_2.vbo_data[iterator].x = vec_4.x;
                            triangle_2.vbo_data[iterator].y = vec_4.y;
                            triangle_2.vbo_data[iterator].z = vec_4.z;
                        }
                    }
                }
                break;
                case engine::event::left:
                {
                    engine::v_4 vec_4;
                    if (event.is_running)
                    {
                        for (size_t iterator{ 0 };
                             iterator < triangle.vbo_data.size(); iterator++)
                        {
                            vec_4.x = triangle.vbo_data[iterator].x;
                            vec_4.y = triangle.vbo_data[iterator].y;
                            vec_4.z = triangle.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            move_vertical(vec_4, false);

                            triangle.vbo_data[iterator].x = vec_4.x;
                            triangle.vbo_data[iterator].y = vec_4.y;
                            triangle.vbo_data[iterator].z = vec_4.z;

                            vec_4.x = triangle_2.vbo_data[iterator].x;
                            vec_4.y = triangle_2.vbo_data[iterator].y;
                            vec_4.z = triangle_2.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            move_vertical(vec_4, false);
                            triangle_2.vbo_data[iterator].x = vec_4.x;
                            triangle_2.vbo_data[iterator].y = vec_4.y;
                            triangle_2.vbo_data[iterator].z = vec_4.z;
                        }
                    }
                }
                break;
                case engine::event::right:
                {
                    engine::v_4 vec_4;
                    if (event.is_running)
                    {
                        for (size_t iterator{ 0 };
                             iterator < triangle.vbo_data.size(); iterator++)
                        {
                            vec_4.x = triangle.vbo_data[iterator].x;
                            vec_4.y = triangle.vbo_data[iterator].y;
                            vec_4.z = triangle.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            move_vertical(vec_4, true);

                            triangle.vbo_data[iterator].x = vec_4.x;
                            triangle.vbo_data[iterator].y = vec_4.y;
                            triangle.vbo_data[iterator].z = vec_4.z;

                            vec_4.x = triangle_2.vbo_data[iterator].x;
                            vec_4.y = triangle_2.vbo_data[iterator].y;
                            vec_4.z = triangle_2.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            move_vertical(vec_4, true);
                            triangle_2.vbo_data[iterator].x = vec_4.x;
                            triangle_2.vbo_data[iterator].y = vec_4.y;
                            triangle_2.vbo_data[iterator].z = vec_4.z;
                        }
                    }
                }
                break;
                case engine::event::start:
                {
                    engine::v_4 vec_4;
                    if (event.is_running)
                    {
                        for (size_t iterator{ 0 };
                             iterator < triangle.vbo_data.size(); iterator++)
                        {
                            vec_4.x = triangle.vbo_data[iterator].x;
                            vec_4.y = triangle.vbo_data[iterator].y;
                            vec_4.z = triangle.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            rotation(vec_4, true);

                            triangle.vbo_data[iterator].x = vec_4.x;
                            triangle.vbo_data[iterator].y = vec_4.y;
                            triangle.vbo_data[iterator].z = vec_4.z;

                            vec_4.x = triangle_2.vbo_data[iterator].x;
                            vec_4.y = triangle_2.vbo_data[iterator].y;
                            vec_4.z = triangle_2.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            rotation(vec_4, true);
                            triangle_2.vbo_data[iterator].x = vec_4.x;
                            triangle_2.vbo_data[iterator].y = vec_4.y;
                            triangle_2.vbo_data[iterator].z = vec_4.z;
                        }
                    }
                }
                break;
                case engine::event::select:
                {
                    engine::v_4 vec_4;
                    if (event.is_running)
                    {
                        for (size_t iterator{ 0 };
                             iterator < triangle.vbo_data.size(); iterator++)
                        {
                            vec_4.x = triangle.vbo_data[iterator].x;
                            vec_4.y = triangle.vbo_data[iterator].y;
                            vec_4.z = triangle.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            rotation(vec_4, false);

                            triangle.vbo_data[iterator].x = vec_4.x;
                            triangle.vbo_data[iterator].y = vec_4.y;
                            triangle.vbo_data[iterator].z = vec_4.z;

                            vec_4.x = triangle_2.vbo_data[iterator].x;
                            vec_4.y = triangle_2.vbo_data[iterator].y;
                            vec_4.z = triangle_2.vbo_data[iterator].z;
                            vec_4.w = 1;

                            std::cout << "x:" << vec_4.x << " y:" << vec_4.y
                                      << " z:" << vec_4.z << " w:" << vec_4.w
                                      << std::endl;
                            rotation(vec_4, false);
                            triangle_2.vbo_data[iterator].x = vec_4.x;
                            triangle_2.vbo_data[iterator].y = vec_4.y;
                            triangle_2.vbo_data[iterator].z = vec_4.z;
                        }
                    }
                }
                break;
                default:
                    break;
            }
        }
        u.u0 = sin(engine->get_time_for_init());
        // r_txt_sh.set_uniform_4f(u_name, u);
        engine->render_(triangle, r_txt_sh, txt, txt1);
        engine->render_(triangle_2, r_txt_sh, txt, txt1);

        engine->swap_buffers();
    }
    return EXIT_SUCCESS;
}

void scale_up(engine::v_4& vect)
{
    engine::trans_mat_4x4 met_4x4;
    met_4x4 = met_4x4.scale(1.1);
    vect    = met_4x4 * vect;
    std::cout << "new position is:\nx:" << vect.x << " y:" << vect.y
              << " z:" << vect.z << " w:" << vect.w << std::endl;
    std::cout << "--------------------\n";
}

void scale_down(engine::v_4& vect)
{
    engine::trans_mat_4x4 met_4x4;
    met_4x4 = met_4x4.scale(0.9);
    vect    = met_4x4 * vect;
    std::cout << "new position is:\nx:" << vect.x << " y:" << vect.y
              << " z:" << vect.z << " w:" << vect.w << std::endl;
    std::cout << "--------------------\n";
}

void move_vertical(engine::v_4& vect, const bool& direction)
{
    engine::trans_mat_4x4 met_4x4;
    if (direction)
    {
        met_4x4 = engine::trans_mat_4x4::move(0.1f, 0.f, 0.f);
        vect    = met_4x4 * vect;
    }
    else
    {
        met_4x4 = engine::trans_mat_4x4::move(-0.1f, 0.f, 0.f);
        vect    = met_4x4 * vect;
    }
    std::cout << "new position is:\nx:" << vect.x << " y:" << vect.y
              << " z:" << vect.z << " w:" << vect.w << std::endl;
    std::cout << "--------------------\n";
}

void move_horizontal(engine::v_4& vect, const bool& direction)
{
    engine::trans_mat_4x4 met_4x4;
    if (direction)
    {
        met_4x4 = engine::trans_mat_4x4::move(0.f, 0.1f, 0.f);
        vect    = met_4x4 * vect;
    }
    else
    {
        met_4x4 = engine::trans_mat_4x4::move(0.f, -0.1f, 0.f);
        vect    = met_4x4 * vect;
    }
    std::cout << "new position is:\nx:" << vect.x << " y:" << vect.y
              << " z:" << vect.z << " w:" << vect.w << std::endl;
    std::cout << "--------------------\n";
}

void rotation(engine::v_4& vect, const bool& direction)
{
    engine::trans_mat_4x4 met_4x4;
    if (direction)
    {
        met_4x4 = engine::trans_mat_4x4::rotate(0.1f);
        vect    = met_4x4 * vect;
    }
    else
    {
        met_4x4 = engine::trans_mat_4x4::rotate(-0.1f);
        vect    = met_4x4 * vect;
    }
    std::cout << "new position is:\nx:" << vect.x << " y:" << vect.y
              << " z:" << vect.z << " w:" << vect.w << std::endl;
    std::cout << "--------------------\n";
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
        GL_CLAMP_TO_BORDER); // установка метода наложения текстуры
                             // GL_REPEAT (стандартный метод наложения)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // установка параметров фильтрации текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // загрузка изображения, создание текстуры и генерирование
    // mipmap-уровней
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
