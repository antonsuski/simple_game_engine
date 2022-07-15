#pragma once

#include <string_view>
#include <vector>

#include "glad.h"

#include "stuff.hxx"

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

// TODO: make all vertex buffers template

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

    uint64_t vertex_count;
    size_t   vbo_size;
    size_t   ebo_size;
    GLuint   vbo_id;
    GLuint   vao_id;
    GLuint   ebo_id;

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
    void bind_vao() const;

    const uint64_t& get_vertex_conut();
    const uint64_t& get_vertex_conut() const;
};

class ENGINE_DECLSPEC vbo_v_3
{
    std::vector<v_3>* vbo_data = nullptr;
    GLuint            vbo_id;
    GLuint            vao_id;

    uint64_t vertex_count;

public:
    ~vbo_v_3();
    vbo_v_3(std::string_view);

    v_3* get_data();

    const uint64_t& get_vertex_count();
    const uint64_t& get_vertex_count() const;

    void bind_buffer();
    void buffer_data(GLenum);
    void vertex_attrib_pointer(GLuint);
    void bind_vao();

    void bind_vao() const;
};

class ENGINE_DECLSPEC vbo_6
{
private:
    std::vector<v_6> vbo_data;
    GLuint           vbo_id;
    GLuint           vao_id;

    uint64_t vertex_count;

public:
    vbo_6(std::string);

    const uint64_t& get_vertex_count() const;
    void            bind_vao() const;
};

class ENGINE_DECLSPEC vbo_8
{
private:
    std::vector<v_8>      vbo_data;
    std::vector<uint32_t> ebo_data;
    GLuint                vbo_id;
    GLuint                vao_id;
    GLuint                ebo_id;

    uint64_t vertex_count;
    uint64_t index_count;
    v_2      shape_size;

public:
    ~vbo_8();
    vbo_8(std::string);

    const uint64_t& get_vertex_count() const;
    const uint64_t& get_index_count() const;
    v_2             get_size() const;

    void bind_vao() const;
};

} // namespace engine
