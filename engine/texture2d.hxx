#pragma once

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

#include <array>
#include <string_view>
#include <vector>

#include "shader.hxx"
#include "stb_image.h"
#include "stuff.hxx"

namespace engine
{
class ENGINE_DECLSPEC texture
{
private:
    std::string path_to_texture;

    unsigned char* tex_data;

    uint32_t tex_id;
    int      tex_h;
    int      tex_w;
    int      tex_channals;

public:
    enum img_format
    {
        RGB  = GL_RGB,
        RGBA = GL_RGBA,
    };

    ~texture();
    texture(std::string path = "unknown", img_format format = RGB);

    void bind_texture() const;
};

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

    std::uint32_t get_width();
    std::uint32_t get_height();
    void          bind();
    int           get_id();
};

class ENGINE_DECLSPEC texture_2d_es_32
{
protected:
    GLuint texture_id;

    int width;
    int height;
    int nr_channels;

    GLuint txt_location;

public:
    explicit texture_2d_es_32(std::string_view);

    std::uint32_t get_width();
    std::uint32_t get_height();
    void          bind();
    int           get_id();
};

class ENGINE_DECLSPEC animation_2d_es_32 : public texture_2d_es_32
{
    size_t col;
    size_t row;
    size_t current_frame;

    float offset_x;
    float offset_y;

    float animation_speed;

    // the best way it is make with v_2[4],
    // but now i need to go

    std::vector<v_8> atlas;

    void make_atlas_map();

public:
    animation_2d_es_32(std::string_view, size_t, size_t);

    void switch_frame();
    void switch_frame(size_t);
    void set_animation_speed(float);

    v_8 get_current_frame();
};

} // namespace engine
