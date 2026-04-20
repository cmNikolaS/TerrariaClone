#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.hpp"
#include "Player.hpp"
#include "DayNightCycle.hpp"
#include <string>
#include <memory>
#include <array>

class Mob {
protected:
	sf::Vector2f pos;
	sf::Vector2f size;

public:
	Mob(sf::Vector2f p, sf::Vector2f s) : pos(p), size(s) {}
	sf::Vector2f getSize()const {
		return size;
	}
	sf::Vector2f getPos()const {
		return pos;
	}
	virtual ~Mob() = default;
	virtual MobId getId() const = 0;
	virtual void update(WorldContext& wc, const Player& player, const float dt) = 0;
	virtual void draw(sf::RenderWindow& window) const = 0;

};
class Zombie : public Mob
{
private:
	mutable sf::Sprite sprite;
	float moveSpeed = 50.f;
	float velocityY = 0.f;
	float gravity = 800.f;
	float jumpStr = -300.f;
	bool onGround = false;
	float velocityX = 0.f;

public:
	Zombie(sf::Vector2f p, const MobTextures& mobT);
	MobId getId() const override {
		return MobId::Zombie;
	}
	void draw(sf::RenderWindow& window) const override;
	void update(WorldContext& wc, const Player& player, const float dt) override;
	void updateDirection();
};

void drawMobs(RenderContext& rc, const std::vector<std::unique_ptr<Mob>>& mobs);
void updateMobs(std::vector<std::unique_ptr<Mob>>& mobs, WorldContext& wc, const Player& player, const MobTextures &mobTextures ,const float dt, const DayNightCycle& cycle, bool spawn);
void loadMobs(MobTextures& text);