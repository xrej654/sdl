#include "player.h"

using namespace std;

Player::Player()
{ }

Player::~Player()
{ }

//metody odpowiedzialne za poruszanie gracza i atakowanie
void Player::moveHitbox(const Uint8* keys, float speed, float deltaTime)
{
	float dx = (keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A]);
	float dy = (keys[SDL_SCANCODE_S] - keys[SDL_SCANCODE_W]);

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

//metoda z innymi metodami
void Player::update(const Uint8* keys, float speed, float deltaTime, Uint32 mouseButtons)
{
	this->moveHitbox(keys, speed, deltaTime);
	this->attack(mouseButtons);
}