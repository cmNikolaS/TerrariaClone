#include "World.hpp"
#include "Constants.hpp"
#include <array>
#include <cstdint>
#include <string>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <cassert>
#include "block.hpp"


void loadBlocks(BlockTextures& text, Blocks &blocks)
{
	//auto loadText = [&text](const Block bi, const std::string& path)
	//	{
	//		assert(text[(size_t)bi].loadFromFile(path) && ("Failed to load texture: " + path).c_str());
	//	};


	

	assert(blocksAtlas.loadFromFile("RESOURCES/Textures/textures.png") && "Failed to load textures.png atlas");



	//loadText(Block::Dirt, "RESOURCES/Textures/dirt.png");
	//loadText(Block::Grass, "RESOURCES/Textures/grass.png");
	//loadText(Block::Stone, "RESOURCES/Textures/stone.png");
	//loadText(Block::Water, "RESOURCES/Textures/water.png");
	//loadText(Block::DiamondOre, "RESOURCES/Textures/diamondOre.png");
	//loadText(Block::CoalOre, "RESOURCES/Textures/coalOre.png");
	//loadText(Block::GoldOre, "RESOURCES/Textures/goldOre.png");
	//loadText(Block::IronOre, "RESOURCES/Textures/ironOre.png");
	//loadText(Block::Barrier, "RESOURCES/Textures/barrier.png");
	//loadText(Block::SnowGrass, "RESOURCES/Textures/snowGrass.png");
	//loadText(Block::SnowDirt, "RESOURCES/Textures/snowDirt.png");


	auto addBlock = [&blocks](const int bl, const bool solid = true, const bool fluid = false)
		{
			blocks.at(bl).type = bl;
			blocks.at(bl).solid = solid;
			blocks.at(bl).fluid = fluid;
		};


	addBlock(Block::air, false, false);
	addBlock(Block::grassBlock, true, false);
	addBlock(Block::dirt, true, false);
	addBlock(Block::stone, true, false);
	//addBlock(Block::Water, false, true);
	//addBlock(Block::CoalOre, true, false);
	//addBlock(Block::DiamondOre, true, false);
	addBlock(Block::iron, true, false);
	addBlock(Block::gold, true, false);
	//addBlock(Block::Barrier, true, false);
	addBlock(Block::snow, true, false);
	addBlock(Block::snow, true, false);
}



std::vector<ui16> generateHeightMap()
{
	std::vector<ui16> hm(worldW, averageWorldGenHeight);

	for (ui16 i = 1; i < worldW; i++)
	{
		ui16 r = rand() % 11;
		ui16 nh = hm[i - 1];

		if (r <= 3)
		{
		}
		else if (r <= 5)
		{
			nh += 1;
		}
		else if (r <= 6)
		{
			nh += 2;
		}
		else if (r <= 8)
		{
			nh -= 1;
		}
		else if (r <= 9)
		{
			nh -= 2;
		}
		else if (r <= 10)
		{
			if (rand() % 2) {
				nh += 3;
			}
			else nh -= 3;
		}

		if (nh > maxWorldGenHeight)nh = maxWorldGenHeight;
		else if (nh < minimalWorldGenHeight) nh = minimalWorldGenHeight;
		hm[i] = nh;
	}
	return hm;
}
void generateSurface(Map& map, const std::vector<ui16>& hm)
{
	for (int x = 0; x < worldW; x++)
	{
		ui16 surf = worldH - hm[x];
		for (ui16 y = 0; y < worldH; y++)
		{
			if (y < surf)
				map[y][x] = Block::air;
			else if (y == surf)
				map[y][x] = Block::grassBlock;
			else if (y < surf + 3)
				map[y][x] = Block::dirt;
			else
				map[y][x] = Block::stone;
			map[y][0] = map[y][worldW - 1] = Block::clay;
		}
		map[0][x] = map[worldH - 1][x] = Block::clay;
	}
}
void generateSubsurface(Map& map, std::vector<sf::Vector2i>& worms)
{
	for (ui16 i = worldH - maxWorldGenHeight; i < worldH; i++)
	{
		for (ui16 j = 0; j < worldW; j++)
		{
			int b = map[i][j];

			if (b == Block::air)
			{
				ui16 d = worldH - i;
				if (d >= minimalWorldGenHeight && d <= waterGenHeight)
				{
					b = Block::snowBricks;
				}
			}
			else if (b == Block::grassBlock && i < (worldH - coldBiomHeight))
			{
				b = Block::snow;
			}
			else if (b == Block::dirt && i < (worldH - coldBiomHeight))
			{
				b = Block::snow;
			}
			else if (b == Block::stone)
			{
				if (rand() % wormChance == 0)
				{
					b = Block::air;
					worms.push_back({ i, j });
					continue;
				}
				if (i < 1 || i >= worldH - 1 || j < 1 || j >= worldW - 1) continue;
				ui16 r = rand() % 10000;
				if (r < 9000) continue;
				if (r < 9400)
				{
					b = Block::air;
				}
				else if (r < 9700)
				{
					b = Block::iron;
				}
				else if (r < 9900)
				{
					b = Block::gold;
				}
				else
				{
					b = Block::rubyBlock;
				}
				if (b == Block::stone) continue;
				std::array<int, 8> sides{
					map[i - 1][j],
					map[i + 1][j],
					map[i][j - 1],
					map[i][j + 1],
					map[i - 1][j - 1],
					map[i - 1][j + 1],
					map[i + 1][j - 1],
					map[i + 1][j + 1]
				};

				ui16 chance = 60;

				for (auto& s : sides)
				{
					if (s == Block::stone)
					{
						if (rand() % 100 < chance)
						{
							s = b;
							chance -= 10;
						}
						else break;
					}
				}

			}
		}
	}
}

void doWorm(Map& map, sf::Vector2i worm)
{
	bool up = rand() % 2;
	bool right = rand() % 2;
	ui16 hor = rand() % 70 + 1;
	ui16 ver = rand() % 40 + 1;

	sf::Vector2i np = worm;

	while (ver > 0 || hor > 0)
	{
		if (ver <= 0)
		{
			if (right)
				np.y++;
			else
				np.y--;

			hor--;
		}
		else if (hor <= 0)
		{
			if (up)
				np.x--;
			else
				np.x++;

			ver--;
		}
		else
		{
			if (rand() % 2)
			{
				if (up)
					np.x--;
				else
					np.x++;

				ver--;
			}
			else
			{
				if (right)
					np.y++;
				else
					np.y--;

				hor--;
			}
		}
		if (np.x >= worldH - 2 || np.x < 0 || np.y < 0 || np.y >= worldW - 2 || map[np.x][np.y] != Block::stone) return;
		map[np.x][np.y] = Block::air;
		if (np.y < worldW - 2 && np.y - 1 > 0 && rand() % 2)
			map[np.x][np.y + 1] = Block::air;
	}
}

void generateCaves(Map& map, const std::vector<sf::Vector2i>& worms)
{
	for (auto w : worms)
		doWorm(map, w);
}

Map generateMap()
{
	Map map(worldH, std::vector<int>(worldW, Block::air));
	std::vector<sf::Vector2i> worms;
	generateSurface(map, generateHeightMap());
	generateSubsurface(map, worms);
	generateCaves(map, worms);
	return map;
}
const sf::Vector2f playerSpawnPos(const WorldContext& wc)
{
	auto isSolid = [&](int x, int y) { return wc.blocks[(size_t)wc.map[y][x]].solid; };
	auto isFluid = [&](int x, int y) { return wc.blocks[(size_t)wc.map[y][x]].fluid; };

	constexpr int tilesW = static_cast<int>((playerW + tileSize - 1) / tileSize);
	constexpr int tilesH = static_cast<int>((playerH + tileSize - 1) / tileSize);

	auto trySpawn = [&](int x) -> std::optional<sf::Vector2f>
		{
			if (x < 0 || x + tilesW > worldW) return std::nullopt;

			int surfaceY = -1;
			for (int y = 1; y < worldH; y++) {
				for (int dx = 0; dx < tilesW; dx++) {
					if (isSolid(x + dx, y)) { surfaceY = y; break; }
				}
				if (surfaceY >= 0) break;
			}
			if (surfaceY < tilesH) return std::nullopt; 

			for (int dx = 0; dx < tilesW; dx++) {
				if (!isSolid(x + dx, surfaceY)) return std::nullopt;
			}

			for (int dy = 1; dy <= tilesH; dy++) {
				for (int dx = 0; dx < tilesW; dx++) {
					if (isSolid(x + dx, surfaceY - dy) || isFluid(x + dx, surfaceY - dy))
						return std::nullopt;
				}
			}

			return sf::Vector2f{
				float(x * tileSize),
				float((surfaceY - tilesH) * tileSize)
			};
		};

	const int cx = worldW / 2;
	for (int offset = 0; offset < worldW / 2; offset++) {
		if (auto pos = trySpawn(cx + offset)) return *pos;
		if (offset > 0)
			if (auto pos = trySpawn(cx - offset)) return *pos;
	}
	return { float(cx * tileSize), 0.f };
}