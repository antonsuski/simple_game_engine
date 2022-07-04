#pragma once
#include "engine.hxx"

class game_actor
{
};

class command
{
public:
    virtual ~command();
    virtual void execute(game_actor& actor) = 0;
};

class move_left : public command
{
public:
    virtual void execute(game_actor& actor) { std::cout << "move_left\n"; }
};

class input_handler
{
private:
    engine::engine* engine_;
    command*        button_a{ new move_left };

public:
    input_handler(engine::engine* engine) { engine_ = engine; }
    command* handle_imput()
    {
        using namespace engine;
        event system_event;
        while (engine_->handl_imput(system_event))
        {
            if (system_event.event_state)
            {
                if (system_event.type == event::left)
                {
                    return button_a;
                }
            }
        }
        return nullptr;
    }

    ~input_handler()
    {
        if (button_a != nullptr)
        {
            delete button_a;
        }
    }
};