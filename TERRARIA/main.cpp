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

int main()
{
	GameMusic gm;
	gm.loadAll();
	std::srand(static_cast<unsigned int>(time(0)));
	MainGameState mgs = MainGameState::Menu;
	GameState gs = {true, false, true, false};
	while (gs.restart)
	{
		gs.restart = false;
		//background and darkness (for night or contrast)
		sf::Texture backgroundText("RESOURCES/background.png");
		sf::Sprite background(backgroundText);
		background.setScale({ (float)WINDOW_W / backgroundText.getSize().x, (float)WINDOW_H / backgroundText.getSize().y });
		sf::RectangleShape darkness({ (float)WINDOW_W, (float)WINDOW_H });
		darkness.setFillColor(sf::Color({ 0, 0, 0, darknessLevel }));

		//RENDER CONTEXT && WORLD CONTEXT
		RenderContext rc(sf::VideoMode({ WINDOW_W, WINDOW_H }), window_title);
		assert(rc.font.openFromFile("RESOURCES/cursyger.ttf") && "Cant open Font");
		WorldContext wc;
		wc.map = generateMap();
		loadBlocks(rc.blockTextures, wc.blocks);
		loadMobs(rc.mobTextures);

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
					handlePlayerInput(rc.window, player, dt, gs.creativeMode);
					updatePlayer(player, wc, dt, gs.creativeMode);

					//View
					player.getCamera().setCenter(player.getPos());
					rc.window.setView(player.getCamera());
					background.setPosition(getCameraPos(player.getCamera()));
					darkness.setPosition(background.getPosition());
					updateDarkness(darkness, cycle);
					
					//Drawing
					rc.window.draw(background);
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
					drawScreen(rc, wc, mapView);
				}
			}
			rc.window.display();
		}
	}
	
	return 0;
}