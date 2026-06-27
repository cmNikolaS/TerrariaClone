#pragma once
#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "inventory.hpp"
#include <cmath>
#include "block.hpp"

class Player final
{
private:
	sf::Texture text;
	sf::Sprite sprite;
	const float gravity = 2200.f;
	sf::Vector2f velocity = { 0, 0 };
	float walkingSpeed = 940.f;
	const sf::Vector2f maxvel = { 940.f, 800.f };
	float jumpVel = 900;
	bool jumped = false;
	bool creativeGoingDown = false;
	InventoryLayout inventoryLayout;
	Inventory inventory;
	ItemStack itemOnHold;
	sf::View camera;
	bool inWater = false;
	bool CREATIVE = false;

public:
	bool isInCreative() const
	{
		return CREATIVE;
	}
	void setIsInCreative(bool creative)
	{
		CREATIVE = creative;
	}

	Player(sf::Texture &tex, sf::Vector2f pos, sf::Vector2f size) : text(tex), sprite(text)
	{
		itemOnHold = ItemStack(Item::none, 1);
		//sprite.setOrigin({
			//text.getSize().x / 2.f,
			//0.f
			//});
		sprite.setPosition(pos);
		sprite.setScale({ size.x / text.getSize().x, size.y / text.getSize().y });
	}
	void updateDirection()
	{
		auto s = text.getSize();
		if (velocity.x > 0)
			sprite.setTextureRect({ {0, 0}, {(int)s.x, (int)s.y} });
		else if (velocity.x < 0)
			sprite.setTextureRect({ {(int)s.x, 0}, {-(int)s.x, (int)s.y} });
	}
	bool getInWater()const
	{
		return inWater;
	}
	void setInWater(bool iw)
	{
		inWater = iw;
	}
	bool getCreativeGoingDown() const
	{
		return creativeGoingDown;
	}
	void setCreativeGoingDown(bool gd)
	{
		creativeGoingDown = gd;
	}
	void draw(sf::RenderWindow& window)const {
		window.draw(sprite);
	}
	float getSpeed() const
	{
		return walkingSpeed;
	}
	float getGravity()const
	{
		return gravity;
	}
	void move(sf::Vector2f pos)
	{
		sprite.move(pos);
	}
	sf::Vector2f getPos()const
	{
		return sprite.getPosition();
	}
	void setPos(sf::Vector2f pos)
	{
		sprite.setPosition(pos);
	}
	bool getJumped() const 
	{
		return jumped;
	}
	void setJumped(bool j) {
		jumped = j;
	}
	sf::FloatRect getHitbox() const
	{
		sf::FloatRect hb;
		//hb.position = { sprite.getPosition().x - playerW / 2.f, sprite.getPosition().y };
		hb.position = sprite.getPosition();
		hb.size = { (float)playerW, (float)playerH };
		return hb;
	}

	sf::Vector2f getVelocity() const {
		return velocity;
	}
	void setVelocity(sf::Vector2f vel) {
		
		if (vel.x > maxvel.x) vel.x = maxvel.x;
		else if (vel.x < -maxvel.x) vel.x = -maxvel.x;
		if (vel.y > maxvel.y) vel.y = maxvel.y;
		else if (vel.y < -maxvel.y) vel.y = -maxvel.y;
		velocity = vel;
	}
	float getJumpVel()const {
		return jumpVel;
	}
	void increaseVel(sf::Vector2f vel)
	{
		sf::Vector2f newVel = vel;
		setVelocity({ newVel.x + velocity.x, newVel.y + velocity.y });
	}
	InventoryLayout& getInventoryLayout() {
		return inventoryLayout;
	}
	Inventory& getInventory() {
		return inventory;
	}
	ItemStack& getItemOnHold()
 	{
		return itemOnHold;
	}
	sf::View& getCamera() {
		return camera;
	}

};