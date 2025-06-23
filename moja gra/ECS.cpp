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
			if (e->getIsPlayer())
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

		if (e->hasComponent<HitboxComponent>() && e->hasComponent<VelocityComponent>())
		{
			if (e->getIsEnemy())
			{
				for (auto& en : manager.getVectorOfEntities())
				{
					if (en->getIsPlayer())
					{
						//wykrywanie i podarzanie z graczem
						if (e->hasComponent<DetectedRectComponent>())
						{
							if (SDL_HasIntersectionF(e->getComponent<DetectedRectComponent>().getHitboxReference(), en->getComponent<HitboxComponent>().getHitboxReference()))
							{
								//cout << "Wykryto" << endl;
								e->getComponent<DetectedRectComponent>().setLastDetectionTime(SDL_GetTicks());
								e->getComponent<DetectedRectComponent>().setHasSthDetected(true);
							}
							else
							{
								Uint32 currentTime = SDL_GetTicks();
								int cooldown = 3000;
								if (e->getComponent<DetectedRectComponent>().getHasSthDetected() && currentTime - e->getComponent<DetectedRectComponent>().getLastDetectionTime() >= cooldown)
								{
									//cout << "Koniec \n";
									e->getComponent<DetectedRectComponent>().setHasSthDetected(false);
								}
							}
						}


						//poruszanie bota
						if (e->hasComponent<DetectedRectComponent>() && e->hasComponent<VelocityComponent>() && e->hasComponent<AttackRectComponent>())
						{
							if (e->getComponent<DetectedRectComponent>().getHasSthDetected())
							{
								int dx = 0, dy = 0;

								if (e->getComponent<HitboxComponent>().getX() < en->getComponent<HitboxComponent>().getX() - 10) { dx = 1; }
								else if (e->getComponent<HitboxComponent>().getX() > en->getComponent<HitboxComponent>().getX() + 10) { dx = -1; }
								else if (e->getComponent<HitboxComponent>().getX() == en->getComponent<HitboxComponent>().getX() + 10 ||
									e->getComponent<HitboxComponent>().getX() == en->getComponent<HitboxComponent>().getX() - 10
									) {
									dx = 0;
								}

								if (e->getComponent<HitboxComponent>().getY() < en->getComponent<HitboxComponent>().getY() - 10) { dy = 1; }
								else if (e->getComponent<HitboxComponent>().getY() > en->getComponent<HitboxComponent>().getY() + 10) { dy = -1; }
								else if (e->getComponent<HitboxComponent>().getY() == en->getComponent<HitboxComponent>().getY() + 10 ||
									e->getComponent<HitboxComponent>().getY() == en->getComponent<HitboxComponent>().getY() - 10
									) {
									dy = 0;
								}

								e->getComponent<VelocityComponent>().setVels(dx * (speed / 2) * deltaTime, dy * (speed / 2) * deltaTime);
								//zmiana pozycji
								e->getComponent<HitboxComponent>().setPosition(e->getComponent<VelocityComponent>().getXVel(), e->getComponent<VelocityComponent>().getYVel());
								e->getComponent<DetectedRectComponent>().setPosition(e->getComponent<VelocityComponent>().getXVel(), e->getComponent<VelocityComponent>().getYVel());
								e->getComponent<AttackRectComponent>().setPosition(e->getComponent<VelocityComponent>().getXVel(), e->getComponent<VelocityComponent>().getYVel());

							}
						}
					}
				}
			}
		}
	}
}

void Systems::renderingSystem(Manager& manager, SDL_Renderer* ren)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		//rysowanie hitboxa jesli nie ma sprite'a
		if (!e->hasComponent<SpriteComponent>() && e->hasComponent<HitboxComponent>()) e->getComponent<HitboxComponent>().drawHitbox(ren, e->getComponent<HitboxComponent>().getHitbox(), 0, 0, 255);
		if (!e->hasComponent<SpriteComponent>() && e->hasComponent<DetectedRectComponent>()) e->getComponent<DetectedRectComponent>().drawHitbox(ren, e->getComponent<DetectedRectComponent>().getHitbox(), 255, 255, 0, 100);
		if (!e->hasComponent<SpriteComponent>() && e->hasComponent<AttackRectComponent>()) e->getComponent<AttackRectComponent>().drawHitbox(ren, e->getComponent<AttackRectComponent>().getHitbox(), 255, 0, 0, 130);

		if (e->hasComponent<SpriteComponent>() && e->hasComponent<HitboxComponent>() && e->hasComponent<AttackComponent>())
		{
			//ustawianie srcrect'ow i destrect'ow na texture
			e->getComponent<SpriteComponent>().setRects(e->getComponent<HitboxComponent>().getHitbox());

			e->getComponent<AttackSpriteComponent>().setRects({
				e->getComponent<HitboxComponent>().getX() - 32.f,
				e->getComponent<HitboxComponent>().getY() + e->getComponent<HitboxComponent>().getHeight(),
				128,64
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
			if (e->getComponent<AttackComponent>().getWasAttacking() && e->hasComponent<RotatedRectComponent>() && e->hasComponent<AttackSpriteComponent>())
			{
				//kolor okreslajacy rogi (do debbugu)
				SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // Czarny kolor

				//robocze okreslenie zdjecia ataku -> animacja
				e->getComponent<AttackSpriteComponent>().changeAsset("attack", 8, 51 ,ren);

				//dobry punkt obrotu textury (jest dobrze dla rogow ale nie dla textury xd)
				SDL_Point centerOfAPlayerWithAttackOffset =
				{
					e->getComponent<RotatedRectComponent>().getCenter().x - e->getComponent<AttackSpriteComponent>().getDestRect().x,
					e->getComponent<RotatedRectComponent>().getCenter().y - e->getComponent<AttackSpriteComponent>().getDestRect().y
				};

				//obracanie ataku wzgledem myszy
				if (SDL_RenderCopyEx(ren, e->getComponent<AttackSpriteComponent>().getTexture(), e->getComponent<AttackSpriteComponent>().getSrcRectReference(), e->getComponent<AttackSpriteComponent>().getDestRectReference(), (e->getComponent<AttackComponent>().getAngle() * 180 / M_PI) + 90, &centerOfAPlayerWithAttackOffset, SDL_FLIP_NONE) != 0) {
					//cout << "Error during rendering texture: %s\n" << SDL_GetError() << endl;
				}

				//rysowanie rogow ataku (tez debbug)
				/*SDL_FPoint* attackCorners = e->getComponent<AttackComponent>().getCorners();
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
	}
}

void Systems::atackSystem(Manager& manager, Uint32 mouseButtons, float mouseX, float mouseY)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<AttackComponent>() && e->hasComponent<RotatedRectComponent>() && e->hasComponent<HitboxComponent>() && e->hasComponent<SpriteComponent>())
		{
			if (e->getIsPlayer())
			{
				//zmienne potrzebne do limitowania atkow (brak spamienia co klatke)
				Uint32 cooldown = 600;

				if ((mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT))
					&& (SDL_GetTicks() - e->getComponent<AttackComponent>().getLastHitTime() >= cooldown)
					&& !e->getComponent<AttackComponent>().getHasBeenPressed()) // Przyciœniêcie przycisku
				{
					// Oznaczenie, ¿e gracz zaatakowa³
					e->getComponent<AttackComponent>().setHasBeenPressed(true);
					e->getComponent<AttackComponent>().setWasAttacking(true);
					cout << "ATAK" << endl;
					e->getComponent<AttackComponent>().setLastHitTime(SDL_GetTicks());
				}
				else if (!(mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)))
				{
					e->getComponent<AttackComponent>().setHasBeenPressed(false);
				}

				if (e->getComponent<AttackComponent>().getWasAttacking())
				{
					//okreslenia srodka gracza potrzebne do rogow ataku i obrotu
					e->getComponent<RotatedRectComponent>().setCenter(
						e->getComponent<HitboxComponent>().getX() + (e->getComponent<HitboxComponent>().getWidth() / 2),
						e->getComponent<HitboxComponent>().getY() + (e->getComponent<HitboxComponent>().getHeight() / 2)
					);

					//strona w ktora jest zwrocony atak
					e->getComponent<AttackComponent>().setDxAndDy(e->getComponent<HitboxComponent>().getHitbox(), mouseX, mouseY);

					//zmiana kata na radian
					e->getComponent<RotatedRectComponent>().setRad(e->getComponent<AttackComponent>().getAngle() + M_PI / 2.0);

					//okreslanie rogow
					e->getComponent<AttackComponent>().setCorners(

						e->getComponent<RotatedRectComponent>().rotate(
							e->getComponent<AttackSpriteComponent>().getDestRect().x,
							e->getComponent<AttackSpriteComponent>().getDestRect().y
						),

						e->getComponent<RotatedRectComponent>().rotate(
							e->getComponent<AttackSpriteComponent>().getDestRect().x + e->getComponent<AttackSpriteComponent>().getDestRect().w,
							e->getComponent<AttackSpriteComponent>().getDestRect().y
						),

						e->getComponent<RotatedRectComponent>().rotate(
							e->getComponent<AttackSpriteComponent>().getDestRect().x + e->getComponent<AttackSpriteComponent>().getDestRect().w,
							e->getComponent<AttackSpriteComponent>().getDestRect().y + e->getComponent<AttackSpriteComponent>().getDestRect().h
						),

						e->getComponent<RotatedRectComponent>().rotate(
							e->getComponent<AttackSpriteComponent>().getDestRect().x,
							e->getComponent<AttackSpriteComponent>().getDestRect().y + e->getComponent<AttackSpriteComponent>().getDestRect().h
						)
					);
				}
			}
		}

		if (e->hasComponent<HitboxComponent>() && e->getIsEnemy())
		{
			for (auto& en : manager.getVectorOfEntities())
			{
				if (en->getIsPlayer())
				{
					//wykrywanie i atakowanie gracza
					if (e->hasComponent<AttackRectComponent>())
					{
						if (SDL_HasIntersectionF(e->getComponent<AttackRectComponent>().getHitboxReference(), en->getComponent<HitboxComponent>().getHitboxReference()) && !e->getComponent<AttackRectComponent>().getHasAttacked())
						{
							cout << "atak" << endl;
							e->getComponent<AttackRectComponent>().setLastAttackTime(SDL_GetTicks());
							e->getComponent<AttackRectComponent>().setHasAttacked(true);
						}
						else
						{
							Uint32 currentTime = SDL_GetTicks();
							if (e->getComponent<AttackRectComponent>().getHasAttacked() && currentTime - e->getComponent<AttackRectComponent>().getLastAttackTime() > 600)
							{
								e->getComponent<AttackRectComponent>().setHasAttacked(false);
							}
						}
					}
				}
			}
		}
	}
}

void Systems::collisionSystem(Manager& manager)
{
	Uint32 currentTime = SDL_GetTicks();
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<HitboxComponent>() && e->hasComponent<AttackComponent>() && e->getIsPlayer())
		{
			//okreslenie cooldown'u i pobranie rogow ataku
			Uint32 cooldown = 400;

			SDL_FPoint* attackCorners = e->getComponent<AttackComponent>().getCorners(); // 4 rogi po obrocie

			//for na drugi obiekt aby sprawdzac kolizje miedzy dwoma obiektami
			for (auto& en : manager.getVectorOfEntities())
			{
				if (e != en && en->getIsEnemy())
				{
					//sprawdzanie kolizji po obrotu strict'e pod atak
					if (rotatedRectCollides(attackCorners, en->getComponent<HitboxComponent>().getHitbox()) &&
						(currentTime - e->getComponent<AttackComponent>().getLastHitTime() >= cooldown) &&
						e->getComponent<AttackComponent>().getWasAttacking())
					{
						currentTime = SDL_GetTicks();
						cout << "Zadano obrazenia!" << endl;
						e->getComponent<AttackComponent>().setWasAttacking(false);
						e->getComponent<AttackComponent>().setLastHitTime(currentTime);
						float valueOfKnonckback = 60;

						float angle = (e->getComponent<AttackComponent>().getAngle() * 180 / M_PI) + 180;

						float x = cos(angle * M_PI / 180);
						float y = sin(angle * M_PI / 180);
						
						en->getComponent<HitboxComponent>().setPosition(x * valueOfKnonckback, y * valueOfKnonckback);
						en->getComponent<AttackRectComponent>().setPosition(x * valueOfKnonckback, y * valueOfKnonckback);
						en->getComponent<DetectedRectComponent>().setPosition(x * valueOfKnonckback, y * valueOfKnonckback);
					}
					else if ((currentTime - e->getComponent<AttackComponent>().getLastHitTime() >= cooldown) &&
							e->getComponent<AttackComponent>().getWasAttacking())
					{
						e->getComponent<AttackComponent>().setWasAttacking(false);
					}
				}
			}
		}

		if (e->hasComponent<HitboxComponent>() && e->hasComponent<VelocityComponent>())
		{
			for (auto& en : manager.getVectorOfEntities())
			{
				if (e != en && (!en->getIsEnemy() && !en->getIsPlayer()))
				{
					if (SDL_HasIntersectionF(e->getComponent<HitboxComponent>().getHitboxReference(), en->getComponent<HitboxComponent>().getHitboxReference()))
					{
						cout << "Sciana\n";
						e->getComponent<HitboxComponent>().setPosition(-e->getComponent<VelocityComponent>().getXVel(), -e->getComponent<VelocityComponent>().getYVel());
						if(e->hasComponent<DetectedRectComponent>() && e->hasComponent<AttackRectComponent>())
						{
							e->getComponent<DetectedRectComponent>().setPosition(-e->getComponent<VelocityComponent>().getXVel(),-e->getComponent<VelocityComponent>().getYVel());
							e->getComponent<AttackRectComponent>().setPosition(-e->getComponent<VelocityComponent>().getXVel(), -e->getComponent<VelocityComponent>().getYVel());
						}
					}
				}
			}
		}
	}
}
