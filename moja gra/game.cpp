#include "Game.h"
#include "Player.h"
#include <cmath>

using namespace std;

//tworzenie Obiektu Player
Player player;
SDL_FRect wall = { 100, 130, 50, 50 };

Game::Game()
{ }

Game::~Game()
{ }

//inicjalizacja okno i innych rzeczy pomocnych
void Game::init(const char* title, int xpos, int ypos, int witdh, int height, bool fullscreen)
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

	if (player.getHitboxAtack().x + player.getHitboxAtack().w > wall.x &&
		player.getHitboxAtack().x < wall.x + wall.w &&
		player.getHitboxAtack().y + player.getHitboxAtack().h > wall.y &&
		player.getHitboxAtack().y < wall.y + wall.h && //do tego momentu jest okreslanie samej kolizji
		(currentTime - lastHitTime >= cooldown) && //tutaj czy jest roznica czasowa jeli nie ma przerwy miedzy obrazeniami 0,5s to sie nie wykona(bez tego jest parenascie razy na raz)
		(player.getcanAttack() == player.getWasAtacking())) //sprawdzanie tych dwoch wartosci jesli sa one rowne atakuje jest to tylko przez jedna klatke kiedy atakujemy i gra nie zdarzy zmienic dugiej przed atakiem (kolejnosc bardzo wazna)
	{
		currentTime = SDL_GetTicks();
		cout << "Zadano obrazenia!" << endl;  
		lastHitTime = currentTime; 
	}


}

//renderowanie wszytkiego
void Game::renderering()
{
	//okreslanie tla
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	//okreslanie hitboxa gracza
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);

	if (!SDL_RenderFillRectF(renderer, &player.getHitbox()))
	{
		SDL_SetError("Nie mozna zaladowac hitboxa gracza: %s", SDL_GetError());
	}
	
	//rysowanie hitboxa ataku
	if (player.getWasAtacking() && !player.getcanAttack())
	{

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		player.setHitboxAtackPosition(player.getHitbox().x, player.getHitbox().y);

		if (!SDL_RenderFillRectF(renderer, &player.getHitboxAtack()))
		{
			SDL_SetError("Nie mozna zaladowac hitboxa ataku: %s", SDL_GetError());
		}

		player.setcanAttack(true);
	}

	//rysowanie aktualnej przeszkody
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	if (!SDL_RenderFillRectF(renderer, &wall))
	{
		SDL_SetError("Nie mozna zaladowac hitboxa gracza: %s", SDL_GetError());
	}

	SDL_RenderPresent(renderer);
}

//metoda zawieracjaca inne metody
void Game::update(const Uint8* keys, float speed, float deltaTime, Uint32 mouseButtons)
{
	player.update(keys, speed, deltaTime, mouseButtons, renderer);
	this->Game::renderering();
	this->Game::handleEvents();
}
