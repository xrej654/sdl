#include <SDL.h>
#include "SDL_ttf.h"
#include "Game.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	//utworzenie obiektu klasy 'Game' w niej sa wszystkie metody odgrywajace w grze

	if (!SDL_Init(SDL_INIT_EVERYTHING) && !TTF_Init())
	{
		SDL_SetError("Something gone wrong: %s", SDL_GetError());
	}
	else
	{
		std::cout << "Initializing..." << std::endl;
	}

	//okreslanie rozdzielczosci ekranu dopasowanego do kazdego monitora
	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
		std::cout << "B³¹d: " << SDL_GetError() << std::endl;
	}

	int screenWidth = dm.w;
	int screenHeight = dm.h;

	Game game("gra", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 0);
	game.setWitdhAndHeight(screenWidth, screenHeight);

	//zmienne potrzebne to klawiszy klawiatury i obsluga myszki
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	Uint32 mouseButtons = NULL;

	//zmienne do deltaTime
	Uint32 lastTicks = SDL_GetTicks();
	float deltaTime = 10.f;

	//glowna petla gry
	while (game.running())
	{
		//ciagle okreslanie pozycji myszki
		int mouseX, mouseY;
		mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);

		game.update(keys, deltaTime, mouseButtons, mouseX, mouseY);

		//obliczanie deltaTime
		Uint32 currentTicks = SDL_GetTicks();
		deltaTime = (currentTicks - lastTicks) / 1000.f;
		lastTicks = currentTicks;

		//limitowanie deltaTime
		if (deltaTime > 0.1f) deltaTime = 0.1f;
	}
	return 0;
}