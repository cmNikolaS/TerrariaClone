#pragma once
#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "constants.hpp"
#include "block.hpp"

void handlePlayerInput(const sf::Window &window, Player& player, const float dt);

bool getMouseLeftClickPos(sf::Vector2i& pos, sf::RenderWindow& window);

bool getMouseRightClickPos(sf::Vector2i& pos, sf::RenderWindow& window);

void handleEvents(RenderContext& rc, Player& player,
	sf::Clock& scrollPress, GameState& gs, MainGameState&mgs);

void handleMouseClicks(RenderContext& rc, WorldContext& wc,
	Player& player, GameClocks &gc);

void handleInventoryInput(RenderContext& rc, Player& player, InventoryClickState& clickState, bool &wasUIClicked);

ui8 handleMainMenuInput(RenderContext& rc);