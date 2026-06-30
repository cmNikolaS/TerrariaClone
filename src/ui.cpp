#include "ui.hpp"
#include "constants.hpp"
#include <SFML/Graphics.hpp>

void UILayout::build(sf::Vector2f windowsSize)
{
	widgets.clear();

	int windowW = windowsSize.x;
	int windowH = windowsSize.y;

	int buttonNumber = 3;

	float buttonSizeY = (windowH / (buttonNumber + 2)) * 0.7;
	float buttonSizeX = std::min(windowH, windowW) * 0.6;
	float padding = buttonSizeY / 2.f;
	float topPadding = (windowH - buttonNumber * (padding + buttonSizeY)) / 1.33f;

	Widget widget;
	widget.rect.size = { buttonSizeX, buttonSizeY };
	for (int i = 0; i < buttonNumber; i++)
	{
		widget.rect.position = { (windowW - buttonSizeX) / 2.f, topPadding + (buttonSizeY + padding) * i };
		
		if (i == 0)
		{
			widget.text = "PLAY";
			widget.action = widget.play;
		}
		else if (i == 1)
		{
			widget.text = "SETTINGS";
			widget.action = widget.settings;
		}
		else if (i == 2)
		{
			widget.text = "LEAVE";
			widget.action = widget.leave;
		}
		
		widgets.push_back(widget);
	}

}