#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
//#include "constants.hpp"
#include <cstdint>

using ui8 = std::uint8_t;

class Widget
{
public:

	sf::FloatRect rect;
	std::string text = {};
	enum widgetType
	{
		button,
		slider,
		label
	};
	enum Action
	{
		none,
		play,
		leave,
		settings
	}action = none;
	
	virtual widgetType getType() const = 0;
	virtual ~Widget() = default;
};

class Button : public Widget
{
public:
	widgetType getType() const override { return widgetType::button; }

};

class Slider : public Widget
{
public:
	widgetType getType() const override { return widgetType::slider; }
	enum Setting
	{
		None,
		Music,
		SFX,
		Zoom
	} setting = Setting::None;
	bool dragging = false;
	sf::FloatRect knobRect;
	float value = 0.5f;
	float minVal = 0.f;
	float maxVal = 1.f;
};

class UILayout
{
public:
	enum {
		MainMenu,
		Settings
	};

	std::vector<std::unique_ptr<Widget>> widgets;

	void build(const sf::Vector2u windowsSize, ui8 menu);

};