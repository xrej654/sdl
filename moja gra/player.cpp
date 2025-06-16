#include "player.h"

using namespace std;

Player::Player()
{ }

Player::~Player()
{ }

//metody odpowiedzialne za poruszanie gracza i atakowanie
void Player::moveHitbox(const Uint8* keys, float speed, float deltaTime, SDL_Renderer* ren)
{
	float dx = (keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A]);
	float dy = (keys[SDL_SCANCODE_S] - keys[SDL_SCANCODE_W]);

	//zmienna okreslajaca kierunek ruchu gracza
	string direction = to_string((int)dx) + to_string((int)dy);

	float magnitude = sqrt(dx * dx + dy * dy);
	if (magnitude > 0)
	{
		dx /= magnitude;
		dy /= magnitude;
	}

	velocity.x = dx * speed * deltaTime;
	velocity.y = dy * speed * deltaTime;

	playerHitbox.x += velocity.x;
	playerHitbox.y += velocity.y;

	//zmiana textury w zaleznosci od kierunku

	if (direction == "-1-1" && lastLink != "assets/ruch w lewo gora.png")
	{ 
		if (texture) 
		{ 
			SDL_DestroyTexture(texture);
			texture = NULL; 
		}

		surface = IMG_Load("assets/ruch w lewo gora.png"); 

		if (!surface) 
		{ 
			SDL_SetError("Error loading image with full path: %s\n", IMG_GetError());
		} 
		else 
		{ 
			 
			lastLink = "assets/ruch w lewo gora.png";
		} 
	}
	else if (direction == "-10" && lastLink != "assets/ruch w lewo.png")
	{
		if (texture)
		{
			SDL_DestroyTexture(texture);
			texture = NULL; 
		} 
		
		surface = IMG_Load("assets/ruch w lewo.png"); 
		
		if (!surface) 
		{ 
			SDL_SetError("Error loading image with full path: %s\n", IMG_GetError()); 
		} 
		else 
		{
			 
			lastLink = "assets/ruch w lewo.png"; 
		} 
	}
	else if (direction == "-11" && lastLink != "assets/ruch w lewo dol.png")
	{
		if (texture) 
		{ 
			SDL_DestroyTexture(texture);
			texture = NULL;
		} 
		
		surface = IMG_Load("assets/ruch w lewo dol.png"); 
		
		if (!surface)
		{ 
			SDL_SetError("Error loading image with full path: %s\n", IMG_GetError());
		} 
		else 
		{ 
			 
			lastLink = "assets/ruch w lewo dol.png"; 
		}
	}
	else if (direction == "0-1" && lastLink != "assets/ruch w gore.png")
	{ 
		if (texture) 
		{
			SDL_DestroyTexture(texture);
			texture = NULL; 
		}
		
		surface = IMG_Load("assets/ruch w gore.png");
		
		if (!surface) 
		{ 
			SDL_SetError("Error loading image with full path: %s\n", IMG_GetError());
		}
		else
		{ 
			 
			lastLink = "assets/ruch w gore.png";
		} 
	}
	else if(direction == "00" && lastLink != "assets/ruch w brak.png")
	{
		if (texture)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
		}

		surface = IMG_Load("assets/ruch w brak.png");

		if (!surface)
		{
			SDL_SetError("Error loading image with full path: %s\n", IMG_GetError());
		}
		else
		{

			lastLink = "assets/ruch w brak.png";
		}
	}
	else if (direction == "01" && lastLink != "assets/ruch w dol.png")
	{ 
		if (texture) 
		{ 
			SDL_DestroyTexture(texture);
			texture = NULL;
		} 
		
		surface = IMG_Load("assets/ruch w dol.png"); 
		
		if (!surface)
		{ 
			SDL_SetError("Error loading image with full path: %s\n", IMG_GetError());
		}
		else 
		{ 
			
			lastLink = "assets/ruch w dol.png"; 
		} 
	}
	else if (direction == "1-1" && lastLink != "assets/ruch w prawo gora.png")
	{
		if (texture) 
		{ 
			SDL_DestroyTexture(texture);
			texture = NULL; 
		}
		
		surface = IMG_Load("assets/ruch w prawo gora.png");
		
		if (!surface)
		{ 
			SDL_SetError("Error loading image with full path: %s\n", IMG_GetError());
		}
		else
		{ 
			
			lastLink = "assets/ruch w prawo gora.png";
		} 
	}
	else if (direction == "10" && lastLink != "assets/ruch w prawo.png")
	{ 
		if (texture) 
		{ 
			SDL_DestroyTexture(texture);
			texture = NULL; 
		} 
		
		surface = IMG_Load("assets/ruch w prawo.png");

		if (!surface) 
		{
			SDL_SetError("Error loading image with full path: %s\n", IMG_GetError());
		} 
		else 
		{ 
			 
			lastLink = "assets/ruch w prawo.png";
		}
	}
	else if (direction == "11" && lastLink != "assets/ruch w prawo dol.png")
	{
		if (texture) 
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
		}
		
		surface = IMG_Load("assets/ruch w prawo dol.png");
		
		if (!surface)
		{
			SDL_SetError("Error loading image with full path: %s\n", IMG_GetError());
		}
		else 
		{ 
			
			lastLink = "assets/ruch w prawo dol.png"; 
		}
	}
	//ten else ciagle sie wykonuje poniewaz potem bedzie ciagle null
	else {
		if (texture)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
		}

		surface = IMG_Load(lastLink);

		if (!surface)
		{
			SDL_SetError("Error loading image with full path: %s\n", IMG_GetError());
		}
		else
		{
			
			lastLink = lastLink;
		}
	}

	texture = SDL_CreateTextureFromSurface(ren, surface);
}

void Player::attack(Uint32 mouseButtons)
{
	//zmienne potrzebne do limitowania atkow (brak spamienia co klatke)
	static Uint32 lastHitTime = 0;
	Uint32 cooldown = 500;
	Uint32 currentTime = SDL_GetTicks();

	if ((mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)) && (currentTime - lastHitTime >= cooldown)) // Przyciœniêcie przycisku
	{
		wasAtacking = true; // Oznaczenie, ¿e gracz zaatakowa³
		canAttack = false;   // Mo¿liwoœæ wykonania ataku
		cout << "ATAK" << endl;
		currentTime = SDL_GetTicks();
		lastHitTime = currentTime;
	}
	else if (!(mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT))) // Kiedy przycisk jest zwolniony
	{
		wasAtacking = false; // Resetowanie stanu wciœniêcia
		canAttack = true;   // Uniemo¿liwienie ataku do nastêpnego klikniêcia
	}
}

void Player::render(SDL_Renderer* ren) {
	
}

//metoda z innymi metodami
void Player::update(const Uint8* keys, float speed, float deltaTime, Uint32 mouseButtons, SDL_Renderer* ren)
{
	this->moveHitbox(keys, speed, deltaTime, ren);
	this->attack(mouseButtons);
	this->render(ren);
}