#pragma once
#include <cstdint>
#include <vector>

struct Block
{
	uint16_t type = 0;
	uint16_t variant = 0;
	bool solid = 0;
	bool fluid = 0;

	enum{
		air = 0,
		dirt,
		grassBlock,
		stone,
		grass,
		sand,
		sandRuby,
		sandStone,
		woodPlank,
		stoneBricks,
		clay,
		woodLog,
		leaves,
		copper,
		iron,
		gold,
		copperBlock,
		ironBlock,
		goldBlock,
		bricks,
		snow,
		ice,
		rubyBlock,
		platform,
		workBench,
		glass,
		furnace,
		painting,
		sappling,
		snowBlueRuby,
		blueRubyBlock,
		door,
		jar,
		table,
		wordrobe,
		bookShelf,
		snowBricks,
		iceTable,
		iceWordrobe,
		iceBookShelf,
		icePlatform,
		sandTable,
		sandWordrobe,
		sandBookShelf,
		sandPlatform,
		woodenChest,
		iceChest,
		sandChest,
		boneChest,
		boneBricks,
		boneBench,
		boneWordrobe,
		boneBookShelf,
		bonePlatform,
		water,

		BLOCKS_COUNT,
	};

	Block& setType(uint16_t type)
	{
		this->type = type;
		return *this;
	}

	Block& setVariant(uint16_t variant)
	{
		this->variant = variant;
		return *this;
	}

	Block& setSolid(bool solid)
	{
		this->solid = solid;
		return *this;
	}

	Block& setFluid(bool fluid)
	{
		this->fluid = fluid;
		return *this;
	}

};

void initBlocks(std::vector<Block> &blocks);