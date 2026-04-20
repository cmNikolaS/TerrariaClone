#pragma once
#include "Constants.hpp"
#include <vector>
#include <array>
#include <cstdint>
#include <string>
#include <SFML/Graphics.hpp>

void loadBlocks(BlockTextures& text, Blocks& blocks);
std::vector<ui16> generateHeightMap();
void generateSurface(Map& map, const std::vector<ui16>& hm);
void generateSubsurface(Map& map, std::vector<sf::Vector2i>& worms);
void doWorm(Map& map, sf::Vector2i worm);
void generateCaves(Map& map, const std::vector<sf::Vector2i>& worms);
Map generateMap();
const sf::Vector2f playerSpawnPos(const WorldContext& wc);