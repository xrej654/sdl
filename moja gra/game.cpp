#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include <cmath>

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
		std::cout << "Initializing..." << std::endl;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		std::cout << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
	}

	window = SDL_CreateWindow(title, xpos, ypos, witdh, height, fullscreen);
	if (!window)
	{
		SDL_SetError("Window isn't initialized properly. %s", SDL_GetError());
	}
	else
	{
		std::cout << "Window creatred" << std::endl;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer)
	{
		SDL_SetError("Renderer isn't initialized properly : %s", SDL_GetError());
	}
	else
	{
		std::cout << "Renderer creatred" << std::endl;
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
	player.addComponent<HealthComponent>();
	player.addComponent<DamageComponent>();
	player.addComponent<SpeedComponent>();

	enemy.addComponent<HitboxComponent>();
	enemy.addComponent<VelocityComponent>();
	enemy.addComponent<DetectedRectComponent>();
	enemy.addComponent<AttackRectComponent>();
	enemy.addComponent<AttackComponent>();
	enemy.addComponent<AttackSpriteComponent>();
	enemy.addComponent<RotatedRectComponent>();
	enemy.addComponent<HealthComponent>();
	enemy.addComponent<DamageComponent>();
	enemy.addComponent<SpeedComponent>();

	//ustawianie potrzebnych zmiennych
	player.getComponent<HitboxComponent>().setVariables(500.0f, 500.0f, 64.0f, 64.0f);
	player.getComponent<VelocityComponent>().setVels(100.f, 100.f);
	player.getComponent<SpriteComponent>().setWidthAndHeight(64, 64);
	player.getComponent<AttackSpriteComponent>().setWidthAndHeight(64, 32);
	player.getComponent<AttackSpriteComponent>().addElementOfAssets("attack", { 
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/4.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/5.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/6.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/7.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/8.png")),
		});

	player.getComponent<HealthComponent>().setHp(100.f);
	player.getComponent<DamageComponent>().setDmg(20.f);
	player.getComponent<SpeedComponent>().setSpeed(100.f);
	player.getComponent<DamageComponent>().setKnockbackPower(60.f);

	enemy.getComponent<HitboxComponent>().setVariables(1100, 500, 64, 64);
	enemy.getComponent<DetectedRectComponent>().setVariables(1100 - 224, 500 - 224, 512, 512);
	enemy.getComponent<AttackRectComponent>().setVariables(1100 - 32, 500 - 32, 128, 128);
	enemy.getComponent<VelocityComponent>().setVels(100.f, 100.f);
	enemy.getComponent<AttackSpriteComponent>().setWidthAndHeight(64, 32);
	enemy.getComponent<AttackSpriteComponent>().addElementOfAssets("attack", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/4.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/5.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/6.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/7.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/atackAnimation/8.png")),
		});

	enemy.getComponent<HealthComponent>().setHp(100.f);
	enemy.getComponent<DamageComponent>().setDmg(10.f);
	enemy.getComponent<SpeedComponent>().setSpeed(50.f);
	enemy.getComponent<DamageComponent>().setKnockbackPower(60.f);

	auto& wall(manager.addObstacle());

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
			std::cout << "Closing window..." << std::endl;
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
void Game::update(const Uint8* keys, float deltaTime, Uint32 mouseButtons, float mouseX, float mouseY)
{
	manager.update(manager, renderer, deltaTime, keys, mouseButtons ,mouseX, mouseY );
	renderering(mouseX, mouseY);
	handleEvents();
}
