#shader vertex
#version 420 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 text_coord;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 curr_pos;

/* Uniform Matrices*/
uniform mat4 u_mMatrix;
uniform mat3 u_normalMatrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	
	//Normalize + Translate Normals
	v_Normal = normalize(u_normalMatrix * normalize(normal) );

	//Texture Coordinates Passthrough
	v_TexCoord = text_coord;

	//Fragment Position Passthrough
	curr_pos = vec3(u_mMatrix * vec4(position, 1.0f));

	//Set Fragment Position
	gl_Position = projection * view * vec4(curr_pos, 1.0f);

}

#shader fragment
#version 420 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 curr_pos;

/* Texture Sampler*/
uniform sampler2D u_Texture;

/* Camera */
uniform vec3 u_CameraPos;

/* UBO */

layout(binding = 1, std140) uniform lightUBO
{

	vec4 lights[128];

};

uniform unsigned int nLights;

/* Constants */

const vec3 ambient_color = vec3(60 / 255, 65 / 255, 106 / 255);

vec3 ambient(float intensity, vec3 color)
{
	return intensity * color;
}

/*
* Blinn-Phong lightning model
*/
vec3 diffuse(vec3 normal, float sq_distance, vec3 light_dir, float intensity, vec3 color)
{

	float diffuse_intensity = (intensity * max(dot(normal, light_dir), 0.0f)) / sq_distance;

	return (diffuse_intensity * color);

}

vec3 specular(vec3 normal, float sq_distance, vec3 light_dir, vec3 frag_pos, vec3 view_dir, float intensity, vec3 color)
{
	
	vec3 halfway = normalize(view_dir + light_dir);
	float specular_intensity = (intensity * pow(max(dot(normal, halfway), 0.0f), 64)) / sq_distance ;

	return specular_intensity * color;
}

void main()
{	
	
	vec3 norm = normalize(v_Normal);
	//Texture Color
	vec3 tex_col = texture(u_Texture, v_TexCoord).rgb;

	vec3 a_light = vec3(0.0f), d_light = vec3(0.0f), s_light = vec3(0.0f);

	a_light = ambient(0.1f, ambient_color);

	vec3 view_dir = normalize(u_CameraPos - curr_pos);

	for (unsigned int indx; indx < nLights; ++indx)
	{
		vec3 light_pos = vec3(lights[indx]);
		float square_dist = 1.0f; // having it be d^2 dims the light, that's a problem for another day!
		vec3 light_dir = normalize(light_pos - curr_pos);

		d_light += diffuse(norm, square_dist, light_dir, 1.0f, vec3(1.0f, 1.0f, 1.0f));
		s_light += specular(norm, square_dist, light_dir, curr_pos, view_dir, 1.0f, vec3(1.0f, 1.0f, 1.0f));
	}

	color = vec4(tex_col * (a_light + d_light + s_light), 1.0f);

}