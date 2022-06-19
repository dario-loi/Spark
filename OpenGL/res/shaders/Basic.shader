#shader vertex
#version 330 core
layout(location = 0) in vec4 position;

out vec3 v_TexCoord;

uniform mat4 u_mMatrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * u_mMatrix * position;

	v_TexCoord = vec3(position.x, position.y, position.z);
}

#shader fragment
#version 330 core
layout(location = 0) out vec4 color;

in vec3 v_TexCoord;

uniform samplerCube u_Texture;

void main()
{

	color = texture(u_Texture, v_TexCoord);
}