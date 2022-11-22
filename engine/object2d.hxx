#pragma once

#include "shader.hxx"
#include "texture2d.hxx"
#include "vbo.hxx"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

namespace engine
{
class ENGINE_DECLSPEC object2d
{
private:
    vbo_8        _buffer;
    shader_es_32 _shader;
    texture      _texture;
    v_2          size;
    v_2          position;

    float resolution_cofficient;

    glm::mat4 model;
    glm::mat4 view;
    float     angle;

public:
    ~object2d();
    object2d();
    object2d(std::string         buffer_path,
             std::string         shader_path  = "../../res/shaders/shader_v_8",
             std::string         texture_path = "../../res/images/wall.jpg",
             texture::img_format format       = texture::img_format::RGBA);

    object2d(std::string buffer_path, std::string vs_path, std::string fs_path,
             std::string texture_path, texture::img_format format);

    const shader_es_32* get_shader() const;
    const vbo_8*        get_vbo() const;
    const texture*      get_texture() const;
    glm::mat4           get_view() const;
    glm::mat4           get_model() const;

    bool is_collide_point(v_2 mouse_coords);

    void scale(glm::vec3 vector);
    void translate(glm::vec3 vector);
    void rotate(float angle);
};

} // namespace engine
