#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 text_coord;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 u_mMatrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{

	gl_Position = projection * view * u_mMatrix * position;
	v_FragPos = vec3(position);
	v_TexCoord = vec2(text_coord.x, text_coord.y);

	mat3 normal_mat = mat3(transpose(inverse(u_mMatrix)));

	v_Normal = normalize(normal_mat * normal);
}

#shader fragment
#version 330 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform sampler2D u_Texture;
uniform vec3 u_CameraPos;

const float ambient_light = 0.1f;
const vec3 sun_dir = normalize(vec3(1.0f, 0.1f, 0.0f));
const vec3 sun_color = vec3(0.9f, 0.9f, 0.9f);

void main()
{

	vec3 viewDir = normalize(u_CameraPos - v_FragPos);
	vec3 reflectDir = reflect(-sun_dir, v_Normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = 0.5 * spec * sun_color;

	vec3 ambient = ambient_light * sun_color;
	float illumination = 2.0f * max(dot(-sun_dir, normalize(v_Normal)), 0.0f);

	vec4 light_component = vec4(ambient + (illumination * sun_color) + specular, 1.0f);
		
	color = light_component * texture(u_Texture, v_TexCoord);
}