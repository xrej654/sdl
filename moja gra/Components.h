#pragma once

#include "ECS.h"
#include "SDL_image.h"
#include "SDL.h"
#include <iostream>

using namespace std;

//wszystkie mozliwe komponenty ktire obiekt moze posiadac

//okresla szybkosc z jaka entity moze sie poruszac
//jest wykorzystywana przy kolizji ze sciana itp. (cofniecie imituje brak ruchu)
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

	void setVels(float x, float y)
	{
		xVel = x;
		yVel = y;
	}

	float getXVel() { return xVel; }
	float getYVel() { return yVel; }
};

//komponent okreslajacy texture entity
class SpriteComponent : public Component
{
private:
	int widthOfPicture, heightOfPicture;

	SDL_Texture* texture = NULL;
	SDL_Surface* surface = NULL;

	SDL_Rect srcRect; //prostokat ktory rysujemy
	SDL_Rect destRect; //prostokat gdzie rysujemy

	map<string, vector<string>> assets;

	int currentFrame = -1, allFrames = 0, frameTime = 0;
	Uint32 lastFrameTime = 0;
public:
	SpriteComponent()
	{
		widthOfPicture = 0;
		heightOfPicture = 0;
		srcRect = { NULL, NULL, NULL, NULL };
		destRect = { NULL, NULL, NULL, NULL };
	}

	void setSurface(SDL_Surface* surface = IMG_Load("assets/blad.png"))
	{
		if (!surface)
		{
			SDL_SetError("Error loading image with full path: %s\n", IMG_GetError());
		}

		if (this->surface) SDL_FreeSurface(this->surface);

		this->surface = surface;
	}

	void setRects(SDL_FRect rect)
	{
		srcRect = { 0, 0, widthOfPicture, heightOfPicture };  // Rozmiar oryginalnej tekstury
		destRect = { (int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h };
	}

	void setWidthAndHeight(int w, int h)
	{
		widthOfPicture = w;
		heightOfPicture = h;
	}

	void createTexture(SDL_Renderer* ren)
	{
		if (!surface) {
			 cout << "Surface is NULL, cannot create texture!" <<  endl;
			return;
		}

		SDL_DestroyTexture(texture); // Jeœli rozmiar siê zmieni³, usuwamy star¹ teksturê
		texture = NULL;

		texture = SDL_CreateTextureFromSurface(ren, surface);
	}

	void addElementOfAssets(string key, vector<string> links)
	{
		assets[key] = links;
	}

	void changeAsset(string key, int frames, int frameTime, SDL_Renderer* ren, Uint32 actualFrameTime = SDL_GetTicks())
	{
		allFrames = frames;
		this->frameTime = frameTime;
		if (actualFrameTime - lastFrameTime >= frameTime)
		{
			currentFrame++;
			if (currentFrame >= allFrames) currentFrame = 0;
			lastFrameTime = actualFrameTime;

			//cout << currentFrame << allFrames <<endl;

			setSurface(IMG_Load(assets[key][currentFrame].c_str()));
			createTexture(ren);
		}
	}

	SDL_Surface* getSurface() { return surface; }
	SDL_Texture* getTexture() { return texture; }
	int getWidth() { return widthOfPicture; }
	int getHeight() { return heightOfPicture; }
	SDL_Rect getSrcRect() { return srcRect; }
	SDL_Rect getDestRect() { return destRect; }

	SDL_Rect* getSrcRectReference() { return &srcRect; }
	SDL_Rect* getDestRectReference() { return &destRect; }
};

//komponnet potrzebny aby obiket mogl miec dwa sprite'y jeden do gracza drugi do ataku
class AttackSpriteComponent : public SpriteComponent
{
public:
	AttackSpriteComponent() : SpriteComponent() { }
};

//komponent okreslajacy komponent
class HitboxComponent : public Component
{
private:
	SDL_FRect hitbox;
public:
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

	void drawHitbox(SDL_Renderer* ren, SDL_FRect obj, int r = 0, int g = 0, int b = 0, int a = 255)
	{
		SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(ren, r, g, b, a);

		if (!SDL_RenderFillRectF(ren, &obj))
		{
			SDL_SetError("Nie mozna zaladowac hitboxa gracza: %s", SDL_GetError());
		}
	}

	float getWidth() { return hitbox.w; }
	float getHeight() { return hitbox.h; }
	float getX() { return hitbox.x; }
	float getY() { return hitbox.y; }
	SDL_FRect getHitbox() { return hitbox; }

	SDL_FRect* getHitboxReference() { return &hitbox; }
};

//komponent do ataku obiektu
class AttackComponent : public Component
{
protected:
	static Uint32 lastHitTime; //zmienna potrzebna do cooldown'u
	bool wasAttacking, hasBeenPressed; //boole do ataku
	float dx, dy, angle; //zmienne potrzebne do obrtu
	SDL_FPoint corners[4]; //rogi ataku
public:
	AttackComponent()
	{
		wasAttacking = false;
		hasBeenPressed = false;
		lastHitTime = 0;
		dx = 0;
		dy = 0;
		angle = 0;
		corners[0] = {NULL, NULL};
		corners[1] = {NULL, NULL};
		corners[2] = {NULL, NULL};
		corners[3] = {NULL, NULL};
	}

	void setWasAttacking(bool attacking)
	{
		wasAttacking = attacking;
	}

	void setHasBeenPressed(bool attacking)
	{
		hasBeenPressed = attacking;
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

	void setCorners(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint p4)
	{
		corners[0] = p1;
		corners[1] = p2;
		corners[2] = p3;
		corners[3] = p4;
	}

	bool getWasAttacking() { return wasAttacking; }
	bool getHasBeenPressed() { return hasBeenPressed; }
	Uint32 getLastHitTime() { return lastHitTime; }
	float getAngle() { return angle; }
	SDL_FPoint* getCorners() { return corners; }
};

//komponent pomagajacy z obrotem ataku bardziej skupiony na obrocie
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

	SDL_Point getCenter() { return center; }
};

class DetectedRectComponent : public HitboxComponent 
{
private:
	bool hasSthDetected = false;
	Uint32 lastDetectionTime;
public:
	DetectedRectComponent() : HitboxComponent()
	{
		lastDetectionTime = 0;
	}

	void setHasSthDetected(bool value)
	{
		hasSthDetected = value;
	}

	void setLastDetectionTime(Uint32 time) { lastDetectionTime = time; }

	bool getHasSthDetected() { return hasSthDetected; }
	Uint32 getLastDetectionTime() { return lastDetectionTime; }
};

class AttackRectComponent : public HitboxComponent
{
private:
	bool hasAttacked = false;
	Uint32 lastAttackTime;
public:
	AttackRectComponent() : HitboxComponent()
	{ 
		lastAttackTime = 0;
	}

	void setHasAttacked(bool value)
	{
		hasAttacked = value;
	}

	void setLastAttackTime(Uint32 time) { lastAttackTime = time; }

	bool getHasAttacked() { return hasAttacked; }
	Uint32 getLastAttackTime() { return lastAttackTime; }
};