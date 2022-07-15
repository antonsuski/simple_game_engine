#version 320 es

precision mediump float;
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_tex_coord;

out vec3 out_color;
out vec2 out_tex_coord;

uniform vec2 resolution;
vec2         new_pos;

void main()
{
    new_pos =
        vec2(a_position.x * float(resolution.y / resolution.x), a_position.y);

    out_color     = a_color;
    out_tex_coord = a_tex_coord;
    gl_Position   = vec4(new_pos.x, new_pos.y, a_position.z, 1.0f);
}
