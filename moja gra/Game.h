#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include <iostream>
#include <cmath>
#include "AlgorithSAT.h"

class Game {
private:
	bool isRunning;

	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Event event;
public:
	Game();
	~Game();

	void update(const Uint8* keys, float speed, float deltaTime, Uint32 mouseButtons, float mouseX, float mouseY);
	void init(const char* title, int xpos, int ypos, int witdh, int height, bool fullscreen);
	void handleEvents();
	void renderering(float mouseX, float mouseY);

	bool running() { return isRunning; }
	SDL_Renderer* getRenderer() { return renderer; }
};

#endif // !GAME_H

