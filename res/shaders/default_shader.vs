#version 320 es

precision mediump float;

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_color;

out vec3 v_color;

void main()
{
 v_color = a_color;
 gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);
}
