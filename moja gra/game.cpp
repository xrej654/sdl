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

	//stworzenie obiektu player i enemy

	auto& enemy(manager.addEntity());
	auto& player(manager.addEntity());
	player.setIsPlayer();
	enemy.setIsEnemy();

	player.addComponent<HitboxComponent>();
	player.addComponent<VelocityComponent>();
	player.addComponent<SpriteComponent>();
	player.addComponent<AttackComponent>();
	player.addComponent<RotatedRectComponent>();
	player.addComponent<AttackSpriteComponent>();

	enemy.addComponent<HitboxComponent>();
	enemy.addComponent<VelocityComponent>();
	enemy.addComponent<DetectedRectComponent>();
	enemy.addComponent<AttackRectComponent>();

	//ustawianie potrzebnych zmiennych
	player.getComponent<HitboxComponent>().setVariables(500.0f, 500.0f, 64.0f, 64.0f);
	player.getComponent<VelocityComponent>().setVels(100.f, 100.f);
	player.getComponent<SpriteComponent>().setWidthAndHeight(64, 64);
	player.getComponent<AttackSpriteComponent>().setWidthAndHeight(64, 32);
	player.getComponent<AttackSpriteComponent>().addElementOfAssets("attack", { 
		"assets/atackAnimation/1.png",
		"assets/atackAnimation/2.png",
		"assets/atackAnimation/3.png",
		"assets/atackAnimation/4.png",
		"assets/atackAnimation/5.png",
		"assets/atackAnimation/6.png",
		"assets/atackAnimation/7.png",
		"assets/atackAnimation/8.png",
		});

	enemy.getComponent<HitboxComponent>().setVariables(1100, 500, 64, 64);
	enemy.getComponent<DetectedRectComponent>().setVariables(1100 - 224, 500 - 224, 512, 512);
	enemy.getComponent<AttackRectComponent>().setVariables(1100 - 32, 500 - 32, 128, 128);
	enemy.getComponent<VelocityComponent>().setVels(100.f, 100.f);

	auto& wall(manager.addEntity());

	wall.addComponent<HitboxComponent>();

	wall.getComponent<HitboxComponent>().setVariables(700.f, 700.f, 64.f, 64.f);

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
