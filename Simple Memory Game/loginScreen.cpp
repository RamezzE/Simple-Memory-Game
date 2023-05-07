


#include <SFML/Graphics.hpp>

#include <fstream>

#include "loginScreen.h"
#include "gameState.h"
#include "textbox.h"

#include "Menu.h"

extern unsigned width;
extern unsigned height;
std::string username, password;

sf::Text outputMsg;
bool buttonSelected, menuKeyboard;
bool SignUp = false;
bool loginOrSignUpPressed;

int loginScreen::currentBox = 0;


void loginScreen::draw(const float dt) {

    //setting up background pic
    sf::RectangleShape backgroundPic;
    sf::Texture texture;
    texture.loadFromFile("pics\\adventure time.jpg");
    backgroundPic.setTexture(&texture);
    backgroundPic.setSize(sf::Vector2f(width, height));

    //font
    sf::Font font;
    font.loadFromFile("fonts\\Gronzy.ttf");

    textArray[0].setString("Username");
    textArray[1].setString("Password");

    textArray[0].setCharacterSize(32);
    textArray[1].setCharacterSize(32);
    textArray[2].setCharacterSize(52);
    textArray[3].setCharacterSize(25);

    outputMsg.setFont(font);
    outputMsg.setCharacterSize(32);
    outputMsg.setPosition(width / 2, height / (4 * 3));

    menuRect[0].setOutlineColor(sf::Color::White);
    menuRect[1].setOutlineColor(sf::Color::White);

    sf::FloatRect maxsize = textArray[0].getLocalBounds();

    sf::FloatRect temp;

    //seting up origins, positions etc
    for (int i = 0; i < 3; i++) {

        textArray[i].setFont(font);

        temp = textArray[i].getLocalBounds();

        // making rectangle origin in centre
        textArray[i].setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f);
        //setting position
        textArray[i].setPosition(sf::Vector2f((width / 2) - 100, height / (4) * (i + 1)));

        temp = menuRect[i].getLocalBounds();
        // making rectangle origin in centre
        menuRect[i].setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f);

        menuRect[i].setSize(sf::Vector2f(maxsize.width * 1.3, maxsize.height * 2));
        menuRect[i].setPosition(sf::Vector2f((width / 2) - 100, height / (4) * (i + 1)));
        menuRect[i].setFillColor(sf::Color(0, 0, 0, 180));
        menuRect[i].setOutlineThickness(2);
    }

    //handling login/sign up rectangle outside for different height
    menuRect[2].setSize(sf::Vector2f(maxsize.width * 1.3, maxsize.height * 3));

    //handling fourth text outside
    temp = textArray[3].getLocalBounds();
    textArray[3].setFont(font);
    textArray[3].setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f);
    textArray[3].setPosition(sf::Vector2f((width / 2) - 100, height / (4) * (2 + 1) + 80));

    //handling fourth menuRect outside

    temp = menuRect[3].getLocalBounds();
    menuRect[3].setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f);
    menuRect[3].setFillColor(sf::Color::Transparent);
    menuRect[3].setSize(sf::Vector2f(maxsize.width * 1.3, maxsize.height * 2));
    menuRect[3].setPosition(sf::Vector2f((width / 2) - 100, height / (4) * (2 + 1) + 80));


    //setting origins and positions of textboxes and surrounding rectangles
    for (int i = 0; i < 2; i++) {
        temp = textBox_rectangle[i].getLocalBounds();
        textBox_rectangle[i].setOrigin(0, temp.top + temp.height / 2.0f);
        textBox_rectangle[i].setFillColor(sf::Color(0, 0, 0, 220));
        textBox_rectangle[i].setOutlineThickness(2);
        textBox_rectangle[i].setPosition(sf::Vector2f((width / 2) - 10, (height / (4) * (i + 1))));
        textBox_rectangle[i].setSize(sf::Vector2f(450, 50));

        temp = textbox[i].getLocalBounds();
        textbox[i].setOrigin(0, temp.top + temp.height / 2.0f);
        textbox[i].setPosition(sf::Vector2f((width / 2), (height / (4) * (i + 1))));
        textbox[i].setFont(font);
        textbox[i].setLimit(true, 15);
        textbox[i].setCharacterSize(50);
    }


    //drawing everything

    mygame->window.draw(backgroundPic);

    mygame->window.draw(textBox_rectangle[0]);
    textbox[0].drawTo(mygame->window);

    mygame->window.draw(textBox_rectangle[1]);
    textbox[1].drawTo(mygame->window);

    mygame->window.draw(outputMsg);

    for (int i = 0; i < 3; i++) {
        mygame->window.draw(menuRect[i]);
        mygame->window.draw(textArray[i]);
    }
    mygame->window.draw(menuRect[3]); // transparent since we dont need it to be visible
    mygame->window.draw(textArray[3]);

}

void loginScreen::update(const float dt) {

    // make the currentBox mouse is hovering over/ clicking on to glow
    switch (currentBox) {
    case 0:
        textBox_rectangle[0].setOutlineColor(sf::Color(102, 153, 255, 255));
        textBox_rectangle[1].setOutlineColor(sf::Color::White);

        textArray[2].setFillColor(sf::Color::White);
        textArray[3].setFillColor(sf::Color::White);
        break;
    case 1:
        textBox_rectangle[1].setOutlineColor(sf::Color(102, 153, 255, 255));
        textBox_rectangle[0].setOutlineColor(sf::Color::White);

        textArray[2].setFillColor(sf::Color::White);
        textArray[3].setFillColor(sf::Color::White);
        break;
    case 2:
        textArray[2].setFillColor(sf::Color(102, 153, 255, 255));
        textArray[3].setFillColor(sf::Color::White);
        break;
    case 3:
        textArray[2].setFillColor(sf::Color::White);
        textBox_rectangle[0].setOutlineColor(sf::Color::White);
        textBox_rectangle[1].setOutlineColor(sf::Color::White);

        textArray[3].setFillColor(sf::Color(102, 153, 255, 255));
        break;
    }

    if (!buttonSelected) {
        textArray[2].setFillColor(sf::Color::White);
    }
    if (menuKeyboard && currentBox == 2) {
        textBox_rectangle[1].setOutlineColor(sf::Color::White);
    }

    //switching texts if user presses on the Sign Up option
    if (!SignUp) {
        textArray[2].setString("Login");
        textArray[3].setString("Sign up");
    }
    else {
        textArray[3].setString("Login");
        textArray[2].setString("Sign up");
    }

    // handling username and password if user clicks on the login or sign up button
    if (loginOrSignUpPressed) {
        loginOrSignUpPressed = false;

        username = ""; password = "";
        username = textbox[0].getText();
        password = textbox[1].getText();

        if (username != "" && password != "") { //if both aren't empty
            bool login = LoginOrSignUp(username, password);
            if (login && !SignUp) { //successful login so we go to menu
                textbox[0].reset();
                textbox[1].reset();
                toMenu();
            }

            if (SignUp && login) { //successful sign up, output msg is changed inside the func
                SignUp = false;
            }
        }
        else { //if any of them is empty then output this msg
            outputMsg.setString("Please enter text in both username and password");
        }
    }

}

void loginScreen::handleInput() {

    sf::Event event;

    while (this->mygame->window.pollEvent(event))
    {
        switch (event.type) {
        case sf::Event::Closed:
            this->mygame->window.close(); //close the window
            // break;

        case sf::Event::KeyPressed:
            switch (event.key.code) {
                break;
            case sf::Keyboard::Return:
                if (currentBox < 2) {
                    textbox[currentBox].setSelected(true);
                }
                else if (currentBox == 2) {
                    //login action or sign up
                    loginOrSignUpPressed = true;
                }
                else if (currentBox == 3) { //toggles the login and sign up to change places
                    if (SignUp) {
                        SignUp = false;
                    }
                    else {
                        SignUp = true;
                    }
                }
                break;
            case sf::Keyboard::Escape:
                if (currentBox < 2) { //deselect the textbox
                    textbox[currentBox].setSelected(false);
                }
                break;
            case sf::Keyboard::Up:
                MoveUp(); //moveup in the login screen list
                break;
            case sf::Keyboard::Down:
                MoveDown(); //move down in the login screen list
                break;
            }

        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button) {
            case sf::Mouse::Left:
                //setting appropriate currentBox if left click is pressed and released on one of them
                if (isMouseOver(mygame->window, textBox_rectangle[0])) {
                    currentBox = 0;
                }
                else if (isMouseOver(mygame->window, textBox_rectangle[1])) {
                    currentBox = 1;
                }
                else if (isMouseOver(mygame->window, menuRect[2])) {
                    loginOrSignUpPressed = true;
                }
                else if (isMouseOver(mygame->window, menuRect[3])) {
                    if (SignUp) {
                        SignUp = false;
                    }
                    else {
                        SignUp = true;
                    }
                }
                if (currentBox < 2) { // if currentBox is one of the 2 textboxes then selects the textbox
                    textbox[currentBox].setSelected(true);
                }
                break;
            }
            break;
        case sf::Event::TextEntered:
            if (currentBox < 2) { // if the currentBox is one of the 2 textboxes, text goes into the textbox
                textbox[currentBox].typedOn(event);
            }
            break;
        }
    }

    if (isMouseOver(mygame->window, menuRect[2])) {
        menuKeyboard = false;
        buttonSelected = true;
        currentBox = 2;
    }
    else if (menuKeyboard && currentBox == 2) {
        buttonSelected = true;
    }
    else {
        buttonSelected = false;
    }

    if (isMouseOver(mygame->window, menuRect[3])) { //if mouse is hovering over last menu item
        currentBox = 3;
    }
}

bool loginScreen::isMouseOver(sf::RenderWindow& window, sf::RectangleShape thing) {
    return thing.getGlobalBounds().contains(sf::Mouse::getPosition(mygame->window).x, sf::Mouse::getPosition(mygame->window).y);
}

void loginScreen::MoveUp() {

    if (currentBox > 0) {
        currentBox--;
        if (currentBox == 2) {
            menuKeyboard = true;
        }
    }
}

void loginScreen::MoveDown() {
    if (currentBox < 3) {
        currentBox++;
        if (currentBox == 2) {
            menuKeyboard = true;
        }
    }
}

bool loginScreen::LoginOrSignUp(std::string username, std::string password) {

    //opens username and password files to input or append if signing up
    std::fstream usernameFile("textfiles\\username.txt", std::ios::app | std::ios::in);

    std::fstream passwordFile("textfiles\\password.txt", std::ios::app | std::ios::in);


    if (SignUp) {
        std::string sample;
        while (!usernameFile.eof()) {
            usernameFile >> sample;
            //checking if username is already taken
            if (sample == username) {
                outputMsg.setString("Username already taken");
                usernameFile.close();
                return false;
            }
        }

        usernameFile.close();
        usernameFile.open("textfiles\\username.txt", std::ios::app | std::ios::in); //closing and reopening to get file to read from the first line of the file

        //appending the new account with the username and password to their files
        usernameFile << username << std::endl;
        passwordFile << password << std::endl;

        usernameFile.close();
        passwordFile.close();

        //initializing playerData with 0 0 0 values for score, time and roundsPlayed
        std::fstream data("textfiles\\playerData.txt", std::ios::app);
        data << username << "\n0\n0\n0\n";

        data.close();
        outputMsg.setString("Account created successfully"); //confirmation for user
        return true;
    }

    //gets here if signup mode is false (normal login)
    int i = -1;
    std::string usernameCompare;

    bool userfound = false;

    while (!usernameFile.eof()) {
        i++;
        usernameFile >> usernameCompare;
        //searching for username in its file
        if (usernameCompare == username) {
            usernameFile.close();
            userfound = true;
            break; //breaks once it's found
        }
    }

    if (!userfound) {
        usernameFile.close();
        outputMsg.setString("Username not found");
        return false; //sets appropriate msg and returns false
    }

    std::string passwordCompare;
    for (int j = 0; j <= i; j++) {
        passwordFile >> passwordCompare;
        //keep inputting until reaching the desired index
        if (j == i) {
            if (passwordCompare == password) {
                passwordFile.close();
                //updating the current logged in user file with the current username
                std::ofstream out("textfiles\\LastUser.txt");
                out << username << std::endl;
                out.close();
                return true;
            }
        }
    }
    usernameFile.close();
    passwordFile.close();
    outputMsg.setString("Wrong password"); //sets appropriate msg and returns false
    return false;
}

loginScreen::loginScreen(Game* game) {
    this->mygame = game;
}

void loginScreen::toMenu() {
    outputMsg.setString("");
    mygame->pushState(new Menu(mygame));
}

void loginScreen::toLogin() {
    mygame->pushState(new loginScreen(mygame));
}
