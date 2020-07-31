#version 320 es

precision mediump float;

in vec3 v_color;
in vec2 v_text_coord;

uniform vec4 my_color;
uniform sampler2D v_texture;

out vec4 fragColor;

void main()
{
 fragColor = texture(v_texture, v_text_coord);
}
