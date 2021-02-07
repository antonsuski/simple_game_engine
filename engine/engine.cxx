#include <array>
#include <string_view>
#include <vector>

#include "SDL2/SDL.h"
#include "engine.hxx"

struct bind
{
    engine::event    e;
    std::string_view name;

    bind(engine::event e_, std::string_view name_)
        : e{ e_ }
        , name{ name_ }
    {
    }
};

// std::array<engine::event, 9> key_map{
//    { engine::event::key_w, "w_" },          { engine::event::key_a, "a_" },
//    { engine::event::key_s, "s_" },          { engine::event::key_d, "d_" },
//    { engine::event::key_q, "q_" },          { engine::event::key_e, "e_" },
//    { engine::event::key_enter, "enter_" },  { engine::event::key_esc, "w_" },
//    { engine::event::turn_off, "turn_off_" }
//};

namespace engine
{
// std::istream& operator>>(std::istream& in, event& e) {}

std::ostream& operator<<(std::ostream& out, const event& e) {}
} // namespace engine
