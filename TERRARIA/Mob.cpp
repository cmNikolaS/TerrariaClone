#include "Mob.hpp"
#include "Constants.hpp"
#include "Player.hpp"
#include "DayNightCycle.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <array>
#include "block.hpp"
#include <cmath>
#include "random.hpp"
#include "textures.hpp"

void Zombie::update(WorldContext& wc, const Player& player, const float dt)
{
	auto pp = player.getPos();
	auto lp = pos;

	float a = std::pow(pp.x - pos.x, 2);
	float b = std::pow(pp.y - pos.y, 2);
	float c = std::sqrt(a + b) / tileSize;

	std::random_device rd;
	std::mt19937 rng(rd());

	auto isSolid = [&wc](size_t x, size_t y)
		{
			return (wc.blocks[(size_t)wc.map[y][x]].solid);
		};
	auto checkCollision = [&](float x, float y) -> bool {
		if (x < 0.f || y < 0.f) return true;

		size_t startX = static_cast<size_t>(x / tileSize);
		size_t endX = static_cast<size_t>((x + size.x - 1.f) / tileSize);
		size_t startY = static_cast<size_t>(y / tileSize);
		size_t endY = static_cast<size_t>((y + size.y - 1.f) / tileSize);

		for (size_t tileY = startY; tileY <= endY; ++tileY)
		{
			for (size_t tileX = startX; tileX <= endX; ++tileX)
			{
				if (isSolid(tileX, tileY))
				{
					return true;
				}
			}
		}
		return false;
		};

	if (c < 15.f && player.isInCreative() == false)
	{
		currentState = state::followPlayer;
	}
	else if (currentState == state::followPlayer || currentState == state::None)
	{
		currentState = state::moveRandomly;
		randomPositionToGo = { pos.x + getRandomFloat(rng, -7.f, 7.f)*32.f, pos.y};
		randomPositionToGoTimer = getRandomFloat(rng, 3.f, 7.f);
	}

	switch (currentState)
	{
	case state::followPlayer:
	{
		bool pass = true;

		velocityY += gravity * dt;
		float newY = pos.y + velocityY * dt;

		if (checkCollision(pos.x, newY))
		{
			if (velocityY > 0)
			{
				onGround = true;
			}
			velocityY = 0.f;
		}
		else {
			onGround = false;
			pos.y = newY;
		}

		float dx = pp.x - pos.x;

		if (std::abs(dx) > 5.f)
		{
			float direction = 1.f;
			if (dx < 0) direction = -direction;
			velocityX = -direction;

			float newX = pos.x + moveSpeed * direction * dt;

			if (!checkCollision(newX, pos.y))
			{
				pos.x = newX;
			}
			else if (onGround)
			{
				velocityY = jumpStr;
				onGround = false;
			}
		}
		break;
	}
	case state::moveRandomly:
	{
		randomPositionToGoTimer -= dt;
		if (randomPositionToGoTimer <= 0.f)
		{
			currentState = state::standStill;
			standStillTimer = getRandomFloat(rng, 1.5f, 3.f);
			break;
		}

		bool pass = true;

		velocityY += gravity * dt;
		float newY = pos.y + velocityY * dt;

		if (checkCollision(pos.x, newY))
		{
			if (velocityY > 0)
			{
				onGround = true;
			}
			velocityY = 0.f;
		}
		else {
			onGround = false;
			pos.y = newY;
		}

		float dx = randomPositionToGo.x - pos.x;

		if (std::abs(dx) > 5.f)
		{
			float direction = 1.f;
			if (dx < 0) direction = -direction;
			velocityX = -direction;

			float newX = pos.x + moveSpeed * direction * dt;

			if (!checkCollision(newX, pos.y))
			{
				pos.x = newX;
			}
			else if (onGround)
			{
				velocityY = jumpStr;
				onGround = false;
			}
		}

		break;
	}
	case state::standStill:
	{
		standStillTimer -= dt;
		if (standStillTimer <= 0.f)
		{
			currentState = state::moveRandomly;
			randomPositionToGo = { pos.x + getRandomFloat(rng, -7.f, 7.f) * 32.f, pos.y };
			randomPositionToGoTimer = getRandomFloat(rng, 3.f, 7.f);
			break;
		}

		bool pass = true;

		velocityY += gravity * dt;
		float newY = pos.y + velocityY * dt;

		if (checkCollision(pos.x, newY))
		{
			if (velocityY > 0)
			{
				onGround = true;
			}
			velocityY = 0.f;
		}
		else {
			onGround = false;
			pos.y = newY;
		}
		break;
	}
	default:
		break;
	}

	updateDirection();
}

void Zombie::updateDirection()
{
	if (velocityX < 0)
		sprite.setTextureRect(getAtlasTextures(0, 0, 32, 64, false));
	else if (velocityX > 0)
		sprite.setTextureRect(getAtlasTextures(0, 0, 32, 64, true));
}

void Zombie::draw(sf::RenderWindow& window) const{

	sprite.setPosition(pos);
	window.draw(sprite);
}

Zombie::Zombie(sf::Vector2f p, const MobTextures& mobT) : Mob(p, ZombieSize), sprite(mobT[MobId::Zombie])
{
	sprite.setTextureRect(getAtlasTextures(0, 0, 32, 64));
	float idealX = ZombieSize.x / 32;
	float idealY = ZombieSize.y / 64;
	sprite.setScale({ idealX , idealY });
}

void updateMobs(std::vector<std::unique_ptr<Mob>>& mobs, WorldContext& wc, const Player& player, const MobTextures &mobTextures,  const float dt, const DayNightCycle &cycle, bool spawn)
{
	//if (cycle.isItDay())
	//{
	//	mobs.clear();
	//	return;
	//}
	for (const auto& m : mobs)
	{
		m->update(wc, player, dt);
	}
	if (!spawn) return;
	if (rand() % ZombieChance == 1)
	{
		float dist = WINDOW_W / 2.f;
		if (rand() % 2)
			dist = -dist;
		sf::Vector2f zp = { player.getPos().x + dist, worldH - maxWorldGenHeight - tileSize*2.f};
		if (zp.x / tileSize >= worldW - 1 || zp.x / tileSize < 1)return;
		while (zp.y+=tileSize)
		{
			if (zp.y / tileSize >= worldH) return;
			if (wc.map[(size_t)zp.y / tileSize + 1][(size_t)zp.x / tileSize] != Block::air) break;
		}
		zp.y-=tileSize*3.f;
		
		mobs.push_back(std::make_unique<Zombie>(zp, mobTextures));
	}
}
void drawMobs(RenderContext& rc, const std::vector<std::unique_ptr<Mob>>& mobs)
{
	for (const auto& m : mobs)
	{
		m->draw(rc.window);
	}
}