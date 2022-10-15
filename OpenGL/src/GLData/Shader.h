#pragma once
#include <string>
#include "GL/glew.h"
#include "mat4x4.hpp"
#include "boost/uuid/uuid.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>

#include <unordered_map>
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string TCSource;
	std::string TESource;
	std::string GeometrySource;
};

namespace spark
{

	class Shader
	{
	private:
		unsigned int m_RendererID;
		std::string m_FilePath;

		boost::uuids::uuid UUID;
		std::string name;
	
		std::unordered_map<std::string, int> m_UniformLocationCache;
	public:

		explicit Shader(boost::uuids::uuid UUID_, std::string const& filepath_, std::string const& name_);
		~Shader();

		void Bind() const;
		void Unbind() const;

		[[nodiscard]] const boost::uuids::uuid getUUID() const
		{
			return UUID;
		}

		[[nodiscard]] std::string getName() const
		{
			return name;
		}

		GLuint CompileShader(GLuint type, const std::string& source);
		GLuint CreateShader(std::string const& vertexShader, std::string const& fragmentShader,
			std::string const& TCShader,
			std::string const& TEShader,
			std::string const& geometryShader
		);
		ShaderProgramSource ParseShader();
		/*
			Uniforms
		*/

		void SetUniform1i(std::string const& name, int v0);
		void SetUniform1ui(std::string const& name, unsigned int ui0);
		void SetUniform1f( std::string const& name, float v0);
		void SetUniform4f( std::string const& name, float v0, float v1, float f2, float f3);
		void SetUniform3f( std::string const& name, float v0, float v1, float v2);
		void SetUniform3f( std::string const& name, glm::vec3 v);
		void setUniform4mat(std::string const& name, const glm::mat4& matrix);

		void setUniform3mat(std::string const& name, const glm::mat3& matrix);

	private:
		int GetUniformLocation(std::string const& name);

	};

}