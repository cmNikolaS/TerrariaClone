#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "constants.hpp"

class Widget
{
public:

	sf::FloatRect rect;
	std::string text = {};
	enum
	{
		none,
		play,
		leave,
		settings
	};
	ui8 action = none;

};


class UILayout
{
public:

	std::vector<Widget> widgets;

	void build(const sf::Vector2f windowsSize);

};