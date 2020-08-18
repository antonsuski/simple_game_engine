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

#include "SDL.h"
#include "engine.hxx"
#include "shader.hxx"
#include "stuff.hxx"
#include "texture2d.hxx"
#include "vbo.hxx"

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
    engine::vbo_v_8 tank_0("../../../res/rgb_triangle.txt");
    engine::vbo_v_8 tank_1("../../../res/rgb_triangle_1.txt");

    // uniforms
    engine::uniform u{ "my_color", 0, 0, 0, 0 };
    // engine

    // expirements
    engine::texture_2d_es_32 tank_1_txt("../../../res/images/tank_1.png");

    engine::vbo_v_8          cube("../../../res/3d_cube.txt");
    engine::texture_2d_es_32 cube_txt("../../../res/images/wood.png");

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    // end of expirements
    engine::v_3 current_pos(0.f, 0.f, 0.f);
    engine::v_2 current_scale(1.f, 1.f);
    float       current_direction(0.f);
    float       glm_direction(0.f);
    float       perspective_fov(45.f);

    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 direction;

    float yaw   = -90.f;
    float pitch = -45.f;

    direction.x = cos(glm::radians(yaw));
    direction.z = sin(glm::radians(yaw));
    direction.y = sin(glm::radians(pitch));

    const float pi = std::numbers::pi_v<float>;

    bool continue_loop = true;
    while (continue_loop)
    {
        engine::event event;
        SDL_CaptureMouse(SDL_TRUE);
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

        engine->mouse_capture(true);
        const float cam_speed{ 0.05f };

        if (event.is_running && event.key == engine::event::up)
        {
            //            current_pos.y += 0.05f;
            //            current_direction = 0.f;
            //            glm_direction     = 0.f;

            cameraPos += (cam_speed * cameraFront);
        }
        else if (event.is_running && event.key == engine::event::down)
        {
            //            current_pos.y -= 0.05f;
            //            current_direction = -pi;
            //            glm_direction     = 180.f;

            cameraPos -= cam_speed * cameraFront;
        }
        else if (event.is_running && event.key == engine::event::left)
        {
            //            current_direction = pi / 2.f;
            //            current_pos.x -= 0.05f;
            //            glm_direction = 90.f;
            cameraPos -=
                glm::normalize(glm::cross(cameraFront, cameraUp)) * cam_speed;
        }
        else if (event.is_running && event.key == engine::event::right)
        {
            //            current_pos.x += 0.05f;
            //            current_direction = -pi / 2.f;
            //            glm_direction     = -90.f;
            cameraPos +=
                glm::normalize(glm::cross(cameraFront, cameraUp)) * cam_speed;
        }
        else if (event.is_running && event.key == engine::event::button_1)
        {
            current_scale.x += 0.1;
            current_scale.y += 0.1;
        }
        else if (event.is_running && event.key == engine::event::button_2)
        {
            current_scale.x -= 0.1;
            current_scale.y -= 0.1;
        }
        else if (event.is_running && event.key == engine::event::select)
        {
            perspective_fov--;
        }
        else if (event.is_running && event.key == engine::event::start)
        {
            perspective_fov++;
        }

        // engine matrix
        //        engine::trans_mat_4x4 scale_m =
        //            engine::trans_mat_4x4::scale(current_scale.x,
        //            current_scale.y,
        //                                         1.f) *
        //            engine::trans_mat_4x4::scale((766.f / 1364.f), 766.f /
        //            1364.f, 1.f);
        //        engine::trans_mat_4x4 move_m =
        //        engine::trans_mat_4x4::move(current_pos);
        //        engine::trans_mat_4x4 rot_m =
        //            engine::trans_mat_4x4::rotate(current_direction);
        //        engine::trans_mat_4x4 mat = move_m * rot_m * scale_m;

        // glm matrix

        glm::mat4 trans = glm::mat4(1.0f);
        trans           = glm::translate(trans,
                               glm::vec3(current_pos.x, current_pos.y, 0.0f));
        trans           = glm::rotate(trans, glm::radians(glm_direction),
                            glm::vec3(0.0f, 0.0f, 1.0f));
        trans           = glm::scale(trans,
                           glm::vec3(current_scale.x, current_scale.y, 1.0f));

        glm::mat4 model = glm::mat4(1.0f);
        //        model           = glm::rotate(model,
        //                            engine->get_time_for_init() *
        //                            glm::radians(50.0f),
        //                            glm::vec3(0.5f, 1.0f, 0.0f));
        //        glm::mat4 view = glm::mat4(1.0f);
        //        view           = glm::translate(view, glm::vec3(0.0f, 0.0f,
        //        -3.0f));

        const float radius = 10.0f;
        float       camX   = sin(engine->get_time_for_init()) * radius;
        float       camZ   = cos(engine->get_time_for_init()) * radius;

        glm::mat4 view =
            glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(perspective_fov),
                                      800.0f / 600.0f, 0.1f, 100.0f);

        // render
        //        u.u0 = sin(engine->get_time_for_init());

        //        r_txt_sh.set_uniform_4mat("u_matrix", mat);

        glm_txt_sh.set_uniform_4mat("u_matrix", trans);
        //        glm_txt_sh.set_uniform_4mat("u_matrix_model", model);
        glm_txt_sh.set_uniform_4mat("u_matrix_view", view);
        glm_txt_sh.set_uniform_4mat("u_matrix_projection", projection);

        for (size_t i{ 0 }; i < 10; i++)
        {
            model       = glm::mat4(1.0f);
            float angle = 20.0f * i;
            if (i % 3 == 1)
            {
                angle *= engine->get_time_for_init();
            }
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(angle),
                                glm::vec3(1.0f, 0.3f, 0.5f));
            glm_txt_sh.set_uniform_4mat("u_matrix_model", model);
            engine->render(cube, glm_txt_sh, cube_txt);
        }
        //        engine->render(tank_0, glm_txt_sh,
        //        tank_1_txt); engine->render(tank_1,
        //        glm_txt_sh, tank_1_txt);

        engine->swap_buffers();
    }
    return EXIT_SUCCESS;
}
