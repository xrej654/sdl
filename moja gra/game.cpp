#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include "mapy.h"
#include <cmath>

//tworzenie Obiektu Manager
Manager manager;

//funkcje tworzacze przeciwnkow
void addMeleeEnemy(Manager& manager, SDL_Renderer* ren, int x, int y)
{
	auto& enemy(manager.addEntity());
	enemy.setIsEnemy();

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
	enemy.addComponent<KnockbackComponent>();
	enemy.addComponent<AnimationComponent>();

	enemy.getComponent<HitboxComponent>().setVariables(x, y, 64, 64);
	enemy.getComponent<DetectedRectComponent>().setVariables(x - 224, y - 224, 512, 512);
	enemy.getComponent<AttackRectComponent>().setVariables(x - 32, y - 32, 128, 128);
	enemy.getComponent<VelocityComponent>().setVels(100.f, 100.f);
	enemy.getComponent<AttackSpriteComponent>().setWidthAndHeight(64, 32);
	enemy.getComponent<AttackSpriteComponent>().addElementOfAssets("attack", {
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/atackAnimation/1.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/atackAnimation/2.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/atackAnimation/3.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/atackAnimation/4.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/atackAnimation/5.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/atackAnimation/6.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/atackAnimation/7.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/atackAnimation/8.png")),
		});

	enemy.getComponent<AnimationComponent>().addElementOfAssets("no-move", {
	SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/no-move/1.png")),
	SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/no-move/2.png")),
	SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/no-move/3.png")),
	SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/no-move/4.png")),
		});
	enemy.getComponent<AnimationComponent>().addElementOfAssets("up", {
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/up/1.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/up/2.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/up/3.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/up/4.png")),
		});
	enemy.getComponent<AnimationComponent>().addElementOfAssets("down", {
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/down/1.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/down/2.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/down/3.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/down/4.png")),
		});
	enemy.getComponent<AnimationComponent>().addElementOfAssets("left", {
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/left/1.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/left/2.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/left/3.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/left/4.png")),
		});
	enemy.getComponent<AnimationComponent>().addElementOfAssets("right", {
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/right/1.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/right/2.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/right/3.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/right/4.png")),
		});
	enemy.getComponent<AnimationComponent>().addElementOfAssets("attack-no-move", {
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/no-move/1.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/no-move/2.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/no-move/3.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/no-move/4.png"))
		});
	enemy.getComponent<AnimationComponent>().addElementOfAssets("attack-left", {
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/left/1.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/left/2.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/left/3.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/left/4.png"))
		});
	enemy.getComponent<AnimationComponent>().addElementOfAssets("attack-right", {
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/right/1.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/right/2.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/right/3.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/right/4.png"))
		});
	enemy.getComponent<AnimationComponent>().addElementOfAssets("attack-up", {
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/up/1.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/up/2.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/up/3.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/up/4.png"))
		});
	enemy.getComponent<AnimationComponent>().addElementOfAssets("attack-down", {
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/down/1.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/down/2.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/down/3.png")),
		SDL_CreateTextureFromSurface(ren, IMG_Load("assets/Enemy/Meele/attack/down/4.png"))
		});

	enemy.getComponent<HealthComponent>().setHp(100.f);
	enemy.getComponent<HealthComponent>().setMaxHp(100.f);
	enemy.getComponent<DamageComponent>().setArrowDmg(5.f);
	enemy.getComponent<DamageComponent>().setMeleeDmg(10.f);
	enemy.getComponent<SpeedComponent>().setSpeed(50.f);
	enemy.getComponent<DamageComponent>().setKnockbackPower(60.f);
}

void addDistanceEnemy(Manager& manager, SDL_Renderer* ren, int x, int y)
{
	auto& enemy(manager.addEntity());
	enemy.setIsEnemy();

	enemy.addComponent<HitboxComponent>();
	enemy.addComponent<VelocityComponent>();
	enemy.addComponent<RotatedRectComponent>();
	enemy.addComponent<DetectedRectComponent>();
	enemy.addComponent<HealthComponent>();
	enemy.addComponent<DamageComponent>();
	enemy.addComponent<SpeedComponent>();
	enemy.addComponent<ShootingComponent>();
	enemy.addComponent<ShootingSpriteComponent>();
	enemy.addComponent<ShootingRectComponent>();
	enemy.addComponent<KnockbackComponent>();

	enemy.getComponent<HitboxComponent>().setVariables(x, y, 64, 64);
	enemy.getComponent<DetectedRectComponent>().setVariables(x - 224, y - 224, 512, 512);
	enemy.getComponent<ShootingRectComponent>().setVariables(x - 224, y - 224, 512, 512);
	enemy.getComponent<VelocityComponent>().setVels(100.f, 100.f);
	enemy.getComponent<ShootingSpriteComponent>().setTexture(SDL_CreateTextureFromSurface(ren, IMG_Load("assets/arrow.png")));
	enemy.getComponent<ShootingSpriteComponent>().setWidthAndHeight(16, 48);

	enemy.getComponent<HealthComponent>().setHp(100.f);
	enemy.getComponent<HealthComponent>().setMaxHp(100.f);
	enemy.getComponent<DamageComponent>().setArrowDmg(5.f);
	enemy.getComponent<SpeedComponent>().setSpeed(50.f);
	enemy.getComponent<DamageComponent>().setKnockbackPower(60.f);
}

Game::Game(const char* title, int xpos, int ypos, int witdh, int height, bool fullscreen)
{
	//inicjalizacja okna
	isRunning = true;

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

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	//dodawanie zdjec asetow

	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/001.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/002.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/003.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/004.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/005.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/006.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/007.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/008.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/009.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/010.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/011.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/012.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/013.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/014.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/015.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/016.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/017.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/018.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/019.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/020.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/021.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/022.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/023.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/024.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/stone ruin/025.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/grass/026.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/grass/027.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/grass/028.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/grass/029.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/grass/030.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/grass/031.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/grass/032.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/grass/033.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/grass/034.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/bases/grass/035.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/008.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/009.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/010.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/011.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/012.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/013.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/014.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/015.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/016.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/017.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/018.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/019.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/020.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/021.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/022.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/023.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/024.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/025.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/026.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/027.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/028.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/029.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/030.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/031.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/032.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/033.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/034.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/035.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/036.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/037.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/038.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/039.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/040.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/041.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/042.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/043.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/044.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/045.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/046.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/047.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/048.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/049.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/050.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/051.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/052.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/053.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/054.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/055.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/wall/stone ruin/056.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/stone/005.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/stone/006.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/obstacles/stone/007.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/HUD icons/heal1.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/HUD icons/heal2.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/HUD icons/dash1.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/HUD icons/dash2.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/HUD icons/arrow1.png")));
	manager.addTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/HUD icons/arrow2.png")));

	//tworzenie gracza
	auto& player(manager.addEntity());

	manager.setPlayer(&player);

	player.setIsPlayer();

	player.addComponent<HitboxComponent>();
	player.addComponent<VelocityComponent>();
	player.addComponent<SpriteComponent>();
	player.addComponent<AttackComponent>();
	player.addComponent<RotatedRectComponent>();
	player.addComponent<AttackSpriteComponent>();
	player.addComponent<HealthComponent>();
	player.addComponent<DamageComponent>();
	player.addComponent<SpeedComponent>();
	player.addComponent<AnimationComponent>();
	player.addComponent<DashComponent>();
	player.addComponent<ShootingComponent>();
	player.addComponent<ShootingSpriteComponent>();	
	player.addComponent<KnockbackComponent>();	

	//ustawianie potrzebnych zmiennych
	player.getComponent<HitboxComponent>().setVariables(500.0f, 500.0f, 64.0f, 64.0f);
	player.getComponent<VelocityComponent>().setVels(100.f, 100.f);
	player.getComponent<SpriteComponent>().setWidthAndHeight(64, 64);
	player.getComponent<AnimationComponent>().setWidthAndHeight(64, 64);
	player.getComponent<AttackSpriteComponent>().setWidthAndHeight(64, 32);
	player.getComponent<DashComponent>().setDashCooldown(2500);
	player.getComponent<ShootingSpriteComponent>().setTexture(SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/arrow.png")));
	player.getComponent<ShootingSpriteComponent>().setWidthAndHeight(16, 48);

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

	player.getComponent<AnimationComponent>().addElementOfAssets("no-move", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/no-move/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/no-move/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/no-move/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/no-move/4.png")),
		});
	player.getComponent<AnimationComponent>().addElementOfAssets("up", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/up/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/up/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/up/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/up/4.png")),
		});
	player.getComponent<AnimationComponent>().addElementOfAssets("down", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/down/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/down/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/down/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/down/4.png")),
		});
	player.getComponent<AnimationComponent>().addElementOfAssets("left", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/left/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/left/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/left/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/left/4.png")),
		});
	player.getComponent<AnimationComponent>().addElementOfAssets("right", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/right/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/right/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/right/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/right/4.png")),
		});
	player.getComponent<AnimationComponent>().addElementOfAssets("attack-no-move", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/no-move/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/no-move/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/no-move/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/no-move/4.png"))
		});
	player.getComponent<AnimationComponent>().addElementOfAssets("attack-left", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/left/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/left/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/left/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/left/4.png"))
		});
	player.getComponent<AnimationComponent>().addElementOfAssets("attack-right", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/right/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/right/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/right/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/right/4.png"))
		});
	player.getComponent<AnimationComponent>().addElementOfAssets("attack-up", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/up/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/up/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/up/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/up/4.png"))
		});
	player.getComponent<AnimationComponent>().addElementOfAssets("attack-down", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/down/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/down/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/down/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/attack/down/4.png"))
		});
	player.getComponent<AnimationComponent>().addElementOfAssets("shoot-no-move", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/no-move/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/no-move/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/no-move/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/no-move/4.png"))
		});
	player.getComponent<AnimationComponent>().addElementOfAssets("shoot-left", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/left/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/left/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/left/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/left/4.png"))
		});
	player.getComponent<AnimationComponent>().addElementOfAssets("shoot-right", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/right/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/right/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/right/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/right/4.png"))
		});
	player.getComponent<AnimationComponent>().addElementOfAssets("shoot-up", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/up/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/up/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/up/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/up/4.png"))
		});
	player.getComponent<AnimationComponent>().addElementOfAssets("shoot-down", {
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/down/1.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/down/2.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/down/3.png")),
		SDL_CreateTextureFromSurface(renderer, IMG_Load("assets/Player/shoot/down/4.png"))
		});


	player.getComponent<HealthComponent>().setHp(100.f);
	player.getComponent<HealthComponent>().setMaxHp(100.f);
	player.getComponent<HealthComponent>().setPotionCooldown(10000);
	player.getComponent<DamageComponent>().setMeleeDmg(20.f);
	player.getComponent<DamageComponent>().setArrowDmg(15.f);
	player.getComponent<SpeedComponent>().setSpeed(100.f);
	player.getComponent<DamageComponent>().setKnockbackPower(60.f);

	//tworzenie scian i przeciwnikow
	for (int i = 0; i < 15; i++)
	{
		SDL_Rect mapSrcRect = { 0,0,32,32 };
		SDL_Rect mapDestRect = { 0,0,128,128 };
		for (int j = 0; j < 8; j++)
		{
			mapDestRect = { i * 128 - 32, j * 128 + 32,128,128 };
			if (mapRuinBossFight2[j][i] == 50 || mapRuinBossFight2[j][i] == 51 || mapRuinBossFight2[j][i] ==52)
			{
				mapDestRect = { i * 128, j * 128 + 32,64,64 };

				auto& wall(manager.addObstacle());

				wall.addComponent<HitboxComponent>();

				wall.getComponent<HitboxComponent>().setVariables(mapDestRect.x, mapDestRect.y, mapDestRect.w, mapDestRect.h);
			}
			else if (mapRuinBossFight2[j][i] != 0)
			{
				auto& wall(manager.addObstacle());

				wall.addComponent<HitboxComponent>();

				wall.getComponent<HitboxComponent>().setVariables(mapDestRect.x, mapDestRect.y, mapDestRect.w, mapDestRect.h);
			}
		}
	}

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			if (mapRuinBossFightEntities[j][i] == 1)
			{
				//addMeleeEnemy(manager, renderer, i * 64, j * 64);
			}
			if (mapRuinBossFightEntities[j][i] == 2)
			{
				addDistanceEnemy(manager, renderer, i * 64, j * 64);

			 }
		}
	}
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
			for (auto& obj : manager.getVectorOfEntities())
			{
				obj->destroy();
			}
			for(auto& obj : manager.getVectorOfObstacles())
			{
				obj->destroy();
			}
			break;
		}
	}
}

//renderowanie wszytkiego
void Game::renderering(float mouseX, float mouseY, float deltaTime)
{
	//okreslanie tla
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	manager.draw(manager, renderer, deltaTime, screenWidth, screenHeight);

	SDL_RenderPresent(renderer);
}

//metoda zawieracjaca inne metody
void Game::update(const Uint8* keys, float deltaTime, Uint32 mouseButtons, float mouseX, float mouseY)
{
	manager.update(manager, renderer, deltaTime, keys, mouseButtons ,mouseX, mouseY );
	renderering(mouseX, mouseY, deltaTime);
	handleEvents();
}
