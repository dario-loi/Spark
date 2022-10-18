#pragma once
#include "VBO.h"
#include "GL/glew.h"
#include <cmath>
#include <ranges>


VBO::VBO(std::vector<float>&& vertices) noexcept
	: verts(std::move(vertices))
{
	glGenBuffers(1, &RenderID);
	glBindBuffer(GL_ARRAY_BUFFER, RenderID);
	
	instanceBaseOffset = std::distance(verts.begin(), verts.end());
	std::vector<float> dest(verts.size() + spark::INITIAL_INSTANCES * spark::FLOATS_IN_INSTANCE);

	
	/*
	We use memcpy to perform type erasure to
		void*, this (should) not be a problem
		since we are sure that the underlying 
		types are all POD combinations of floats.
		*/
	/*move vertices into vector*/
	memcpy(std::to_address(dest.begin()),
		std::to_address(verts.begin()),
		verts.size() * sizeof(float));

	/*move instances into vector*/
	memcpy(std::to_address(dest.begin() + instanceBaseOffset),
		std::to_address(instances.begin()),
		instances.size() * spark::SIZEOF_INSTANCE_DATA
	);

	glBufferData(GL_ARRAY_BUFFER, 
		static_cast<GLsizeiptr>(dest.size() * sizeof(float)),
		dest.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VBO::~VBO() noexcept
{
	glDeleteBuffers(1, &RenderID);
}

void VBO::Bind() const 
{
	glBindBuffer(GL_ARRAY_BUFFER, RenderID);
}

void VBO::Unbind() const 
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::setData(std::vector<float>&& newArr)
{
	verts = std::move(newArr);
	this->Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0UI64, static_cast<GLintptr>(verts.size()), verts.data());
	this->Unbind();
}

void VBO::setInstanceSubData(std::vector<spark::SparkInstanceData> const& data, size_t offset) const noexcept
{
	this->Bind();
	glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(instanceBaseOffset +
		offset * spark::SIZEOF_INSTANCE_DATA),
		sizeof(float), (void*)data.data());
	this->Unbind();
}

void VBO::setInstanceData(std::vector<spark::SparkInstanceData> const& data) const noexcept
{
	this->Bind();
	glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(instanceBaseOffset * sizeof(float)),
		static_cast<GLsizeiptr>(data.size()), (void*)data.data());
	this->Unbind();
}


/* 
VAO MUST be bound 
in order for this to work properly
since we substitute the underlying VBO
with a new one in case of error

return the index of the inserted instance.
*/
size_t VBO::addInstance(spark::SparkInstanceData& inst) noexcept
{

	instances.emplace_back(inst);
	auto curr_indx = instances.size();
	this->Bind();
	if (instances.size() > instanceCapacity)//Realloc VBO
	{
		/*expand by a power of two*/
		instanceCapacity = static_cast<size_t>(std::exp2(std::ceil(std::log2(instanceCapacity)) + 1.0));

		std::vector<float> dest(verts.size() + instanceCapacity * spark::FLOATS_IN_INSTANCE);
		
		/*
		We use memcpy to perform type erasure to
			void*, this (should) not be a problem
			since we are sure that the underlying
			types are all POD combinations of floats.
			*/
			/*move vertices into vector*/
		memcpy(std::to_address(dest.begin()),
			std::to_address(verts.begin()),
			verts.size() * sizeof(float));

		/*move instances into vector*/
		memcpy(std::to_address(dest.begin() + instanceBaseOffset),
			std::to_address(instances.begin()),
			instances.size() * spark::SIZEOF_INSTANCE_DATA);


		glDeleteBuffers(1, &RenderID);
		glGenBuffers(1, &RenderID);
		glBindBuffer(GL_ARRAY_BUFFER, RenderID);

		glBufferData(GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(verts.size() * sizeof(float) + instanceCapacity * spark::SIZEOF_INSTANCE_DATA),
			dest.data(), GL_DYNAMIC_DRAW); 

		/*
		
			The specifier is changed to GL_DYNAMIC_DRAW cause it 
			is now safe to assume that this Model will be spawned
			repeatedly.
		
		*/
		
		
	}
	else //Append element to previous storage
	{
	glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(instanceBaseOffset * sizeof(float) +
		curr_indx * spark::SIZEOF_INSTANCE_DATA), 
		spark::SIZEOF_INSTANCE_DATA, (void*) std::to_address(instances.begin() + curr_indx));
	}
	this->Unbind();
}

