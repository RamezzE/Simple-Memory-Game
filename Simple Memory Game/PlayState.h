#pragma once
#ifndef GAME_STATE_EDITOR_HPP
#define GAME_STATE_EDITOR_HPP


#include <SFML/Graphics.hpp>

#include "gameState.h"
#include "textbox.h"


class PlayState : public GameState
{
public:

    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

    PlayState(Game* game);
    bool isMouseOver(sf::RenderWindow& window, sf::RectangleShape thing);

    void resetGame();

    void fileEdit(std::string filename, std::string search, std::string replace);

    void saveGame(int score, int time, int roundsPlayed);

private:
    void BacktoMenu();
    sf::Text textArray[4];
    sf::Sprite lives[3];
    static int currentBox;
    int maxLevel;

    sf::Clock glowClock;
    sf::Clock pauseClock;
    sf::Clock gameClock;
    sf::Clock winTextClock;

};

#endif /* GAME_STATE_EDITOR_HPP */
