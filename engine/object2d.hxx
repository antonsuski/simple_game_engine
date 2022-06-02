#pragma once

#include "shader.hxx"
#include "texture2d.hxx"
#include "vbo.hxx"

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
    object2d(std::string buffer_path  = "../../res/rgba_square.txt",
             std::string shader_path  = "../../res/shaders/shader_v_8",
             std::string texture_path = "../../res/images/wall.jpg");

    void render() const;
};

} // namespace engine
