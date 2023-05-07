#pragma once


#include <SFML/Graphics.hpp>
#include "gameState.h"


class reportPage : public GameState {
public:
    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

    reportPage(Game* game);
    bool isMouseOver(sf::RenderWindow& window, sf::RectangleShape thing);

private:

    sf::RectangleShape backButton;
    sf::Text gameNO, scoreText, timeText;
    void toMenu();
    void getData(int& scoreAvg, int& timeAvg, int& roundsNum);

};

