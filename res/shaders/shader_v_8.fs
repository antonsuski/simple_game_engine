#version 320 es

precision mediump float;
out vec4  fragColor;

in vec2 out_tex_coord;
in vec3 out_color;

uniform sampler2D out_texture;

void main()
{
    fragColor = texture(out_texture, out_tex_coord);
}
