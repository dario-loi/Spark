#pragma once
class VBO
{
private:
	unsigned int RenderID;
	float* verts;
	unsigned int size;


public:

	 VBO(float* vertices);
	~VBO();

	void Bind();
	void Unbind();

	void setArray(float* newArr);
	float* getArray();

	unsigned int getRenderID();

	unsigned int getSize();

	//const and dest

};

