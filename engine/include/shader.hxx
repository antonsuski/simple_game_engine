#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

#include "glad/glad.h"
#include "stuff.hxx"

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

namespace engine
{
class ENGINE_DECLSPEC shader_es_32
{

    std::vector<GLint> uniforms_location;

public:
    unsigned int id;

    explicit shader_es_32(const char*, const char*);
    // explicit shader_es_32(const char*, const char*, );

    void use();
    void set_uniform_4f(std::string_view&, uniform&);
    void set_uniform_1i(std::string_view, GLint&);
};
} // namespace engine
