#include "reportPage.h"

#include <SFML/Graphics.hpp>
#include <fstream>

#include "PlayState.h"
#include "Menu.h"
#include "game.h"
#include "gameState.h"

extern unsigned width, height;

bool accessData = true;
void reportPage::draw(const float dt) {

    //setting up background
    sf::RectangleShape report;
    report.setSize(sf::Vector2f(width, height));
    sf::Texture texture;
    texture.loadFromFile("pics\\report.jpg");

    if (!texture.loadFromFile("pics\\report.jpg")) {
        std::cout << "report texture not found" << std::endl;
    }
    report.setTexture(&texture);

    sf::FloatRect temp;

    //setting up backtoMenu button
    backButton.setSize(sf::Vector2f(70, 50));
    sf::Texture backArrow;
    backArrow.loadFromFile("pics\\back arrow.png");
    backButton.setTexture(&backArrow);

    //setting up texts and font

    sf::Font menufont;
    menufont.loadFromFile("fonts\\GothicA1-Black.ttf");


    gameNO.setCharacterSize(40);
    gameNO.setFont(menufont);
    gameNO.setPosition(750, 245);

    scoreText.setCharacterSize(40);
    scoreText.setFont(menufont);
    scoreText.setPosition(750, 415);

    timeText.setCharacterSize(40);
    timeText.setFont(menufont);
    timeText.setPosition(750, 570);


    temp = backButton.getLocalBounds();
    backButton.setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f); // making origin in center of object

    mygame->window.draw(report);
    mygame->window.draw(backButton);

    mygame->window.draw(gameNO);
    mygame->window.draw(scoreText);
    mygame->window.draw(timeText);

}

void reportPage::update(const float dt) {
    if (accessData) {
        int scoreAvg, timeAvg, roundsNum;
        getData(scoreAvg, timeAvg, roundsNum);

        gameNO.setString(std::to_string(roundsNum));
        scoreText.setString(std::to_string(scoreAvg));
        timeText.setString(std::to_string(timeAvg));
        accessData = false;
    }
}

void reportPage::handleInput() {
    sf::Event event;

    while (this->mygame->window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            this->mygame->window.close(); //close the window
            break;
        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button) {
            case sf::Mouse::Left:
                if (isMouseOver(mygame->window, backButton)) {
                    toMenu();
                }
                break;
            }
            break;
        }
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

bool reportPage::isMouseOver(sf::RenderWindow& window, sf::RectangleShape thing) {
    return thing.getGlobalBounds().contains(sf::Mouse::getPosition(mygame->window).x, sf::Mouse::getPosition(mygame->window).y);
}

void reportPage::getData(int& scoreAvg, int& timeAvg, int& roundsNum) {

    std::ifstream LastUser("textfiles\\LastUser.txt");
    std::string username;
    LastUser >> username;
    LastUser.close();
    std::cout << "opening to dislay data" << std::endl;

    std::ifstream data("textfiles\\playerData.txt");
    int i = -1;
    while (!data.eof()) {
        i++;
        std::string text;
        data >> text;
        if (text == username) {
            break;
        }
    }
    std::string ScoreText, TimeText, RoundsText;
    data >> ScoreText;
    data >> TimeText;
    data >> RoundsText;

    data.close();

    int score, time, rounds;

    score = stoi(ScoreText);
    time = stoi(TimeText);
    rounds = stoi(RoundsText);

    if (rounds != 0) {
        scoreAvg = score / rounds;
        timeAvg = time / rounds;
        roundsNum = rounds;
    }
    else {
        scoreAvg = 0; timeAvg = 0; roundsNum = 0;
    }
}

reportPage::reportPage(Game* game) {
    this->mygame = game;
}

void reportPage::toMenu() {
    accessData = true;
    mygame->pushState(new Menu(mygame));
}