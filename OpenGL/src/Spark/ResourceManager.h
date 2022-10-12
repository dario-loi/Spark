#pragma once
#include <vector>
#include <boost/multi_index_container.hpp>
#include <map>
#include "../Utility/RandomGenerator.h"
#include <stdint.h>

class ResourceManager
{
public:

	/*
	* Singleton
	*/
	static ResourceManager* const manager;

private:

	/*
	* Singleton
	*/
	ResourceManager() = default;
	~ResourceManager() = default;
	ResourceManager(ResourceManager& /*other*/) = default;
	ResourceManager operator=(ResourceManager& /*other*/) {};

	/*
	* Heavy-handed boost multi-index definition
	* 
	*/
	using namespace boost;

	typedef 
	
	std::uniform_int_distribution<uint64_t> UUIDdistribution = 
		sparkutils::RandomGenerator::randGen.getIntegerDistribution<uint64_t>(0ui64, UINT64_MAX);
};


