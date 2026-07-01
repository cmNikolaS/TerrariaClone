#include "ui.hpp"
#include "constants.hpp"
#include <SFML/Graphics.hpp>
using ui8 = std::uint8_t;

void UILayout::build(sf::Vector2u windowsSize, ui8 menu)
{
	widgets.clear();

	int windowW = windowsSize.x;
	int windowH = windowsSize.y;

	if (menu == UILayout::MainMenu)
	{
		int buttonNumber = 3;

		float buttonSizeY = (windowH / (buttonNumber + 2)) * 0.7;
		float buttonSizeX = std::min(windowH, windowW) * 0.6;
		float padding = buttonSizeY / 2.f;
		float topPadding = (windowH - buttonNumber * (padding + buttonSizeY)) / 1.33f;

		for (int i = 0; i < buttonNumber; i++)
		{
			auto btn = std::make_unique<Button>();

			btn->rect.size = { buttonSizeX, buttonSizeY };
			btn->rect.position = { (windowW - buttonSizeX) / 2.f, topPadding + (buttonSizeY + padding) * i };
		
			if (i == 0)
			{
				btn->text = "PLAY";
				btn->action = Widget::Action::play;
			}
			else if (i == 1)
			{
				btn->text = "SETTINGS";
				btn->action = Widget::Action::settings;
			}
			else if (i == 2)
			{
				btn->text = "LEAVE";
				btn->action = Widget::Action::leave;
			}
		
			widgets.push_back(std::move(btn));
		}
	}
	else if (menu == UILayout::Settings)
	{
		int sliderNumber = 3;

		sf::FloatRect sliderLine;
		float sliderLineX = windowW * 0.5f;
		float sliderLineY = sliderLineX * 0.07f;

		float heightFactor = sliderNumber + (sliderNumber - 1) * 1.5f;
		float maxSliderLineY = (windowH * 0.9f) / heightFactor;

		sliderLineY = std::min(sliderLineY, maxSliderLineY);
		sliderLine.size = { sliderLineX, sliderLineY };

		float gap = sliderLineY * 1.5f;
		float totalH = sliderNumber * sliderLineY + (sliderNumber - 1) * gap;
		float startY = (windowH - totalH) * 0.4f;

		sf::FloatRect slider;
		float sliderSize = (sliderLineY + gap) * 0.8f;
		slider.size = { sliderSize, sliderSize };

		for (int i = 0; i < sliderNumber; i++)
		{
			float sliderLinePosX = (windowW - sliderLineX) / 2.f;
			float sliderLinePosY = startY + i * (gap + sliderLineY);
			sliderLine.position={ sliderLinePosX, sliderLinePosY };

			auto sliderW = std::make_unique<Slider>();
			sliderW->rect = sliderLine;

			float fullness = (sliderLineX - sliderSize);

			if (i == 0)
			{
				sliderW->text = "SFX";
				sliderW->setting = Slider::SFX;
				fullness *= gSettings.SFX;
			}
			else if (i == 1)
			{
				sliderW->text = "Music";
				sliderW->setting = Slider::Music;
				fullness *= gSettings.Music;
			}
			else if (i == 2)
			{
				sliderW->text = "Zoom";
				sliderW->setting = Slider::Zoom;
				fullness *= gSettings.Zoom;
			}

			float sliderPosX = sliderLinePosX + fullness;
			float sliderPosY = sliderLinePosY - (sliderSize - sliderLineY) / 2.f;
			slider.position = { sliderPosX, sliderPosY };
			
			sliderW->knobRect = slider;



			widgets.push_back(std::move(sliderW));
		}
	}
	

}