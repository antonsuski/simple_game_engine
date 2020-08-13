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
    engine::shader_es_32 glm_txt_sh("../../../res/shaders/glm_txt_sh.vs",
                                    "../../../res/shaders/glm_txt_sh.fs");

    // buffers
    engine::vbo_v_8 triangle("../../../res/rgb_triangle.txt");
    engine::vbo_v_8 triangle_2("../../../res/rgb_triangle_1.txt");
    engine::vbo_v_8 tank_0("../../../res/tank_1_0.txt");
    engine::vbo_v_8 tank_1("../../../res/tank_1_1.txt");

    // uniforms
    engine::uniform u{ "my_color", 0, 0, 0, 0 };
    // load_txt("../../../res/images/tank.png", txt);
    // load_txt("../../../res/images/tank_1.png", txt1);
    r_txt_sh.use();
    // engine

    // expirements
    engine::texture_2d_es_32 tank_txt("../../../res/images/tank.png");
    engine::texture_2d_es_32 tank_1_txt("../../../res/images/tank_1.png");
    engine::trans_mat_4x4    mat1{
        { 4, 0, 0, 0 }, { 2, 8, 1, 0 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 }
    };
    engine::trans_mat_4x4 mat2{
        { 4, 2, 9, 0 }, { 2, 0, 4, 0 }, { 1, 4, 2, 0 }, { 0, 0, 0, 0 }
    };
    engine::v_4 vec = { -0.5f, 0.5f, 0.f, 0.f };

    engine::trans_mat_4x4 scale_m_ =
        engine::trans_mat_4x4::scale(0.9, 0.9, 1.f);
    engine::trans_mat_4x4 move_m_ = engine::trans_mat_4x4::move(0.5, 0.5, 0.0);
    engine::trans_mat_4x4 mat_    = move_m_ * scale_m_;

    std::cout << scale_m_ << move_m_ << mat_ << std::endl;
    std::cout << scale_m_ * vec << std::endl;
    std::cout << move_m_ * vec << std::endl;
    std::cout << mat_ * vec << std::endl;

    // end of expirements
    engine::v_3 current_pos(0.f, 0.f, 0.f);
    engine::v_2 current_scale(1.f, 1.f);
    float       current_direction(0.f);
    float       glm_direction(0.f);
    const float pi            = std::numbers::pi_v<float>;
    bool        continue_loop = true;
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
                    if (event.is_running)
                    {
                        current_scale.x += 0.1;
                        current_scale.y += 0.1;
                    }
                }
                break;
                case engine::event::button_2:
                {
                    if (event.is_running)
                    {
                        current_scale.x -= 0.1;
                        current_scale.y -= 0.1;
                    }
                }
                break;
                case engine::event::up:
                {
                    if (event.is_running)
                    {
                        current_pos.y += 0.05f;
                        current_direction = 0.f;
                        glm_direction     = 0.f;
                    }
                }
                break;
                case engine::event::down:
                {
                    if (event.is_running)
                    {
                        current_pos.y -= 0.05f;
                        current_direction = -pi;
                        glm_direction     = 180.f;
                    }
                }
                break;
                case engine::event::left:
                {
                    if (event.is_running)
                    {
                        current_direction = pi / 2.f;
                        current_pos.x -= 0.05f;
                        glm_direction = 90.f;
                    }
                }
                break;
                case engine::event::right:
                {
                    if (event.is_running)
                    {
                        current_pos.x += 0.05f;
                        current_direction = -pi / 2.f;
                        glm_direction     = -90.f;
                    }
                }
                break;
                case engine::event::start:
                {
                }
                break;
                case engine::event::select:
                {
                }
                break;
                default:
                    break;
            }
        }
        // engine matrix
        engine::trans_mat_4x4 scale_m =
            engine::trans_mat_4x4::scale(current_scale.x, current_scale.y,
                                         1.f) *
            engine::trans_mat_4x4::scale((766.f / 1364.f), 766.f / 1364.f, 1.f);
        engine::trans_mat_4x4 move_m = engine::trans_mat_4x4::move(current_pos);
        engine::trans_mat_4x4 rot_m =
            engine::trans_mat_4x4::rotate(current_direction);
        engine::trans_mat_4x4 mat = move_m * rot_m * scale_m;

        // glm matrix
        glm::mat4 trans = glm::mat4(1.0f);
        trans           = glm::scale(trans,
                           glm::vec3(current_scale.x, current_scale.y, 1.0f));
        trans           = glm::rotate(trans, glm::radians(glm_direction),
                            glm::vec3(0.0f, 0.0f, 1.0f));
        trans           = glm::translate(trans,
                               glm::vec3(current_pos.x, current_pos.y, 0.0f));

        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::rotate(model, glm::radians(-55.0f),
                            glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 view  = glm::mat4(1.0f);
        view            = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f,
                                      0.1f, 100.0f);

        // render
        u.u0 = sin(engine->get_time_for_init());

        r_txt_sh.set_uniform_4mat("u_matrix", mat);

        glm_txt_sh.set_uniform_4mat("u_matrix", trans);
        glm_txt_sh.set_uniform_4mat("u_matrix_model", model);
        glm_txt_sh.set_uniform_4mat("u_matrix_view", view);
        glm_txt_sh.set_uniform_4mat("u_matrix_projection", projection);

        engine->render(triangle, r_txt_sh, tank_txt);
        engine->render(triangle_2, r_txt_sh, tank_txt);
        engine->render(tank_0, glm_txt_sh, tank_1_txt);
        engine->render(tank_1, glm_txt_sh, tank_1_txt);
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
