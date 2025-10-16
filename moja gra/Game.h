#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include <iostream>
#include <cmath>
#include "AlgorithSAT.h"

class Game {
private:
	bool isRunning;
	static bool startScreen;

	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Event event;

	int screenWidth;
	int screenHeight;
public:
	Game(const char* title, int xpos, int ypos, int witdh, int height, bool fullscreen);
	~Game();

	void update(const Uint8* keys, float deltaTime, Uint32 mouseButtons, float mouseX, float mouseY);
	void handleEvents();
	void renderering(float mouseX, float mouseY, float deltaTime, const Uint8* keys);

	void setWitdhAndHeight(int w, int h) { screenWidth = w; screenHeight = h; }

	void StartScreen(const Uint8* keys);

	static void createEveryObject(SDL_Renderer* ren);

	static void setStartScreen(bool value) { startScreen = value; }

	bool running() { return isRunning; }
	static bool getStartScreen() { return startScreen; }
	SDL_Renderer* getRenderer() { return renderer; }
};

#endif // !GAME_H

