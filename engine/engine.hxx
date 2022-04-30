#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "shader.hxx"
#include "stuff.hxx"
#include "texture2d.hxx"
#include "vbo.hxx"

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
        button_1,
        button_2,
        select,
        start,
        unknown,
        turn_off,
    };

    event_type  type;
    std::string name;
    bool        event_state;

    event();
    event(event::event_type e_type, std::string e_name);
};

std::ostream& operator<<(std::ostream& out, const event& e);

class engine
{
public:
    virtual ~engine() = 0;

    virtual void update()                                     = 0;
    virtual void render()                                     = 0;
    virtual void render(const vbo_v_3& vbo_buffer)            = 0;
    virtual void render(const vbo_v_3&      vbo_buffer,
                        const shader_es_32& shader)           = 0;
    virtual void render(const vbo_v_8&      vbo_buffer,
                        const shader_es_32& shader)           = 0;
    virtual void swap_buffers()                               = 0;
    virtual void tmp_test_method(unsigned int& sh_prog,
                                 unsigned int& vao_id)        = 0;
    virtual void tmp_test_method2(const unsigned int& sh_prog,
                                  const unsigned int& vao_id) = 0;
    virtual bool handl_imput(event&)                          = 0;
    virtual bool init(int32_t, int32_t)                       = 0;
};

engine* create_engine();
void    destroy_engine(engine*);

} // namespace engine
