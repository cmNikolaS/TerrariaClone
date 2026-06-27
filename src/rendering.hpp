#pragma once
#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "player.hpp"
#include "inventory.hpp"
#include "block.hpp"

void drawScreen(RenderContext& rc, const WorldContext& wc, sf::View& camera);

void drawGameUI(RenderContext& rc, Player& player);

void drawMenu(RenderContext& rc);