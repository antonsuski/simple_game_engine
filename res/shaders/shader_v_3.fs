#version 320 es

precision mediump float;
out vec4 frag_color;
uniform vec2 mouse_coord;

void main(void)
{
  float b_color = abs((mouse_coord.x + mouse_coord.y) / 2.0f);
  frag_color = vec4(mouse_coord.y, mouse_coord.x, 0.2f, 1.0f);
}

