#include "Game.h"
#include "Player.h"
#include "ECS.h"
#include "Components.h"
#include <cmath>

using namespace std;

//tworzenie Obiektu Player
Player player;
SDL_FRect wall = { 100, 130, 50, 50 };

Game::Game(const char* title, int xpos, int ypos, int witdh, int height, bool fullscreen)
{
	isRunning = true;

	if (!SDL_Init(SDL_INIT_EVERYTHING))
	{
		SDL_SetError("Something gone wrong: %s", SDL_GetError());
	}
	else
	{
		cout << "Initializing..." << endl;
	}

	if (!IMG_Init(IMG_INIT_PNG))
	{
		SDL_SetError("SDL_image initialization failed: %s\n", IMG_GetError());
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

	
	if (player.getHitbox().x + player.getHitbox().w > wall.x &&
		player.getHitbox().x < wall.x + wall.w &&
		player.getHitbox().y + player.getHitbox().h > wall.y &&
		player.getHitbox().y < wall.y + wall.h)
	{
		cout << "Sciana" << endl;
		player.setPositionAfterCollision(-player.getVelocity().x, -player.getVelocity().y);
	}

	static Uint32 lastHitTime = 0;
	Uint32 cooldown = 500;
	Uint32 currentTime = SDL_GetTicks();

	SDL_FPoint* attackCorners = player.getCorners(); // 4 rogi po obrocie

	if (rotatedRectCollides(attackCorners, wall) &&
		(currentTime - lastHitTime >= cooldown) &&
		player.getcanAttack() == player.getWasAtacking())
	{
		currentTime = SDL_GetTicks();
		cout << "Zadano obrazenia!" << endl;
		lastHitTime = currentTime;
	}
	
}

//renderowanie wszytkiego
void Game::renderering(float mouseX, float mouseY)
{
	SDL_Rect srcRect; //zmienna okreslajaca co sie rysuje ze zdjecia
	SDL_Rect destRect = { (int)player.getHitbox().x, (int)player.getHitbox().y, (int)player.getHitbox().w, (int)player.getHitbox().h };  // Pozycja na ekranie

	//okreslanie tla
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// zmienne pomagajace w atakowaniu
	float dx = (player.getHitbox().x + (player.getHitbox().w / 2)) - mouseX;
	float dy = (player.getHitbox().y + (player.getHitbox().h / 2)) - mouseY;
	float angle = atan2(dy, dx);

	// statyczna zmienna do trzymania czasu rozpoczêcia rysowania
	static Uint32 attackDrawStartTime = 0;

	// jeœli gracz w³aœnie zaatakowa³ – zapisujemy czas rozpoczêcia rysowania
	if (player.getWasAtacking() && !player.getcanAttack())
	{
		srcRect = { 0, 0, 70, 40 };
		destRect = { (int)player.getHitboxAtack().x, (int)player.getHitboxAtack().y, (int)player.getHitboxAtack().w, (int)player.getHitboxAtack().h };

		SDL_Surface* atak = NULL;

		atak = IMG_Load("assets/atackAnimation/5.png");

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, atak);

		int centerOfPlayerX = (player.getHitbox().x + (player.getHitbox().w / 2));
		int centerOfPlayerY = (player.getHitbox().y + (player.getHitbox().h / 2));

		SDL_Point center = { centerOfPlayerX - destRect.x, centerOfPlayerY - destRect.y };
		SDL_Point* centerPtr = &center;

		if (SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, (angle * 180 / M_PI) + 90, centerPtr, SDL_FLIP_NONE) != 0) {
			printf("Error during rendering texture: %s\n", SDL_GetError());
		}

		float rad = angle + M_PI / 2.0f;
		auto rotate = [&](float x, float y) -> SDL_FPoint {
			float dx = x - centerOfPlayerX;
			float dy = y - centerOfPlayerY;
			return {
				centerOfPlayerX + dx * cos(rad) - dy * sin(rad),
				centerOfPlayerY + dx * sin(rad) + dy * cos(rad)
			};
			};
		//
		player.setCorners(rotate(destRect.x, destRect.y),
			rotate(destRect.x + destRect.w, destRect.y),
			rotate(destRect.x + destRect.w, destRect.y + destRect.h),
			rotate(destRect.x, destRect.y + destRect.h));

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_FPoint* attackCorners = player.getCorners();
		for (int i = 0; i < 4; i++) {
			SDL_Rect pointRect = {
				(int)(attackCorners[i].x - 2),
				(int)(attackCorners[i].y - 2),
				4, 4
			};
			SDL_RenderFillRect(renderer, &pointRect);
		}

		SDL_FreeSurface(atak);
		SDL_DestroyTexture(texture);
		atak = NULL;
		texture = NULL;

		player.setHitboxAtackPosition(player.getHitbox().x, player.getHitbox().y);
		player.setcanAttack(true); // sygna³ ¿e animacja dzia³a – a nie ¿e gracz znowu zaatakowa³
	}

	//rysowanie aktualnej przeszkody
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//rysowanie textury gracza
	if (!SDL_RenderFillRectF(renderer, &wall))
	{
		SDL_SetError("Nie mozna zaladowac hitboxa gracza: %s", SDL_GetError());
	}

	SDL_FreeSurface(player.getSurface());
	player.setSurfaceNull();

	if (!player.getTexture()) {
		cout << "Texture is NULL, cannot render!" << endl;
		return;
	}

	srcRect = { 0, 0, 64, 64 };  // Rozmiar oryginalnej tekstury
	destRect = { (int)player.getHitbox().x, (int)player.getHitbox().y, (int)player.getHitbox().w, (int)player.getHitbox().h };  // Pozycja na ekranie

	if (SDL_RenderCopy(renderer, player.getTexture(), &srcRect, &destRect) != 0) {
		printf("Error during rendering texture: %s\n", SDL_GetError());
	}

	SDL_RenderPresent(renderer);
}

//metoda zawieracjaca inne metody
void Game::update(const Uint8* keys, float speed, float deltaTime, Uint32 mouseButtons, float mouseX, float mouseY)
{
	player.update(keys, speed, deltaTime, mouseButtons, renderer);
	this->Game::renderering(mouseX, mouseY);
	this->Game::handleEvents();
}
