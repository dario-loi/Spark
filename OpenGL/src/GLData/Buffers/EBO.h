#pragma once
class EBO
{
private:
	unsigned int RenderID;
	unsigned int* indx;
	unsigned int size;


public:

	EBO(unsigned int* indices);
	~EBO();

	void Bind();
	void Unbind();

	void setArray(unsigned int* newArr);
	unsigned int* getArray();

	unsigned int getRenderID();

	unsigned int getSize();

	//const and dest

};

