#pragma once
#include <random>
#include <cstdint>

namespace sparkutils
{
	
	class [[deprecated("DEPRECATED: doesn't have sufficient use cases and is superseded by boost::uuid")]] RandomGenerator
	{

	public:

		RandomGenerator() = default;

		template<typename T>
		static std::uniform_real_distribution<T> getRealDistribution(const T from, const T to) 
		{ 
			return std::uniform_real_distribution<T>(from, to);  
		};

		template<typename T>
		static std::uniform_int_distribution<T> getIntegerDistribution(const T from, const T to)
		{ 
			return std::uniform_int_distribution<T>(from, to); 
		};

		inline auto getGenerator() { return mersenne; };

	private:
		std::mt19937_64 mersenne;
		
	};

	static inline RandomGenerator randGen;
}