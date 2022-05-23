#version 320 es

precision mediump float;
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_tex_coord;

out vec3 out_color;
out vec2 out_tex_coord;

void main()
{
    out_color     = a_color;
    gl_Position   = vec4(a_position, 1.0f);
    out_tex_coord = a_tex_coord;
}
