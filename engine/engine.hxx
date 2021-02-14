#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace engine
{

class event
{
public:
    enum event_type
    {
        left,
        right,
        up,
        down,
        turn_off
    };

    event_type  type;
    std::string name;
    bool        event_state;

    event(event::event_type e_type, std::string e_name);

private:
    void release_event();
    void push_event();
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
