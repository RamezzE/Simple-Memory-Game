#pragma once

#include <SFML/Graphics.hpp>

#include "gameState.h"
#include "textbox.h"

class loginScreen : public GameState {

public:
    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

    loginScreen(Game* game);
    bool isMouseOver(sf::RenderWindow& window, sf::RectangleShape thing);
    void MoveUp();
    void MoveDown();

    void toMenu();

    void toLogin();

    bool LoginOrSignUp(std::string username, std::string password);

private:
    //void BacktoMenu();
    sf::RectangleShape textBox_rectangle[2];
    sf::RectangleShape menuRect[4];
    Textbox textbox[2];
    sf::Text textArray[4];
    static int currentBox;

};

