#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include "Constants.hpp"

struct GameMusic
{
	sf::Music day;
	sf::Music night;
	sf::Music titleScreen;

	enum class Track { None, Day, Night, Title };
	Track current = Track::None;

	void loadAll()
	{
		auto load = [](sf::Music& m, const std::string& path) {
			assert(m.openFromFile(path));
			m.setLooping(true);
			m.setVolume(25.f);
			};
		load(day, "RESOURCES/MUSIC/Overworld Day.mp3");
		load(night, "RESOURCES/MUSIC/Overworld Night.mp3");
		load(titleScreen, "RESOURCES/MUSIC/Title Screen.mp3");
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