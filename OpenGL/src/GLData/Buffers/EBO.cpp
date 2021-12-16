#include "EBO.h"
#include "GL/glew.h"

EBO::EBO(unsigned int* indices)
	: indx(indices), size(sizeof(indices))
{
	glGenBuffers(1, &RenderID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indx, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

EBO::~EBO()
{
}

void EBO::Bind()
{
}

void EBO::Unbind()
{
}

void EBO::setArray(unsigned int* newArr)
{
}

unsigned int* EBO::getArray()
{
	return nullptr;
}

unsigned int EBO::getRenderID()
{
	return 0;
}

unsigned int EBO::getSize()
{
	return 0;
}
