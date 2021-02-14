#include <algorithm>
#include <array>
#include <string_view>
#include <vector>

#include "SDL2/SDL.h"
#include "engine.hxx"

struct bind
{
    engine::event    event;
    std::string_view name;

    bind(engine::event event_, std::string_view name_)
        : event{ event_ }
        , name(name_)
    {
    }
};

std::array<bind, 5> key_map{ { { engine::event::left, "left" },
                               { engine::event::right, "right" },
                               { engine::event::up, "up" },
                               { engine::event::down, "down" },
                               { engine::event::turn_off, "turn_off" } } };
static bool         cheack_event{};
namespace engine
{
// std::ostream& operator<<(std::ostream& out, const event& e)
//{
//    return out;
//}

} // namespace engine
