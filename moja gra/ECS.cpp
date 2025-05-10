#include "ECS.h"
#include "Components.h"
#include <string>

void Systems::movmentSystem(Manager& manager, float deltaTime, float speed, const Uint8* keys, SDL_Renderer* ren)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<HitboxComponent>() && e->hasComponent<VelocityComponent>())
		{
			float dx = (keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A]);
			float dy = (keys[SDL_SCANCODE_S] - keys[SDL_SCANCODE_W]);

			float magnitude = sqrt(dx * dx + dy * dy);
			if (magnitude > 0)
			{
				dx /= magnitude;
				dy /= magnitude;
			}

			string direction = to_string((int)dx) + to_string((int)dy);

			e->getComponent<VelocityComponent>().setVels(dx * speed * deltaTime, dy * speed * deltaTime);
			e->getComponent<HitboxComponent>().setPos(e->getComponent<VelocityComponent>().getXVel(), e->getComponent<VelocityComponent>().getYVel());

			if (direction == "-1-1")
			{
				e->getComponent<SpriteComponent>().setSurface("assets/ruch w lewo gora.png");
			}
			else if (direction == "-10") {

				e->getComponent<SpriteComponent>().setSurface("assets/ruch w lewo.png");
			}
			else if (direction == "-11")
			{
				e->getComponent<SpriteComponent>().setSurface("assets/ruch w lewo dol.png");
			}
			else if (direction == "0-1")
			{
				e->getComponent<SpriteComponent>().setSurface("assets/ruch w gore.png");
			}
			else if (direction == "00") 
			{
				e->getComponent<SpriteComponent>().setSurface("assets/ruch w brak.png");
			}
			else if (direction == "01")
			{
				e->getComponent<SpriteComponent>().setSurface("assets/ruch w dol.png");
			}
			else if (direction == "1-1") 
			{
				e->getComponent<SpriteComponent>().setSurface("assets/ruch w prawo gora.png");
			}
			else if (direction == "10") 
			{
				e->getComponent<SpriteComponent>().setSurface("assets/ruch w prawo.png");
			}
			else if (direction == "11") 
			{
				e->getComponent<SpriteComponent>().setSurface("assets/ruch w prawo dol.png");
			}

			e->getComponent<SpriteComponent>().createTexture(ren);

			if (!e->getComponent<SpriteComponent>().getTexture())
			{
				std::cerr << "Failed to create texture!" << std::endl;
			}

			SDL_FreeSurface(e->getComponent<SpriteComponent>().getSurface());
			e->getComponent<SpriteComponent>().setSurface(NULL);
		}
	}
}

void Systems::renderingSystem(Manager& manager, SDL_Renderer* ren)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		SDL_Rect srcRect = { 0, 0, e->getComponent<SpriteComponent>().getWidth(), e->getComponent<SpriteComponent>().getHeight()};  // Rozmiar oryginalnej tekstury
		SDL_Rect destRect = { (int)e->getComponent<HitboxComponent>().getX(), (int)e->getComponent<HitboxComponent>().getY(), (int)e->getComponent<HitboxComponent>().getWidth(), (int)e->getComponent<HitboxComponent>().getHeight() };  // Pozycja na ekranie

		if (!e->getComponent<SpriteComponent>().getTexture())
		{
			cout << "Texture is NULL, cannot render!" << endl;
			return;
		}

		if (SDL_RenderCopy(ren, e->getComponent<SpriteComponent>().getTexture(), &srcRect, &destRect) != 0) {
			printf("Error during rendering texture: %s\n", SDL_GetError());
		}

	}
}