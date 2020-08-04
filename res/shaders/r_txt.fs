#version 320 es

precision mediump float;

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
//uniform vec4 my_color;

// texture sampler
uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoord) * vec4(ourColor.r, ourColor.g, ourColor.b, 1.0f);
}
