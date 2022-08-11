#pragma once
#include <vector>
#include "GL/glew.h"

class VAO
{
private:
	
	unsigned int RenderID;
	unsigned int stride;

	struct attribute {

		unsigned int size;
		unsigned int count;
		GLenum type;

		attribute(unsigned int size, unsigned int count, GLenum type)
			: size(size), count(count), type(type) { }
	};

	std::vector<attribute> attributes;
	
public:

	template<typename T>
	void add_attr(unsigned int count);
	void init_VAO() const;

	void Bind() const;
	void Unbind() const;

	explicit VAO();
	~VAO();
};

