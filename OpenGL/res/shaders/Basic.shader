#shader vertex
#version 330 core
layout(location = 0) in vec4 position;

uniform mat4 u_mMatrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * u_mMatrix * position;
}

#shader fragment
#version 330 core
layout(location = 0) out vec4 color;

void main()
{
	color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}