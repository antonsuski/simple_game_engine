#version 320 es

precision mediump float;

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_text_coord;

out vec3 v_color;
out vec2 v_text_coord;

void main()
{
 v_text_coord = a_text_coord;
 v_color = vec3(a_color.r, a_color.g, a_color.b);
 gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);
}
