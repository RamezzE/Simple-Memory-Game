#pragma once

#ifndef GAME_STATE_START_HPP
#define GAME_STATE_START_HPP
#define MAX_NUMBER_OF_ITEMS 3

#include <SFML/Graphics.hpp>
#include "game.h"
#include "gameState.h"



class Menu : public GameState {
public:
	Menu(Game* mygame);
	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	void MoveUp();
	void MoveDown();
	bool isMouseOver(sf::RenderWindow& window, sf::RectangleShape thing);

private:
	static int selectedItemIndex;
	sf::Text menuList[MAX_NUMBER_OF_ITEMS];
	sf::RectangleShape menuRect[MAX_NUMBER_OF_ITEMS];
	sf::RectangleShape backButton;
	void loadgame();
	void toLogin();
	void toReport();
};

#endif