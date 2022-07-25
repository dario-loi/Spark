#shader vertex
#version 420 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 text_coord;
layout(location = 3) in vec3 tangent;

out VS_OUT
{
	vec2 v_TexCoord;
	vec3 curr_pos;
	mat3 TBN;

} vs_out;

/* Uniform Matrices*/
uniform mat4 u_mMatrix;		 //Model Matrix
uniform mat4 u_normalMatrix; //Normal Matrix

layout(binding = 2, std140) uniform cameraUBO
{

	mat4 view;			//View Mat
	mat4 projection;	//Proj Mat
	vec3 camera_pos;

};

void main()
{
	
	vec3 tangent_n = normalize(mat3(u_normalMatrix) * tangent);
	vec3 normal_n = normalize(mat3(u_normalMatrix) * normal);

	//Gram-Schmidt Orthonormalization of the tangent and normal vectors (To let TBN be an orthonormal Basis)
	tangent_n = normalize(tangent_n - dot(tangent_n, normal_n) * normal_n);

	vec3 bitang = cross(tangent_n, normal_n);

	vs_out.TBN = mat3(tangent_n, bitang, normal_n);

	//Texture Coordinates Passthrough
	vs_out.v_TexCoord = text_coord;

	//Fragment Position Passthrough
	vs_out.curr_pos = vec3(u_mMatrix * vec4(position, 1.0f));

	//Set Fragment Position
	gl_Position = projection * view * vec4(vs_out.curr_pos, 1.0f);

}

#shader fragment
#version 420 core
layout(location = 0) out vec4 color;


in VS_OUT
{
	vec2 v_TexCoord;
	vec3 curr_pos;
	mat3 TBN;
} fs_in;

/* Texture Sampler*/
uniform sampler2D u_Texture;
uniform sampler2D u_Specular;
uniform sampler2D u_NormalMap;

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
const vec3 light_color = vec3(1.0f, 0.996f, 0.878f);

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

vec3 specular(vec3 normal, float attenuation, vec3 light_dir, vec3 view_dir, float intensity, vec3 color)
{
	
	vec3 halfway = normalize(view_dir + light_dir);
	float specular_intensity = (intensity * pow(max(dot(normal, halfway), 0.0f), 64)) / attenuation;

	return specular_intensity * color;
}

vec3 specular_gauss(vec3 normal, float attenuation, vec3 light_dir, vec3 view_dir, float intensity, vec3 color)
{
	vec3 halfway = normalize(view_dir + light_dir);

	float incidence = clamp(dot(normal, light_dir), 0.0f, 1.0f);

	const float shininess = 0.75f;

	float angle = acos(max(dot(normal, halfway), 0.0f));

	float exponent = -(pow(angle / shininess, 2));
	float specular_intensity = exp(exponent);

	specular_intensity = (incidence != 0.0f) ? specular_intensity : 0.0f;

	return specular_intensity * color;
	
}



void main()
{	

	//Texture Color
	vec3 tex_col = texture(u_Texture, fs_in.v_TexCoord).rgb;
	//Specular
	vec3 spec_comp = texture(u_Specular, fs_in.v_TexCoord).rgb;
	//Normal
	vec3 norm = normalize(fs_in.TBN * texture(u_NormalMap, fs_in.v_TexCoord).rgb);

	vec3 a_light = vec3(0.0f), d_light = vec3(0.0f), s_light = vec3(0.0f);

	a_light = ambient(0.01f, ambient_color);

	vec3 view_dir = normalize(camera_pos - fs_in.curr_pos);

	for (unsigned int indx; indx < nLights; ++indx)
	{
		vec3 light_pos = vec3(lights[indx]);

		float dist = distance(light_pos, fs_in.curr_pos);

		const float alpha = 0.06f;
		const float beta = 0.015f;

		float attenuation = 1.0f + alpha * dist + beta * pow(dist, 2);
		vec3 light_dir = normalize(light_pos - fs_in.curr_pos);

		d_light += diffuse(norm, attenuation, light_dir, 1.0f, light_color);
		s_light += specular_gauss(norm, attenuation, light_dir, view_dir, 1.0f, light_color);
	}

	color = vec4(tex_col * ((a_light + d_light + (spec_comp * s_light))), 1.0f); //

}