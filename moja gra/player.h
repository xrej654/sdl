#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <string>
#include "Game.h"

using namespace std;

class Player {
private:
	SDL_FRect playerHitbox = { 200.0f, 200.0f, 32.0f, 32.0f };
	SDL_FRect hitboxAtack = { playerHitbox.x - 22.f, playerHitbox.y + playerHitbox.h, playerHitbox.w + 22.f, playerHitbox.h - 12.f  + 10.f};
	SDL_Surface* surface = NULL;
	SDL_Texture* texture = NULL;

	SDL_FPoint corners[4];

	struct Velocity {
		float x = 0;
		float y = 0;
	} velocity;

	const char* lastLink;

	bool wasAtacking = false;
	bool canAttack = false;
public:
	Player();
	~Player();

	void update(const Uint8* keys, float speed, float deltaTime, Uint32 mouseButtons, SDL_Renderer* ren);

	void moveHitbox(const Uint8* keys, float speed, float deltaTime, SDL_Renderer* ren);
	void attack(Uint32 mouseButtons);
	void render(SDL_Renderer* renderer);

	//zwracanie i zmienianie wartosci obiektow
	SDL_FRect& getHitbox() { return playerHitbox; }
	SDL_FRect& getHitboxAtack() { return hitboxAtack; }
	SDL_Surface* getSurface() { return surface; }
	SDL_Texture* getTexture() { return texture; }
	SDL_FPoint* getCorners() { return corners; }

	Velocity getVelocity() { return velocity; }

	bool getWasAtacking() { return wasAtacking; }
	bool getcanAttack() { return canAttack; }

	void setcanAttack(bool valueOfBool) { canAttack = valueOfBool; }
	void setPositionAfterCollision(float x, float y) { playerHitbox.x += x; playerHitbox.y += y; }
	void setHitboxAtackPosition(float x, float y) { hitboxAtack.x = x - 10; hitboxAtack.y = y + playerHitbox.h; }
	void setCorners(SDL_FPoint corner1, SDL_FPoint corner2, SDL_FPoint corner3, SDL_FPoint corner4) { corners[0] = corner1;corners[1] = corner2; corners[2] = corner3; corners[3] = corner4; }
	
	void setSurfaceNull() { surface = NULL; }
};

#endif // !PLAYER_H

