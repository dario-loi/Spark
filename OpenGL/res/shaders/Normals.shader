#shader vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 text_coord;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 curr_pos;
flat out vec3 light_dir;

uniform mat4 u_mMatrix;
uniform mat3 u_normalMatrix;

uniform mat4 view;
uniform mat4 projection;

const vec3 sunDir = normalize(vec3(0.0f, -1.0f, 0.0f));

void main()
{
	
	//Normalize + Translate Normals
	v_Normal = normalize(mat3(u_mMatrix) * normalize(normal));

	//Texture Coordinates Passthrough
	v_TexCoord = text_coord;

	//Fragment Position Passthrough
	curr_pos = vec3(u_mMatrix * vec4(position, 1.0f));

	//Set Fragment Position
	gl_Position = projection * view * vec4(curr_pos, 1.0f);

	light_dir = normalize(sunDir);
}

#shader fragment
#version 330 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 curr_pos;
flat in vec3 light_dir;

uniform sampler2D u_Texture;
uniform vec3 u_CameraPos;

void main()
{	
	color = vec4(v_Normal, 1.0f);
}