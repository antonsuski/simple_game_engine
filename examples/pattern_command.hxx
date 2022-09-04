#pragma once

#include <iostream>

#include "engine.hxx"

class command
{
private:
public:
    virtual ~command();
    virtual void execute() = 0;
};

class imput_handler
{
private:
    command* button_up;
    command* button_down;

public:
    command* handle_imput(engine::engine* engine, bool& continue_loop);
};

command* imput_handler::handle_imput(engine::engine* engine,
                                     bool&           continue_loop)
{
    using namespace engine;

    event system_event;
    while (engine->handl_imput(system_event))
    {
        if (system_event.type == event::turn_off)
        {
            continue_loop = false;
            std::cout << system_event << '\n';
            return nullptr;
        }
    }
    if (system_event.type == event::up)
    {
        std::cout << system_event << '\n';
        return button_up->execute();
    }
    if (system_event.type == event::down)
    {
        std::cout << system_event << '\n';
        return button_down->execute();
    }
    return nullptr;
}
