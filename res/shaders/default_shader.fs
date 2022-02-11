#version 320 es

precision mediump float;

in vec3 v_color;

uniform vec4 my_color;

out vec4 fragColor;

void main()
{
 fragColor = vec4(v_color.r, v_color.g, v_color.b, 1.0);
}
