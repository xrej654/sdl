#pragma once

#include "ECS.h"
#include "SDL_image.h"
#include "SDL.h"
#include <iostream>

using namespace std;

class VelocityComponent : public Component
{
private:
	float xVel, yVel;
public:
	VelocityComponent()
	{
		xVel = 0;
		yVel = 0;
	}

	float getXVel() { return xVel; }
	float getYVel() { return yVel; }

	void setVels(float x, float y)
	{
		xVel = x;
		yVel = y;
	}
};

class SpriteComponent : public Component
{
private:
	int witdhOfPicture, heightOfPicture;
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = NULL;
	SDL_Rect srcRect;
	SDL_Rect destRect;
public:

	SpriteComponent()
	{
		witdhOfPicture = 0;
		heightOfPicture = 0;
		srcRect = { NULL, NULL, NULL, NULL };
		destRect = { NULL, NULL, NULL, NULL };
	}

	void setSurface(const char* link)
	{
		surface = IMG_Load(link);

		if (!surface)
		{
			SDL_SetError("Error loading image with full path: %s\n", IMG_GetError());
		}
	}

	void setRects(SDL_FRect rect)
	{
		srcRect = { 0, 0, witdhOfPicture, heightOfPicture };  // Rozmiar oryginalnej tekstury
		destRect = { (int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h };
	}

	void setWidthAndHeight(int w, int h)
	{
		witdhOfPicture = w;
		heightOfPicture = h;
	}

	void createTexture(SDL_Renderer* ren)
	{
		if (!surface) {
			 cout << "Surface is NULL, cannot create texture!" <<  endl;
			return;
		}

		if (texture)
		{
			Uint32 format;
			int access, w, h;
			SDL_QueryTexture(texture, &format, &access, &w, &h);

			if (w == surface->w && h == surface->h) {
				return; // Nie tworzymy nowej tekstury, jeœli rozmiar siê nie zmieni³
			}

			SDL_DestroyTexture(texture); // Jeœli rozmiar siê zmieni³, usuwamy star¹ teksturê
			texture = NULL;
		}

		texture = SDL_CreateTextureFromSurface(ren, surface);
	}

	void drawHitbox(SDL_Renderer* ren, SDL_FRect obj, int r = 0, int g = 0, int b = 0, int a = 255)
	{
		SDL_SetRenderDrawColor(ren, r, b, g, a);

		if (!SDL_RenderFillRectF(ren, &obj))
		{
			SDL_SetError("Nie mozna zaladowac hitboxa gracza: %s", SDL_GetError());
		}
	}

	SDL_Surface* getSurface() { return surface; }
	SDL_Texture* getTexture() { return texture; }
	int getWidth() { return witdhOfPicture; }
	int getHeight() { return heightOfPicture; }
	SDL_Rect* getSrcRectReference() { return &srcRect; }
	SDL_Rect* getDestRectReference() { return &destRect; }
	SDL_Rect getSrcRect() { return srcRect; }
	SDL_Rect getDestRect() { return destRect; }
};

class HitboxComponent : public Component
{
private:
	SDL_FRect hitbox;
	SDL_FPoint corners[4];
public:
	float getWidth() { return hitbox.w; }
	float getHeight() { return hitbox.h; }
	float getX() { return hitbox.x; }
	float getY() { return hitbox.y; }
	SDL_FRect getHitbox() { return hitbox; }
	SDL_FRect* getHitboxReference() { return &hitbox; }
	SDL_FPoint* getCorners() { return corners; }

	HitboxComponent()
	{
		hitbox.x = 0;
		hitbox.y = 0;
		hitbox.w = 0;
		hitbox.h = 0;
	}

	void setVariables(float x, float y, float w, float h)
	{
		hitbox.x = x;
		hitbox.y = y;
		hitbox.w = w;
		hitbox.h = h;
	}

	void setPosition(float x, float y)
	{
		hitbox.x += x;
		hitbox.y += y;
	}

	void setCorners(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint p4)
	{
		corners[0] = p1;
		corners[1] = p2;
		corners[2] = p3;
		corners[3] = p4;
	}
};

class AtackComponent : public Component
{
protected:
	static Uint32 lastHitTime;
	bool wasAttacking, canAttack;
	float dx, dy, angle;
public:

	bool getWasAttacking() { return wasAttacking; }
	bool getCanAttacking() { return canAttack; }
	Uint32 getLastHitTime() { return lastHitTime; }
	float getAngle() { return angle; }

	AtackComponent()
	{
		wasAttacking = false;
		canAttack = true;
		lastHitTime = 0;
		dx = 0;
		dy = 0;
		angle = 0;
	}

	void setAttackState(bool attacking)
	{
		wasAttacking = attacking;
		canAttack = !attacking;
	}

	void setLastHitTime(Uint32 ticks)
	{
		lastHitTime = ticks;
	}

	void setDxAndDy(SDL_FRect obj, float mouseX, float mouseY)
	{
		float dx = (obj.x + (obj.w / 2)) - mouseX;
		float dy = (obj.y + (obj.h / 2)) - mouseY;
		angle = atan2(dy, dx);
	}
};

class RotatedRectComponent : public Component
{
private:
	SDL_Point center;
	float rad;
public:
	RotatedRectComponent()
	{
		center = { NULL, NULL };
		rad = 0.0f;
	}

	//funkcja lambdowa
	SDL_FPoint rotate(float x, float y) {
		float dx = x - center.x;
		float dy = y - center.y;
		return {
			center.x + dx * cos(rad) - dy * sin(rad),
			center.y + dx * sin(rad) + dy * cos(rad)
		};
		};

	void setCenter(int x,int y)
	{
		this->center = {x, y};
	}

	void setRad(float rad)
	{
		this->rad = rad;
	}

	SDL_Point* getPtrCenter() { return &center; }
};