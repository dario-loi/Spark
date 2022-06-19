#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 text_coord;

out vec2 v_TexCoord;

uniform mat4 u_mMatrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{

	gl_Position = projection * view * u_mMatrix * position;
	v_TexCoord = vec2(text_coord.x, text_coord.y);

}

#shader fragment
#version 330 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord);
}