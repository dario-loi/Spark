#shader vertex
#version 420 core
layout(location = 0) in vec4 position;

out vec3 v_TexCoord;

uniform mat4 u_mMatrix;

layout(binding = 2, std140) uniform cameraUBO
{

	mat4 view;			//View Matrix
	mat4 projection;	//Proj Matrix
	vec3 camera_pos;	//Camera Position Vector

};

void main()
{
	gl_Position = projection * view * u_mMatrix * position;

}

#shader fragment
#version 420 core
layout(location = 0) out vec4 color;

void main()
{

	color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}