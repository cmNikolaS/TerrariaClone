#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <array>
#include "Hotbar.hpp"
#include "Player.hpp"
#include "Constants.hpp"
#include "World.hpp"
#include "Rendering.hpp"
#include "Physics.hpp"
#include "Input.hpp"
#include "Mob.hpp"
#include "GameAudio.hpp"
#include "DayNightCycle.hpp"
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

int main()
{
	Background background;
	GameMusic gm;
	WorldContext wc;
	sf::Texture blocksAtlas;
	initBlocks(wc.blocks);
	gm.loadAll();
	std::srand(static_cast<unsigned int>(time(0)));
	MainGameState mgs = MainGameState::Menu;
	GameState gs = {true, false, true, false};
	while (gs.restart)
	{
		gs.restart = false;
		wc.map = generateMap();

		//background and darkness (for night or contrast)
		
		RenderContext rc(sf::VideoMode({ WINDOW_W, WINDOW_H }), window_title);

		initTextures(rc);
		
		sf::RectangleShape darkness({ (float)worldW * 32, (float)worldH * 32 });
		darkness.setFillColor(sf::Color({ 19, 24, 98, darknessLevel }));

		//loading player
		sf::Texture pt;
		assert(pt.loadFromFile("RESOURCES/Textures/player.png") && "Failed to load player.png");
		Player player(pt, { playerSpawnPos(wc) }, { playerW, playerH });

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

		float timer = 0.f;

		while (rc.window.isOpen())
		{
			float dt = gc.dtClock.restart().asSeconds();
			handleEvents(rc.window, player, gc.scrollPress, gs, mgs);
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
					bool spawn = false;
					cycle.unpause();
					timer += dt;
					if (timer > 1.f)
					{
						timer = 0.f;
						spawn = true;
					}
					//Handle input
					handleMouseClicks(rc, wc, player, gc);
					updateMobs(mobs, wc, player, rc.mobTextures, dt, cycle, spawn);
					player.setVelocity({ 0.f, player.getVelocity().y });
					handlePlayerInput(rc.window, player, dt);
					updatePlayer(player, wc, dt);
					
					updateCamera(player.getCamera(), player.getPos());

					//View
					rc.window.setView(player.getCamera());
					updateDarkness(darkness, cycle);
					
					//Drawing
					background.draw(dt, rc.window, player.getCamera(), rc.backgroundTexture, player.getPos());
					drawScreen(rc, wc, player.getCamera());
					rc.window.draw(darkness);
					drawMobs(rc, mobs);
					player.draw(rc.window);
					drawGameUI(rc, player);
				}
				else
				{
					cycle.pause();
					rc.window.setView(mapView);
					background.draw(0.f, rc.window, mapView, rc.backgroundTexture, player.getPos());
					drawScreen(rc, wc, mapView);
				}
			}
			rc.window.display();
		}
	}
	
	return 0;
}