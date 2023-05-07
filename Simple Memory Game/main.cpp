
#include <SFML/Graphics.hpp>
#include <windows.h>
#include "Menu.h"
#include "gameState.h"
#include "textbox.h"
#include "PlayState.h"
#include "game.h"
#include "loginScreen.h"



int main()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, 0);

	Game mygame;

	//mygame.pushState(new PlayState(&mygame));

	mygame.pushState(new loginScreen(&mygame));

	mygame.gameLoop();
}

