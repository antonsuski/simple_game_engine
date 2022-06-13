#include "gl_assist.hxx"
#include "stuff.hxx"
#include "vbo.hxx"

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
    GL_CHECK()

    glDeleteBuffers(1, &vbo_id);
    GL_CHECK()

    glDeleteBuffers(1, &ebo_id);
    GL_CHECK()
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

    vbo_size = vbo_data.size();

    for (size_t iterator = 0; iterator < vbo_size; iterator++)
    {
        ebo_data->push_back(iterator);
    }

    ebo_size = ebo_data->size();

    std::clog << "----ebo----" << std::endl;
    for (auto i : *ebo_data)
    {

        std::clog << i << std::endl;
    }
    std::clog << "----vbo----" << std::endl;
    for (auto i : vbo_data)
    {
        std::clog << i;
    }

    std::clog << std::endl;

    glGenVertexArrays(1, &vao_id);
    GL_CHECK()

    glGenBuffers(1, &vbo_id);
    GL_CHECK()

    glGenBuffers(1, &ebo_id);
    GL_CHECK()

    glBindVertexArray(vao_id);
    GL_CHECK()

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    GL_CHECK()

    glBufferData(GL_ARRAY_BUFFER, sizeof(v_8) * vbo_data.size(),
                 vbo_data.data(), GL_STATIC_DRAW);
    GL_CHECK()

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    GL_CHECK()

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * ebo_data->size(),
                 ebo_data->data(), GL_DYNAMIC_DRAW);
    GL_CHECK()

    vertex_attrib_pointer();

    // glEnableVertexAttribArray(0);
    // GL_CHECK()

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // GL_CHECK()

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // GL_CHECK()

    restart_file(vertex_file);
    vertex_file.close();
}

size_t vbo_v_8::size()
{
    return vbo_size;
}

void vbo_v_8::bind_buffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    GL_CHECK()
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
    GL_CHECK()
}

void vbo_v_8::buffer_ebo()
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * ebo_data->size(),
                 ebo_data->data(), GL_DYNAMIC_DRAW);
    GL_CHECK()
}

void vbo_v_8::vertex_attrib_pointer()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(v_8),
                          reinterpret_cast<void*>(0));
    GL_CHECK()

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(v_8),
                          reinterpret_cast<void*>(3 * sizeof(float)));
    GL_CHECK()

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(v_8),
                          reinterpret_cast<void*>(6 * sizeof(float)));

    // clang-format on
    GL_CHECK()

    glEnableVertexAttribArray(0);
    GL_CHECK()

    glEnableVertexAttribArray(1);
    GL_CHECK()

    glEnableVertexAttribArray(2);
    GL_CHECK()
}

void vbo_v_8::bind_vao()
{
    glBindVertexArray(vao_id);
    GL_CHECK()
}

void vbo_v_8::bind_vao() const
{
    glBindVertexArray(vao_id);
    GL_CHECK()
}

const uint64_t& vbo_v_8::get_vertex_conut()
{
    return vertex_count;
}

const uint64_t& vbo_v_8::get_vertex_conut() const
{
    return vertex_count;
}

void vbo_v_8::bind_ebo()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    GL_CHECK()
}

void vbo_v_8::unbind_vao()
{
    glBindVertexArray(0);
    GL_CHECK()
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
    for (size_t iterator = 0; iterator < vbo_size; ++iterator)
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
        std::cout << "vbo:\n";
        for (size_t iterator = 0; iterator < count; iterator++)
        {
            vertex_file >> tr;
            std::cout << tr;
            vbo_data->push_back(tr);
        }
    }

    vertex_count = vbo_data->size();

    glGenVertexArrays(1, &vao_id);
    GL_CHECK()

    glGenBuffers(1, &vbo_id);
    GL_CHECK()

    glBindVertexArray(vao_id);
    GL_CHECK();

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    GL_CHECK()
    glBufferData(GL_ARRAY_BUFFER, sizeof(v_3) * vbo_data->size(),
                 vbo_data->data(), GL_STATIC_DRAW);
    GL_CHECK()

    // clang-format off

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),
                          nullptr);

    // clang-format on
    GL_CHECK()

    glEnableVertexAttribArray(0);
    GL_CHECK()

    //    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //    GL_CHECK()

    //    glBindVertexArray(0);
    //    GL_CHECK()

    restart_file(vertex_file);
    vertex_file.close();
} // namespace engine

v_3* vbo_v_3::get_data()
{
    return vbo_data->data();
}

const uint64_t& vbo_v_3::get_vertex_count()
{
    return vertex_count;
}

const uint64_t& vbo_v_3::get_vertex_count() const
{
    return vertex_count;
}

void vbo_v_3::bind_buffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    GL_CHECK()
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
    GL_CHECK()
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
    GL_CHECK()

    glEnableVertexAttribArray(attribute_id);
    GL_CHECK()

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GL_CHECK()

    glBindVertexArray(0);
}

void vbo_v_3::bind_vao()
{
    glBindVertexArray(vao_id);
    GL_CHECK()
}

void vbo_v_3::bind_vao() const
{
    glBindVertexArray(vao_id);
    GL_CHECK()
}

std::istream& operator>>(std::istream& is, vbo_v_8& obj)
{
    return is;
}

vbo_6::vbo_6(std::string path)
{
    std::fstream vertex_file;

    vertex_file.exceptions(std::fstream::failbit | std::fstream::badbit);
    vertex_file.open(path.data(), std::ios::in | std::ios::out);

    if (vertex_file.is_open())
    {
        size_t count = get_line_count(vertex_file) - 1;
        restart_file(vertex_file);
        v_6 tr;

        for (size_t iterator = 0; iterator < count; iterator++)
        {
            vertex_file >> tr;
            std::cout << tr;
            vbo_data.push_back(tr);
        }
    }

    vertex_count = vbo_data.size();

    glGenVertexArrays(1, &vao_id);
    glGenBuffers(1, &vbo_id);
    bind_vao();
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v_6) * vertex_count, vbo_data.data(),
                 GL_STATIC_DRAW);
    GL_CHECK();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(v_6), nullptr);
    GL_CHECK();
    glEnableVertexAttribArray(0);
    GL_CHECK();

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(v_6),
                          (void*)(sizeof(v_3)));
    GL_CHECK();
    glEnableVertexAttribArray(1);
    GL_CHECK();

    glBindVertexArray(0);
    GL_CHECK();

    restart_file(vertex_file);
    vertex_file.close();
}

const uint64_t& vbo_6::get_vertex_count() const
{
    return vertex_count;
}

void vbo_6::bind_vao() const
{
    glBindVertexArray(vao_id);
    GL_CHECK();
}

vbo_8::~vbo_8()
{
    glDeleteVertexArrays(1, &vao_id);
    GL_CHECK()

    glDeleteBuffers(1, &vbo_id);
    GL_CHECK()

    glDeleteBuffers(1, &ebo_id);
    GL_CHECK()
}

vbo_8::vbo_8(std::string path)
{
    std::fstream vertex_file;

    vertex_file.exceptions(std::fstream::failbit | std::fstream::badbit);
    vertex_file.open(path.data(), std::ios::in | std::ios::out);

    if (vertex_file.is_open())
    {
        size_t count = get_line_count(vertex_file) - 1;
        restart_file(vertex_file);
        v_8 tr;
        vertex_file >> shape_size;
        for (size_t iterator = 1; iterator < count; iterator++)
        {
            vertex_file >> tr;
            vbo_data.push_back(tr);
        }
    }

    vertex_count = vbo_data.size();
    for (size_t iterator = 0; iterator < vertex_count; iterator++)
    {
        ebo_data.push_back(iterator);
    }
    index_count = ebo_data.size();
    std::clog << "shape_size " << shape_size << std::endl;
    std::clog << "----ebo----" << std::endl;
    for (auto i : ebo_data)
    {

        std::clog << i << std::endl;
    }
    std::clog << "----vbo----" << std::endl;
    for (auto i : vbo_data)
    {
        std::clog << i;
    }

    glGenVertexArrays(1, &vao_id);
    glGenBuffers(1, &vbo_id);

    glGenBuffers(1, &ebo_id);
    GL_CHECK();
    bind_vao();
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v_8) * vbo_data.size(),
                 vbo_data.data(), GL_STATIC_DRAW);
    GL_CHECK();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * index_count,
                 ebo_data.data(), GL_DYNAMIC_DRAW);
    GL_CHECK();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(v_8), nullptr);
    GL_CHECK();
    glEnableVertexAttribArray(0);
    GL_CHECK();

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(v_8),
                          (void*)(sizeof(v_3)));
    GL_CHECK();
    glEnableVertexAttribArray(1);
    GL_CHECK();

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(v_8),
                          (void*)(sizeof(v_3) * 2));
    GL_CHECK();
    glEnableVertexAttribArray(2);
    GL_CHECK();

    glBindVertexArray(0);
    GL_CHECK();

    restart_file(vertex_file);
    vertex_file.close();
}

const uint64_t& vbo_8::get_vertex_count() const
{
    return vertex_count;
}

const uint64_t& vbo_8::get_index_count() const
{
    return index_count;
}

void vbo_8::bind_vao() const
{
    glBindVertexArray(vao_id);
    GL_CHECK();
}

v_2 vbo_8::get_size() const
{
    return shape_size;
}

} // namespace engine
