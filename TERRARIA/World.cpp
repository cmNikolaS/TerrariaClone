#include "World.hpp"
#include "Constants.hpp"
#include <vector>
#include <array>
#include <cstdint>
#include <string>
#include <SFML/Graphics.hpp>

void loadBlocks(BlockTextures& text, Blocks& blocks)
{
	auto loadText = [&text](const BlockId bi, const std::string& path)
		{
			assert(text[(size_t)bi].loadFromFile(path) && ("Failed to load texture: " + path).c_str());
		};

	auto addBlock = [&blocks](const BlockId bi, const bool solid = true, const bool fluid = false)
		{
			blocks[(size_t)bi] = { bi, solid, fluid };
		};

	loadText(BlockId::Dirt, "RESOURCES/Textures/dirt.png");
	loadText(BlockId::Grass, "RESOURCES/Textures/grass.png");
	loadText(BlockId::Stone, "RESOURCES/Textures/stone.png");
	loadText(BlockId::Water, "RESOURCES/Textures/water.png");
	loadText(BlockId::DiamondOre, "RESOURCES/Textures/diamondOre.png");
	loadText(BlockId::CoalOre, "RESOURCES/Textures/coalOre.png");
	loadText(BlockId::GoldOre, "RESOURCES/Textures/goldOre.png");
	loadText(BlockId::IronOre, "RESOURCES/Textures/ironOre.png");
	loadText(BlockId::Barrier, "RESOURCES/Textures/barrier.png");
	loadText(BlockId::SnowGrass, "RESOURCES/Textures/snowGrass.png");
	loadText(BlockId::SnowDirt, "RESOURCES/Textures/snowDirt.png");

	addBlock(BlockId::Air, false, false);
	addBlock(BlockId::Grass, true, false);
	addBlock(BlockId::Dirt, true, false);
	addBlock(BlockId::Stone, true, false);
	addBlock(BlockId::Water, false, true);
	addBlock(BlockId::CoalOre, true, false);
	addBlock(BlockId::DiamondOre, true, false);
	addBlock(BlockId::IronOre, true, false);
	addBlock(BlockId::GoldOre, true, false);
	addBlock(BlockId::Barrier, true, false);
	addBlock(BlockId::SnowGrass, true, false);
	addBlock(BlockId::SnowDirt, true, false);
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
				map[y][x] = BlockId::Air;
			else if (y == surf)
				map[y][x] = BlockId::Grass;
			else if (y < surf + 3)
				map[y][x] = BlockId::Dirt;
			else
				map[y][x] = BlockId::Stone;
			map[y][0] = map[y][worldW - 1] = BlockId::Barrier;
		}
		map[0][x] = map[worldH - 1][x] = BlockId::Barrier;
	}
}
void generateSubsurface(Map& map, std::vector<sf::Vector2i>& worms)
{
	for (ui16 i = worldH - maxWorldGenHeight; i < worldH; i++)
	{
		for (ui16 j = 0; j < worldW; j++)
		{
			BlockId& b = map[i][j];

			if (b == BlockId::Air)
			{
				ui16 d = worldH - i;
				if (d >= minimalWorldGenHeight && d <= waterGenHeight)
				{
					b = BlockId::Water;
				}
			}
			else if (b == BlockId::Grass && i < (worldH - coldBiomHeight))
			{
				b = BlockId::SnowGrass;
			}
			else if (b == BlockId::Dirt && i < (worldH - coldBiomHeight))
			{
				b = BlockId::SnowDirt;
			}
			else if (b == BlockId::Stone)
			{
				if (rand() % wormChance == 0)
				{
					b = BlockId::Air;
					worms.push_back({ i, j });
					continue;
				}
				if (i < 1 || i >= worldH - 1 || j < 1 || j >= worldW - 1) continue;
				ui16 r = rand() % 10000;
				if (r < 9000) continue;
				if (r < 9400)
				{
					b = BlockId::CoalOre;
				}
				else if (r < 9700)
				{
					b = BlockId::IronOre;
				}
				else if (r < 9900)
				{
					b = BlockId::GoldOre;
				}
				else
				{
					b = BlockId::DiamondOre;
				}
				if (b == BlockId::Stone) continue;
				std::array<std::reference_wrapper<BlockId>, 8> sides{
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
					if (s == BlockId::Stone)
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
		if (np.x >= worldH - 2 || np.x < 0 || np.y < 0 || np.y >= worldW - 2 || map[np.x][np.y] != BlockId::Stone) return;
		map[np.x][np.y] = BlockId::Air;
		if (np.y < worldW - 2 && np.y - 1 > 0 && rand() % 2)
			map[np.x][np.y + 1] = BlockId::Air;
	}
}

void generateCaves(Map& map, const std::vector<sf::Vector2i>& worms)
{
	for (auto w : worms)
		doWorm(map, w);
}

Map generateMap()
{
	std::vector<std::vector<BlockId>> map(worldH, std::vector<BlockId>(worldW, BlockId::Air));
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

	constexpr int tilesW = (playerW + tileSize - 1) / tileSize;
	constexpr int tilesH = (playerH + tileSize - 1) / tileSize;

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