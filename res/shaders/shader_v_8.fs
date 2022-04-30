#version 320 es

precision mediump float;
in vec3 out_color;
out vec4 fragColor;
// uniform vec2 mouse_coord;

void main()
{
    fragColor = vec4(out_color , 1.0f);
}
