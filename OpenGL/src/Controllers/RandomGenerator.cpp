#include "RandomGenerator.h"

std::random_device RandomGenerator::device;
std::mt19937 RandomGenerator::mersenne(RandomGenerator::device());



