#pragma once

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

#include "glad.h"
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

    void use();
    void set_uniform_4mat(std::string_view, glm::mat4&);
    void set_uniform_4mat(std::string_view, trans_mat_4x4&);
    void set_uniform_4f(std::string_view&, uniform&);
    void set_uniform_4f(uniform&);
    void set_uniform_1i(std::string_view, GLint&);
};
} // namespace engine
