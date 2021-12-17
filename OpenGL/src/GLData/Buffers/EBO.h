#pragma once
class EBO
{
private:
	unsigned int RenderID;
	unsigned int* indx;
	unsigned int size;
	bool isBound;

public:

	EBO(unsigned int* indices, unsigned int length);
	~EBO();

	void Bind();
	void Unbind();

	void setArray(unsigned int* newArr);
	unsigned int* getArray();

	unsigned int getRenderID();

	unsigned int getSize();

	//const and dest

};

