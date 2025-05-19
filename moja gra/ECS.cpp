#include "ECS.h"
#include "Components.h"
#include <string>

void Systems::movementSystem(Manager& manager, float deltaTime, float speed, const Uint8* keys, SDL_Renderer* ren)
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
			e->getComponent<HitboxComponent>().setPosition(e->getComponent<VelocityComponent>().getXVel(), e->getComponent<VelocityComponent>().getYVel());

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
				cout << "Failed to create texture!" << endl;
			}

			if (e->getComponent<SpriteComponent>().getSurface()) {
				SDL_FreeSurface(e->getComponent<SpriteComponent>().getSurface());
				e->getComponent<SpriteComponent>().setSurface(NULL);
			}
		}
	}
}

void Systems::renderingSystem(Manager& manager, SDL_Renderer* ren)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		
		e->getComponent<SpriteComponent>().setRects(e->getComponent<HitboxComponent>().getHitbox());

		if (!e->getComponent<SpriteComponent>().getTexture())
		{
			cout << "Texture is NULL, cannot render!" << endl;
			return;
		}

		if (SDL_RenderCopy(ren, e->getComponent<SpriteComponent>().getTexture(), e->getComponent<SpriteComponent>().getSrcRectReference(), e->getComponent<SpriteComponent>().getDestRectReference()) != 0) {
			cout << "Error during rendering texture: %s\n" << SDL_GetError() << endl;
		}

		if (e->getComponent<AtackComponent>().getWasAttacking() && !e->getComponent<AtackComponent>().getCanAttacking())
		{
			if (SDL_RenderCopyEx(ren, e->getComponent<SpriteComponent>().getTexture(), e->getComponent<SpriteComponent>().getSrcRectReference(), e->getComponent<SpriteComponent>().getDestRectReference(), (e->getComponent<AtackComponent>().getAngle() * 180 / M_PI) + 90, e->getComponent<RotatedRectComponent>().getPtrCenter(), SDL_FLIP_NONE) != 0) {
				cout << "Error during rendering texture: %s\n" << SDL_GetError() << endl;
			}

			SDL_FPoint* attackCorners = e->getComponent<HitboxComponent>().getCorners();
			for (int i = 0; i < 4; i++) {
				SDL_Rect pointRect = {
					(int)(attackCorners[i].x - 2),
					(int)(attackCorners[i].y - 2),
					4, 4
				};
				SDL_RenderFillRect(ren, &pointRect);
			}
		}
	}
}

void Systems::atackSystem(Manager& manager, Uint32 mouseButtons)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		//zmienne potrzebne do limitowania atkow (brak spamienia co klatke)
		Uint32 cooldown = 500;

		if ((mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)) && (SDL_GetTicks() - e->getComponent<AtackComponent>().getLastHitTime() >= cooldown)) // Przyciœniêcie przycisku
		{
			// Oznaczenie, ¿e gracz zaatakowa³
			e->getComponent<AtackComponent>().setAttackState(true);
			cout << "ATAK" << endl;
			e->getComponent<AtackComponent>().setLastHitTime(SDL_GetTicks());

			e->getComponent<RotatedRectComponent>().setCenter(
				e->getComponent<HitboxComponent>().getX() + (e->getComponent<HitboxComponent>().getWidth() / 2) - e->getComponent<SpriteComponent>().getDestRect().x,
				e->getComponent<HitboxComponent>().getY() + (e->getComponent<HitboxComponent>().getHeight() / 2) - e->getComponent<SpriteComponent>().getDestRect().y
			);

			e->getComponent<RotatedRectComponent>().setRad(e->getComponent<AtackComponent>().getAngle() + M_PI / 2.0);
			e->getComponent<HitboxComponent>().setCorners(
				e->getComponent<RotatedRectComponent>().rotate(
					e->getComponent<SpriteComponent>().getDestRect().x,
					e->getComponent<SpriteComponent>().getDestRect().y
				),
				e->getComponent<RotatedRectComponent>().rotate(
					e->getComponent<SpriteComponent>().getDestRect().x + e->getComponent<SpriteComponent>().getDestRect().w,
					e->getComponent<SpriteComponent>().getDestRect().y
				),
				e->getComponent<RotatedRectComponent>().rotate(
					e->getComponent<SpriteComponent>().getDestRect().x + e->getComponent<SpriteComponent>().getDestRect().w,
					e->getComponent<SpriteComponent>().getDestRect().y + e->getComponent<SpriteComponent>().getDestRect().h
				),
				e->getComponent<RotatedRectComponent>().rotate(
					e->getComponent<SpriteComponent>().getDestRect().x,
					e->getComponent<SpriteComponent>().getDestRect().y + e->getComponent<SpriteComponent>().getDestRect().h
				)
			);
		}
		else if (!(mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT))) // Kiedy przycisk jest zwolniony
		{
			//Zwolnienie przycisku mozna zaatakowac
			e->getComponent<AtackComponent>().setAttackState(false);
		}
	}
}

void Systems::collisionSystem(Manager& manager)
{
	Uint32 currentTime = SDL_GetTicks();
	for (auto& e : manager.getVectorOfEntities())
	{
		Uint32 cooldown = 500;

		SDL_FPoint* attackCorners = e->getComponent<HitboxComponent>().getCorners(); // 4 rogi po obrocie
		for (auto& en : manager.getVectorOfEntities())
		{
			if (e != en)
			{
				if (e->hasComponent<AtackComponent>())
				{
					if (rotatedRectCollides(attackCorners, en->getComponent<HitboxComponent>().getHitbox()) &&
						(currentTime - e->getComponent<AtackComponent>().getLastHitTime() >= cooldown) &&
						e->getComponent<AtackComponent>().getCanAttacking() == e->getComponent<AtackComponent>().getWasAttacking())
					{
						currentTime = SDL_GetTicks();
						cout << "Zadano obrazenia!" << endl;
						e->getComponent<AtackComponent>().setLastHitTime(currentTime);
					}
				}

				if (SDL_HasIntersectionF(e->getComponent<HitboxComponent>().getHitboxReference(), en->getComponent<HitboxComponent>().getHitboxReference()))
				{
					cout << "Sciana" << endl;
					e->getComponent<HitboxComponent>().setPosition(e->getComponent<VelocityComponent>().getXVel(), e->getComponent<VelocityComponent>().getYVel());
				}

			}
		}
	}
}