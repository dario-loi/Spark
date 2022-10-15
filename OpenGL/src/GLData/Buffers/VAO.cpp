#include "VAO.h"

VAO::VAO()
	: RenderID{0ui32}, stride{0ui64}
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
void VAO::add_attr<float>(unsigned int count)
{
	attributes.emplace_back(count * sizeof(float), count, GL_FLOAT);
	stride += count * sizeof(float);
}

void VAO::init_VAO() const
{
	GLuint c = 0u;
	GLuint offset = 0u;
	Bind();

	for (const attribute& attr : attributes)
	{
		glVertexAttribPointer(c, attr.count, attr.type, GL_FALSE, stride, (void*) offset);	//cast to (void*) is an ugly relic of the past.
		offset += attr.size;
		glEnableVertexAttribArray(c++);
	}
	
	Unbind();
}