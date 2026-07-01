#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include "constants.hpp"

struct GameMusic
{
	sf::Music day;
	sf::Music night;
	sf::Music titleScreen;

	enum class Track { None, Day, Night, Title };
	Track current = Track::None;

	void updateVolume()
	{
		day.setVolume(50.f * gSettings.Music);
		night.setVolume(50.f * gSettings.Music);
		titleScreen.setVolume(50.f * gSettings.Music);
	}

	void loadAll()
	{
		auto load = [](sf::Music& m, const std::string& path) {
			assert(m.openFromFile(path));
			m.setLooping(true);
			m.setVolume(50.f * gSettings.Music);
			};
		load(day, "RESOURCES/MUSIC/OverworldDay.mp3");
		load(night, "RESOURCES/MUSIC/OverworldNight.mp3");
		load(titleScreen, "RESOURCES/MUSIC/TitleScreen.mp3");
	}

	void playNow(Track tr)
	{
		if (tr == current) return;
		switch (current)
		{
		case GameMusic::Track::Day: day.stop();
			break;
		case GameMusic::Track::Night: night.stop();
			break;
		case GameMusic::Track::Title: titleScreen.stop();
			break;
		default:
			break;
		}
		current = tr;
		switch (tr)
		{
		case GameMusic::Track::Day: day.play();
			break;
		case GameMusic::Track::Night: night.play();
			break;
		case GameMusic::Track::Title: titleScreen.play();
			;			break;
		default:
			break;
		}
	}

};