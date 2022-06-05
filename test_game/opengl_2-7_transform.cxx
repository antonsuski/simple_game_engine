#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <string_view>

#include "engine.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    std::string_view lol{ "lol? Hello world" };
    std::cout << lol << std::endl;

    std::unique_ptr<engine::engine, void (*)(engine::engine*)> engine(
        engine::create_engine(), engine::destroy_engine);

    uint32_t h = 640;
    uint32_t w = 640;

    engine->init(w, h);

    engine::object2d obj{ "../../res/rgba_square.txt",
                          "../../res/shaders/shader_transform_v_8.vs",
                          "../../res/shaders/shader_v_8.fs",
                          "../../res/images/awesomeface.png",
                          engine::texture::RGBA };

    std::cout << "-------------------glm------------------" << std::endl;
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans           = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
    vec             = trans * vec;
    std::cout << "x:" << vec.x << " y:" << vec.y << " z:" << vec.z << std::endl;

    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    std::cout << "-------------------glm------------------" << std::endl;

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
        trans = glm::rotate(
            trans, static_cast<float>(engine->get_time_from_init() * 0.001),
            glm::vec3(0.0, 0.0, 1.0));
        v_2 win_size = engine->get_windonw_size();
        obj.get_shader()->set_uniform_2f("resolution", win_size);
        obj.get_shader()->set_uniform_4mat("transform", trans);
        engine->render(obj);
        engine->swap_buffers();
    }

    return 0;
}
