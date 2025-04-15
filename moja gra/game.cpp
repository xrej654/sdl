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
void Game::renderering(float mouseX, float mouseY)
{
	SDL_Rect srcRect; //zmienna okreslajaca co sie rysuje ze zdjecia
	SDL_Rect destRect = { (int)player.getHitbox().x, (int)player.getHitbox().y, (int)player.getHitbox().w, (int)player.getHitbox().h };  // Pozycja na ekranie

	//okreslanie tla
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	float dx = (player.getHitbox().x + (player.getHitbox().w / 2)) - mouseX;
	float dy = (player.getHitbox().y + (player.getHitbox().h / 2)) - mouseY;

	float angle = atan2(dy, dx);

	//dodac save, restor, transfore(na srodek gracza) i dodac sin i cos do pozycji gracza
	
	//rysowanie hitboxa ataku
	if (player.getWasAtacking() && !player.getcanAttack())
	{
		srcRect = { 0,0,70,40 };
		destRect = { (int)player.getHitboxAtack().x, (int)player.getHitboxAtack().y, (int)player.getHitboxAtack().w, (int)player.getHitboxAtack().h };

		SDL_Surface* atak = IMG_Load("assets/atak.png");
		SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, atak);
		int centerOfPlayerX = (player.getHitbox().x + (player.getHitbox().w / 2));
		int centerOfPlayerY = (player.getHitbox().y + (player.getHitbox().h / 2));

		// Tworzymy strukturê SDL_Point
		SDL_Point center = { centerOfPlayerX - destRect.x, centerOfPlayerY - destRect.y };

		SDL_Point* centerPtr = &center;

		if (SDL_RenderCopyEx(renderer, text, &srcRect, &destRect, (angle * 180 / M_PI) + 90, centerPtr, SDL_FLIP_NONE) != 0) {
			printf("Error during rendering texture: %s\n", SDL_GetError());
		}

		SDL_FreeSurface(atak);
		atak = NULL;

		float tempX = player.getHitboxAtack().x - centerOfPlayerX;
		float tempY = player.getHitboxAtack().y - centerOfPlayerY;

		float newX = centerOfPlayerX + (sqrt(tempX * tempX + tempY * tempY)) * cos(angle);

		player.setHitboxAtackPosition(player.getHitbox().x, player.getHitbox().y);

		player.setcanAttack(true);
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
		std::cout << "Texture is NULL, cannot render!" << std::endl;
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
