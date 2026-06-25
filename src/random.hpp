#pragma once
#include <random>

int getRandomInt(std::mt19937& rng, int low, int high);
float getRandomFloat(std::mt19937& rng, float low, float high);
bool getRandomChance(std::mt19937& rng, float chance);