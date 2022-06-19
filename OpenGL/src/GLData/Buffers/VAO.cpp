#include "VAO.h"




VAO::VAO(size_t index)
	: RenderID(0), stride(0), indx(index), isBound(false)
{

	glGenVertexArrays(1, &RenderID);
	
}

VAO::~VAO()
{
	glDisableVertexAttribArray(indx);
	glDeleteVertexArrays(1, &RenderID);

}

void VAO::Bind()
{

	glBindVertexArray(RenderID);
	isBound = true;
}

void VAO::Unbind()
{

	glBindVertexArray(0);
	isBound = false;
}

template<typename T>
void VAO::add_attr(unsigned int count)
{
	static_assert(false);
}

/*TODO more overloads*/
template<>
void VAO::add_attr<float>(unsigned int count)
{

	attributes.emplace_back(count * sizeof(float), count, GL_FLOAT);
	stride += count * sizeof(float);

}

void VAO::init_VAO() const
{
	int c = 0;
	unsigned int offset = 0;

	for (const attribute& attr : attributes)
	{
		glVertexAttribPointer(c, attr.count, attr.type, GL_FALSE, stride, (void*) offset);
		offset += attr.size;
		glEnableVertexAttribArray(c++);
		
	}
	
}