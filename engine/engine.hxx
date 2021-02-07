#pragma once

#include <iostream>

namespace engine
{
enum class event
{
    key_w,
    key_a,
    key_s,
    key_d,
    key_q,
    key_e,
    key_enter,
    key_esc,
    turn_off
};

// std::istream& operator>>(std::istream& in, event& e);
std::ostream& operator<<(std::ostream& out, const event& e);

class engine
{
public:
    virtual void handl_imput() = 0;
    virtual void update()      = 0;
    virtual void render()      = 0;
};
} // namespace engine
