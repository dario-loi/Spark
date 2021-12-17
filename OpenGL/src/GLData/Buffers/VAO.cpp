#include "VAO.h"




VAO::VAO(unsigned int index)
	: indx(index), stride(0), RenderID(0), isBound(false)
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

	attributes.push_back({ count * sizeof(float), count, GL_FLOAT });
	stride += count * sizeof(float);

}

void VAO::init_VAO()
{
	int c = 0;
	for (const attribute& attr : attributes)
	{
		glVertexAttribPointer(c, attr.count, attr.type, GL_FALSE, attr.size, (void*)0);
	}
	glEnableVertexAttribArray(indx);
}