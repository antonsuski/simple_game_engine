#version 320 es

precision mediump float;

in vec3 v_color;
in vec2 v_text_coord;

uniform vec4 my_color;

out vec4 fragColor;

void main()
{
  fragColor = vec4(v_color.r,v_color.g,my_color.x, 1); 
}
