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

int main(int /*argc*/, char* /*argv*/[])
{
    float trololo = 0 / 100.f;
    std::clog << trololo;

    std::unique_ptr<engine::core, void (*)(engine::core*)> engine(
        engine::create_engine(), engine::destroy_engine);
    int width{ 1364 }, height{ 766 };
    if (!engine->init(width, height))
    {
        std::cerr << "init failed" << std::endl;
        return EXIT_FAILURE;
    }

    // shaders
    engine::shader_es_32 default_shader(
        "../../res/shaders/default_shader_v_8.vs",
        "../../res/shaders/default_shader_v_8.fs");
    engine::shader_es_32 def_txt_sh(
        "../../res/shaders/default_texture_shader_v_8.vs",
        "../../res/shaders/default_texture_shader_v_8.fs");
    engine::shader_es_32 r_txt_sh("../../res/shaders/r_txt.vs",
                                  "../../res/shaders/r_txt.fs");
    engine::shader_es_32 glm_txt_sh("../../res/shaders/glm_txt_sh.vs",
                                    "../../res/shaders/glm_txt_sh.fs");

    // buffers
    engine::vbo_v_8 tank_0("../../res/rgb_triangle.txt");
    engine::vbo_v_8 tank_1("../../res/rgb_triangle_1.txt");

    // uniforms
    engine::uniform u{ "my_color", 0, 0, 0, 0 };
    // engine

    /// expirements

    engine::texture_2d_es_32   tank_1_txt("../../res/images/tank_1.png");
    engine::animation_2d_es_32 explosion(
        "../../res/animation/explosion/explosion_atlas4.png", 8, 8);
    engine::texture_2d_es_32 cube_txt("../../res/images/wood.png");

    engine::vbo_v_8 cube("../../res/3d_cube.txt");
    engine::vbo_v_8 square("../../res/square.txt");
    /// end of expirements

    engine::v_3 current_pos(0.f, 0.f, 0.f);
    engine::v_3 current_scale(1.f, 1.f, 1.f);
    float       current_direction(0.f);
    float       glm_direction(0.f);
    float       perspective_fov(45.f);

    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 direction;

    float yaw   = -90.f;
    float pitch = 0.f;

    //    direction.x = cos(glm::radians(yaw));
    //    direction.z = sin(glm::radians(yaw));
    //    direction.y = sin(glm::radians(pitch));

    const float pi = std::numbers::pi_v<float>;
    const float cam_speed{ 0.05f };
    const float cam_sens{ 0.05f };
    bool        mouse_capture{ false };
    // engine->mouse_capture(true);

    // this is impletation of imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplSDL2_InitForOpenGL(engine->window, engine->gl_context);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    //    ImGui::StyleColorsDark();

    bool   show_demo_window    = true;
    bool   show_another_window = false;
    ImVec4 clear_color         = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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
        ImGui_ImplSDL2_ProcessEvent(&sdl_event);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(engine->window);
        ImGui::NewFrame();

        if (event.is_running && event.key == engine::event::up)
        {
            //            current_pos.y += 0.05f;
            //            current_direction = 0.f;
            //            glm_direction     = 0.f;

            cameraPos += (cam_speed * cameraFront);
        }
        if (event.is_running && event.key == engine::event::down)
        {
            //            current_pos.y -= 0.05f;
            //            current_direction = -pi;
            //            glm_direction     = 180.f;

            cameraPos -= cam_speed * cameraFront;
        }
        if (event.is_running && event.key == engine::event::left)
        {
            //            current_direction = pi / 2.f;
            //            current_pos.x -= 0.05f;
            //            glm_direction = 90.f;
            cameraPos -=
                glm::normalize(glm::cross(cameraFront, cameraUp)) * cam_speed;
        }
        if (event.is_running && event.key == engine::event::right)
        {
            //            current_pos.x += 0.05f;
            //            current_direction = -pi / 2.f;
            //            glm_direction     = -90.f;
            cameraPos +=
                glm::normalize(glm::cross(cameraFront, cameraUp)) * cam_speed;
        }
        if (event.is_running && event.key == engine::event::button_1)
        {
            current_scale.x += 0.1;
            current_scale.y += 0.1;
        }
        if (event.is_running && event.key == engine::event::button_2)
        {
            current_scale.x -= 0.1;
            current_scale.y -= 0.1;
        }
        if (event.is_running && event.key == engine::event::select)
        {
            mouse_capture = true;
            engine->mouse_capture(mouse_capture);
        }
        if (event.is_running && event.key == engine::event::start)
        {
            mouse_capture = false;
            engine->mouse_capture(mouse_capture);
        }
        if (event.is_running && event.key == engine::event::mouse_move &&
            mouse_capture)
        {
            yaw += event.mouse_delta.x * cam_sens;
            pitch -= event.mouse_delta.y * cam_sens;

            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        static float x_scale = 0.0f;
        static float y_scale = 0.0f;
        static float z_scale = 0.0f;
        {

            ImGuiWindowFlags window_flags = 0;
            window_flags |= ImGuiWindowFlags_NoTitleBar;
            window_flags |= ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoResize;

            ImGui::Begin("Context menu", nullptr,
                         window_flags); // Create a window called "Hello,
            // world!" and append into it.
            ImGui::SetWindowPos({ 0, 0 });

            ImGui::Text("Scale tuning."); // Display some text (you can use
                                          // a format strings too)
            if (ImGui::SliderFloat("x_scale", &x_scale, -1.0f, 1.0f))
            {
                current_scale.x += (x_scale / 100);
            }
            if (ImGui::SliderFloat("y_scale", &y_scale, -1.0f, 1.0f))
            {
                current_scale.y += (y_scale / 100);
            }
            if (ImGui::SliderFloat("z_scale", &z_scale, -1.0f, 1.0f))
            {
                current_scale.z += (z_scale / 100);
            }
            ImGui::ColorEdit3(
                "clear color",
                (float*)&clear_color); // Edit 3 floats representing a color

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();
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
        trans = glm::scale(trans, glm::vec3(current_scale.x, current_scale.y,
                                            current_scale.z));

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

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraFront = glm::normalize(direction);

        glm::mat4 view =
            glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(perspective_fov),
                                                800.0f / 600.0f, 0.1f, 100.0f);

        // render

        glm_txt_sh.set_uniform_4mat("u_matrix", trans);
        glm_txt_sh.set_uniform_4mat("u_matrix_model", model);
        glm_txt_sh.set_uniform_4mat("u_matrix_view", view);
        glm_txt_sh.set_uniform_4mat("u_matrix_projection", projection);

        engine->render(square, glm_txt_sh, explosion);
        //        for (int i{ 0 }; i < 10; i++)
        //        {
        //            model       = glm::mat4(1.0f);
        //            float angle = 20.0f * i;
        //            if (i % 3 == 1)
        //            {
        //                angle *= engine->get_time_for_init();
        //            }
        //            model = glm::translate(model, cubePositions[i]);
        //            model = glm::rotate(model, glm::radians(angle),
        //                                glm::vec3(1.0f, 0.3f, 0.5f));
        //            glm_txt_sh.set_uniform_4mat("u_matrix_model", model);
        //            engine->render(cube, glm_txt_sh, cube_txt);
        //        }
        //        engine->render(tank_0, glm_txt_sh,
        //        tank_1_txt); engine->render(tank_1,
        //        glm_txt_sh, tank_1_txt);
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        engine::v_3 color{ clear_color.x, clear_color.y, clear_color.z };
        engine->swap_buffers(color);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    return EXIT_SUCCESS;
}
