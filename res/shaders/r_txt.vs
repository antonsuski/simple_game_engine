#version 320 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform vec4 my_color;
uniform mat4 u_matrix;
void main()
{
    TexCoord = vec2((aTexCoord.x + my_color.x), aTexCoord.y + my_color.x);
//    TexCoord = aTexCoord;
	vec4 rez_pos = vec4(aPos, 1.0) * u_matrix;
	gl_Position = vec4(rez_pos.x, rez_pos.y, 0.f, 1.0);
	ourColor = vec3(aColor.r, aColor.g, my_color.x);
	//TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
