#shader vertex
#version 420 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 text_coord;

layout(location = 0) out vec2 v_TexCoord;
layout(location = 1) out vec3 v_Normal;
layout(location = 2) out vec3 curr_pos;

/* Uniform Matrices*/
uniform mat4 u_mMatrix;		 //Model Matrix
uniform mat3 u_normalMatrix; //Normal Matrix

layout(binding = 2, std140) uniform cameraUBO
{

	mat4 view;			//View Mat
	mat4 projection;	//Proj Mat
	vec3 camera_pos;

};

void main()
{
	
	//Normalize + Translate Normals
	v_Normal = mat3(u_mMatrix) * normal;

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

layout(location = 0) in vec2 v_TexCoord;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec3 curr_pos;

/* Texture Sampler*/
uniform sampler2D u_Texture;
uniform sampler2D u_Specular;

/* UBO */

layout(binding = 1, std140) uniform lightUBO
{

	vec4 lights[128];

};

layout(binding = 2, std140) uniform cameraUBO
{

	mat4 view;			//View Matrix
	mat4 projection;	//Proj Matrix
	vec3 camera_pos;	//Camera Position Vector

};

uniform unsigned int nLights;

/* Constants */

const vec3 ambient_color = vec3(0.95f, 0.92f, 0.61f);

vec3 ambient(float intensity, vec3 color)
{
	return intensity * color;
}

/*
* Blinn-Phong lightning model
*/
vec3 diffuse(vec3 normal, float attenuation, vec3 light_dir, float intensity, vec3 color)
{

	float diffuse_intensity = (intensity * max(dot(normal, light_dir), 0.0f)) / attenuation;

	return (diffuse_intensity * color);

}

vec3 specular(vec3 normal, float attenuation, vec3 light_dir, vec3 frag_pos, vec3 view_dir, float intensity, vec3 color)
{
	
	vec3 halfway = normalize(view_dir + light_dir);
	float specular_intensity = (intensity * pow(max(dot(normal, halfway), 0.0f), 64)) / attenuation;

	return specular_intensity * color;
}

vec3 specular_gauss(vec3 normal, float attenuation, vec3 light_dir, vec3 frag_pos, vec3 view_dir, float intensity, vec3 color)
{
	vec3 halfway = normalize(view_dir + light_dir);

	const float shininess = 0.5f;

	float angle = acos(max(dot(normal, halfway), 0.0f));

	float exponent = -(pow(angle / shininess, 2));
	float specular_intensity = exp(exponent);

	return specular_intensity * color;
	
}

void main()
{	
	
	vec3 norm = normalize(v_Normal);
	//Texture Color
	vec3 tex_col = texture(u_Texture, v_TexCoord).rgb;
	//Specular
	vec3 spec_comp = texture(u_Specular, v_TexCoord).rgb;

	vec3 a_light = vec3(0.0f), d_light = vec3(0.0f), s_light = vec3(0.0f);

	a_light = ambient(0.1f, ambient_color);

	vec3 view_dir = normalize(camera_pos - curr_pos);

	for (unsigned int indx; indx < nLights; ++indx)
	{
		vec3 light_pos = vec3(lights[indx]);

		float dist = distance(light_pos, curr_pos);
		float attenuation = 1.0f + 0.07f * dist + 0.017f * pow(dist, 2);
		vec3 light_dir = normalize(light_pos - curr_pos);

		d_light += diffuse(norm, attenuation, light_dir, 0.5f, vec3(1.0f, 0.996f, 0.878f));
		s_light += specular_gauss(norm, attenuation, light_dir, curr_pos, view_dir, 0.5f, vec3(1.0f, 0.996f, 0.878f));
	}

	if (d_light == vec3(0.0f))
	{
		s_light = vec3(0.0f);
	}
		
	color = vec4(tex_col * ((a_light + d_light + (spec_comp * s_light))), 1.0f); //

}