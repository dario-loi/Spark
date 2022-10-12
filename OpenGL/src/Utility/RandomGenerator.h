#pragma once
#include <random>
#include <stdint.h>

namespace sparkutils
{

	class RandomGenerator
	{

	public:

		static RandomGenerator randGen;

		template<typename T>
		std::uniform_real_distribution<T> getRealDistribution(const T from, const T to) 
		{ 
			return std::uniform_real_distribution<T>(from, to);  
		};

		template<typename T>
		std::uniform_int_distribution<T> getIntegerDistribution(const T from, const T to) 
		{ 
			return std::uniform_int_distribution<T>(from, to); 
		};


		auto getGenerator() { return mersenne; };

	private:
		std::random_device device;
		std::mt19937 mersenne;
		RandomGenerator() : device(), mersenne(device) {};
	};


}