#pragma once
#include "Constants.hpp"
#include "Player.hpp"
#include "Physics.hpp"
#include <SFML/Graphics.hpp>

void updatePlayer(Player& player, const WorldContext& wc, const float dt, bool creative)
{
	float velY = (player.getVelocity().y);
	player.setInWater(false);
	if (wc.blocks.at(static_cast<size_t>(wc.map[static_cast<size_t>(player.getPos().y / tileSize)][static_cast<size_t>(player.getPos().x / tileSize)])).fluid)
	{
		player.setInWater(true);
		if (player.getVelocity().y > 0) player.setJumped(false);
		if(velY > 0)
		velY *= 0.9f;
	}
	if (!creative)
	{
		velY += player.getGravity() * dt;
	}
	else
	{
		if (!player.getCreativeGoingDown())
		{
			if (velY > 0)
			{
				velY = 0;
			}
			velY /= 2;
		}
		else
		{
			if (velY < 0)
			{
				velY = 0;
			}
			velY /= 2;
		}

	}
	player.setVelocity({ player.getVelocity().x, velY });

	sf::Vector2f lastPos = player.getHitbox().position;

	player.move({ 0.f, player.getVelocity().y * dt });

	int left = static_cast<int>(player.getPos().x / tileSize);
	int top = static_cast<int>(player.getPos().y / tileSize);
	int right = static_cast<int>((player.getPos().x + playerW - 1) / tileSize);
	int bottom = static_cast<int>((player.getPos().y + playerH) / tileSize);
	if (bottom >= worldH || top < 0 || left < 0 || right >= worldW)
	{
		player.setPos(lastPos);
		return;
	}	
	bool cD = false, cU = false;
	for (ui16 x = left; x <= right; x++)
	{
		if (wc.blocks[(size_t)wc.map[bottom][x]].solid)
		{
			cD = true;
			break;
		}
		else if (wc.blocks[(size_t)wc.map[top][x]].solid)
		{
			cU = true;
			break;
		}
	}

	if (cD || cU)
	{
		player.setVelocity({ player.getVelocity().x, 0.f });
		player.setPos({ player.getPos().x, lastPos.y });
	}
	if (cD)
	{
		player.setJumped(false);
	}
	lastPos = player.getPos();

	player.move({ player.getVelocity().x * dt, 0.f });
	 left = static_cast<int>(player.getPos().x / tileSize);
	 top = static_cast<int>(player.getPos().y / tileSize);
	 right = static_cast<int>((player.getPos().x + playerW - 1) / tileSize);
	 bottom = static_cast<int>((player.getPos().y + playerH) / tileSize);
	if (bottom >= worldH || top < 0 || left < 0 || right >= worldW)
	{
		player.setPos(lastPos);
		return;
	}

	bool cR = false, cL = false;
	for (ui16 y = top; y <= bottom; y++)
	{
		if (wc.blocks[(size_t)wc.map[y][right]].solid)
		{
			cR = true;
			break;
		}
		else if (wc.blocks[(size_t)wc.map[y][left]].solid)
		{
			cL = true;
			break;
		}
	}

	if (cL || cR)
	{
		player.setVelocity({ 0.f, player.getVelocity().y });
		player.setPos({ lastPos.x, player.getPos().y });
	}
	player.updateDirection();
}