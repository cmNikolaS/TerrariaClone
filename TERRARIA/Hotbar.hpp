#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "constants.hpp"
#include "block.hpp"

struct HotbarSlot
{
	int block = Block::air;
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
		slots[0] = { Block::grassBlock, 64 };
		slots[1] = { Block::dirt, 64 };
		slots[2] = { Block::stone, 64 };
		slots[3] = { Block::water, 64 };
		slots[4] = { Block::iron, 64 };
		slots[5] = { Block::gold, 64 };
		slots[6] = { Block::clay, 64 };
		slots[7] = { Block::rubyBlock, 64 };
		slots[8] = { Block::barrier, 64 };
	}

	int getSelectedBlock() const
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
	int getBlockAt(ui8 id)
	{
		if (id >= SLOTS) return Block::air;
		return slots[id].block;
	}
	void setSelected(ui8 sel)
	{
		if (sel >= SLOTS) return;
		selected = sel;
	}

};