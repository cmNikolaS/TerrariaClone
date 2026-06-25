#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.hpp"

class DayNightCycle
{
private:
	const float dayLength = DAY_LENGTH;
	float dayTime = dayLength / 2.f;
	bool cyclePaused = false;
public:
	bool isItDay()const
	{
		return !(dayTime < dayLength / 2.f);
	}
	void pause()
	{
		cyclePaused = true;
	}
	void unpause()
	{
		cyclePaused = false;
	}
	void update(const float dt)
	{
		if (cyclePaused) return;
		dayTime = std::fmod(dayTime + dt, dayLength);
	}
	float getDayTime() const {
		return dayTime;
	}
	float getDayLength()const {
		return dayLength;
	}
};

inline sf::Color updateDarknessColor(sf::Color color, const DayNightCycle& cycle)
{
	ui8 alpha = static_cast<ui8>(((1.f + std::sin(cycle.getDayTime() / cycle.getDayLength() * PI * 2.f)) / 2.f)*230);
	color.a = alpha;
	return color;
}