#include "block.hpp"
#include <vector>
#include <cstdint>

void initBlocks(std::vector<Block>& blocks)
{
	blocks.resize(Block::BLOCKS_COUNT, {});
	
	for (int i = 0; i < Block::BLOCKS_COUNT; i++)
		blocks[i].setType(i);

	blocks[Block::grassBlock].setSolid(true);
	blocks[Block::dirt].setSolid(true);
	blocks[Block::stone].setSolid(true);
	blocks[Block::water].setFluid(true);
	blocks[Block::iron].setSolid(true);
	blocks[Block::gold].setSolid(true);
	blocks[Block::snow].setSolid(true);
	blocks[Block::barrier].setSolid(true);
}