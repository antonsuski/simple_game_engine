#pragma once

#include "glad/glad.h"

#include <algorithm>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

namespace engine
{
struct ENGINE_DECLSPEC v_2
{
    float x;
    float y;

    v_2()
        : x(0.f)
        , y(0.f)
    {
    }

    v_2(float x_, float y_)
        : x(x_)
        , y(y_)
    {
    }
};

struct ENGINE_DECLSPEC v_3
{
    float x;
    float y;
    float z;

    v_3()
        : x(0.f)
        , y(0.f)
        , z(0.f)
    {
    }

    v_3(float x_, float y_, float z_)
        : x(x_)
        , y(y_)
        , z(z_)
    {
    }
};

struct ENGINE_DECLSPEC v_4
{
    float x;
    float y;
    float z;
    float w;

    v_4()
        : x(0.f)
        , y(0.f)
        , z(0.f)
        , w(0.f)
    {
    }

    v_4(float x_, float y_, float z_, float w_)
        : x(x_)
        , y(y_)
        , z(z_)
        , w(w_)
    {
    }
};

struct ENGINE_DECLSPEC v_8
{
    v_8()
        : x(0.f)
        , y(0.f)
        , z(0.f)
        , r(0.f)
        , g(0.f)
        , b(0.f)
        , s(0.f)
        , t(0.f)
    {
    }

    v_8(float x_, float y_, float z_, float r_, float g_, float b_, float s_,
        float t_)
        : x(x_)
        , y(y_)
        , z(z_)
        , r(r_)
        , g(g_)
        , b(b_)
        , s(s_)
        , t(t_)

    {
    }

    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
    float s;
    float t;
};

struct ENGINE_DECLSPEC triangle
{
    triangle()
    {
        v[0] = v_8();
        v[1] = v_8();
        v[2] = v_8();
    }
    v_8 v[3];
};

struct ENGINE_DECLSPEC uniform
{
private:
    std::string_view uniform_name;

public:
    float u0;
    float u1;
    float u2;
    float u3;

    uniform()
        : uniform_name("unknown")
        , u0(0.f)
        , u1(0.f)
        , u2(0.f)
        , u3(0.f)
    {
    }
    uniform(std::string_view uniform_name_, float a, float b, float c, float d)
        : uniform_name(uniform_name_)
        , u0(a)
        , u1(b)
        , u2(c)
        , u3(d)
    {
    }

    std::string_view get_name();
};

struct ENGINE_DECLSPEC m_2x3
{
    m_2x3();
    static m_2x3 identiry();
    static m_2x3 scale(float scale);
    static m_2x3 scale(float sx, float sy);
    static m_2x3 rotation(float thetha);
    static m_2x3 move(const v_2& delta);
    v_2          col0;
    v_2          col1;
    v_2          delta;
};

struct ENGINE_DECLSPEC m_3x3
{
    m_3x3();
    static m_3x3 identiry();
    static m_3x3 scale(float scale);
    static m_3x3 scale(float sx, float sy);
    static m_3x3 rotation(float thetha);
    static m_3x3 move(const v_3& delta);
    v_3          col0;
    v_3          col1;
    v_3          delta;
};

class ENGINE_DECLSPEC trans_mat_4x4
{
public:
    trans_mat_4x4();
    trans_mat_4x4(v_4, v_4, v_4, v_4);
    static trans_mat_4x4 reset();
    static trans_mat_4x4 move(const float& m_x, const float& m_y,
                              const float& m_z);
    static trans_mat_4x4 move(const v_3& vec3);
    static trans_mat_4x4 scale(const float& scale);
    static trans_mat_4x4 scale(float s_x, float s_y, float s_z);
    static trans_mat_4x4 rotate(const float& angle);

    v_4 col_0;
    v_4 col_1;
    v_4 col_2;
    v_4 delta;
};

class ENGINE_DECLSPEC cam
{
    v_3   current_pos{ 0.f, 0.f, 0.f };
    v_2   current_scale{ 1.f, 1.f };
    float current_direction{ 0.f };
    float glm_direction{ 0.f };
    float perspective_fov{ 45.f };

    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 direction;

    float yaw   = -90.f;
    float pitch = 0.f;

public:
};

ENGINE_DECLSPEC void multiply(v_4& vector, trans_mat_4x4& mat);

ENGINE_DECLSPEC std::istream& operator>>(std::istream& is, v_2&);
ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const v_2&);

ENGINE_DECLSPEC std::istream& operator>>(std::istream& is, v_3&);
ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const v_3&);

ENGINE_DECLSPEC std::istream& operator>>(std::istream& is, v_4&);
ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const v_4&);

ENGINE_DECLSPEC std::istream& operator>>(std::istream& is, v_8&);
ENGINE_DECLSPEC std::istream& operator>>(std::istream& is, triangle&);
ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const v_8&);
ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const triangle&);

ENGINE_DECLSPEC std::istream& operator>>(std::istream& is, trans_mat_4x4&);
ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream,
                                         const trans_mat_4x4&);

ENGINE_DECLSPEC v_2 operator+(const v_2& left, const v_2 right);
ENGINE_DECLSPEC v_3 operator+(const v_3& left, const v_3 right);
ENGINE_DECLSPEC v_4 operator+(const v_4& left, const v_4 right);
ENGINE_DECLSPEC v_8 operator+(const v_8& left, const v_8 right);

ENGINE_DECLSPEC v_4 operator*(const trans_mat_4x4& left, const v_4& right);
ENGINE_DECLSPEC trans_mat_4x4 operator*(const trans_mat_4x4& left,
                                        const trans_mat_4x4& right);

ENGINE_DECLSPEC bool operator==(const v_2& left, const v_2 right);
ENGINE_DECLSPEC bool operator==(const v_3& left, const v_3 right);
ENGINE_DECLSPEC bool operator==(const v_8& left, const v_8 right);

ENGINE_DECLSPEC v_2    ndc_mesh(v_2 vec, v_2 res_ratio);
ENGINE_DECLSPEC v_2    ndc_txture(v_2 vec, v_2 res_ratio);
ENGINE_DECLSPEC size_t get_line_count(std::fstream& file);
ENGINE_DECLSPEC void   restart_file(std::fstream& file);
} // namespace engine
