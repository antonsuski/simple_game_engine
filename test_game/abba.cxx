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
    glm::mat4 view =
        glm::rotate(view, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));

    glm::mat4 trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.0));
    std::cout << "-------------------glm------------------" << std::endl;

    bool continue_loop = true;
    while (continue_loop)
    {
        using namespace engine;
        event system_event;
        while (engine->handl_imput(system_event))
        {
            if (system_event.type == event::turn_off)
            {
                continue_loop = false;
            }
            if (system_event.event_state)
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
                        obj.translate(glm::vec3(-0.01, 0.0, 0.0));
                    }
                    break;
                    case event::right:
                    {
                        obj.translate(glm::vec3(0.01, 0.0, 0.0));
                    }
                    break;
                    case event::up:
                    {
                        obj.scale(glm::vec3(1.2f, 1.2f, 0.0f));
                    }
                    break;
                    case event::down:
                    {
                        obj.scale(glm::vec3(0.8f, 0.8f, 0.0f));
                    }
                    break;
                    case event::button_1:
                    {
                        obj.rotate(1);
                    }
                    break;
                    case event::button_2:
                    {
                        obj.rotate(-1);
                    }
                    break;
                    default:
                        break;
                }
            }
        }

        if (obj.is_collide_point(engine->get_mouse_coords()))
        {
            std::cout << "lol\n";
        }

        engine->render(obj);
        engine->swap_buffers();
    }

    return 0;
}
