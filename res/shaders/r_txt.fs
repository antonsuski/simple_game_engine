#version 320 es

precision mediump float;

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
//uniform vec4 my_color;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    float a = 0.0f;
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), a);
}
