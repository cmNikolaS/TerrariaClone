#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include "constants.hpp"
#include "block.hpp"
#include "items.hpp"

struct ItemStack
{
	ui8 item = Item::none;
	ui8 stackSize = 64;
	ui8 count = 0;
	ItemStack()
	{

	}
	ItemStack(ui8 id, ui8 count, ui8 stackSize = 64)
	{
		item = id;
		this->count = count;
		this->stackSize = stackSize;
	}
};

class Inventory
{
private:

public:
	static constexpr ui8 HOTBAR = 9;
	static constexpr ui8 ROWS = 3;
	static constexpr ui8 COLUMNS = 9;
	static constexpr ui8 SIZE = HOTBAR + ROWS * COLUMNS;
	std::array<ItemStack, SIZE> slots;
	int selectedHotbar = 0;
};

struct slotUI
{
	sf::FloatRect rect;
	int inventoryIndex = 0;
	bool isHotbar = false;
	int index = 0;
};

class InventoryLayout
{
public:
	std::vector<slotUI> slots;

	float slotSize = 48.f;
	float padding = 6.f;

	void build(const sf::Vector2u& windowsSize, bool open);
};