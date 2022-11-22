#pragma once

#include <SDL.h>
#include <iostream>

#include "engine.hxx"

// class command
// {
// private:
//     virtual ~command(){};

// public:
//     virtual void execute(engine::object2d* obj) = 0;
// };

class command_go_up : public engine::command
{
private:
public:
    virtual ~command_go_up(){};
    virtual void execute(engine::object2d* obj)
    {
        obj->translate({ 0.f, 0.005f, 0.f });
    }
} command_go_up;

class command_go_down : public engine::command
{
private:
public:
    virtual ~command_go_down(){};
    virtual void execute(engine::object2d* obj)
    {
        obj->translate({ 0.f, -0.005f, 0.f });
    }
} command_go_down;

class imput_handler
{
private:
    engine::command* button_up{ &command_go_up };
    engine::command* button_down{ &command_go_down };

public:
    engine::command* handle_imput(engine::engine* engine, bool& continue_loop);
    engine::command* new_handle_imput(bool& continue_loop);
};

engine::command* imput_handler::handle_imput(engine::engine* engine,
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
    if (system_event.type == event::up && system_event.event_state)
    {
        // std::cout << system_event << '\n';
        return button_up;
    }
    if (system_event.type == event::down && system_event.event_state)
    {
        // std::cout << system_event << '\n';
        return button_down;
    }
    return nullptr;
}
