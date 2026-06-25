#pragma once
#include <SFML/Graphics.hpp>

class Background
{
public:
	
	void draw(float deltaTime, sf::RenderWindow& window, sf::View &camera, sf::Texture &backgroundTexture, sf::Vector2f playerPosition);
};