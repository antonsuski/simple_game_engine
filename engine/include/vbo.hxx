#pragma once

#include <string_view>
#include <vector>

#include "glad/glad.h"
#include "stuff.hxx"

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

namespace engine
{
class ENGINE_DECLSPEC vbo
{
    virtual ~vbo() = 0;
};

class ENGINE_DECLSPEC vbo_v_8
{
    std::vector<v_8>* vbo_data = nullptr;
    GLuint            vbo_id;
    GLuint            vao_id;

public:
    ~vbo_v_8();
    vbo_v_8(std::string_view);

    std::istream& operator>>(std::istream& is);
    std::ostream& operator<<(std::ostream& stream);

    v_8* get_data();
    void bind_buffer();
    void buffer_data(GLenum);
    void vertex_attrib_pointer(GLuint);
};

} // namespace engine
