#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <array>
#include <SFML/Graphics.hpp>
#include "block.hpp"

extern float mobSpawnTimer;

constexpr float PI = 3.14159265f;
constexpr float DAY_LENGTH = 60.f;

constexpr sf::Vector2f ZombieSize = { 40.f, 84.f };

using ui8 = std::uint8_t;
using ui16 = std::uint16_t;

constexpr ui8 NUM_OF_SLOTS_ = 9;
constexpr float SLOT_SIZE_ = 40.f;
constexpr float SLOT_PADDING_ = 4.f;

constexpr ui8 sd = 3;

const std::string window_title = "Terraria";
inline ui16 WINDOW_W = 1536;
inline ui16 WINDOW_H = 800;

constexpr float tileSize = 32.f;
constexpr ui16 worldW = 4096 / sd;
constexpr ui16 worldH = 1024 / sd;

constexpr ui8 darknessLevel = 100;

constexpr float playerW = 40.f;
constexpr float playerH = 84.f;

constexpr ui16 wormChance = 120; // 1 / wormChance
constexpr ui16 ZombieChance = 3;

constexpr ui16 maxWorldGenHeight = 950 / sd;
constexpr ui16 averageWorldGenHeight = 590 / sd;
constexpr ui16 minimalWorldGenHeight = 500 / sd;
constexpr ui16 waterGenHeight = 550 / sd;
constexpr ui16 coldBiomHeight = 750 / sd;

constexpr float leftMouseClickCooldown = 0.15f;
constexpr float rightMouseClickCooldown = 0.1f;

enum class MainGameState : ui8
{
	Menu = 0,
	Playing,
	Settings
};

using MobTextures = std::vector<sf::Texture>;
using Map = std::vector<std::vector<int>>;

struct RenderContext
{
	sf::RenderWindow window;
	sf::Texture blocksAtlas;
	sf::Texture playerTexture;
	sf::Texture backgroundTexture;
	sf::Texture hearts;
	MobTextures mobTextures;
	sf::Font font;
	bool insideInventory = false;
	RenderContext(sf::VideoMode vm, const std::string& title) : window(vm, title)
	{
	}
};

struct WorldContext
{
	Map map = {};
	std::vector<Block> blocks = {};
};

struct GameState
{
	bool playerCamera = true, restart = true, close = false;
	float zoomLevel = 0.85f;
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

struct InventoryClickState
{
	bool leftWasDown = false;
	bool rightWasDown = false;
};
