#version 320 es

precision mediump float;
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;

out vec3 out_color;

void main()
{
    out_color = a_color;
    gl_Position = vec4(a_position, 1.0f);
}
