#include <SDL.h>
#include "Game.h"
#include <iostream>


using namespace std;

int main(int argc, char* argv[])
{
	//utworzenie obiektu klasy 'Game' w niej sa wszystkie metody odgrywajace w grze
	Game game;

	//inicjalizacja
	game.init("gra", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, 0);

	//zmienne potrzebne to klawiszy klawiatury i obsluga myszki
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	Uint32 mouseButtons = NULL;

	//zmienne do deltaTime
	Uint32 lastTicks = SDL_GetTicks();
	float speed = 100.f;
	float deltaTime = 10.f;

	//glowna petla gry
	while (game.running())
	{
		//ciagle okreslanie pozycji myszki
		int mouseX, mouseY;
		mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);

		game.update(keys, speed, deltaTime, mouseButtons, mouseX, mouseY);

		//obliczanie deltaTime
		Uint32 currentTicks = SDL_GetTicks();
		deltaTime = (currentTicks - lastTicks) / 1000.f;
		lastTicks = currentTicks;

		//limitowanie deltaTime
		if (deltaTime > 0.1f) deltaTime = 0.1f;
	}
	return 0;
}