#include "Mob.hpp"
#include "Constants.hpp"
#include "Player.hpp"
#include "DayNightCycle.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <array>

void Zombie::update(WorldContext& wc, const Player& player, const float dt)
{
	auto pp = player.getPos();
	auto lp = pos;

	bool pass = true;

	auto isSolid = [&wc](size_t x, size_t y)
		{
			return (wc.blocks[(size_t)wc.map[y][x]].solid);
		};

	auto checkCollision = [&](float x, float y) -> bool {
		size_t left = static_cast<size_t>(x / tileSize);
		size_t right = static_cast<size_t>((x + size.x - 1) / tileSize);
		size_t down = static_cast<size_t>((y + size.y - 1) / tileSize);
		size_t top = static_cast<size_t>(y / tileSize);

		return  isSolid(left, top) ||
			isSolid(right, top) ||
			isSolid(left, down) ||
			isSolid(right, down);
		};

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
			//if (!checkCollision(newX, pos.y - tileSize))
			//{
				velocityY = jumpStr;
				onGround = false;
			//}
		}
	}
	updateDirection();

}

void Zombie::updateDirection()
{
	auto text = sprite.getTexture();
	auto s = text.getSize();
	if (velocityX > 0)
		sprite.setTextureRect({ {0, 0}, {(int)s.x, (int)s.y} });
	else if (velocityX < 0)
		sprite.setTextureRect({ {(int)s.x, 0}, {-(int)s.x, (int)s.y} });
}

void Zombie::draw(sf::RenderWindow& window) const{
	sprite.setPosition(pos);
	window.draw(sprite);
}

Zombie::Zombie(sf::Vector2f p, const MobTextures& mobT) : Mob(p, ZombieSize), sprite(mobT.at(static_cast<size_t>(MobId::Zombie)))
{
	const auto texSize = sprite.getTexture().getSize();
	sprite.setScale({ size.x / texSize.x, size.y / texSize.y });
}

void loadMobs(MobTextures& text)
{
	auto loadText = [&text](const MobId mi, const std::string& path)
		{
			assert(text[(size_t)mi].loadFromFile(path) && ("Failed to load texture: " + path).c_str());
		};

	loadText(MobId::Zombie, "RESOURCES/Textures/zombie.png");
}

void updateMobs(std::vector<std::unique_ptr<Mob>>& mobs, WorldContext& wc, const Player& player, const MobTextures &mobTextures,  const float dt, const DayNightCycle &cycle, bool spawn)
{
	if (cycle.isItDay())
	{
		mobs.clear();
		return;
	}
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
			if (wc.map[(size_t)zp.y / tileSize + 1][(size_t)zp.x / tileSize] != BlockId::Air) break;
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