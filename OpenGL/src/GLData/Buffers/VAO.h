#pragma once
#include <vector>
#include "GL/glew.h"

class VAO
{
private:
	
	unsigned int RenderID;
	unsigned int stride;
	unsigned int indx;
	bool isBound;

	struct attribute {

		unsigned int size;
		unsigned int count;
		GLenum type;

	};

	std::vector<attribute> attributes;
	
public:

	template<typename T>
	void add_attr(unsigned int count);
	void init_VAO();

	void Bind();
	void Unbind();

	VAO(unsigned int index);
	~VAO();
};

