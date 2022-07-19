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

const vec3 sunDir = normalize(vec3(0.4f, -1.0f, -0.1f));

void main()
{
	
	//Normalize + Translate Normals
	v_Normal = u_normalMatrix * normalize(normal);

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

const float ambient_light = 0.5f;
const float specular_light = 2.5f;
const vec3 ambient_color = vec3(60 / 255, 65 / 255, 106 / 255);

const vec3 sun_color = vec3(0.4314f, 0.5255f, 0.9412f);

void main()
{	
	//Texture Color
	vec3 norm = normalize(v_Normal);
	vec3 tex_col = texture(u_Texture, v_TexCoord).rgb;

	//Diffuse
	vec3 diffuse = clamp(dot(norm, light_dir), 0.0f, 1.0f) * sun_color;

	//Ambient Light
	vec3 ambient = ambient_light * ambient_color;

	//Specular
	vec3 view_dir = normalize(u_CameraPos - curr_pos);

	vec3 halfway = normalize(light_dir + view_dir);

	float spec = pow(max(dot(halfway, norm), 0.0f), 128);

	if (diffuse == 0.0f)
		spec = 0.0f;

	vec3 specular = sun_color * specular_light * spec;
	color = vec4(tex_col * (ambient + diffuse + specular), 1.0f);
}