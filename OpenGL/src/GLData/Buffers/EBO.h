#pragma once
#include <memory>
class EBO
{
private:
	unsigned int RenderID;
	std::unique_ptr<unsigned int[]> indx;
	unsigned int size;

public:

	EBO(std::unique_ptr<unsigned int[]> indices, unsigned int length);
	~EBO();

	void Bind() const;
	void Unbind() const;

	void setArray(std::unique_ptr<unsigned int[]> newArr);

	unsigned int getRenderID() const { return RenderID; }

	unsigned int getSize() const { return size; }

	//ONLY IF YOU ARE ACCESSING IT FOR OPENGL PURPOSES, I AM WATCHING YOU!

	unsigned int* getArray() const { return indx.get(); }	

};

