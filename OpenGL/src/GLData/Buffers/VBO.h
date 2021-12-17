class VBO
{
private:
	unsigned int RenderID;
	float* verts;
	unsigned int size;
	bool isBound;

public:

	 VBO(float* vertices, unsigned int length);
	~VBO();

	void Bind();
	void Unbind();

	void setArray(float* newArr);
	float* getArray();

	unsigned int getRenderID();

	unsigned int getSize();

	//const and dest

};

