#version 320 es
precision mediump float;
in vec3 out_color;
out vec4 FragColor;
void main()
{
    FragColor = vec4(out_color.r, out_color.g, out_color.b, 1.0);
}
