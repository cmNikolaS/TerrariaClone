#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <array>
#include "inventory.hpp"
#include "player.hpp"
#include "constants.hpp"
#include "worldGenerator.hpp"
#include "rendering.hpp"
#include "physics.hpp"
#include "input.hpp"
#include "mob.hpp"
#include "gameAudio.hpp"
#include "dayNightCycle.hpp"
#include <cmath>
#include <memory>
#include "block.hpp"
#include "textures.hpp"
#include "permaAssert.hpp"
#include "background.hpp"

void updateCamera(sf::View& camera, sf::Vector2f playerPosition)
{
	sf::Vector2f cameraCenter = playerPosition;
	sf::Vector2f cameraSize = camera.getSize();
	
	float worldWidthPixels = worldW * tileSize;
	float worldHeightPixels = worldH * tileSize;

	float minX = cameraSize.x / 2.f + tileSize;
	float maxX = worldWidthPixels - minX;

	float minY = cameraSize.y / 2.f + tileSize;
	float maxY = worldHeightPixels - minY;

	cameraCenter.x = std::clamp(cameraCenter.x, minX, maxX);
	cameraCenter.y = std::clamp(cameraCenter.y, minY, maxY);
	camera.setCenter(cameraCenter);
}

sf::Color darknessColor = { 7, 2, 20, darknessLevel };

int main()
{
	InventoryClickState clickState;
	RenderContext rc(sf::VideoMode({ WINDOW_W, WINDOW_H }), window_title);
	initTextures(rc);
	WorldContext wc;
	initBlocks(wc.blocks);
	Background background;
	GameMusic gm;
	gm.loadAll();
	MainGameState mgs = MainGameState::Menu;
	GameState gs;
	while (gs.restart)
	{
		gs.restart = false;
		wc.map = generateMap();		

		sf::RectangleShape darkness({ (float)worldW * 32, (float)worldH * 32 });
		darkness.setFillColor(darknessColor);

		//loading player
		Player player(rc.playerTexture, { playerSpawnPos(wc) }, { playerW, playerH });

		auto &inv = player.getInventory();

		inv.slots[0] = ItemStack(Item::dirt, 64);
		inv.slots[1] = ItemStack(Item::grassBlock, 64);
		inv.slots[2] = ItemStack(Item::stone, 64);


		//CAMERA
		sf::View playerCamera(sf::FloatRect({ 0.f, 0.f }, { (float)WINDOW_W, (float)WINDOW_H }));
		playerCamera.zoom(0.85f);
		player.getCamera() = playerCamera;
		
		sf::View mapView;
		mapView.setCenter({ worldW * tileSize / 2.f, worldH * tileSize / 2.f });
		mapView.setSize({ worldW * tileSize, worldH * tileSize });

		//clock and framerate limit
		GameClocks gc;
		rc.window.setFramerateLimit(144);

		DayNightCycle cycle;

		std::vector<std::unique_ptr<Mob>> mobs;
		mobs.resize(100);

		while (rc.window.isOpen())
		{
			float dt = gc.dtClock.restart().asSeconds();
			handleEvents(rc, player, gc.scrollPress, gs, mgs);
			if (gs.restart) break;
			if (gs.close) rc.window.close();

			if (dt > 1.f / 20.f)
				dt = 1.f / 20.f;

			rc.window.clear();

			if (mgs == MainGameState::Menu)
			{
				gm.playNow(GameMusic::Track::Title);
				drawMenu(rc);
			}
			else if (mgs == MainGameState::Playing)
			{
				gm.playNow((cycle.isItDay()) ? GameMusic::Track::Day : GameMusic::Track::Night);
				cycle.update(dt);
				if (gs.playerCamera)
				{
					//Cycle
					cycle.unpause();
					//Handle input
					bool wasUIClicked = false;
					handleInventoryInput(rc, player, clickState, wasUIClicked);
					if(!wasUIClicked)
					handleMouseClicks(rc, wc, player, gc);
					updateMobs(mobs, wc, player, rc.mobTextures, dt, cycle);
					player.setVelocity({ 0.f, player.getVelocity().y });
					handlePlayerInput(rc.window, player, dt);
					updatePlayer(player, wc, dt);
					updateCamera(player.getCamera(), player.getPos());

					//View
					rc.window.setView(player.getCamera());
					darkness.setFillColor(updateDarknessColor(darknessColor, cycle));


					//Drawing
					background.draw(dt, rc.window, player.getCamera(), rc.backgroundTexture, player.getPos());
					drawScreen(rc, wc, player.getCamera());
					drawMobs(rc, mobs);
					player.draw(rc.window);
					rc.window.draw(darkness);
					drawGameUI(rc, player);
				}
				else
				{
					cycle.pause();
					rc.window.setView(mapView);
					//background.draw(0.f, rc.window, mapView, rc.backgroundTexture, player.getPos());
					drawScreen(rc, wc, mapView);
				}
			}
			rc.window.display();
		}
	}
	
	return 0;
}