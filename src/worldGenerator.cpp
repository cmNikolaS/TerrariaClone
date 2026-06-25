#include "worldGenerator.hpp"
#include "constants.hpp"
#include <array>
#include <cstdint>
#include <string>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <cassert>
#include "block.hpp"
#include "random.hpp"
#include <random>
#include <memory>
#include "FastNoiseSIMD.h"

std::vector<ui16> generateHeightMap()
{
	std::random_device rd;
	std::mt19937 rng(rd());

	std::unique_ptr<FastNoiseSIMD> heightMapNoiseGen(FastNoiseSIMD::NewFastNoiseSIMD());
	heightMapNoiseGen->SetFrequency(0.0025f);
	heightMapNoiseGen->SetFractalOctaves(6);
	heightMapNoiseGen->SetFractalLacunarity(2.0f);
	heightMapNoiseGen->SetFractalGain(0.45f);
	heightMapNoiseGen->SetSeed(getRandomInt(rng, 1, 1000000));


	float* heightMapNoise = FastNoiseSIMD::GetEmptySet(worldW);

	heightMapNoiseGen->FillSimplexFractalSet(heightMapNoise, 0, 0, 0, worldW, 1, 1);


	std::vector<ui16> hm(worldW, averageWorldGenHeight);
	std::unique_ptr<FastNoiseSIMD> macroGen(FastNoiseSIMD::NewFastNoiseSIMD());
	macroGen->SetSeed(getRandomInt(rng, 1, 1000000));
	macroGen->SetFrequency(0.0008f);
	macroGen->SetFractalOctaves(2);
	float* macroNoise = FastNoiseSIMD::GetEmptySet(worldW);
	macroGen->FillPerlinFractalSet(macroNoise, 0, 0, 0, worldW, 1, 1);

	for (ui16 i = 1; i < worldW; i++)
	{
		int minWH = worldH - minimalWorldGenHeight;
		int maxWH = worldH - maxWorldGenHeight;

		float n = 0.76f * heightMapNoise[i] + 0.24f * macroNoise[i];
		hm[i] = minWH + n * (maxWH - minWH);
	}
	FastNoiseSIMD::FreeNoiseSet(macroNoise);
	FastNoiseSIMD::FreeNoiseSet(heightMapNoise);

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
			else if (y < surf + 14)
				map[y][x] = Block::dirt;
			else
				map[y][x] = Block::stone;
			map[y][0] = map[y][worldW - 1] = Block::barrier;
		}
		map[0][x] = map[worldH - 1][x] = Block::barrier;
	}
}
void generateSubsurface(Map& map, std::vector<sf::Vector2i>& worms)
{
	int minWH = worldH - minimalWorldGenHeight;
	int waterH = worldH - waterGenHeight;
	int coldBH = worldH - coldBiomHeight;
	std::random_device rd;
	std::mt19937 rng(rd());

	std::unique_ptr<FastNoiseSIMD> caveNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());
	caveNoiseGenerator->SetFrequency(0.05f);
	caveNoiseGenerator->SetFractalOctaves(0);
	caveNoiseGenerator->SetSeed(getRandomInt(rng, 1, 100000));
	float* caveNoise = FastNoiseSIMD::GetEmptySet(worldH*worldW);
	caveNoiseGenerator->FillPerlinFractalSet(caveNoise, 0, 0, 0, worldW, worldH, 1);
	
	
	for (ui16 i = 0; i < worldH; i++)
	{
		for (ui16 j = 0; j < worldW; j++)
		{
			int b = map[i][j];

			if (b == Block::air)
			{
				ui16 d = worldH - i;
				if (d <= waterH && waterH != minWH)
				{
					b = Block::water;
				}
			}
			else if (b == Block::grassBlock && i <= ( coldBH))
			{
				b = Block::snow;
			}
			else if (b == Block::dirt && i <= ( coldBH))
			{
				b = Block::snow;
			}
			else if (b == Block::stone && i <= ( coldBH))
			{
				b = Block::snow;
			}
			else if (b == Block::stone || b == Block::dirt || b == Block::grassBlock && getRandomChance(rng,0.5f))
			{
				if (std::abs(caveNoise[j * worldH + i]) > 0.25f)
					b = Block::air;
				if (getRandomChance(rng, 1.f / wormChance))
				{
					b = Block::air;
					worms.push_back({ i, j });
					continue;
				}
			}
			map[i][j] = b;
		}
	}

	FastNoiseSIMD::FreeNoiseSet(caveNoise);

}

void doWorm(Map& map, sf::Vector2i worm)
{
	std::random_device rd;
	std::mt19937 rng(rd());

	bool up = getRandomChance(rng, 0.5f);
	bool right = getRandomChance(rng, 0.5f);
	ui16 hor = getRandomInt(rng, 1, 70);
	ui16 ver = getRandomInt(rng, 1, 40);

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
			if (getRandomChance(rng, 0.5f))
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
		if (np.y < worldW - 2 && np.y - 1 > 0 && getRandomChance(rng, 1.f))
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