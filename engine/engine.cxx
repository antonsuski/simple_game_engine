#include <algorithm>
#include <array>
#include <string_view>
#include <vector>

#include "SDL.h"
#include "engine.hxx"

struct bind
{
    engine::event event_;

    bind(engine::event::event_type event_type, std::string name_)
        : event_(event_type, name_)
    {
    }
};

std::array<bind, 5> key_map{ { { engine::event::left, "left" },
                               { engine::event::right, "right" },
                               { engine::event::up, "up" },
                               { engine::event::down, "down" },
                               { engine::event::turn_off, "turn_off" } } };
namespace engine
{

void event::release_event()
{
    if (event_state)
    {
        event_state = false;
        std::clog << this->name + "_releas";
    }
}

void event::push_event()
{
    event_state = true;
    std::clog << this->name + "_releas";
}

event::event(event::event_type e_type, std::string e_name)
    : type(e_type)
    , name(e_name)
    , event_state(false)
{
}

std::ostream& operator<<(std::ostream& out, const event& e)
{
    std::uint32_t value   = static_cast<std::uint32_t>(e.type);
    std::uint32_t minimal = static_cast<std::uint32_t>(event::up);
    std::uint32_t maximal = static_cast<std::uint32_t>(event::turn_off);
    if (value >= minimal && value <= maximal)
    {
        if (e.event_state)
        {
            out << e.name;
            out << "_running";
            return out;
        }
        out << e.name;
        out << "_is_finished";
        return out;
    }
    else
    {
        throw std::runtime_error("too big event value");
    }
}

// std::ostream& operator<<(std::ostream& out, const event& e)
//{
//    return out;
//}

} // namespace engine
