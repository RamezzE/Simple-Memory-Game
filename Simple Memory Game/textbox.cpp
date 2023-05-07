#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

#include "textbox.h"


#define DELETE_KEY 8
#define ENTER_KEY 13 
#define ESCAPE_KEY 27


Textbox::Textbox() {

}

Textbox::Textbox(int size, sf::Color color, bool sel) {
	textbox.setCharacterSize(size);
	textbox.setFillColor(color);
	isSelected = sel;
	if (sel) {
		textbox.setString("_");
	}
	else {
		textbox.setString("");
	}
}

void Textbox::setCharacterSize(int size) {
	textbox.setCharacterSize(size);
}

void Textbox::setFont(sf::Font& font) {
	textbox.setFont(font);
}
void Textbox::setPosition(sf::Vector2f pos) {
	textbox.setPosition(pos);
}
void Textbox::setLimit(bool ToF) {
	hasLimit = ToF;
}
void Textbox::setLimit(bool ToF, int lim) {
	hasLimit = ToF;
	limit = lim;
}

void Textbox::setSelected(bool sel) {
	isSelected = sel;
	if (!sel) {
		std::string t = text.str();
		std::string newT = "";
		for (int i = 0; i < t.length() - 1; i++) {
			newT += t[i];
		}
		textbox.setString(newT);
	}
}

void Textbox::reset() {
	text.str("");
	textbox.setString("");
}

void Textbox::setOrigin(float x, float y) {
	textbox.setOrigin(x, y);
}

sf::FloatRect Textbox::getLocalBounds() {
	return textbox.getLocalBounds();
}

sf::FloatRect Textbox::getGlobalBounds() {
	return textbox.getGlobalBounds();
}

std::string Textbox::getText() {
	return text.str();
}

void Textbox::drawTo(sf::RenderWindow& window) {
	window.draw(textbox);
}

void Textbox::typedOn(sf::Event input) {
	if (isSelected) {
		int charTyped = input.text.unicode;
		if (charTyped < 128) {
			if (hasLimit) {
				if (text.str().length() <= limit) {
					inputLogic(charTyped);
				}
				else if (text.str().length() <= limit + 1 && charTyped == DELETE_KEY) {
					deleteLastChar();
				}
			}
			else {
				inputLogic(charTyped);
			}
		}
	}
}

bool isSelected = false;
bool hasLimit = false;
int limit;

void Textbox::inputLogic(int charTyped) {
	if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
		text << static_cast<char>(charTyped);
	}
	else if (charTyped == DELETE_KEY) {
		if (text.str().length() > 0) {
			deleteLastChar();
		}
	}
	textbox.setString(text.str() + "_");
}

void Textbox::deleteLastChar() {
	std::string t = text.str();
	std::string newT = "";
	for (int i = 0; i < t.length() - 1; i++) {
		newT += t[i];
	}
	text.str("");
	text << newT;

	textbox.setString(text.str());
}
