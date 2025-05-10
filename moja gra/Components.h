#pragma once

#include "ECS.h"
#include "SDL_image.h"
#include "SDL.h"

class VelocityComponent : public Component
{
private:
	float xVel, yVel;
public:
	void init() override
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
public:

	void init() override
	{
		witdhOfPicture = 0;
		heightOfPicture = 0;
	}

	void setSurface(const char* link)
	{
		surface = IMG_Load(link);

		if (!surface)
		{
			SDL_SetError("Error loading image with full path: %s\n", IMG_GetError());
		}
	}

	void setWidthAndHeight(int w, int h)
	{
		witdhOfPicture = w;
		heightOfPicture = h;
	}

	void createTexture(SDL_Renderer* ren)
	{
		if (texture)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
		}

		texture = SDL_CreateTextureFromSurface(ren, surface);
	}

	SDL_Surface* getSurface() { return surface; }
	SDL_Texture* getTexture() { return texture; }
	int getWidth() { return witdhOfPicture; }
	int getHeight() { return heightOfPicture; }
};

class HitboxComponent : public Component
{
private:
	SDL_FRect hitbox;
public:
	float getWidth() { return hitbox.w; }
	float getHeight() { return hitbox.h; }
	float getX() { return hitbox.x; }
	float getY() { return hitbox.y; }

	void init() override
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

	void setPos(float x, float y)
	{
		hitbox.x += x;
		hitbox.y += y;
	}
};