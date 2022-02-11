#version 320 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform vec4 my_color;
uniform mat4 u_matrix;
uniform mat4 u_matrix_model;
uniform mat4 u_matrix_view;
uniform mat4 u_matrix_projection;

void main()
{
    TexCoord = vec2((aTexCoord.x + my_color.x), aTexCoord.y + my_color.x);
	gl_Position = u_matrix_projection * u_matrix_view * u_matrix_model * u_matrix * vec4(aPos, 1.0);
	ourColor = vec3(aColor.r, aColor.g, my_color.x);
}
