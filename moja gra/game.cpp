#include "Game.h"
#include "Player.h"
#include "ECS.h"
#include "Components.h"
#include <cmath>

using namespace std;

//tworzenie Obiektu Manager
Manager manager;

Game::Game(const char* title, int xpos, int ypos, int witdh, int height, bool fullscreen)
{
	//inicjalizacja okna
	isRunning = true;

	if (!SDL_Init(SDL_INIT_EVERYTHING))
	{
		SDL_SetError("Something gone wrong: %s", SDL_GetError());
	}
	else
	{
		cout << "Initializing..." << endl;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		cout << "SDL_image initialization failed: " << IMG_GetError() << endl;
	}

	window = SDL_CreateWindow(title, xpos, ypos, witdh, height, fullscreen);
	if (!window)
	{
		SDL_SetError("Window isn't initialized properly. %s", SDL_GetError());
	}
	else
	{
		cout << "Window creatred" << endl;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer)
	{
		SDL_SetError("Renderer isn't initialized properly : %s", SDL_GetError());
	}
	else
	{
		cout << "Renderer creatred" << endl;
	}

	//stworzenie obiektu player i wall

	auto& player(manager.addEntity());
	auto& wall(manager.addEntity());

	player.addComponent<HitboxComponent>();
	player.addComponent<VelocityComponent>();
	player.addComponent<SpriteComponent>();
	player.addComponent<AtackComponent>();
	player.addComponent<RotatedRectComponent>();
	player.addComponent<AtttackSpriteComponent>();

	wall.addComponent<HitboxComponent>();

	//ustawianie potrzebnych zmiennych
	player.getComponent<HitboxComponent>().setVariables(200.0f, 200.0f, 32.0f, 32.0f);
	player.getComponent<VelocityComponent>().setVels(100.f, 100.f);
	player.getComponent<SpriteComponent>().setWidthAndHeight(64, 64);
	player.getComponent<AtttackSpriteComponent>().setWidthAndHeight(64, 32);

	wall.getComponent<HitboxComponent>().setVariables(100, 130, 50, 50);
}

Game::~Game()
{ }

//metoda odpowiedzialna za wszystkie wydarzenie dziejace sie podczas gry
void Game::handleEvents()
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			cout << "Closing window..." << endl;
			isRunning = false;
			break;
		}
	}
}

//renderowanie wszytkiego
void Game::renderering(float mouseX, float mouseY)
{
	//okreslanie tla
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	manager.draw(manager, renderer);

	SDL_RenderPresent(renderer);
}

//metoda zawieracjaca inne metody
void Game::update(const Uint8* keys, float speed, float deltaTime, Uint32 mouseButtons, float mouseX, float mouseY)
{
	manager.update(manager, renderer, deltaTime, speed, keys, mouseButtons ,mouseX, mouseY );
	renderering(mouseX, mouseY);
	handleEvents();
}
