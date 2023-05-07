



#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "PlayState.h"
#include "Menu.h"
#include "game.h"
#include "gameState.h"


extern unsigned width;
extern unsigned height;

int PlayState::currentBox = 0;

const int mapsize = 4;
sf::RectangleShape tilemap[mapsize * mapsize];
sf::RectangleShape hintBox, levelBox, backButton;
int currentTile; int pressedTile = -1;
int timeDisplay = 0;


sf::RectangleShape rect;
sf::Time glowTime;
sf::Time pauseTime = sf::seconds(0);


int score = 0;
int mistakes = -1;
int roundsPlayed = 0;

int FinishedScore = 0;
int FinishedTime = 0;
int FinishedRounds = 0;

int levelHardness = 5, correctCount = 0, currentLevel = 1;
int* randomSquares = new int[levelHardness]; // still need to delete array later
bool hint;
bool newLevel = true;
bool win = false;
bool winnerText = false;
bool SAVE = false;
bool lostFocus = false;


void PlayState::draw(const float dt)
{
    //background image
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(width, height));
    sf::Texture backpic;
    backpic.loadFromFile("pics\\gumball and darwin.png");
    background.setTexture(&backpic);

    //pic for correct tile
    sf::Texture checkmark;
    checkmark.loadFromFile("pics\\correct.png");

    //pic for wrong tile
    sf::Texture wrong;
    wrong.loadFromFile("pics\\wrong.png");

    //making a hint button display
    hintBox.setSize(sf::Vector2f(150, 50));
    hintBox.setFillColor(sf::Color(0, 0, 0, 180)); //setting rectangle color 
    hintBox.setPosition(width / 2, 30); //setting position

    //setting up hintbox text
    sf::Font font;
    font.loadFromFile("fonts\\Gronzy.ttf");
    textArray[0].setFont(font);

    if (mistakes + 1 < 3) { //change to retry button if player loses
        textArray[0].setString("Hint");
    }
    else {
        textArray[0].setString("Retry");
    }
    textArray[0].setCharacterSize(50);
    textArray[0].setPosition(width / 2, 30); //setting position

    //setting up box around "Level 1" text
    levelBox.setSize(sf::Vector2f(tilemap[0].getSize().x * 4, 50));
    levelBox.setFillColor(sf::Color(0, 0, 0, 180)); //setting rectangle color 
    levelBox.setPosition(width / 2, tilemap[0].getPosition().y - height * 0.14);

    //setting up level text
    textArray[1].setFont(font);
    std::string levelText = "Level ";
    levelText += std::to_string(currentLevel);


    if (winnerText) {
        if (winTextClock.getElapsedTime() > sf::seconds(1)) {
            winnerText = false; //disable the You win! text after 1 second passes
        }
        else {
            textArray[1].setString("You win!"); // change to You win temporarily after winning a round
        }
    }
    else if (mistakes + 1 >= 3) { //change to "You lost" if player loses 
        textArray[1].setString("You Lost");
    }
    else if (currentLevel == maxLevel) { // change to "Max Level Reached" once the maximum level is reached
        textArray[1].setString("Max Level Reached");
    }
    else {
        textArray[1].setString(levelText); // if everything is normal then display the current level number
    }

    textArray[1].setCharacterSize(50);
    textArray[1].setPosition(width / 2, tilemap[0].getPosition().y - height * 0.14);

    //setting up back button
    backButton.setSize(sf::Vector2f(70, 50));
    sf::Texture backArrow; //image for back button
    backArrow.loadFromFile("pics\\back arrow.png");
    backButton.setTexture(&backArrow);


    //setting up timedisplay
    std::string timeText;
    timeText = "Time: ";
    timeText += std::to_string(timeDisplay);
    textArray[2].setString(timeText);
    textArray[2].setCharacterSize(50);
    textArray[2].setPosition(width - textArray[2].getCharacterSize() - 40, 30);
    textArray[2].setFont(font);

    //setting up score
    std::string scoreText;
    scoreText = "Score: ";
    scoreText += std::to_string(score);
    textArray[3].setString(scoreText);
    textArray[3].setCharacterSize(50);
    textArray[3].setPosition(width - textArray[2].getCharacterSize() - 40, 90);
    textArray[3].setFont(font);



    //setting up origins to be in centers of object
    sf::FloatRect temp;
    temp = hintBox.getLocalBounds();
    hintBox.setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f); // making origin in center of object

    temp = backButton.getLocalBounds();
    backButton.setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f); // making origin in center of object

    temp = textArray[0].getLocalBounds();
    textArray[0].setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f); //making origin in center of object

    temp = levelBox.getLocalBounds();
    levelBox.setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f); //making origin in center of object

    temp = textArray[1].getLocalBounds();
    textArray[1].setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f); //making origin in center of object

    temp = textArray[2].getLocalBounds();
    textArray[2].setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f); //making origin in center of object

    temp = textArray[3].getLocalBounds();
    textArray[3].setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f); //making origin in center of object

    //setting up 3 lives (hearts)
    int space = 0;

    sf::Texture heart;
    heart.loadFromFile("pics\\heart.png");
    for (int i = 2; i >= 0; i--) {
        temp = lives[i].getLocalBounds();
        lives[i].setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f); //making origin in center of object
        lives[i].setScale(0.2f, 0.2f);
        lives[i].setTexture(heart);
        lives[i].setPosition(width - 200 - space, 30);
        space += 80;
    }
    //check if a tile is pressed and to what texture it will change to
    if (pressedTile >= 0) {
        bool correctSquare = false;
        for (int i = 0; i < levelHardness; i++) {
            if (pressedTile == randomSquares[i]) {
                correctSquare = true;
                break;
            }
        }
        if (correctSquare) { //if square is one of the correct ones changes texture to checkmark 
            if (tilemap[pressedTile].getTexture() != &checkmark) { //if the tile is not already pressed
                tilemap[pressedTile].setTexture(&checkmark);
                correctCount++;
                score += 10;
            }
            pressedTile = -1;
        }
        else if (tilemap[pressedTile].getTexture() != &wrong) {
            tilemap[pressedTile].setTexture(&wrong); //otherwise changes texture to wrong mark if not already pressed
            pressedTile = -1;
            score -= 2;
            lives[++mistakes].setScale(0, 0);
            if (mistakes + 1 == 3) { //once player loses SAVE = true so it saves in the update section
                SAVE = true;
                FinishedRounds = roundsPlayed;
                FinishedScore = score;
                FinishedTime = timeDisplay;
            }
        }
    }

    unsigned originX = width / 2.8, originY = height / 3;

    for (int i = 0; i < mapsize * mapsize; i++) {
        //setting origin of each square in the grid to its center
        temp = tilemap[i].getLocalBounds();
        tilemap[i].setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f);
    }

    int n = 0, l, w = 0;
    for (int i = 0; i < mapsize; i++) {
        l = 0;
        for (int j = 0; j < mapsize; j++, n++) {
            tilemap[n].setSize(sf::Vector2f(100, 100));
            tilemap[n].setPosition(originX + l, originY + w);
            l += 120;
        }
        w += 120;
    }

    //drawing everything

    mygame->window.draw(background);
    mygame->window.draw(hintBox);
    mygame->window.draw(levelBox);
    mygame->window.draw(backButton);


    for (int i = mistakes + 1; i < 3; i++) {
        mygame->window.draw(lives[i]);
    }
    for (int i = 0; i < 4; i++) {
        mygame->window.draw(textArray[i]);
    }
    for (int i = 0; i < mapsize * mapsize; i++) {
        mygame->window.draw(tilemap[i]);
    }

}

void PlayState::update(const float dt) {

    if (newLevel) { //make new random glowing tiles in case of new level
        glowTime = sf::seconds(1.5) + pauseTime;
        srand(time(NULL)); //randomized places of glowing tiles
        int m;
        for (int i = 0; i < levelHardness; i++) {
            m = rand() % (mapsize * mapsize);

            //checking there are no duplicate random numbers
            bool repeat = true;
            while (repeat) {
                repeat = false;
                for (int j = 0; j <= i; j++) {
                    if (m == randomSquares[j]) {
                        repeat = true;
                        m = rand() % 16;
                        break;
                    }
                }
            }
            tilemap[m].setFillColor(sf::Color(32, 68, 160, 255)); // shade of blue
            randomSquares[i] = m;
        }
        glowClock.restart();
        newLevel = false;
    }

    if (correctCount == levelHardness) { // win condition achieved
        win = true;
        newLevel = true;
        hint = false;
        winnerText = true;

        roundsPlayed++;
        correctCount = 0;
        glowTime = sf::seconds(1.5);
        if (levelHardness <= 10) {
            levelHardness++;
            currentLevel++;
        }
        else {
            maxLevel = currentLevel;
        }
        winTextClock.restart();
        return;
    }

    for (int i = 0; i < mapsize * mapsize; i++) { //making all outlines thickness 2
        if (i == currentTile) {
            tilemap[i].setOutlineThickness(4);
        }
        else {
            tilemap[i].setOutlineThickness(2);
        }
    }

    for (int i = 0; i < mapsize * mapsize; i++) {
        if (i == currentTile) {
            tilemap[i].setOutlineColor(sf::Color(32, 68, 160, 255)); //outline blue if mouse hovered over
        }
        else {
            tilemap[i].setOutlineColor(sf::Color::Black); //if not then outline black
        }
    }

    if (gameClock.getElapsedTime() >= sf::seconds(1)) {
        if (!win && mistakes + 1 < 3 && glowClock.getElapsedTime() > glowTime) {
            gameClock.restart();
            if (!lostFocus) { //if window is not focused then time won't update (pause)
                timeDisplay++; //update time display every 1 second
            }
        }
    }

    if (hint) { //unpressed correct tiles glow 
        for (int i = 0; i < levelHardness; i++) {
            int index = randomSquares[i];
            if (tilemap[index].getTexture() == nullptr) {
                tilemap[index].setFillColor(sf::Color(32, 68, 160, 255)); //blue
            }
        }
        glowTime = sf::seconds(0.5);
        hint = false;
        glowClock.restart();
    }

    if (glowClock.getElapsedTime() > glowTime) { //revert color to white when glowTime is up
        for (int i = 0; i < levelHardness; i++) {
            int index = randomSquares[i];
            tilemap[index].setFillColor(sf::Color::White);
        }
    }

    if (SAVE) { //save game only once if player loses
        saveGame(FinishedScore, FinishedTime, FinishedRounds);
        SAVE = false;
    }

    pauseTime = sf::seconds(1);

    if (win) { // after winning
        FinishedRounds = roundsPlayed;
        FinishedScore = score;
        FinishedTime = timeDisplay;
        for (int i = 0; i < mapsize * mapsize; i++) {
            tilemap[i].setTexture(nullptr);
        }
        win = false;
        pauseClock.restart();
        while (pauseClock.getElapsedTime() <= pauseTime) {
            //do nothing for some delay time after winning
        }
        mistakes = -1; //reset to no mistakes
    }
}

void PlayState::handleInput() {

    sf::Event event;

    while (this->mygame->window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::GainedFocus:
            lostFocus = false;
            break;
        case sf::Event::LostFocus:
            lostFocus = true;
            break;
        case sf::Event::Closed:
            if (mistakes + 1 != 3) { //if player exited before he loses -> then save game
                saveGame(FinishedScore, FinishedTime, FinishedRounds);
            }
            mygame->window.close(); //close the window in all cases
            break;
        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button) {
            case sf::Mouse::Left:
                if (isMouseOver(mygame->window, backButton)) { //mouse pressed on back button
                    if (mistakes + 1 != 3) { //if player pressed on back button before he loses
                        if (roundsPlayed > 0) { // if he passed at least 1 round
                            saveGame(FinishedScore, FinishedTime, FinishedRounds); //save game if clicked on backbutton only midgame
                        }
                    }
                    resetGame();
                    BacktoMenu();
                }
                if (glowClock.getElapsedTime() > glowTime) { // do not handle certain events except after squares stop glowing
                    if (isMouseOver(mygame->window, hintBox)) {
                        if (mistakes + 1 < 3) {
                            hint = true;
                        }
                        else {
                            resetGame();
                        }
                    }
                    if (mistakes + 1 != 3) {
                        for (int i = 0; i < mapsize * mapsize; i++) {
                            if (isMouseOver(mygame->window, tilemap[i])) {
                                pressedTile = i;
                                break;
                            }
                        }
                    }
                }
                break;
            }
        }   break;
    }

    for (int i = 0; i < mapsize * mapsize; i++) {
        if (isMouseOver(mygame->window, tilemap[i])) {
            currentTile = i; // mouse hovered over tile
            break;
        }
    }

    if (isMouseOver(mygame->window, hintBox)) { // hintbox text to be colored if mouse hovering over
        textArray[0].setFillColor(sf::Color(102, 153, 255, 255));
    }
    else if (textArray[0].getFillColor() == sf::Color(102, 153, 255, 255)) { //changes back if mouse not hovering
        textArray[0].setFillColor(sf::Color::White);
    }
    if (isMouseOver(mygame->window, backButton)) { // back button grows bigger if mouse hovered over
        backButton.setScale(1.5, 1.5);
        backButton.setPosition(70, 50);
    }
    else { //back button grows back to normal scale if mouse not hovering
        backButton.setScale(1, 1);
        backButton.setPosition(60, 30);
    }
}

bool PlayState::isMouseOver(sf::RenderWindow& window, sf::RectangleShape thing) {
    //std::cout << sf::Mouse::getPosition(mygame->window).x << std::endl;
    return thing.getGlobalBounds().contains(sf::Mouse::getPosition(mygame->window).x, sf::Mouse::getPosition(mygame->window).y);
}

PlayState::PlayState(Game* game) {
    this->mygame = game;
}

void PlayState::resetGame() {

    currentLevel = 1; //reset to inital level 1
    levelHardness = 5; //reset to initial value

    //reset to 0
    roundsPlayed = 0;
    score = 0;
    timeDisplay = 0;
    correctCount = 0;

    //reset to 0
    FinishedRounds = 0;
    FinishedScore = 0;
    FinishedTime = 0;

    //reset to no mistakes
    mistakes = -1;

    pauseTime = sf::seconds(0); //reset to initial value 

    //resetting to initial values
    newLevel = true;
    winnerText = false;
    win = false;
    lostFocus = false;

    //changing back all square textures to none
    for (int i = 0; i < mapsize * mapsize; i++) {
        tilemap[i].setTexture(nullptr);
        tilemap[i].setFillColor(sf::Color::White);
    }
    gameClock.restart();
}

void PlayState::BacktoMenu() {
    mygame->pushState(new Menu(mygame));
}

void PlayState::saveGame(int score, int time, int roundsPlayed) {

    //checking the current user username from file and saving it into username
    std::ifstream LastUser("textfiles\\LastUser.txt");
    std::string username;
    LastUser >> username;
    LastUser.close();

    std::cout << username << " saving " << std::endl;

    // searches for the username in this file
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

    //once username search is found, takes the 3 inputs afterwards which are score, time and number of rounds played in this order
    std::string ScoreText, TimeText, RoundsText;
    data >> ScoreText;
    data >> TimeText;
    data >> RoundsText;

    data.close();

    //put together the sentence to be replaced in the file which is the username along with the player's data
    std::string search = username + "\n" + ScoreText + "\n" + TimeText + "\n" + RoundsText + "\n";

    int score1, time1, rounds1;
    score1 = stoi(ScoreText); time1 = stoi(TimeText); rounds1 = stoi(RoundsText);

    // adding score, time and roundsPlayed values.
    score += score1;
    time += time1;
    roundsPlayed += rounds1;

    // put together the sentence to replace the previous string with, which is the username along with the new calculated data
    std::string replace = username + "\n" + std::to_string(score) + "\n" + std::to_string(time) + "\n" + std::to_string(roundsPlayed) + "\n";

    // calling function to edit the file
    fileEdit("textfiles\\playerData.txt", search, replace);

}

void PlayState::fileEdit(std::string filename, std::string search, std::string replace)
{
    std::ostringstream text;
    std::ifstream in_file(filename);

    text << in_file.rdbuf();
    std::string str = text.str();
    std::string str_search = search;
    std::string str_replace = replace;
    size_t pos = str.find(str_search);
    str.replace(pos, std::string(str_search).length(), str_replace);
    in_file.close();

    std::ofstream out_file(filename);
    out_file << str;

}



