#pragma once
#include <memory>

class VBO
{
private:
	unsigned int RenderID;
	std::unique_ptr<float[]> verts;
	unsigned int size;

public:

	 VBO(std::unique_ptr<float[]> vertices, unsigned int length);
	~VBO();
	

	void Bind() const;
	void Unbind() const;

	void setArray(std::unique_ptr<float[]> newArr);

	unsigned int getRenderID() const { return RenderID; }

	unsigned int getSize() const { return size; }


};

