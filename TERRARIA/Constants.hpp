#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <array>
#include <SFML/Graphics.hpp>

constexpr float PI = 3.14159265f;
constexpr float DAY_LENGTH = 600.f;

constexpr sf::Vector2f ZombieSize = { 16.f, 32.f };

using ui8 = std::uint8_t;
using ui16 = std::uint16_t;

constexpr ui8 NUM_OF_SLOTS_ = 9;
constexpr float SLOT_SIZE_ = 40.f;
constexpr float SLOT_PADDING_ = 4.f;

constexpr ui8 sd = 2;

const std::string window_title = "Terraria";
inline ui16 WINDOW_W = 1536;
inline ui16 WINDOW_H = 800;

constexpr float tileSize = 16.f;
constexpr ui16 worldW = 4096 / sd;
constexpr ui16 worldH = 1024 / sd;

constexpr ui8 darknessLevel = 100;

constexpr float playerW = 15.f;
constexpr float playerH = 31.f;

constexpr ui16 wormChance = 120; // 1 / wormChance
constexpr ui16 ZombieChance = 3;

constexpr ui16 maxWorldGenHeight = 740 / sd;
constexpr ui16 averageWorldGenHeight = 590 / sd;
constexpr ui16 minimalWorldGenHeight = 500 / sd;
constexpr ui16 waterGenHeight = 590 / sd;
constexpr ui16 coldBiomHeight = 700 / sd;

constexpr float leftMouseClickCooldown = 0.15f;
constexpr float rightMouseClickCooldown = 0.1f;

enum class MainGameState : ui8
{
	Menu = 0,
	Playing
};

enum class BlockId : ui8 {
	None = 0,
	Air,
	Grass,
	Dirt,
	Stone,
	Water,
	CoalOre,
	IronOre,
	GoldOre,
	DiamondOre,
	Barrier,
	SnowGrass,
	SnowDirt,
	Count
};

enum class MobId : ui8 {
	None = 0,
	Zombie,
	Count
};

struct Block {
	BlockId id = BlockId::None;
	bool solid = true;
	bool fluid = false;
};

using Blocks = std::array<Block, (size_t)BlockId::Count>;
using BlockTextures = std::array<sf::Texture, (size_t)BlockId::Count>;
using MobTextures = std::array<sf::Texture, (size_t)MobId::Count>;
using Map = std::vector<std::vector<BlockId>>;

struct RenderContext
{
	sf::RenderWindow window;
	BlockTextures blockTextures;
	MobTextures mobTextures;
	sf::Font font;
	RenderContext(sf::VideoMode vm, const std::string& title) : window(vm, title)
	{
	}
};

struct WorldContext
{
	Map map;
	Blocks blocks;
};

struct GameState
{
	bool playerCamera = true, creativeMode = false, restart = true, close = false;
};

struct GameClocks
{
	sf::Clock dtClock;
	sf::Clock leftClickPress;
	sf::Clock rightClickPress;
	sf::Clock scrollPress;
};

inline const sf::Vector2f getCameraPos(const sf::View& camera)
{
	return  sf::Vector2f({ camera.getCenter().x - camera.getSize().x / 2.f, camera.getCenter().y - camera.getSize().y / 2.f });
}