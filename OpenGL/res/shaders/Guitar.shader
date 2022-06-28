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
	v_Normal = normalize(transpose(inverse(mat3(u_mMatrix))) * normalize(normal));

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
uniform sampler2D u_Specular;
uniform vec3 u_CameraPos;

const float ambient_light = 0.2f;
const float specular_light = 2.5f;
const vec3 ambient_color = vec3(60 / 255, 65 / 255, 106 / 255);

const vec3 sun_color = vec3(0.4314f, 0.5255f, 0.9412f);

void main()
{	


	//Texture Color
	vec4 tex_col = texture(u_Texture, v_TexCoord);

	//Diffuse
	vec3 diffuse = max(dot(v_Normal, light_dir), 0.0f) * sun_color;

	//Ambient Light
	vec3 ambient = ambient_light * ambient_color;

	//Specular
	vec3 spec_amount = texture(u_Texture, v_TexCoord).xyz;

	vec3 view_dir = normalize(u_CameraPos - curr_pos);
	vec3 reflect_dir = reflect(-light_dir, v_Normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), 32);
	vec3 specular = spec_amount * specular_light * spec * sun_color;

	color = vec4(tex_col.rgb * (ambient + diffuse + specular), 1.0f);
}