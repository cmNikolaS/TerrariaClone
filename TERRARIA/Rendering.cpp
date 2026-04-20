#pragma once
#include <SFML/Graphics.hpp>
#include "Rendering.hpp"
#include "Constants.hpp"
#include "Player.hpp"
#include "Hotbar.hpp"
#include <string>

void drawMenu(RenderContext& rc)
{
	sf::View MenuView;
	rc.window.setView(MenuView);
	sf::Text wmsg(rc.font, "WELCOME TO TERRARIA CLONE", 30);
	auto size = wmsg.getGlobalBounds().size;
	wmsg.setPosition({MenuView.getCenter().x - size.x / 2.f, MenuView.getCenter().y / 2.f});
	std::string controlsStr = \
		"Controls\n\n"\
		"\tESC - Menu / Game\n"\
		"\tV - Open Map View\n"\
		"\tR - Render New World\n"\
		"\tC - Enter / Leave Creative Mode\n"\
		"\tA - Left					D - Right\n"\
		"\tSPACE - Jump/Fly			S - Go Down\n"\
		"\tLeftClick - Destroy		RightClick - Place\n"\
		"\tScroll - Navigate Hotbar\n\n"\
		"\tQ - Quit";

	sf::Text options(rc.font, controlsStr, 15);
	options.setPosition({ wmsg.getPosition().x + 50.f, wmsg.getPosition().y + 150.f });

	rc.window.draw(wmsg);
	rc.window.draw(options);
}

void drawScreen(RenderContext& rc, const WorldContext& wc, const sf::View& camera)
{
	sf::Vector2f center = camera.getCenter();
	sf::Vector2f size = camera.getSize();

	int by = static_cast<int>((center.y - size.y / 2.f) / tileSize - 1);
	int ey = static_cast<int>((center.y + size.y / 2.f) / tileSize + 1);
	int bx = static_cast<int>((center.x - size.x / 2.f) / tileSize - 1);
	int ex = static_cast<int>((center.x + size.x / 2.f) / tileSize + 1);

	sf::Sprite sprite(rc.blockTextures[(size_t)BlockId::Water]);
	for (int r = by; r < ey; r++)
	{
		for (int c = bx; c < ex; c++)
		{
			if (r < 0 || r >= worldH || c < 0 || c >= worldW || wc.map[r][c] == BlockId::Air) continue;
			size_t t = (size_t)wc.map[r][c];
			sprite.setTexture(rc.blockTextures[t]);
			sprite.setPosition({ c * tileSize, r * tileSize });
			rc.window.draw(sprite);
		}
	}
}

void drawCoordinates(RenderContext& rc, Player& player)
{
	sf::Text text(rc.font, "Coordinates\n\tX: " + std::to_string(static_cast<ui16>(player.getPos().x / tileSize)) + "\n\tY: " + std::to_string(worldH - static_cast<ui16>(player.getPos().y / tileSize)), 10);
	sf::Vector2f pos = getCameraPos(player.getCamera());
	text.setFillColor(sf::Color::Black);
	text.setPosition({ pos.x + 20, pos.y + 20 });
	rc.window.draw(text);
}

void drawHotbar(RenderContext& rc, Player& player)
{
	const float total = Hotbar::SLOTS * (Hotbar::SLOT_SIZE + Hotbar::PADDING) - Hotbar::PADDING;
	sf::Vector2f camPos = getCameraPos(player.getCamera());
	sf::Vector2f camSize = player.getCamera().getSize();

	float startX = camPos.x + (camSize.x - total) / 2.f;
	float startY = camPos.y + camSize.y - Hotbar::SLOT_SIZE - 10.f;

	for (ui16 i = 0; i < Hotbar::SLOTS; i++)
	{
		float x = startX + i * (Hotbar::SLOT_SIZE + Hotbar::PADDING);
		//slot
		sf::RectangleShape slot({ Hotbar::SLOT_SIZE, Hotbar::SLOT_SIZE });
		slot.setPosition({ x, startY });
		slot.setFillColor(sf::Color(50, 50, 50, 180));
		slot.setOutlineThickness(i == player.getHotbar().getSelected() ? 2.f : 1.f);
		slot.setOutlineColor(i == player.getHotbar().getSelected() ? sf::Color::White : sf::Color(150, 150, 150));
		rc.window.draw(slot);
		//block on slot
		sf::Sprite bl(rc.blockTextures[(ui16)player.getHotbar().getBlockAt(i)]);
		bl.setPosition({ slot.getPosition().x + 4.f, slot.getPosition().y + 4.f });
		bl.setScale({ 2.f, 2.f });
		rc.window.draw(bl);
		//num of slot
		sf::Text num(rc.font, std::to_string(i + 1), 10);
		num.setPosition({ x + 2.f, startY + 2.f });
		num.setFillColor(sf::Color(200, 200, 200));
		rc.window.draw(num);
	}
}

void drawGameUI(RenderContext& rc, Player& player)
{
	//coordinates
	drawCoordinates(rc, player);

	//hotbar
	drawHotbar(rc, player);
}