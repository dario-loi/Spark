#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &RenderID);
	Bind();
}

VAO::~VAO()
{
	Bind();

	for (size_t c = 0; c < attributes.size(); ++c)
	{
		glDisableVertexAttribArray(static_cast<GLuint>(c));
	}

	Unbind();
	glDeleteVertexArrays(1, &RenderID);

}

void VAO::Bind() const
{
	glBindVertexArray(RenderID);
}

void VAO::Unbind() const
{
	glBindVertexArray(0);
}

template<>
void VAO::add_attr<float>(int const count)
{
	attributes.emplace_back(count * sizeof(float), count, GL_FLOAT);
	stride += count * sizeof(float);
}

template<>
void VAO::add_attr_inst<float>(int const count)
{
	instance_attr.emplace_back(count * sizeof(float), count, GL_FLOAT);
	stride += count * sizeof(float);
}

template<>
void VAO::add_attr<glm::mat4>(int const count)
{
	attributes.emplace_back(count * sizeof(float), count, GL_FLOAT);
	stride += count * sizeof(float);
}

template<>
void VAO::add_attr_inst<glm::mat4>(int const count)
{
	for (int i = 0; i < 4 * count; ++i)
	{
		instance_attr.emplace_back(4 * sizeof(float), 4, GL_FLOAT);
	}
	
	i_stride += count * 16 * sizeof(float);
}

template<>
void VAO::add_attr<glm::mat4>(int const count)
{
	for (int i = 0; i < 4 * count; ++i)
	{
		attributes.emplace_back(4 * sizeof(float), 4, GL_FLOAT);
	}

	stride += count * 16 * sizeof(float);
}

void VAO::init_VAO() const
{
	GLuint c = 0U;
	GLuint offset = 0U;
	GLuint i_offset = 0U;
	Bind();

	for (auto const& attr : attributes)
	{
		glVertexAttribPointer(c, attr.count, attr.type, GL_FALSE, stride, reinterpret_cast<void*>(offset));	//cast to (void*) is an ugly relic of the past.
		offset += attr.size;
		glEnableVertexAttribArray(c++);
	}

	for (auto const& i_attr : instance_attr)
	{
		
		glVertexAttribPointer(c, i_attr.count, i_attr.type, GL_FALSE, i_stride, reinterpret_cast<void*>(i_offset));
		i_offset += i_attr.size;
		glEnableVertexAttribArray(c++);
	}
	
	Unbind();
}