#pragma once

#include <SDL.h>
#include <iostream>

#include "engine.hxx"

// class command
// {
// private:
// public:
//     virtual ~command(){};
//     virtual void execute(engine::object2d* obj) = 0;
// };

class command_go_up : public engine::command
{
private:
public:
    virtual ~command_go_up(){};
    virtual void execute(engine::object2d* obj)
    {
        obj->translate({ 0.f, 0.00001f, 0.f });
    }
} command_go_up;

class command_go_down : public engine::command
{
private:
public:
    virtual ~command_go_down(){};
    virtual void execute(engine::object2d* obj)
    {
        obj->translate({ 0.f, -0.00001f, 0.f });
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
    if (engine->new_handler(system_event))
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
        // std::cout << system_event << '\n';
        return button_up;
    }
    if (system_event.type == event::down)
    {
        // std::cout << system_event << '\n';
        return button_down;
    }
    return nullptr;
}

engine::command* imput_handler::new_handle_imput(bool& continue_loop)
{
    using namespace engine;
    event system_event;

    SDL_Event sdl_e;

    while (SDL_PollEvent(&sdl_e) != 0)
    {
        if (sdl_e.type == SDL_QUIT)
        {
            continue_loop = false;
            return nullptr;
        }
    }

    const Uint8* current_key_status = SDL_GetKeyboardState(nullptr);
    if (current_key_status[SDL_SCANCODE_W])
    {
        std::cout << "key_donw_w\n";
        return button_up;
    }
    else if (sdl_e.key.repeat == 1 && sdl_e.key.keysym.sym == SDLK_s)
    {
        std::cout << "key_down_s\n";
        return button_down;
    }

    return nullptr;
}
