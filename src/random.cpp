#include <random>
#include "random.hpp"

int getRandomInt(std::mt19937& rng, int low, int high)
{
	std::uniform_int_distribution<int> distr(low, high);
	return distr(rng);
}
float getRandomFloat(std::mt19937& rng, float low, float high)
{
	std::uniform_real_distribution<float> distr(low, high);
	return distr(rng);
}
bool getRandomChance(std::mt19937& rng, float chance)
{
	std::uniform_real_distribution<float> distr(0.f, 1.f);
	return distr(rng) <= chance;
}