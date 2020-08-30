#pragma once

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

namespace engine
{
class ENGINE_DECLSPEC command
{
public:
    ~command(){};
    virtual void execute() = 0;
};

class ENGINE_DECLSPEC input_handler
{
public:
    void handle_input();
};

} // namespace engine
