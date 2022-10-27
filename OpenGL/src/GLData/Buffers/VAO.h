#pragma once
#include <vector>
#include "GL/glew.h"
#include "mat4x4.hpp"

class VAO
{
private:
	
	unsigned int RenderID{0UI32};
	unsigned int stride{ 0UI32 };
	unsigned int i_stride{ 0UI32 };

	struct Attribute {

		int size{ 0I32 };
		int count{ 0I32 };
		GLenum type;

	};

	std::vector<Attribute> attributes;
	std::vector<Attribute> instance_attr;
	
public:

	template<typename TYPE_NOT_SPECIFIED>
	void add_attr(int const) = delete; //If we had reflection, I wouldn't be doing this :)

	template<>
	void add_attr<float>(int const count);

	template<>
	void add_attr<glm::mat4>(int const count);

	template<typename TYPE_NOT_SPECIFIED>
	void add_attr_inst(int const count) = delete;

	template<>
	void add_attr_inst<float>(int const count);

	template<>
	void add_attr_inst<glm::mat4>(int const count);

	void init_VAO() const;

	void Bind() const;
	void Unbind() const;

	explicit VAO();
	~VAO();
};

