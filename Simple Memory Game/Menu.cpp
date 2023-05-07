

#define MAX_NUMBER_OF_ITEMS 3

#include "SFML/Graphics.hpp"

#include <iostream>

#include <vector>

#include "Menu.h"
#include "gameState.h"
#include "loginScreen.h"
#include "reportPage.h"
#include "playState.h"
#include "game.h"


extern unsigned width; extern unsigned height;
int pressedBox = -1;


Menu::Menu(Game* mygame) {
	this->mygame = mygame;
}

int Menu::selectedItemIndex = -1;

bool Menu::isMouseOver(sf::RenderWindow& window, sf::RectangleShape thing) {
	return thing.getGlobalBounds().contains(sf::Mouse::getPosition(mygame->window).x, sf::Mouse::getPosition(mygame->window).y);
}


void Menu::MoveUp() {
	if (selectedItemIndex > 0) {
		selectedItemIndex--;
	}
}

void Menu::MoveDown() {
	if (selectedItemIndex < MAX_NUMBER_OF_ITEMS) {
		selectedItemIndex++;
	}
}

void Menu::draw(const float dt) {

	sf::Font menufont;
	menufont.loadFromFile("fonts\\Gronzy.ttf");

	menuList[0].setString("Play");

	menuList[1].setString("Report");

	menuList[2].setString("Exit");

	//setting background image
	sf::Texture backpic;
	backpic.loadFromFile("pics\\sponge and patrick2.jpg");
	sf::RectangleShape background;
	background.setSize(sf::Vector2f(width, height));
	background.setTexture(&backpic);


	sf::FloatRect temp;

	//setting up backtoLogin button
	backButton.setSize(sf::Vector2f(70, 50));
	sf::Texture backArrow;
	backArrow.loadFromFile("pics\\back arrow.png");
	backButton.setTexture(&backArrow);

	temp = backButton.getLocalBounds();
	backButton.setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f); // making origin in center of object

	//drawing 
	mygame->window.draw(background);
	mygame->window.draw(backButton);

	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++) {

		//making text origin in centre 
		sf::FloatRect textRect = menuList[i].getLocalBounds();
		menuList[i].setOrigin(textRect.left + textRect.width / 2.0f,
			textRect.top + textRect.height / 2.0f);

		// making rectangle origin in centre
		temp = menuRect[i].getLocalBounds();
		menuRect[i].setOrigin(temp.left + temp.width / 2.0f,
			temp.top + temp.height / 2.0f);

		//setting font
		menuList[i].setFont(menufont);

		//setting character size
		menuList[i].setCharacterSize(64);

		//setting position in middle of the screen
		menuList[i].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * (i + 1)));
		menuRect[i].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * (i + 1)));


		menuRect[i].setFillColor(sf::Color(0, 0, 0, 180)); //setting rectangle color 
		sf::FloatRect maxsize = menuList[1].getLocalBounds();
		menuRect[i].setSize(sf::Vector2f(maxsize.width * 1.2, maxsize.height * 2)); //setting size of rect

		menuRect[i].setOutlineColor(sf::Color::White);
		menuRect[i].setOutlineThickness(2);

		//drawing the rest
		mygame->window.draw(menuRect[i]); //drawing rectangle around text
		mygame->window.draw(menuList[i]); //drawing text
	}
}

void Menu::handleInput() {
	sf::Event event;
	while (mygame->window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			mygame->window.close();
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case  sf::Keyboard::Escape: //close window if esc is pressed
				mygame->window.close();
				break;
			case sf::Keyboard::BackSpace:
				//toLogin();
				break;
			case sf::Keyboard::Return:
				//choose up apporopriate pressedBox
				pressedBox = selectedItemIndex;
				break;
			case sf::Keyboard::Up:
				MoveUp();
				break;
			case sf::Keyboard::Down:
				MoveDown();
				break;
			}
			break;
		case sf::Event::MouseButtonPressed:
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { //if left button is pressed
				if (isMouseOver(mygame->window, menuRect[0])) {
					pressedBox = 0;
				}
				else if (isMouseOver(mygame->window, menuRect[1])) {
					pressedBox = 1;
				}
				else if (isMouseOver(mygame->window, menuRect[2])) {
					pressedBox = 2;
				}
				else if (isMouseOver(mygame->window, backButton)) {
					toLogin();
				}
				break;
			}
		}
	}

	//constantly monitors if mouse is hovering over any of these
	if (isMouseOver(mygame->window, menuRect[0])) {
		selectedItemIndex = 0;
	}
	if (isMouseOver(mygame->window, menuRect[1])) {
		selectedItemIndex = 1;
	}
	if (isMouseOver(mygame->window, menuRect[2])) {
		selectedItemIndex = 2;
	}
	if (isMouseOver(mygame->window, backButton)) {
		backButton.setScale(1.5, 1.5);
		backButton.setPosition(70, 50);
	}
	else {
		backButton.setScale(1, 1);
		backButton.setPosition(60, 30);
	}
}


void Menu::update(const float dt) {

	//colors menu item if mouse is hovering over
	switch (selectedItemIndex) {
	case 0:
		menuList[0].setFillColor(sf::Color(102, 153, 255, 255));
		menuList[1].setFillColor(sf::Color::White);
		menuList[2].setFillColor(sf::Color::White);
		break;
	case 1:
		menuList[0].setFillColor(sf::Color::White);
		menuList[1].setFillColor(sf::Color(102, 153, 255, 255));
		menuList[2].setFillColor(sf::Color::White);
		break;
	case 2:
		menuList[0].setFillColor(sf::Color::White);
		menuList[1].setFillColor(sf::Color::White);
		menuList[2].setFillColor(sf::Color(102, 153, 255, 255));
		break;
	}

	//performs appropriate action when user chooses a menu option
	switch (pressedBox) {
	case 0:
		loadgame();
		break;
	case 1:
		toReport();
		break;
	case 2:
		mygame->window.close();
		break;
	}

}

void Menu::toLogin() {
	//to login screen
	pressedBox = -1;
	mygame->pushState(new loginScreen(mygame));

}

void Menu::loadgame() {
	//load the playgame state
	pressedBox = -1;
	mygame->pushState(new PlayState(mygame));
}

void Menu::toReport() {
	pressedBox = -1;
	mygame->pushState(new reportPage(mygame));
}
