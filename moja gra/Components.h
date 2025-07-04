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

	float getXVel() const { return xVel; }
	float getYVel() const { return yVel; }
};

//komponent okreslajacy texture entity
class SpriteComponent : public Component
{
protected:
	int widthOfPicture, heightOfPicture;

	SDL_Texture* texture = NULL;
	SDL_Surface* surface = NULL;

	SDL_Rect srcRect; //prostokat ktory rysujemy
	SDL_Rect destRect; //prostokat gdzie rysujemy
public:
	SpriteComponent()
	{
		widthOfPicture = 0;
		heightOfPicture = 0;
		srcRect = { NULL, NULL, NULL, NULL };
		destRect = { NULL, NULL, NULL, NULL };
	}

	~SpriteComponent() {
		if (texture) SDL_DestroyTexture(texture);
		if (surface) SDL_FreeSurface(surface);
	}

	void setSurface(SDL_Surface* surface)
	{
		if (!surface)
		{
			surface = IMG_Load("assets/blad.png");
		}

		if (this->surface) SDL_FreeSurface(this->surface);

		this->surface = surface;
	}

	void setTexture(SDL_Texture* texture)
	{
		this->texture = texture;
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
			 //cout << "Surface is NULL, cannot create texture!" <<  endl;
			return;
		}

		SDL_DestroyTexture(texture); // Jeœli rozmiar siê zmieni³, usuwamy star¹ teksturê
		texture = NULL;

		texture = SDL_CreateTextureFromSurface(ren, surface);
	}

	SDL_Surface* getSurface() const { return surface; }
	SDL_Texture* getTexture() const { return texture; }
	int getWidth() const { return widthOfPicture; }
	int getHeight() const { return heightOfPicture; }
	SDL_Rect getSrcRect() const { return srcRect; }
	SDL_Rect getDestRect() const { return destRect; }

	SDL_Rect* getSrcRectReference() { return &srcRect; }
	SDL_Rect* getDestRectReference() { return &destRect; }
};

class AnimationComponent : public SpriteComponent
{
private:
	map<string, vector<SDL_Texture*>> assets;

	int currentFrame = -1, allFrames = 0, frameTime = 0;
	Uint32 lastFrameTime = 0;
public:
	AnimationComponent() : SpriteComponent()
	{ }

	void addElementOfAssets(string key, vector<SDL_Texture*> textures)
	{
		assets[key] = textures;
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

			setTexture(assets[key][currentFrame]);
		}
	}
};

//komponnet potrzebny aby obiket mogl miec dwa sprite'y jeden do gracza drugi do ataku
class AttackSpriteComponent : public AnimationComponent
{
public:
	AttackSpriteComponent() : AnimationComponent() { }
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
	Uint32 lastHitTime; //zmienna potrzebna do cooldown'u
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
		corners[0] = {0.f, 0.f };
		corners[1] = { 0.f, 0.f };
		corners[2] = { 0.f, 0.f };
		corners[3] = { 0.f, 0.f };
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
		angle = atan2((obj.y + (obj.h / 2)) - mouseY, (obj.x + (obj.w / 2)) - mouseX);
	}

	void setCorners(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint p4)
	{
		corners[0] = p1;
		corners[1] = p2;
		corners[2] = p3;
		corners[3] = p4;
	}

	bool getWasAttacking() const { return wasAttacking; }
	bool getHasBeenPressed() const { return hasBeenPressed; }
	Uint32 getLastHitTime() const { return lastHitTime; }
	float getAngle() const { return angle; }
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
		center = { 0, 0 };
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

	SDL_Point getCenter() const { return center; }
};

//komponenty to hitboxow przeciwnika
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

	bool getHasSthDetected() const { return hasSthDetected; }
	Uint32 getLastDetectionTime() const { return lastDetectionTime; }
};

class AttackRectComponent : public HitboxComponent
{
public:
	AttackRectComponent() : HitboxComponent()
	{ 
		
	}
};

//komponenty statystyk
class HealthComponent : public Component
{
private:
	float hp, hpBoost, armourHp;
	bool getHit = false;
public:
	HealthComponent()
	{
		hp = 0.f;
		hpBoost = 0.f;
		armourHp = 0.f;
	}

	void setHp(float hp) { this->hp = hp + (hp * (hpBoost / 100)) + armourHp; }
	void setHpBoost(float hpBoost) { this->hpBoost = hpBoost; }
	void setArmourHp(float armourHp) { this->armourHp = armourHp; }
	void subtractHp(float subtractedHp) { hp -= subtractedHp; }
	void addHp(float addedHp) { hp += addedHp; }
	void setGetHit(bool getHit) { this->getHit = getHit; }

	float getHp() const { return hp; }
	float getHpBoost() const { return hpBoost; }
	bool getGetHit() const { return getHit; }
	float getArmourHp() const { return armourHp; }
};

class DamageComponent : public Component
{
private:
	float dmg, dmgBoost, weaponDmg, knockbackPower, knockbackResistance;
public:
	DamageComponent()
	{
		dmg = 0.f;
		dmgBoost = 0.f;
		weaponDmg = 0.f;
		knockbackPower = 0.f;
		knockbackResistance = 0.f;
	}

	void setDmg(float dmg) { this->dmg = dmg + (dmg * (dmgBoost / 100)) + weaponDmg; }
	void setDmgBoost(float dmgBoost) { this->dmgBoost = dmgBoost; }
	void setWeaponDmg(float weaponDmg) { this->weaponDmg = weaponDmg; }
	void setKnockbackPower(float knockbackPower) { this->knockbackPower = knockbackPower - (knockbackPower * (knockbackResistance / 100)); }

	void setKnockbackResistance(float knockbackResistance) 
	{
		if (knockbackResistance <= 100) this->knockbackResistance = knockbackResistance;
		else this->knockbackResistance = 100;
	}

	float getDmg() const { return dmg; }
	float getDmgBoost() const { return dmgBoost; }
	float getWeaponDmg() const { return weaponDmg; }
	float getKnockbackPower() const { return knockbackPower; }
	float getKnockbackResistance() const { return knockbackResistance; }
};

class SpeedComponent : public Component
{
private:
	float speed, speedBoost;
public:
	SpeedComponent()
	{
		speed = 0.f;
		speedBoost = 0.f;
	}

	void setSpeed(float speed) { this->speed = speed; }
	void setSpeedBoost(float speedBoost) { this->speedBoost = speedBoost; }

	float getSpeed() const { return speed; }
	float getSpeedBoost() const { return speedBoost; }
};

class DashComponent : public Component
{
private:
	bool isDashing = false;
	float lenghtOfDash = 120.f;
	int dx = 0, dy = 0;
	int dashCooldown = 0;
	Uint32 lastDashTime = 0;
public:
	DashComponent() {}

	void setIsDashing(bool value) { isDashing = value; }
	void setLenghtOfDash(float lenght) { lenghtOfDash = lenght; }
	void setDirection(int x, int y) { dx = x; dy = y; }
	void setDashCooldown(int time) { dashCooldown = time; }
	void setLastDashTime(int time) { lastDashTime = time; }

	bool getIsDashing() const { return isDashing; }
	float getLenghtOfDash() const { return lenghtOfDash; }
	int getDx() const { return dx; }
	int getDy() const { return dy; }
	int getDashCooldown() const { return dashCooldown; }
	int getLastDashTime() const { return lastDashTime; }
};