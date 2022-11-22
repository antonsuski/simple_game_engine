#include "game_object.hxx"

actor::actor()
    : shader{ "../../res/shaders/default_shader_v_8.vs",
              "../../res/shaders/default_shader_v_8.fs" }
{
    std::cout << "Actor class - default constructor\n";
}
