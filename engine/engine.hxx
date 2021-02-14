#pragma once

#include <iostream>

namespace engine
{
enum class event
{
    left,
    right,
    up,
    down,
    turn_off
};

std::ostream& operator<<(std::ostream& out, const event& e);

class engine
{
public:
    virtual void handl_imput() = 0;
    virtual void update()      = 0;
    virtual void render()      = 0;
};
} // namespace engine
