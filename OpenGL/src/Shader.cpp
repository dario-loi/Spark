#include "Shader.h"
#include "gtc/type_ptr.hpp"
#include <array>

Shader::Shader(const std::string& filepath)
	: m_RendererID(0), m_FilePath(filepath)
{
    ShaderProgramSource source = ParseShader();
    m_RendererID = CreateShader(source.VertexSource, 
        source.FragmentSource, source.TCSource,
        source.TESource,
        source.GeometrySource);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

ShaderProgramSource Shader::ParseShader()
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1, TCS = 2, TES = 3, GEOMETRY = 4
    };

    std::ifstream stream(m_FilePath);
    std::string line;
    std::array<std::stringstream, 5> ss;
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
            else if (line.find("TCS") != std::string::npos)
                type = ShaderType::TCS;
            else if (line.find("TES") != std::string::npos)
                type = ShaderType::TES;
            else if (line.find("geometry") != std::string::npos)
                type = ShaderType::GEOMETRY;
        }
        else if (type != ShaderType::NONE)
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

GLuint Shader::CompileShader(GLuint type,
    const std::string& source)
{
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)_malloca(length * sizeof(char));


        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!" << std::endl << m_FilePath << std::endl;

        std::cout << message << std::endl;
        glDeleteShader(id);
        assert(false);
        return 0;
    }

    return id;
}


GLuint Shader::CreateShader(const std::string& vertexShader,
    const std::string& fragmentShader, 
    std::string const& TCShader,
    std::string const& TEShader, 
    std::string const& geometryShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLuint tc = 0;
    GLuint te = 0;
    GLuint gs = 0;

    if (!TCShader.empty())
    {
        tc = CompileShader(GL_TESS_CONTROL_SHADER, TCShader);
        glAttachShader(program, tc);
    }

    if (!TEShader.empty())
    {
        te = CompileShader(GL_TESS_EVALUATION_SHADER, TEShader);
        glAttachShader(program, te);
    }

    if (!geometryShader.empty())
    {
        gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
        glAttachShader(program, gs);
    }


    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    if (!TCShader.empty())
    {
        glDeleteShader(tc);
        glDetachShader(program, tc);
    }

    if (!TEShader.empty())
    {
        glDeleteShader(te);
        glDetachShader(program, te);
    }

    if (!geometryShader.empty())
    {
        glDeleteShader(gs);
        glDetachShader(program, gs);
    }

    return program;
}


void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform3f(const std::string& name, glm::vec3 v)
{
    glUniform3f(GetUniformLocation(name), v.x, v.y, v.z);
}

void Shader::setUniform4mat(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform3mat(const std::string& name, const glm::mat3& matrix)
{
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniform1i(const std::string& name, const int v0)
{
    glUniform1i(GetUniformLocation(name), v0);
}

void Shader::SetUniform1ui(const std::string& name, const unsigned int ui0)
{
    glUniform1ui(GetUniformLocation(name), ui0);
}

void Shader::SetUniform1f(const std::string& name, const float v0)
{
    glUniform1f(GetUniformLocation(name), v0);
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.contains(name))
    {
        return m_UniformLocationCache[name];
    }
        

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
    {
        std::cerr << "Warning: uniform " << name << " Doesn't exist!" << std::endl;
    }

    m_UniformLocationCache[name] = location;

    return location;
}
