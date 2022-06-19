#pragma once
#include <vector>

class VBO
{
private:
	unsigned int RenderID;
	std::vector<float> verts;

public:

	 explicit VBO(std::vector<float>&& vertices);
	~VBO();
	

	void Bind() const;
	void Unbind() const;

	void setData(std::vector<float>&& newArr);

	unsigned int getRenderID() const { return RenderID; }

	size_t getSize() const { return verts.size(); }

	std::vector<float> const& getData() const { return verts; }

};

