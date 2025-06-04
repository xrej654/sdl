#include "ECS.h"
#include "Components.h"
#include <string>

//systemy polegaja na petli we vector'ze z entity'ami
//po tym sa if'y sprawdzajacy czy sa komponenty aby nie bylo bledu ze wykorzystuje komponenty na obikie ktory nie ma tych komponentow

void Systems::movementSystem(Manager& manager, float deltaTime, float speed, const Uint8* keys, SDL_Renderer* ren)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<HitboxComponent>() && e->hasComponent<VelocityComponent>() && e->hasComponent<SpriteComponent>())
		{
			//okreslnie kierunku poruszania sie
			float dx = (keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A]);
			float dy = (keys[SDL_SCANCODE_S] - keys[SDL_SCANCODE_W]);

			string direction = to_string((int)dx) + to_string((int)dy);

			//obliczanie drogi na klatke (na skosk jest sqrt z 2 a nie 1)
			float magnitude = sqrt(dx * dx + dy * dy);
			if (magnitude > 0)
			{
				dx /= magnitude;
				dy /= magnitude;
			}

			//okreslanie szybkosci ruchu
			e->getComponent<VelocityComponent>().setVels(dx * speed * deltaTime, dy * speed * deltaTime);
			e->getComponent<HitboxComponent>().setPosition(e->getComponent<VelocityComponent>().getXVel(), e->getComponent<VelocityComponent>().getYVel());

			//robocza zmiana textury podczas ruchu -> mapa z vectorami na klatki
			if (direction == "-1-1")
			{
				e->getComponent<SpriteComponent>().setSurface(IMG_Load("assets/ruch w lewo gora.png"));
			}
			else if (direction == "-10") 
			{
				e->getComponent<SpriteComponent>().setSurface(IMG_Load("assets/ruch w lewo.png"));
			}
			else if (direction == "-11")
			{
				e->getComponent<SpriteComponent>().setSurface(IMG_Load("assets/ruch w lewo dol.png"));
			}
			else if (direction == "0-1")
			{
				e->getComponent<SpriteComponent>().setSurface(IMG_Load("assets/ruch w gore.png"));
			}
			else if (direction == "00") 
			{
				e->getComponent<SpriteComponent>().setSurface(IMG_Load("assets/ruch w brak.png"));
			}
			else if (direction == "01")
			{
				e->getComponent<SpriteComponent>().setSurface(IMG_Load("assets/ruch w dol.png"));
			}
			else if (direction == "1-1") 
			{
				e->getComponent<SpriteComponent>().setSurface(IMG_Load("assets/ruch w prawo gora.png"));
			}
			else if (direction == "10") 
			{
				e->getComponent<SpriteComponent>().setSurface(IMG_Load("assets/ruch w prawo.png"));
			}
			else if (direction == "11") 
			{
				e->getComponent<SpriteComponent>().setSurface(IMG_Load("assets/ruch w prawo dol.png"));
			}

			e->getComponent<SpriteComponent>().createTexture(ren);

			if (!e->getComponent<SpriteComponent>().getTexture())
			{
				cout << "Failed to create texture!" << endl;
			}
		}
	}
}

void Systems::renderingSystem(Manager& manager, SDL_Renderer* ren)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<SpriteComponent>() && e->hasComponent<HitboxComponent>() && e->hasComponent<AtackComponent>())
		{
			//ustawianie srcrect'ow i destrect'ow na texture
			e->getComponent<SpriteComponent>().setRects(e->getComponent<HitboxComponent>().getHitbox());

			e->getComponent<AtttackSpriteComponent>().setRects({
				e->getComponent<HitboxComponent>().getX() - 16.f,
				e->getComponent<HitboxComponent>().getY() + e->getComponent<HitboxComponent>().getHeight(),
				64,32
				});

			//rysowanie
			if (!e->getComponent<SpriteComponent>().getTexture())
			{
				cout << "Texture is NULL, cannot render!" << endl;
				return;
			}

			if (SDL_RenderCopy(ren, e->getComponent<SpriteComponent>().getTexture(), e->getComponent<SpriteComponent>().getSrcRectReference(), e->getComponent<SpriteComponent>().getDestRectReference()) != 0) {
				cout << "Error during rendering texture: %s\n" << SDL_GetError() << endl;
			}

			//rysowanie ataku
			if (e->getComponent<AtackComponent>().getWasAttacking() && e->hasComponent<RotatedRectComponent>() && e->hasComponent<AtttackSpriteComponent>())
			{
				//kolor okreslajacy rogi (do debbugu)
				//SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // Czarny kolor

				//robocze okreslenie zdjecia ataku -> animacja
				e->getComponent<AtttackSpriteComponent>().setSurface(IMG_Load("assets/atackAnimation/4.png"));
				e->getComponent<AtttackSpriteComponent>().createTexture(ren);

				//dobry punkt obrotu textury (jest dobrze dla rogow ale nie dla textury xd)
				SDL_Point centerOfAPlayerWithAttackOffset =
				{
					e->getComponent<RotatedRectComponent>().getCenter().x - e->getComponent<AtttackSpriteComponent>().getDestRect().x,
					e->getComponent<RotatedRectComponent>().getCenter().y - e->getComponent<AtttackSpriteComponent>().getDestRect().y
				};

				//obracanie ataku wzgledem myszy
				if (SDL_RenderCopyEx(ren, e->getComponent<AtttackSpriteComponent>().getTexture(), e->getComponent<AtttackSpriteComponent>().getSrcRectReference(), e->getComponent<AtttackSpriteComponent>().getDestRectReference(), (e->getComponent<AtackComponent>().getAngle() * 180 / M_PI) + 90, &centerOfAPlayerWithAttackOffset, SDL_FLIP_NONE) != 0) {
					cout << "Error during rendering texture: %s\n" << SDL_GetError() << endl;
				}

				//rysowanie rogow ataku (tez debbug)
				/*SDL_FPoint* attackCorners = e->getComponent<AtackComponent>().getCorners();
				for (int i = 0; i < 4; i++) {
					SDL_Rect pointRect = {
						(int)(attackCorners[i].x - 2),
						(int)(attackCorners[i].y - 2),
						4, 4
					};
					SDL_RenderFillRect(ren, &pointRect);
				}*/
			}
		}

		//rysowanie hitboxa jesli nie ma sprite'a
		if(!e->hasComponent<SpriteComponent>()) e->getComponent<HitboxComponent>().drawHitbox(ren, e->getComponent<HitboxComponent>().getHitbox(), 0, 0, 0);
	}
}

void Systems::atackSystem(Manager& manager, Uint32 mouseButtons, float mouseX, float mouseY)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<AtackComponent>() && e->hasComponent<RotatedRectComponent>() && e->hasComponent<HitboxComponent>() && e->hasComponent<SpriteComponent>())
		{
			//zmienne potrzebne do limitowania atkow (brak spamienia co klatke)
			Uint32 cooldown = 500;

			if ((mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)) && (SDL_GetTicks() - e->getComponent<AtackComponent>().getLastHitTime() >= cooldown)) // Przyciœniêcie przycisku
			{
				// Oznaczenie, ¿e gracz zaatakowa³
				e->getComponent<AtackComponent>().setAttackState(true);
				cout << "ATAK" << endl;
				e->getComponent<AtackComponent>().setLastHitTime(SDL_GetTicks());

				//okreslenia srodka gracza potrzebne do rogow ataku i obrotu
				e->getComponent<RotatedRectComponent>().setCenter(
					e->getComponent<HitboxComponent>().getX() + (e->getComponent<HitboxComponent>().getWidth() / 2),
					e->getComponent<HitboxComponent>().getY() + (e->getComponent<HitboxComponent>().getHeight() / 2)
				);

				//strona w ktora jest zwrocony atak
				e->getComponent<AtackComponent>().setDxAndDy(e->getComponent<HitboxComponent>().getHitbox(), mouseX, mouseY);

				//zmiana kata na radian
				e->getComponent<RotatedRectComponent>().setRad(e->getComponent<AtackComponent>().getAngle() + M_PI / 2.0);

				//okreslanie rogow
				e->getComponent<AtackComponent>().setCorners(

					e->getComponent<RotatedRectComponent>().rotate(
						e->getComponent<AtttackSpriteComponent>().getDestRect().x,
						e->getComponent<AtttackSpriteComponent>().getDestRect().y
					),

					e->getComponent<RotatedRectComponent>().rotate(
						e->getComponent<AtttackSpriteComponent>().getDestRect().x + e->getComponent<AtttackSpriteComponent>().getDestRect().w,
						e->getComponent<AtttackSpriteComponent>().getDestRect().y
					),

					e->getComponent<RotatedRectComponent>().rotate(
						e->getComponent<AtttackSpriteComponent>().getDestRect().x + e->getComponent<AtttackSpriteComponent>().getDestRect().w,
						e->getComponent<AtttackSpriteComponent>().getDestRect().y + e->getComponent<AtttackSpriteComponent>().getDestRect().h
					),

					e->getComponent<RotatedRectComponent>().rotate(
						e->getComponent<AtttackSpriteComponent>().getDestRect().x,
						e->getComponent<AtttackSpriteComponent>().getDestRect().y + e->getComponent<AtttackSpriteComponent>().getDestRect().h
					)
				);
			}
		}
	}
}

void Systems::collisionSystem(Manager& manager)
{
	Uint32 currentTime = SDL_GetTicks();
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<HitboxComponent>() && e->hasComponent<AtackComponent>())
		{
			//okreslenie cooldown'u i pobranie rogow ataku
			Uint32 cooldown = 500;

			SDL_FPoint* attackCorners = e->getComponent<AtackComponent>().getCorners(); // 4 rogi po obrocie

			//for na drugi obiekt aby sprawdzac kolizje miedzy dwoma obiektami
			for (auto& en : manager.getVectorOfEntities())
			{
				if (e != en)
				{
					if (e->hasComponent<AtackComponent>())
					{
						//sprawdzanie kolizji po obrotu strict'e pod atak
						if (rotatedRectCollides(attackCorners, en->getComponent<HitboxComponent>().getHitbox()) &&
							(currentTime - e->getComponent<AtackComponent>().getLastHitTime() >= cooldown) &&
							e->getComponent<AtackComponent>().getWasAttacking())
						{
							currentTime = SDL_GetTicks();
							cout << "Zadano obrazenia!" << endl;
							e->getComponent<AtackComponent>().setLastHitTime(currentTime);
							e->getComponent<AtackComponent>().setAttackState(false);
						}
					}

					//sprawdzanie kolizji dla poruszania sie
					if (SDL_HasIntersectionF(e->getComponent<HitboxComponent>().getHitboxReference(), en->getComponent<HitboxComponent>().getHitboxReference()) && e->hasComponent<VelocityComponent>())
					{
						cout << "Sciana" << endl;
						e->getComponent<HitboxComponent>().setPosition(-e->getComponent<VelocityComponent>().getXVel(), -e->getComponent<VelocityComponent>().getYVel());
					}
				}
			}
		}
	}
}
