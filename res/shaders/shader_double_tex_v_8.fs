#version 320 es

precision mediump float;
out vec4  fragColor;

in vec2 out_tex_coord;
in vec3 out_color;

uniform sampler2D first_texture;
uniform sampler2D second_texture;

void main()
{
    fragColor =
        mix((texture(first_texture, out_tex_coord) * vec4(out_color, 1.0)),
            texture(second_texture, out_tex_coord), 1.0);
}
