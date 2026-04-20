#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.hpp"
#include "Player.hpp"
#include "Hotbar.hpp"

void drawScreen(RenderContext& rc, const WorldContext& wc, const sf::View& camera);

const sf::Vector2f getCameraPos(const sf::View& camera);

void drawCoordinates(RenderContext& rc, Player& player);

void drawHotbar(RenderContext& rc, Player& player);

void drawGameUI(RenderContext& rc, Player& player);

void drawMenu(RenderContext& rc);