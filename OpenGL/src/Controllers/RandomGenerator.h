#pragma once
#include <random>

class RandomGenerator
{

public:
	static auto getIntegerDistribution(const int from, const int to) { return std::uniform_int_distribution<int>(from, to); };
	static auto getRealDistribution(const float from, const float to) { return std::uniform_real_distribution<float>(from, to); };
	static auto getGenerator() { return mersenne; };

private:
	static std::random_device device;
	static std::mt19937 mersenne;
	RandomGenerator() = default;
};



