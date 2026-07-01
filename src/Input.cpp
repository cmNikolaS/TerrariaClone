#pragma once
#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "constants.hpp"
#include "input.hpp"
#include "ui.hpp"
#include "block.hpp"
#include <iostream>

void handlePlayerInput(const sf::Window &window, Player& player, const float dt)
{
	if (!window.hasFocus()) return;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
		player.setActiveSlot(0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
		player.setActiveSlot(1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
		player.setActiveSlot(2);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4))
		player.setActiveSlot(3);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5))
		player.setActiveSlot(4);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6))
		player.setActiveSlot(5);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7))
		player.setActiveSlot(6);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8))
		player.setActiveSlot(7);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9))
		player.setActiveSlot(8);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		player.increaseVel({ player.getSpeed(), 0 });
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		player.increaseVel({ -player.getSpeed(), 0 });
	if (player.isInCreative() == false)
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

ui8 handleMainMenuInput(RenderContext& rc)
{
	if (!rc.window.hasFocus()) return 0;

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		return 0;

	UILayout mainMenuLayout;

	sf::View uiView(sf::FloatRect({ 0.f, 0.f }, sf::Vector2f(rc.window.getSize())));
	rc.window.setView(uiView);

	mainMenuLayout.build(rc.window.getSize(), UILayout::MainMenu);

	sf::Vector2i mousePixel = sf::Mouse::getPosition(rc.window);
	sf::Vector2f mousePos = rc.window.mapPixelToCoords(mousePixel, uiView);

	for (const auto &w : mainMenuLayout.widgets)
	{
		if (w->rect.contains(mousePos))
		{
			return w->action;
		}
	}
	return 0;
}

void handleSettingsMenuInput(RenderContext& rc)
{
	if (!rc.window.hasFocus()) return;

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (rc.activeSlider)
			rc.activeSlider->dragging = false;
		rc.activeSlider = nullptr;
		return;
	}

	sf::View uiView(sf::FloatRect({ 0.f, 0.f }, sf::Vector2f(rc.window.getSize())));
	rc.window.setView(uiView);

	sf::Vector2i mousePixel = sf::Mouse::getPosition(rc.window);
	sf::Vector2f mousePos = rc.window.mapPixelToCoords(mousePixel, uiView);

	for (const auto& w : rc.settingsLayout.widgets)
	{
		if (w->getType() == Widget::widgetType::slider)
		{
			Slider* slider = static_cast<Slider*>(w.get());
			if (!slider->knobRect.contains(mousePos))continue;
			rc.activeSlider = slider;
			slider->dragging = true;
			break;
		}
	}

	if (rc.activeSlider)
	{
		float x = std::clamp(mousePos.x,
			rc.activeSlider->rect.position.x,
			rc.activeSlider->rect.position.x + rc.activeSlider->rect.size.x);
		
		rc.activeSlider->value = (x - rc.activeSlider->rect.position.x) / rc.activeSlider->rect.size.x;
	
		rc.activeSlider->knobRect.position.x = x - rc.activeSlider->knobRect.size.x / 2.f;

		if (rc.activeSlider->setting == Slider::Music)
		{
			gSettings.Music = rc.activeSlider->value;
		}
		else if (rc.activeSlider->setting == Slider::SFX)
		{
			gSettings.SFX = rc.activeSlider->value;
		}
		else if (rc.activeSlider->setting == Slider::Zoom)
		{
			gSettings.Zoom = 0.66f + (1.f - rc.activeSlider->value) * 0.75f;
		}
	}
}

void handleInventoryInput(RenderContext& rc, Player& player, InventoryClickState &clickState, bool& wasUIClicked)
{
	wasUIClicked = false;
	if (!rc.window.hasFocus()) return;
	
	bool leftDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	bool rightDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

	bool leftClicked = leftDown && !clickState.leftWasDown;
	bool rightClicked = rightDown && !clickState.rightWasDown;
	
	clickState.leftWasDown = leftDown;
	clickState.rightWasDown = rightDown;

	sf::View uiView(sf::FloatRect({ 0.f, 0.f }, sf::Vector2f(rc.window.getSize())));
	sf::Vector2i mousePixel = sf::Mouse::getPosition(rc.window);
	sf::Vector2f mousePos = rc.window.mapPixelToCoords(mousePixel, uiView);

	InventoryLayout& layout = player.getInventoryLayout();
	Inventory& inventory = player.getInventory();

	layout.build(rc.window.getSize(), true);
				
	auto hotbarStart = layout.slots.at(0);
	auto hotbarEnd = layout.slots.at(Inventory::HOTBAR - 1);

	sf::FloatRect rect;
	rect.position = hotbarStart.rect.position;
	rect.size = { hotbarEnd.rect.position.x - hotbarStart.rect.position.x + hotbarEnd.rect.size.x,   hotbarEnd.rect.size.y };

	if (rect.contains(mousePos))
		wasUIClicked = true;

	if (rc.insideInventory)
	{
		auto hotbarStart = layout.slots.at(Inventory::HOTBAR);
		auto hotbarEnd = layout.slots.at(Inventory::SIZE - 1);
		sf::FloatRect rect;
		rect.position = hotbarStart.rect.position;
		rect.size = { hotbarEnd.rect.position.x - hotbarStart.rect.position.x + hotbarEnd.rect.size.x, hotbarEnd.rect.position.y - hotbarStart.rect.position.y + hotbarEnd.rect.size.y};
		if (rect.contains(mousePos))
			wasUIClicked = true;
	}

	slotUI* hoveredSlot = nullptr;

	int c = 0;
	for (slotUI& slot : layout.slots)
	{
		if (!rc.insideInventory && c >= Inventory::HOTBAR) return;
		c++;
		if (slot.rect.contains(mousePos))
		{
			hoveredSlot = &slot;
			break;
		}
	}
	
	if(!wasUIClicked)
		wasUIClicked = (hoveredSlot != nullptr);
	
	if (!hoveredSlot || (!leftClicked && !rightClicked)) return;

	ItemStack& clicked = inventory.slots[hoveredSlot->inventoryIndex];
	auto &held = player.getItemOnHold();

	if (leftClicked)
	{
		if (clicked.item == held.item && held.item == Item::none)
		{
			if (hoveredSlot->inventoryIndex < Inventory::HOTBAR)
				player.setActiveSlot(hoveredSlot->inventoryIndex);
		}
		else if (held.item == Item::none)
			{
				held = clicked;
				clicked = ItemStack{};
			}
			else if (clicked.item == Item::none)
			{
				clicked = held;
				held = ItemStack{};
			}
			else if (clicked.item == held.item)
			{
				ui8 space = held.stackSize - clicked.count;
				ui8 move = std::min(space, held.count);
				clicked.count += move;
				held.count -= move;
				if (held.count == 0)
					held = ItemStack{};
			}
			else
			{
				ItemStack temp = clicked;
				clicked = held;
				held = temp;
			}
		}
		else
		{
			if (held.item == Item::none && clicked.count > 0)
			{
				ui8 takeAmount = (clicked.count + 1) / 2;
				held = clicked;
				held.count = takeAmount;
				clicked.count -= takeAmount;
				if (clicked.count == 0)
					clicked = ItemStack{};
			}
			else if (held.item != Item::none && (clicked.item == Item::none || clicked.item == held.item))
			{
				if (clicked.item == Item::none)
				{
					clicked.item = held.item;
					clicked.stackSize = held.stackSize;
				}
				if (clicked.count < clicked.stackSize)
				{
					clicked.count += 1;
					held.count -= 1;
					if (held.count == 0)
						held = ItemStack{};
				}
			}
		}
}


void handleEvents(RenderContext &rc, Player& player,
	sf::Clock& scrollPress, GameState& gs, MainGameState& mgs)
{
	if (rc.window.hasFocus())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
			gs.restart = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
			gs.close = true;
	}
	while (std::optional event = rc.window.pollEvent())
	{
		if (!rc.window.hasFocus()) continue;
		if (event->is<sf::Event::Closed>())
			rc.window.close();
		if (auto resized = event->getIf<sf::Event::Resized>())
		{
			float width = static_cast<float>(resized->size.x);
			float height = static_cast<float>(resized->size.y);

			sf::View& camera = player.getCamera();
			camera.setSize({ width * gSettings.Zoom, height * gSettings.Zoom});

			rc.settingsLayout.build(sf::Vector2u(resized->size), UILayout::Settings);
		}
		if (auto key = event->getIf<sf::Event::KeyPressed>())
		{
			if (key->code == sf::Keyboard::Key::V && mgs == MainGameState::Playing)
				gs.playerCamera = !gs.playerCamera;
			else if (key->code == sf::Keyboard::Key::C && gs.playerCamera && mgs == MainGameState::Playing)
			{
				player.setIsInCreative(!player.isInCreative());
				if (player.isInCreative())
					player.setVelocity({ player.getVelocity().x, 0.f });
			}
			else if (key->code == sf::Keyboard::Key::Escape)
			{
				if (mgs == MainGameState::Menu)
					mgs = MainGameState::Playing;
				else
					mgs = MainGameState::Menu;
			}
			else if (key->code == sf::Keyboard::Key::Tab)
			{
				rc.insideInventory = !rc.insideInventory;
			}
		}

		if (auto scroll = event->getIf<sf::Event::MouseWheelScrolled>())
		{
			if (scrollPress.getElapsedTime().asSeconds() < 0.05f) continue;
			scrollPress.restart();
			if (gs.playerCamera)
			{
				if (scroll->delta > 0)
					player.setActiveSlot(player.getActiveSlot() - 1);
				else
					player.setActiveSlot(player.getActiveSlot() + 1);
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
			wc.map[tileY][tileX] = Block::air;
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
			ItemStack stack = player.getInventory().slots[player.getActiveSlot()];
			int item = stack.item;
			if(item != Item::none)
			wc.map[tileY][tileX] = item;
		}
	}

	sf::Vector2f camPos = getCameraPos(player.getCamera());
	sf::Vector2f camSize = player.getCamera().getSize();
}