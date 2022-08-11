#shader vertex
#version 420 core
layout(location = 0) in vec4 position;

out vec3 v_TexCoord;

uniform mat4 u_mMatrix;
uniform mat4 u_normalMatrix;


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

const vec3 light_color = vec3(0.6f, 0.696f, 0.878f);


void main()
{

	color = vec4(light_color, 1.0f);
}