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

    float resolution_cofficient;

public:
    ~object2d();
    object2d(std::string         buffer_path  = "../../res/rgba_square.txt",
             std::string         shader_path  = "../../res/shaders/shader_v_8",
             std::string         texture_path = "../../res/images/wall.jpg",
             texture::img_format format       = texture::img_format::RGBA);

    object2d(std::string buffer_path, std::string vs_path, std::string fs_path,
             std::string texture_path, texture::img_format format);

    const shader_es_32* get_shader() const;

    void render() const;
};

} // namespace engine
