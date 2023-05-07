#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

#define DELETE_KEY 8
#define ENTER_KEY 13 
#define ESCAPE_KEY 27


class Textbox {
public:
	Textbox();

	Textbox(int size, sf::Color color, bool sel);

	void setFont(sf::Font& font);

	void setCharacterSize(int size);

	void setPosition(sf::Vector2f pos);

	void setLimit(bool ToF);

	void setLimit(bool ToF, int lim);

	void setSelected(bool sel);

	void reset();

	std::string getText();

	void drawTo(sf::RenderWindow& window);

	void typedOn(sf::Event input);

	void setOrigin(float x, float y);

	sf::FloatRect getLocalBounds();

	sf::FloatRect getGlobalBounds();

private:
	sf::Text textbox;
	std::ostringstream text;
	bool isSelected;
	bool hasLimit;
	int limit;

	void inputLogic(int charTyped);
	void deleteLastChar();


};