#include "block.hpp"
#include <vector>
#include <cstdint>

void initBlocks(std::vector<Block>& blocks)
{
	blocks.resize(Block::BLOCKS_COUNT, {});
	blocks[Block::grassBlock].setType(Block::grassBlock).setSolid(true);
	blocks[Block::dirt].setType(Block::dirt).setSolid(true);
	blocks[Block::stone].setType(Block::stone).setSolid(true);
	blocks[Block::water].setType(Block::water).setFluid(true);
	blocks[Block::iron].setType(Block::iron).setSolid(true);
	blocks[Block::gold].setType(Block::gold).setSolid(true);
	blocks[Block::snow].setType(Block::snow).setSolid(true);
}