#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.hpp"

inline sf::IntRect getAtlasTextures(int block = 0, int type = 0, int w = 32, int h = 32, bool flip = false)
{
	if (!flip)
		return sf::IntRect{ sf::Vector2i(block * w, type * h), sf::Vector2i{w, h} };
	else
		return sf::IntRect{ sf::Vector2i((block + 1) * w, type * h), sf::Vector2i{-w, h} };
}

void initTextures(RenderContext &rc);

void loadMobs(MobTextures& text);