#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.hpp"

inline sf::IntRect getBlocksTextures(uint16_t block, uint16_t type = 0, const int w = 32, const int h = 32)
{
	return sf::IntRect{ sf::Vector2i(block * w, type * 32),  sf::Vector2i{w, h} };
}

void initTextures(RenderContext &rc);

void loadMobs(MobTextures& text);