#include "background.hpp"
#include <SFML/Graphics.hpp>

void Background::draw(float deltaTime, sf::RenderWindow &window, sf::View &camera, sf::Texture& backgroundTexture, sf::Vector2f playerPosition)
{
	sf::Sprite bgTexture(backgroundTexture);

	int bgSizeX = backgroundTexture.getSize().x;
	int bgSizeY = backgroundTexture.getSize().y;

	int viewSizeX = camera.getSize().x;
	int viewSizeY = camera.getSize().y;

	float xScale = (float)viewSizeX / bgSizeX;
	float yScale = (float)viewSizeY / bgSizeY;

	float scale = std::max(xScale, yScale);

	float parallaxFactor = 1.f;

	bgTexture.setScale({ scale, scale });
	bgTexture.setPosition({ camera.getCenter().x * parallaxFactor - camera.getSize().x / 2.f - 1, camera.getCenter().y * parallaxFactor - camera.getSize().y / 2.f - 1 });
	

	window.draw(bgTexture);
}