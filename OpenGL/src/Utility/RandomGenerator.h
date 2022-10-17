#pragma once
#include <random>
#include <cstdint>

namespace sparkutils
{
	
	class RandomGenerator
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

		[[nodiscard]] inline auto getEngine() const { return mersenne; };

	private:
		std::mt19937_64 mersenne{};
		
	};

	static inline const RandomGenerator randGen;
}