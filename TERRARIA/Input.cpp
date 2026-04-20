#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Constants.hpp"
#include "Input.hpp"

void handlePlayerInput(const sf::Window& window, Player& player, const float dt, bool creative)
{
	if (!window.hasFocus()) return;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
		player.getHotbar().setSelected(0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
		player.getHotbar().setSelected(1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
		player.getHotbar().setSelected(2);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4))
		player.getHotbar().setSelected(3);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5))
		player.getHotbar().setSelected(4);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6))
		player.getHotbar().setSelected(5);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7))
		player.getHotbar().setSelected(6);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8))
		player.getHotbar().setSelected(7);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9))
		player.getHotbar().setSelected(8);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		player.increaseVel({ player.getSpeed(), 0 });
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		player.increaseVel({ -player.getSpeed(), 0 });
	if (creative == false)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			if (!player.getJumped())
			{
				player.increaseVel({ 0.f, -player.getJumpVel() });
				player.setJumped(true);
			}
		}
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			player.increaseVel({ 0.f, -player.getJumpVel() });
			player.setCreativeGoingDown(false);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			player.increaseVel({ 0.f, player.getJumpVel() });
			player.setCreativeGoingDown(true);
		}
	}
}

bool getMouseLeftClickPos(sf::Vector2i& pos, sf::RenderWindow& window)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		pos = sf::Mouse::getPosition(window);
		return true;
	}
	return false;
}

bool getMouseRightClickPos(sf::Vector2i& pos, sf::RenderWindow& window)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		pos = sf::Mouse::getPosition(window);
		return true;
	}
	return false;
}

void handleEvents(sf::RenderWindow& window, Player& player,
	sf::Clock& scrollPress, GameState& gs, MainGameState& mgs)
{
	if (window.hasFocus())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
			gs.restart = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
			gs.close = true;
	}
	while (std::optional event = window.pollEvent())
	{
		if (!window.hasFocus()) continue;
		if (event->is<sf::Event::Closed>())
			window.close();

		if (auto key = event->getIf<sf::Event::KeyPressed>())
		{
			if (key->code == sf::Keyboard::Key::V && mgs == MainGameState::Playing)
				gs.playerCamera = !gs.playerCamera;
			else if (key->code == sf::Keyboard::Key::C && gs.playerCamera && mgs == MainGameState::Playing)
			{
				gs.creativeMode = !gs.creativeMode;
				if (gs.creativeMode)
					player.setVelocity({ player.getVelocity().x, 0.f });
			}
			else if (key->code == sf::Keyboard::Key::Escape)
			{
				if (mgs == MainGameState::Menu)
					mgs = MainGameState::Playing;
				else
					mgs = MainGameState::Menu;
			}
		}

		if (auto scroll = event->getIf<sf::Event::MouseWheelScrolled>())
		{
			if (scrollPress.getElapsedTime().asSeconds() < 0.25f) continue;
			scrollPress.restart();
			if (gs.playerCamera)
			{
				if (scroll->delta < 0)
					player.getHotbar().scrollDown();
				else
					player.getHotbar().scrollUp();
			}
		}
	}
}
void handleMouseClicks(RenderContext& rc, WorldContext& wc, Player& player, GameClocks &gc)
{
	if (!rc.window.hasFocus()) return;
	sf::Vector2i clickPos = {0, 0};
	if (getMouseLeftClickPos(clickPos, rc.window) && gc.leftClickPress.getElapsedTime().asSeconds() > leftMouseClickCooldown)
	{
		gc.leftClickPress.restart();
		auto pos = rc.window.mapPixelToCoords(clickPos, player.getCamera());
		int tileX = static_cast<int>((pos.x) / tileSize);
		int tileY = static_cast<int>((pos.y) / tileSize);
		if (tileX >= 0 && tileX < worldW && tileY >= 0 && tileY < worldH)
		{
			wc.map[tileY][tileX] = BlockId::Air;
		}
	}
	if (getMouseRightClickPos(clickPos, rc.window) && gc.rightClickPress.getElapsedTime().asSeconds() > rightMouseClickCooldown)
	{
		gc.rightClickPress.restart();
		auto pos = rc.window.mapPixelToCoords(clickPos, player.getCamera());
		int tileX = static_cast<int>(pos.x / tileSize);
		int tileY = static_cast<int>(pos.y / tileSize);
		if (tileX >= 0 && tileX < worldW && tileY >= 0 && tileY < worldH)
		{
			wc.map[tileY][tileX] = player.getHotbar().getSelectedBlock();
		}
	}

	sf::Vector2f camPos = getCameraPos(player.getCamera());
	sf::Vector2f camSize = player.getCamera().getSize();

	float total = Hotbar::SLOTS * (Hotbar::SLOT_SIZE + Hotbar::PADDING) - Hotbar::PADDING;
	float startX = camPos.x + (camSize.x - total) / 2.f;
	float startY = camPos.y + camSize.y - Hotbar::SLOT_SIZE - 10.f;
	
	auto worldPos = rc.window.mapPixelToCoords(clickPos, player.getCamera());
	
	for (ui8 i = 0; i < Hotbar::SLOTS; i++)
	{
		float x = startX + i * (Hotbar::SLOT_SIZE + Hotbar::PADDING);
		if (worldPos.x >= x && worldPos.x < x + Hotbar::SLOT_SIZE && worldPos.y >= startY && worldPos.y < startY + Hotbar::SLOT_SIZE)
		{
			player.getHotbar().setSelected(i);
			break;
		}
	}

}