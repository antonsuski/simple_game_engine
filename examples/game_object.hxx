#pragma once

#include "engine.hxx"

class actor
{
public:
    engine::vbo_8        buffer;
    engine::shader_es_32 shader;

    actor();
};
