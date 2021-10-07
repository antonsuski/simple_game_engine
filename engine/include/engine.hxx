﻿#pragma once

#include <array>
#include <cassert>
#include <iostream>
#include <string_view>
#include <tuple>
#include <vector>

#include "shader.hxx"
#include "stuff.hxx"
#include "texture2d.hxx"
#include "vbo.hxx"

#include "SDL2/SDL.h"

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

namespace engine
{

class event
{
public:
    enum key
    {
        up,
        down,
        left,
        right,
        select,
        start,
        button_1,
        button_2,
        mouse_move,
        turn_off
    } key;
    std::string_view name;
    bool             is_running;
    v_2              mouse_delta;
    event(enum key event_key, std::string_view event_name,
          bool is_event_running)
        : key(event_key)
        , name(event_name)
        , is_running(is_event_running)
    {
    }
    event() {}
};

using events = const uint8_t*;

ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const event& e);

class ENGINE_DECLSPEC core
{
public:
    SDL_Window*   window     = nullptr;
    SDL_GLContext gl_context = nullptr;

    virtual ~core()                                                    = 0;
    virtual bool  init(size_t, size_t)                                 = 0;
    virtual bool  init_my_opengl()                                     = 0;
    virtual bool  read_event(event&)                                   = 0;
    virtual bool  read_events(events&)                                 = 0;
    virtual void  render(vbo_v_8&, shader_es_32&, GLuint& texture_id)  = 0;
    virtual void  render(vbo_v_8&, shader_es_32&, GLuint& texture_id,
                         GLuint& texture_)                             = 0;
    virtual void  render(vbo_v_8&, shader_es_32&, texture_2d_es_32&)   = 0;
    virtual void  render(vbo_v_8&, shader_es_32&, animation_2d_es_32&) = 0;
    virtual void  render_grid(shader_es_32&)                           = 0;
    virtual void  swap_buffers()                                       = 0;
    virtual void  swap_buffers(v_3&)                                   = 0;
    virtual void  uninitialize()                                       = 0;
    virtual void  mouse_capture(bool)                                  = 0;
    virtual float get_time_for_init()                                  = 0;
};

ENGINE_DECLSPEC engine::core* create_engine();
ENGINE_DECLSPEC void          destroy_engine(engine::core* e);
} // namespace engine