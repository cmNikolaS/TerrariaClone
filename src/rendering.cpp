#pragma once
#include <SFML/Graphics.hpp>
#include "rendering.hpp"
#include "constants.hpp"
#include "player.hpp"
#include "inventory.hpp"
#include <string>
#include "textures.hpp"
#include <cmath>

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

void drawScreen(RenderContext& rc, const WorldContext& wc, sf::View& camera)
{
	sf::Vector2f originalCenter = camera.getCenter();
	
	camera.setCenter({ std::round(originalCenter.x), std::round(originalCenter.y) });
	rc.window.setView(camera);

	sf::Vector2f center = camera.getCenter();
	sf::Vector2f size = camera.getSize();

	int by = static_cast<int>((center.y - size.y / 2.f) / tileSize - 1);
	int ey = static_cast<int>((center.y + size.y / 2.f) / tileSize + 1);
	int bx = static_cast<int>((center.x - size.x / 2.f) / tileSize - 1);
	int ex = static_cast<int>((center.x + size.x / 2.f) / tileSize + 1);

	sf::Sprite sprite(rc.blocksAtlas);

	for (int r = by; r < ey; r++)
	{
		for (int c = bx; c < ex; c++)
		{
			if (r < 0 || r >= worldH || c < 0 || c >= worldW || wc.map[r][c] == Block::air)
			{
				continue;
			}
			int t = wc.map[r][c];
			sprite.setTextureRect(getAtlasTextures(t));
			sprite.setPosition({ c * tileSize, r * tileSize });
			rc.window.draw(sprite);
		}
	}
	camera.setCenter(originalCenter);
	rc.window.setView(camera);
}

void drawFPS(RenderContext& rc)
{

}

void drawCoordinates(RenderContext& rc, Player& player)
{
	sf::Text text(rc.font, "Coordinates\n\tX: " + std::to_string(static_cast<ui16>(player.getPos().x / tileSize)) + "\n\tY: " + std::to_string(worldH - static_cast<ui16>(player.getPos().y / tileSize)), 10);
	sf::Vector2f pos = getCameraPos(player.getCamera());
	text.setFillColor(sf::Color::Black);
	text.setPosition({ pos.x + 20, pos.y + 20 });
	rc.window.draw(text);
}

void drawHearts(RenderContext& rc, Player& player)
{
	int heartType = 0;
	sf::Vector2f pos = getCameraPos(player.getCamera());
	sf::Vector2f topRightPos = { pos.x + player.getCamera().getSize().x, pos.y };
	int heartSize = 32;
	int numberOfHearts = 5;
	int paddingBetweenHearts = 4;
	int topPadding = 16;
	int rightPadding = 8;


	sf::Vector2f drawPosStart = 
	{topRightPos.x - (heartSize + paddingBetweenHearts) * numberOfHearts - rightPadding,
	topRightPos.y + topPadding};

	for (int i = 0; i < numberOfHearts; i++)
	{
		sf::Sprite heartSprite(rc.hearts);
		heartSprite.setTextureRect(getAtlasTextures(heartType));
		heartSprite.setPosition(drawPosStart);
		rc.window.draw(heartSprite);
		drawPosStart.x += (heartSize + numberOfHearts);
	}
}

void drawInventory(RenderContext& rc, Player& player)
{
	auto oldView = rc.window.getView();
	rc.window.setView(rc.window.getDefaultView());

	auto& invLayout = player.getInventoryLayout();
	auto& inv = player.getInventory();
	
	invLayout.build(rc.window.getSize(), true);

	sf::Sprite itemSprite(rc.blocksAtlas);

	float scale = invLayout.slotSize / static_cast<float>(tileSize);
	
	int c = 0;

	for (auto &slot : invLayout.slots)
	{
		if (!rc.insideInventory && c >= Inventory::HOTBAR) break;
		c++;
		sf::RectangleShape sl(slot.rect.size);
		sl.setPosition(slot.rect.position);
		sl.setFillColor({20, 140, 120, 130});
		rc.window.draw(sl);

		ItemStack& stack = inv.slots[slot.inventoryIndex];
		if (stack.item != Item::none && stack.count > 0)
		{
			itemSprite.setTextureRect(getAtlasTextures(stack.item, 4));
			itemSprite.setScale({ scale, scale });
			itemSprite.setPosition(slot.rect.position);
			rc.window.draw(itemSprite);
			
			if (stack.count > 1)
			{
				sf::Text countText(rc.font, std::to_string(stack.count), 12);
				countText.setFillColor(sf::Color::White);
				countText.setPosition({ slot.rect.position.x, slot.rect.position.y });
				rc.window.draw(countText);
			}

		}
	}

	ItemStack& held = player.getItemOnHold();
	if (held.item != Item::none)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(rc.window);
		sf::Sprite heldIcon(rc.blocksAtlas);
		heldIcon.setTextureRect(getAtlasTextures(held.item));
		heldIcon.setPosition({ static_cast<float>(mousePos.x) - 20.f, static_cast<float>(mousePos.y) - 20.f });
		rc.window.draw(heldIcon);
		if (held.count > 1)
		{
			sf::Text countText(rc.font, std::to_string(held.count), 12);
			countText.setFillColor(sf::Color::White);
			countText.setPosition(heldIcon.getPosition());
			rc.window.draw(countText);
		}
	}

	rc.window.setView(oldView);
}

void drawGameUI(RenderContext& rc, Player& player)
{
	//coordinates
	drawCoordinates(rc, player);

	//fps
	drawFPS(rc);

	//hearts
	drawHearts(rc, player);

	drawInventory(rc, player);
}