﻿#include "stuff.hxx"

#include <algorithm>
#include <cmath>
#include <fstream>

namespace engine
{
std::istream& operator>>(std::istream& is, v_2& v)
{
    is >> v.x;
    is >> v.y;
    return is;
}

std::ostream& operator<<(std::ostream& stream, const v_2& v)
{
    stream << v.x << " " << v.y << " " << std::endl;
    return stream;
}

std::istream& operator>>(std::istream& is, v_3& v)
{
    is >> v.x;
    is >> v.y;
    is >> v.z;
    return is;
}

std::ostream& operator<<(std::ostream& stream, const v_3& v)
{
    stream << v.x << " " << v.y << " " << v.z << " " << std::endl;
    return stream;
}

std::istream& operator>>(std::istream& is, v_8& v)
{
    is >> v.x;
    is >> v.y;
    is >> v.z;
    is >> v.r;
    is >> v.g;
    is >> v.b;
    is >> v.s;
    is >> v.t;
    return is;
}

std::istream& operator>>(std::istream& is, triangle& t)
{
    is >> t.v[0];
    is >> t.v[1];
    is >> t.v[2];
    return is;
}

std::ostream& operator<<(std::ostream& stream, const v_8& v)
{
    stream << v.x << " " << v.y << " " << v.z << " " << v.r << " " << v.g << " "
           << v.b << " " << v.s << " " << v.t << std::endl;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const triangle& t)
{
    for (size_t iter = 0; iter < 3; ++iter)
    {
        stream << t.v[iter].x << " " << t.v[iter].y << " " << t.v[iter].z << " "
               << t.v[iter].r << " " << t.v[iter].g << " " << t.v[iter].b << " "
               << t.v[iter].s << " " << t.v[iter].t << std::endl;
    }
    return stream;
}

v_2 operator+(const v_2& left, const v_2 right)
{
    v_2 result;
    result.x = left.x + right.x;
    result.y = left.y + right.y;
    return result;
}

v_3 operator+(const v_3& left, const v_3 right)
{
    v_3 result;
    result.x = left.x + right.x;
    result.y = left.y + right.y;
    result.z = left.z + right.z;
    return result;
}

v_8 operator+(const v_8& left, const v_8 right)
{
    v_8 result;
    result.x = left.x + right.x;
    result.y = left.y + right.y;
    result.z = left.z + right.z;
    result.r = left.r + right.r;
    result.g = left.g + right.g;
    result.b = left.b + right.b;
    result.s = left.s + right.s;
    result.t = left.t + right.t;
    return result;
}

m_2x3::m_2x3()
    : col0(1.0f, 0.f)
    , col1(0.f, 1.f)
    , delta(0.f, 0.f)
{
}

m_2x3 m_2x3::identiry()
{
    return m_2x3::scale(1.f);
}

m_2x3 m_2x3::scale(float scale)
{
    m_2x3 result;
    result.col0.x = scale;
    result.col1.y = scale;
    return result;
}

m_2x3 m_2x3::scale(float sx, float sy)
{
    m_2x3 r;
    r.col0.x = sx;
    r.col1.y = sy;
    return r;
}

m_2x3 m_2x3::rotation(float thetha)
{
    m_2x3 result;

    result.col0.x = std::cos(thetha);
    result.col0.y = std::sin(thetha);

    result.col1.x = -std::sin(thetha);
    result.col1.y = std::cos(thetha);

    return result;
}

m_2x3 m_2x3::move(const v_2& delta)
{
    m_2x3 r = m_2x3::identiry();
    r.delta = delta;
    return r;
}

v_2 operator*(const v_2& v, const m_2x3& m)
{
    v_2 result;
    result.x = v.x * m.col0.x + v.y * m.col0.y + m.delta.x;
    result.y = v.x * m.col1.x + v.y * m.col1.y + m.delta.y;
    return result;
}

m_2x3 operator*(const m_2x3& m1, const m_2x3& m2)
{
    m_2x3 r;

    r.col0.x = m1.col0.x * m2.col0.x + m1.col1.x * m2.col0.y;
    r.col1.x = m1.col0.x * m2.col1.x + m1.col1.x * m2.col1.y;
    r.col0.y = m1.col0.y * m2.col0.x + m1.col1.y * m2.col0.y;
    r.col1.y = m1.col0.y * m2.col1.x + m1.col1.y * m2.col1.y;

    r.delta.x = m1.delta.x * m2.col0.x + m1.delta.y * m2.col0.y + m2.delta.x;
    r.delta.y = m1.delta.x * m2.col1.x + m1.delta.y * m2.col1.y + m2.delta.y;

    return r;
}

v_2 ndc_mesh(v_2 vec, v_2 res_ratio)
{
    if ((-1 <= vec.x && vec.x <= 1) || (-1 <= vec.y && vec.y <= 1))
    {
        std::cerr << "coordinates already normolized!" << std::endl;
    }
    if (vec.x < 1 || vec.y < 1)
    {
        std::cerr << "uncorect mesh coordinate" << std::endl;
        return vec;
    }
    return v_2(((2.f / res_ratio.x) * vec.x) - 1,
               (((2.f / res_ratio.y) * vec.y) - 1) * -1);
}

v_2 ndc_txture(v_2 vec, v_2 res_ratio)
{
    if ((-1 <= vec.x && vec.x <= 1) || (-1 <= vec.y && vec.y <= 1))
    {
        std::cerr << "coordinates already normolized!" << std::endl;
    }
    if (vec.x < 1 || vec.y < 1)
    {
        std::cerr << "uncorect mesh coordinate" << std::endl;
        return vec;
    }
    return v_2(((1.f / res_ratio.x) * vec.x),
               -((1.f / res_ratio.y) * vec.y - 1));
}

size_t get_line_count(std::fstream& file)
{
    size_t count = std::count(std::istreambuf_iterator<char>(file),
                              std::istreambuf_iterator<char>(), '\n');
    return count;
}

void restart_file(std::fstream& file)
{
    file.clear();
    file.seekg(file.beg);
}

bool operator==(const v_2& left, const v_2 right)
{
    return (left.x == right.x && left.y == right.y) ? true : false;
}

bool operator==(const v_3& left, const v_3 right)
{
    return (left.x == right.x && left.y == right.y && left.z == right.z)
               ? true
               : false;
}

bool operator==(const v_8& left, const v_8 right)
{
    return (left.x == right.x && left.y == right.y && left.z == right.z &&
            left.r == right.r && left.g == right.g && left.b == right.b &&
            left.s == right.s && left.t == right.t)
               ? true
               : false;
}

trans_mat_4x4::trans_mat_4x4()
    : col_0(1.f, 0.f, 0.f, 0.f)
    , col_1(0.f, 1.f, 0.f, 0.f)
    , col_2(0.f, 0.f, 1.f, 0.f)
    , delta(0.f, 0.f, 0.f, 1.f)
{
}

trans_mat_4x4::trans_mat_4x4(v_4 col_0_, v_4 col_1_, v_4 col_2_, v_4 delta_)
    : col_0(col_0_)
    , col_1(col_1_)
    , col_2(col_2_)
    , delta(delta_)
{
}

trans_mat_4x4 trans_mat_4x4::reset()
{
    return trans_mat_4x4{ { 1.f, 0.f, 0.f, 0.f },
                          { 0.f, 1.f, 0.f, 0.f },
                          { 0.f, 0.f, 1.f, 0.f },
                          { 0.f, 0.f, 0.f, 1.f } };
}

trans_mat_4x4 trans_mat_4x4::move(const float& m_x, const float& m_y,
                                  const float& m_z)
{
    trans_mat_4x4 rezult = trans_mat_4x4::reset();
    rezult.delta         = { m_x, m_y, m_z, 1.f };
    return rezult;
}

trans_mat_4x4 trans_mat_4x4::rotate(const float& angle)
{
    trans_mat_4x4 result;

    result.col_0.x = std::cos(angle);
    result.col_0.y = std::sin(angle);

    result.col_1.x = -std::sin(angle);
    result.col_1.y = std::cos(angle);

    return result;
}

trans_mat_4x4 trans_mat_4x4::scale(const float& scale)
{
    float final_scale = scale;

    if (!final_scale)
    {
        final_scale = 1.f;
    }

    trans_mat_4x4 rezult;

    rezult.col_0.x = final_scale;
    rezult.col_1.y = final_scale;
    rezult.col_2.z = final_scale;

    return rezult;
}

v_4 operator+(const v_4& left, const v_4 right)
{
    v_4 result;
    result.x = left.x + right.x;
    result.y = left.y + right.y;
    result.z = left.z + right.z;
    result.w = left.w + right.w;
    return result;
}

v_4 operator*(const trans_mat_4x4& left, const v_4& right)
{
    v_4 rezult;
    rezult.x = right.x * left.col_0.x + right.y * left.col_1.x +
               right.z * left.col_2.x + right.w * left.delta.x;
    rezult.y = right.x * left.col_0.y + right.y * left.col_1.y +
               right.z * left.col_2.y + right.w * left.delta.y;
    rezult.z = right.x * left.col_0.z + right.y * left.col_1.z +
               right.z * left.col_2.z + right.w * left.delta.z;
    rezult.w = right.x * left.col_0.w + right.y * left.col_1.w +
               right.z * left.col_2.w + right.w * left.delta.w;

    return rezult;
}

} // namespace engine
