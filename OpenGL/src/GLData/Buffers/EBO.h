#pragma once
#include <vector>

class EBO
{
private:
	unsigned int RenderID;
	std::vector<unsigned int> indx;

public:

	explicit EBO(std::vector<unsigned int>&& indices) noexcept;
	~EBO();

	void Bind() const;
	void Unbind() const;

	void setArray(std::vector<unsigned int>&& newArr);

	unsigned int getRenderID() const { return RenderID; }

	size_t getSize() const { return indx.size(); }

	//ONLY IF YOU ARE ACCESSING IT FOR OPENGL PURPOSES, I AM WATCHING YOU!

	std::vector<unsigned int> const& getArray() const { return indx; }	

};

