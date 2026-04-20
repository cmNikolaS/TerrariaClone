#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Constants.hpp"

void handlePlayerInput(const sf::Window& window, Player& player, const float dt, bool creative);

bool getMouseLeftClickPos(sf::Vector2i& pos, sf::RenderWindow& window);

bool getMouseRightClickPos(sf::Vector2i& pos, sf::RenderWindow& window);

void handleEvents(sf::RenderWindow& window, Player& player,
	sf::Clock& scrollPress, GameState& gs, MainGameState&mgs);

void handleMouseClicks(RenderContext& rc, WorldContext& wc,
	Player& player, GameClocks &gc);
