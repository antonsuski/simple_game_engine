#include "vbo.hxx"
#include "om_gl_check.hxx"
#include "stuff.hxx"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string_view>
#include <vector>

namespace engine
{
engine::vbo_v_8::~vbo_v_8()
{
    if (ebo_data != nullptr)
    {
        delete ebo_data;
    }
    ebo_data = nullptr;

    glDeleteVertexArrays(1, &vao_id);
    OM_GL_CHECK()

    glDeleteBuffers(1, &vbo_id);
    OM_GL_CHECK()

    glDeleteBuffers(1, &ebo_id);
    OM_GL_CHECK()
}

engine::vbo_v_8::vbo_v_8(std::string_view path)
{
    std::fstream vertex_file;

    vertex_file.exceptions(std::fstream::failbit | std::fstream::badbit);
    vertex_file.open(path.data(), std::ios::in | std::ios::out);

    if (vertex_file.is_open())
    {
        size_t count = get_line_count(vertex_file) - 1;
        restart_file(vertex_file);
        ebo_data = new std::vector<uint32_t>;
        v_8 tr;

        for (size_t iterator = 0; iterator < count; iterator++)
        {
            vertex_file >> tr;
            vbo_data.push_back(tr);
        }
    }

    vbo_data_size = vbo_data.size();

    for (size_t iterator = 0; iterator < vbo_data_size; iterator++)
    {
        ebo_data->push_back(iterator);
    }

    ebo_size = ebo_data->size();

    std::clog << "----ebo----" << std::endl;
    for (auto i : *ebo_data)
    {

        std::clog << i << std::endl;
    }
    std::clog << "----ebo----" << std::endl;
    for (auto i : vbo_data)
    {
        std::clog << i;
    }

    std::clog << std::endl;

    glGenVertexArrays(1, &vao_id);
    OM_GL_CHECK()

    glGenBuffers(1, &vbo_id);
    OM_GL_CHECK()

    glGenBuffers(1, &ebo_id);
    OM_GL_CHECK()

    glBindVertexArray(vao_id);
    OM_GL_CHECK();

    bind_buffer();
    buffer_data(GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    OM_GL_CHECK();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * ebo_data->size(),
                 ebo_data->data(), GL_DYNAMIC_DRAW);
    OM_GL_CHECK();

    vertex_attrib_pointer();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    OM_GL_CHECK()

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    OM_GL_CHECK();

    glBindVertexArray(0);
    OM_GL_CHECK()

    restart_file(vertex_file);
    vertex_file.close();
}

size_t vbo_v_8::size()
{
    return vbo_data_size;
}

void vbo_v_8::bind_buffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    OM_GL_CHECK()
}

void vbo_v_8::buffer_data(GLenum flag)
{
    using namespace std;

    //    clog << "szieof(vbo_data): " << sizeof(vbo_data) << endl
    //         << "vbo_data->size(): " << vbo_data->size() << endl
    //         << "sizeof(vbo_data[0]): " << sizeof(vbo_data[0]) << endl
    //         << "szieof(vbo_data[0] * vbo_data->size(): "
    //         << sizeof(vbo_data[0]) * vbo_data->size() << endl
    //         << "sizeof(vertex_8): " << sizeof(v_8) << endl;

    glBufferData(GL_ARRAY_BUFFER, sizeof(v_8) * vbo_data.size(),
                 vbo_data.data(), flag);
    OM_GL_CHECK()
}

void vbo_v_8::buffer_ebo()
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * ebo_data->size(),
                 ebo_data->data(), GL_DYNAMIC_DRAW);
    OM_GL_CHECK()
}

void vbo_v_8::vertex_attrib_pointer()
{
    // clang-format off

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(v_8),
                          reinterpret_cast<void*>(0));
    OM_GL_CHECK()

    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(v_8),
                          reinterpret_cast<void*>(3 * sizeof(float)));
    OM_GL_CHECK()

    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(v_8),
                          reinterpret_cast<void*>(6 * sizeof(float)));

    // clang-format on
    OM_GL_CHECK()

    glEnableVertexAttribArray(0);
    OM_GL_CHECK()

    glEnableVertexAttribArray(1);
    OM_GL_CHECK()

    glEnableVertexAttribArray(2);
    OM_GL_CHECK()
}

void vbo_v_8::bind_vao()
{
    glBindVertexArray(vao_id);
    OM_GL_CHECK()
}

void vbo_v_8::bind_ebo()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    OM_GL_CHECK()
}

void vbo_v_8::unbind_vao()
{
    glBindVertexArray(0);
    OM_GL_CHECK()
}

void vbo_v_8::print_buffer()
{
    for (auto iterator : vbo_data)
    {
        std::cerr << iterator;
    }
}

void vbo_v_8::morf_color(float& time)
{
    for (size_t iterator = 0; iterator < vbo_data_size; ++iterator)
    {
        if (vbo_data.at(iterator % 3).r || vbo_data.at((iterator + 1) % 3).r)
        {
            vbo_data.at(iterator % 3).r =
                fabs(vbo_data.at(iterator % 3).r - 0.2f);
            vbo_data.at((iterator + 1) % 3).r =
                fabs(vbo_data.at((iterator + 1) % 3).r + 0.2f);
        }
        if (vbo_data.at(iterator % 3).g || vbo_data.at((iterator + 1) % 3).g)
        {
            vbo_data.at(iterator % 3).g =
                fabs(vbo_data.at(iterator % 3).g - 0.2f);
            vbo_data.at((iterator + 1) % 3).g =
                fabs(vbo_data.at((iterator + 1) % 3).g + 0.2f);
        }
        if (vbo_data.at(iterator % 3).b || vbo_data.at((iterator + 1) % 3).b)
        {
            vbo_data.at(iterator % 3).b =
                fabs(vbo_data.at(iterator % 3).b - 0.2f);
            vbo_data.at((iterator + 1) % 3).b =
                fabs(vbo_data.at((iterator + 1) % 3).b + 0.2f);
        }
    }
}

engine::vbo_v_3::~vbo_v_3()
{
    if (vbo_data != nullptr)
    {
        delete vbo_data;
    }
    vbo_data = nullptr;
}

engine::vbo_v_3::vbo_v_3(std::string_view path)
{
    std::fstream vertex_file;

    vertex_file.exceptions(std::fstream::failbit | std::fstream::badbit);
    vertex_file.open(path.data(), std::ios::in | std::ios::out);

    if (vertex_file.is_open())
    {
        size_t count = get_line_count(vertex_file) - 1;
        restart_file(vertex_file);
        vbo_data = new std::vector<v_3>;

        v_3 tr;

        for (size_t iterator = 0; iterator < count; iterator++)
        {
            vertex_file >> tr;
            vbo_data->push_back(tr);
        }
    }

    glGenVertexArrays(1, &vao_id);
    OM_GL_CHECK()

    glGenBuffers(1, &vbo_id);
    OM_GL_CHECK()

    glBindVertexArray(vao_id);
    OM_GL_CHECK();

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    OM_GL_CHECK()
    glBufferData(GL_ARRAY_BUFFER, sizeof(v_3) * 9 /*vbo_data->size()*/,
                 &vbo_data[0] /*vbo_data->data()*/, GL_STATIC_DRAW);
    OM_GL_CHECK()

    // clang-format off

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),
                          nullptr);

    // clang-format on
    OM_GL_CHECK()

    glEnableVertexAttribArray(0);
    OM_GL_CHECK()

    //    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //    OM_GL_CHECK()

    //    glBindVertexArray(0);
    //    OM_GL_CHECK()

    restart_file(vertex_file);
    vertex_file.close();
} // namespace engine

v_3* vbo_v_3::get_data()
{
    return vbo_data->data();
}

void vbo_v_3::bind_buffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    OM_GL_CHECK()
}

void vbo_v_3::buffer_data(GLenum flag)
{
    for (auto i : *this->vbo_data)
    {
        std::cerr << i;
    }

    std::cerr << sizeof(v_3) << " " << vbo_data->size() << std::endl
              << vbo_data->size() * sizeof(v_3) << std::endl;

    glBufferData(GL_ARRAY_BUFFER, sizeof(v_3) * vbo_data->size(),
                 vbo_data->data(), flag);
    OM_GL_CHECK()
}

void vbo_v_3::vertex_attrib_pointer(GLuint attribute_id)
{
    // clang-format off

    glVertexAttribPointer(attribute_id,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),
                          nullptr);

    // clang-format on
    OM_GL_CHECK()

    glEnableVertexAttribArray(attribute_id);
    OM_GL_CHECK()

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    OM_GL_CHECK()

    glBindVertexArray(0);
}

void vbo_v_3::bind_vao()
{
    glBindVertexArray(vao_id);
    OM_GL_CHECK()
}

std::istream& operator>>(std::istream& is, vbo_v_8& obj)
{
    return is;
}

} // namespace engine
