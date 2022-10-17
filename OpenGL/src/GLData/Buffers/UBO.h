#pragma once
#include <vector>
#include <assert.h>
#include <iostream>
#include "GL/glew.h"
#include <gtx/string_cast.hpp>
#include "..\..\Spark\SparkConfig.h"

template<typename T>
class UBO
{
private:
	unsigned int RenderID;
	std::vector<T> data;
	unsigned int channel;
	bool isUpdated{false};

public:


	explicit UBO(std::vector<T>&& data_in, unsigned int channel);
	explicit UBO(unsigned int channel);

	~UBO();
	UBO(UBO const&) = delete;
	UBO& operator=(UBO const&) = delete;
	UBO(UBO&&) = delete;

	void Bind();
	void Unbind() const;

	void setData(std::vector<T>&& newArr);
	void emplaceData(T&& element);
	void emplaceData(T& element);
	void setSubData(T&& element, size_t indx);
	void setSubData(T& element, size_t indx);
	T& getElementAt(size_t indx);

	void Update();

	[[nodiscard]] unsigned int getRenderID() const { return RenderID; }
	[[nodiscard]] unsigned int getChannel() const { return channel; }

#ifdef _DEBUG
	void printContents() const{ 
		for (auto const& elem : data) 
		{ 
			std::cout << glm::to_string(elem) << " "; 
		} 
		std::cout << std::endl; }
#endif // _DEBUG


	[[nodiscard]] size_t getSize() const { return data.size(); }

};

template<typename T>
inline UBO<T>::UBO(std::vector<T>&& data_in, const unsigned int channel)
	: data(std::move(data_in)), channel(channel)
{

	assert(data_in.size() < SPARK_MAXIMUM_UBO_SIZE);

	glGenBuffers(1, &RenderID);
	glBindBuffer(GL_UNIFORM_BUFFER, RenderID);
	glBufferData(GL_UNIFORM_BUFFER, 
		SPARK_MAXIMUM_UBO_SIZE * sizeof(T),
		data.data(), /*very descriptive*/
		GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template<typename T>
inline UBO<T>::UBO(unsigned int channel)
	: channel(channel)
{
	std::vector<T> default_init(SPARK_MAXIMUM_UBO_SIZE);

	glGenBuffers(1, &RenderID);
	glBindBuffer(GL_UNIFORM_BUFFER, RenderID);
	glBufferData(GL_UNIFORM_BUFFER,
		SPARK_MAXIMUM_UBO_SIZE * sizeof(T),
		default_init.data(), /*Placeholder filler*/
		GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


template<typename T>
inline UBO<T>::~UBO()
{
	glDeleteBuffers(1, &RenderID);
}

template<typename T>
inline void UBO<T>::Bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, RenderID);
	glBindBufferRange(GL_UNIFORM_BUFFER, channel, RenderID, 0, SPARK_MAXIMUM_UBO_SIZE * sizeof(T));
	Update();
}

template<typename T>
inline void UBO<T>::Unbind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template<typename T>
inline void UBO<T>::setData(std::vector<T>&& newArr)
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
	data.at(indx) = std::move(element);
	isUpdated = true;
}

template<typename T>
inline void UBO<T>::setSubData(T& element, size_t indx)
{
	data.at(indx) = element;
	isUpdated = true;
}

/**
 * Method that wraps an .at() call on the underlying vector, it allows
 * the utilization of an UBO class when T is a struct, by allowing the 
 * user to access the underlying struct and modify its members, to do so,
 * it must break encapsulation.
 * 
 * \param indx index of the item to fetch
 * \return a Reference to the item at index indx
 */
template<typename T>
inline T& UBO<T>::getElementAt(size_t indx)
{
	isUpdated = true;		//we MUST assume that the data has changed, even if it's not the case.
	return data.at(indx);	
}

template<typename T>
inline void UBO<T>::Update()
{
	if (isUpdated)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, channel, RenderID, 0, SPARK_MAXIMUM_UBO_SIZE * sizeof(T));
		glBufferSubData(GL_UNIFORM_BUFFER, 0, data.size() * sizeof(T), data.data());
	}
	isUpdated = false;
}
