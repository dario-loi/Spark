#pragma once
#include <vector>
#include <assert.h>
#include <iostream>
#include "GL/glew.h"
#include <gtx/string_cast.hpp>

constexpr const unsigned int SPARK_MAXIMUM_UBO_SIZE = 128;

template<typename T>
class UBO
{
private:
	unsigned int RenderID;
	std::vector<T> data;
	unsigned int channel;
	bool isUpdated;

public:

	static unsigned int availableChannel;

	explicit UBO(std::vector<T>&& data_in);
	~UBO();
	UBO(UBO const&) = delete;
	UBO& operator=(UBO const&) = delete;
	UBO(UBO&&) = delete;

	void Bind() const;
	void Unbind() const;

	void setData(std::vector<float>&& newArr);
	void emplaceData(T&& element);
	void emplaceData(T& element);
	void setSubData(T&& element, size_t indx);
	void setSubData(T& element, size_t indx);

	void Update();

	unsigned int getRenderID() const { return RenderID; }
	unsigned int getChannel() const { return channel; }

#ifdef _DEBUG

	void printContents() const{ 
		for (auto const& elem : data) 
		{ 
			std::cout << glm::to_string(elem) << " "; 
		} 
		std::cout << std::endl; }

#endif // _DEBUG


	size_t getSize() const { return data.size(); }

};

template<typename T>
inline UBO<T>::UBO(std::vector<T>&& data_in)
{

	assert(data_in.size() < SPARK_MAXIMUM_UBO_SIZE);
	data = std::move(data_in);
	channel = availableChannel++;
	isUpdated = false;

	glGenBuffers(1, &RenderID);
	glBindBuffer(GL_UNIFORM_BUFFER, RenderID);
	glBufferData(GL_UNIFORM_BUFFER, 
		SPARK_MAXIMUM_UBO_SIZE * sizeof(T),
		data.data(), /*very descriptive*/
		GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template<typename T>
inline UBO<T>::~UBO()
{
	glDeleteBuffers(1, &RenderID);
}

template<typename T>
inline void UBO<T>::Bind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, RenderID);
	glBindBufferRange(GL_UNIFORM_BUFFER, channel, RenderID, 0, SPARK_MAXIMUM_UBO_SIZE * sizeof(T));
	if (isUpdated)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 0, data.size(), data.data());
	}
}

template<typename T>
inline void UBO<T>::Unbind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template<typename T>
inline void UBO<T>::setData(std::vector<float>&& newArr)
{

	assert(newArr.size() < SPARK_MAXIMUM_UBO_SIZE);

	data = std::move(newArr);
	isUpdated = true;
}

template<typename T>
inline void UBO<T>::emplaceData(T&& element)
{
	assert(data.size() < SPARK_MAXIMUM_UBO_SIZE);

	data.emplace_back(std::move(element));
	isUpdated = true;
}

template<typename T>
inline void UBO<T>::emplaceData(T& element)
{
	assert(data.size() < SPARK_MAXIMUM_UBO_SIZE);
	data.emplace_back(element);
	isUpdated = true;
}


template<typename T>
inline void UBO<T>::setSubData(T&& element, size_t indx)
{
	data.emplace(indx, std::move(element));
	isUpdated = true;
}

template<typename T>
inline void UBO<T>::setSubData(T& element, size_t indx)
{
	data.emplace(indx, element);
	isUpdated = true;
}

template<typename T>
inline void UBO<T>::Update()
{
	if (isUpdated)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 0, data.size(), data.data());
	}
	isUpdated = false;
}

template<typename T>
unsigned int UBO<T>::availableChannel = 0;
