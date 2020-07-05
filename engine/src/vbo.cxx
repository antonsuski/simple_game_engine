#include "vbo.hxx"
#include "om_gl_check.hxx"
#include "stuff.hxx"

#include <fstream>
#include <sstream>
#include <string_view>
#include <vector>

namespace engine
{
engine::vbo_v_8::~vbo_v_8()
{
    if (vbo_data != nullptr)
    {
        delete[] vbo_data;
    }
    vbo_data = nullptr;
}

engine::vbo_v_8::vbo_v_8(std::string_view path)
{
    std::fstream vertex_file;

    glGenVertexArrays(1, &vao_id);
    OM_GL_CHECK()

    glGenBuffers(1, &vbo_id);
    OM_GL_CHECK()

    glBindVertexArray(vao_id);
    OM_GL_CHECK();

    vertex_file.exceptions(std::fstream::failbit | std::fstream::badbit);
    vertex_file.open(path.data(), std::ios::in | std::ios::out);

    if (vertex_file.is_open())
    {
        size_t count = get_line_count(vertex_file) - 1;
        restart_file(vertex_file);
        vbo_data = new std::vector<v_8>;

        v_8 tr;

        for (size_t iterator = 0; iterator < count; iterator++)
        {
            vertex_file >> tr;
            vbo_data->push_back(tr);
        }
    }

    bind_buffer();
    buffer_data(GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    OM_GL_CHECK()

    vertex_attrib_pointer(0);

    glEnableVertexAttribArray(0);
    OM_GL_CHECK()

    restart_file(vertex_file);
    vertex_file.close();
}

v_8* vbo_v_8::get_data()
{
    return vbo_data->data();
}

void vbo_v_8::bind_buffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    OM_GL_CHECK()
}

void vbo_v_8::buffer_data(GLenum flag)
{
    glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data[0]) * vbo_data->size(),
                 vbo_data->data(), flag);
}

void vbo_v_8::vertex_attrib_pointer(GLuint attribute_id)
{
    // clang-format off

    glVertexAttribPointer(attribute_id,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),
                          static_cast<void*>(0));

    // clang-format on
    OM_GL_CHECK()

    glEnableVertexAttribArray(attribute_id);
    OM_GL_CHECK()
}

std::istream& operator>>(std::istream& is, vbo_v_8& obj)
{
    return is;
}

} // namespace engine
