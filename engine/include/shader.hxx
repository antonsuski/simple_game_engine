#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

#include "glad/glad.h"

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

namespace engine
{
class ENGINE_DECLSPEC shader_es_32
{
public:
    unsigned int id;

    explicit shader_es_32(const char*, const char*);
    // explicit shader_es_32(const char*, const char*, );

    void use();
    void set_uniform();
};
} // namespace engine
