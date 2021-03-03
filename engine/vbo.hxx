﻿#pragma once

#include <string_view>
#include <vector>

#include "glad.h"

#include "stuff.hxx"

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

namespace engine
{
using attribute_es_32 = std::vector<std::tuple<GLuint, std::string_view>>;

class ENGINE_DECLSPEC vbo
{
    virtual ~vbo() = 0;
};

class ENGINE_DECLSPEC vbo_v_8
{
public:
    std::vector<v_8>       vbo_data;
    std::vector<uint32_t>* ebo_data = nullptr;

    size_t vbo_size;
    size_t ebo_size;
    GLuint vbo_id;
    GLuint vao_id;
    GLuint ebo_id;

public:
    ~vbo_v_8();
    vbo_v_8(std::string_view);

    std::istream& operator>>(std::istream& is);
    std::ostream& operator<<(std::ostream& stream);

    size_t size();

    void bind_buffer();
    void buffer_data(GLenum);
    void buffer_ebo();
    void vertex_attrib_pointer();
    void bind_vao();
    void bind_ebo();
    void unbind_vao();
    void print_buffer();
    void morf_color(float&);
};

class ENGINE_DECLSPEC vbo_v_3
{
    std::vector<v_3>* vbo_data = nullptr;
    GLuint            vbo_id;
    GLuint            vao_id;

public:
    ~vbo_v_3();
    vbo_v_3(std::string_view);

    v_3* get_data();
    void bind_buffer();
    void buffer_data(GLenum);
    void vertex_attrib_pointer(GLuint);
    void bind_vao();
};
} // namespace engine