
#include <iostream>
#include "SFML/Graphics.hpp"
#include "game.h"
#include "gameState.h"


unsigned width = 1280; // original values: 1280 
unsigned height = 720; //720


Game::Game() {

	window.create(sf::VideoMode(width, height), "Memory Game", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);

}

void Game::pushState(GameState* state) {
	states.push_back(state);
}

void Game::popState() {
	states.back();
	delete states.back();
	states.pop_back();
}


Game:: ~Game() {
	while (!states.empty()) {
		popState();
	}
}

GameState* Game::CurrentState() {
	if (states.empty()) {
		return nullptr;
	}
	else {
		return states.back();
	}
}



void Game::gameLoop() {

	sf::Clock clock;
	while (window.isOpen()) {

		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();

		if (CurrentState() == nullptr) {
			continue;
		}

		window.clear();

		CurrentState()->handleInput();

		CurrentState()->update(dt);

		window.clear();

		CurrentState()->draw(dt);

		window.display();

	}
}
