#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "object2d.hxx"
#include "shader.hxx"
#include "stuff.hxx"
#include "texture2d.hxx"
#include "vbo.hxx"

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

namespace engine
{

class ENGINE_DECLSPEC event
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

class ENGINE_DECLSPEC engine
{
public:
    virtual ~engine() = 0;

    virtual void update()                           = 0;
    virtual void render()                           = 0;
    virtual void render(const vbo_v_3& vbo_buffer)  = 0;
    virtual void render(const vbo_v_3&      vbo_buffer,
                        const shader_es_32& shader) = 0;
    virtual void render(const vbo_6&        vbo_buffer,
                        const shader_es_32& shader) = 0;
    virtual void render(const vbo_v_8&      vbo_buffer,
                        const shader_es_32& shader) = 0;
    virtual void render(const vbo_8&        vbo_buffer,
                        const shader_es_32& shader) = 0;
    virtual void render(const vbo_8& vbo_buffer, const shader_es_32& shader,
                        const texture& texture)     = 0;
    virtual void render(
        const vbo_8& vbo_buffer, const shader_es_32& shader,
        const std::vector<std::pair<texture, tex_unit>>& tex_vector) = 0;
    virtual void  render(const object2d& object)                     = 0;
    virtual void  swap_buffers()                                     = 0;
    virtual void  tmp_test_method(unsigned int& sh_prog,
                                  unsigned int& vao_id)              = 0;
    virtual void  tmp_test_method2(const unsigned int& sh_prog,
                                   const unsigned int& vao_id)       = 0;
    virtual v_2   get_windonw_size()                                 = 0;
    virtual bool  handl_imput(event&)                                = 0;
    virtual bool  init(int32_t, int32_t)                             = 0;
    virtual float get_time_from_init()                               = 0;
};

ENGINE_DECLSPEC engine* create_engine();
ENGINE_DECLSPEC void    destroy_engine(engine*);

} // namespace engine
