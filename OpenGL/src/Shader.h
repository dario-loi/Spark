#pragma once
#include <string>
#include "GL/glew.h"
#include "mat4x4.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <unordered_map>
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string TCSource;
	std::string TESource;
	std::string GeometrySource;
};


class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	

	std::unordered_map<std::string, int> m_UniformLocationCache;
public:

	explicit Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	GLuint CompileShader(GLuint type, const std::string& source);
	GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader,
		std::string const& TCShader,
		std::string const& TEShader,
		std::string const& geometryShader
	);
	ShaderProgramSource ParseShader();
	/*
		Uniforms
	*/

	void SetUniform1i(const std::string& name, int v0);
	void SetUniform1ui(const std::string& name, const unsigned int ui0);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform3f(const std::string& name, glm::vec3 v);
	void setUniform4mat(const std::string& name, const glm::mat4& matrix);

	void setUniform3mat(const std::string& name, const glm::mat3& matrix);

private:
	int GetUniformLocation(const std::string& name);

};

