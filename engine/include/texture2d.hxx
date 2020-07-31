﻿#pragma once

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

#include <string_view>

#include "shader.hxx"
#include "stb_image.h"
#include "stuff.hxx"

namespace engine
{
// class ENGINE_DECLSPEC texture
//{
// public:
//    virtual ~texture()                       = 0;
//    virtual std::uint32_t get_width() const  = 0;
//    virtual std::uint32_t get_height() const = 0;
//};

class ENGINE_DECLSPEC texture2d
{
    int              id;
    std::string_view path;
    float            x_pos;
    float            y_pos;
    std::uint32_t    width;
    std::uint32_t    height;
    v_8              coordinates;
    shader_es_32*    txt_shader;
    bool             load_texture(std::string_view);

public:
    ~texture2d();
    explicit texture2d(std::string_view, size_t, shader_es_32*);

    std::uint32_t get_width() const;
    std::uint32_t get_height() const;
    void          bind();
    int           get_id() const;
};

class ENGINE_DECLSPEC texture_2d_es_320
{
    GLuint texture_id;

    int width;
    int height;
    int nr_channels;

public:
    explicit texture_2d_es_320(std::string_view);

    std::uint32_t get_width() const;
    std::uint32_t get_height() const;
    void          bind();
    int           get_id() const;
};

} // namespace engine
