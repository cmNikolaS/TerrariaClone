#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "Constants.hpp"

struct HotbarSlot
{
	BlockId block = BlockId::Air;
	ui8 count = 0;
};

class Hotbar {

private:
	ui8 selected = 0;
public:
	static constexpr ui8 SLOTS = NUM_OF_SLOTS_;
	static constexpr float SLOT_SIZE = SLOT_SIZE_;
	static constexpr float PADDING = SLOT_PADDING_;
	std::array<HotbarSlot, SLOTS> slots;


	Hotbar() {
		slots[0] = { BlockId::Grass, 64 };
		slots[1] = { BlockId::Dirt, 64 };
		slots[2] = { BlockId::Stone, 64 };
		slots[3] = { BlockId::Water, 64 };
		slots[4] = { BlockId::IronOre, 64 };
		slots[5] = { BlockId::GoldOre, 64 };
		slots[6] = { BlockId::CoalOre, 64 };
		slots[7] = { BlockId::DiamondOre, 64 };
		slots[8] = { BlockId::Barrier, 64 };
	}

	BlockId getSelectedBlock() const
	{
		return slots[selected].block;
	}
	ui8 getSelected()const
	{
		return selected;
	}
	void scrollUp()
	{
		selected++;
		if (selected >= SLOTS) selected = 0;
	}
	void scrollDown()
	{
		if (selected == 0) selected = SLOTS - 1;
		else selected--;
	}
	BlockId getBlockAt(ui8 id)
	{
		if (id >= SLOTS) return BlockId::None;
		return slots[id].block;
	}
	void setSelected(ui8 sel)
	{
		if (sel >= SLOTS) return;
		selected = sel;
	}

};