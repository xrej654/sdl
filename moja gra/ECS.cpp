#include "ECS.h"
#include "Components.h"
#include <string>
#include "mapy.h"

template<typename E>

//funkcja opymalizacji kodu (kod se powtarzal)
static void handleCalculationOfAttacking(E& e, float targetX, float targetY)
{
	if(e->hasComponent<RotatedRectComponent>() && e->hasComponent<HitboxComponent>())
	{
		if(e->hasComponent<AttackComponent>() && e->hasComponent<AttackSpriteComponent>())
		{
			if (SDL_GetTicks() - e->getComponent<AttackComponent>().getLastHitTime() <= 408)
			{
				//okreslenia srodka gracza potrzebne do rogow ataku i obrotu
				e->getComponent<RotatedRectComponent>().setCenter(
					e->getComponent<HitboxComponent>().getX() + (e->getComponent<HitboxComponent>().getWidth() / 2),
					e->getComponent<HitboxComponent>().getY() + (e->getComponent<HitboxComponent>().getHeight() / 2)
				);
				//strona w ktora jest zwrocony atak

				e->getComponent<AttackComponent>().setDxAndDy(e->getComponent<HitboxComponent>().getHitbox(), targetX, targetY);

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

		if(e->hasComponent<ShootingComponent>() && e->hasComponent<ShootingSpriteComponent>())
		{
			if (e->getComponent<ShootingComponent>().getHasShooted())
			{
				//okreslenia srodka gracza potrzebne do rogow ataku i obrotu
				e->getComponent<RotatedRectComponent>().setCenter(
					e->getComponent<ShootingComponent>().getStarterPos().x - 24 + (e->getComponent<HitboxComponent>().getWidth() / 2),
					e->getComponent<ShootingComponent>().getStarterPos().y + 48 + (e->getComponent<HitboxComponent>().getHeight() / 2)
				);

				//strona w ktora jest zwrocony atak
				e->getComponent<ShootingComponent>().setDxAndDy(e->getComponent<HitboxComponent>().getHitbox(), targetX, targetY);

				//zmiana kata na radian
				e->getComponent<RotatedRectComponent>().setRad(e->getComponent<ShootingComponent>().getAngle() + 3 * M_PI / 2.0);

				//okreslanie rogow
				e->getComponent<ShootingComponent>().setCorners(

					e->getComponent<RotatedRectComponent>().rotate(
						e->getComponent<ShootingSpriteComponent>().getDestRect().x,
						e->getComponent<ShootingSpriteComponent>().getDestRect().y
					),

					e->getComponent<RotatedRectComponent>().rotate(
						e->getComponent<ShootingSpriteComponent>().getDestRect().x + e->getComponent<ShootingSpriteComponent>().getDestRect().w,
						e->getComponent<ShootingSpriteComponent>().getDestRect().y
					),

					e->getComponent<RotatedRectComponent>().rotate(
						e->getComponent<ShootingSpriteComponent>().getDestRect().x + e->getComponent<ShootingSpriteComponent>().getDestRect().w,
						e->getComponent<ShootingSpriteComponent>().getDestRect().y + e->getComponent<ShootingSpriteComponent>().getDestRect().h
					),

					e->getComponent<RotatedRectComponent>().rotate(
						e->getComponent<ShootingSpriteComponent>().getDestRect().x,
						e->getComponent<ShootingSpriteComponent>().getDestRect().y + e->getComponent<ShootingSpriteComponent>().getDestRect().h
					)
				);
			}
		}
	}
}

//systemy polegaja na petli we vector'ze z entity'ami
//po tym sa if'y sprawdzajacy czy sa komponenty aby nie bylo bledu ze wykorzystuje komponenty na obikie ktory nie ma tych komponentow

void Systems::playerMovementSystem(Manager& manager, float deltaTime, const Uint8* keys, SDL_Renderer* ren)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if(e->hasComponent<HealthComponent>() && e->hasComponent<DashComponent>() && e->hasComponent<VelocityComponent>() && e->hasComponent<HitboxComponent>() && e->hasComponent<SpeedComponent>() && e->hasComponent<SpriteComponent>() && e->hasComponent<AnimationComponent>() && e->getIsPlayer())
		{
			auto& dash = e->getComponent<DashComponent>();
			auto& velo = e->getComponent<VelocityComponent>();
			auto& hitbox = e->getComponent<HitboxComponent>();
			auto& anim = e->getComponent<AnimationComponent>();
			auto& sprite = e->getComponent<SpriteComponent>();
			auto& speed = e->getComponent<SpeedComponent>();
			auto& hp = e->getComponent<HealthComponent>();

				//okreslnie kierunku poruszania sie
				float dx = (keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A]);
				float dy = (keys[SDL_SCANCODE_S] - keys[SDL_SCANCODE_W]);

				if (keys[SDL_SCANCODE_SPACE] && SDL_GetTicks() - dash.getLastDashTime() >= dash.getDashCooldown())
				{
					dash.setIsDashing(true);
					dash.setLastDashTime(SDL_GetTicks());
					dash.setDirection(dx, dy);
				}

				if(!dash.getIsDashing())
				{
					string direction = to_string((int)dx) + to_string((int)dy);

					//obliczanie drogi na klatke (na skosk jest sqrt z 2 a nie 1)
					float magnitude = sqrt(dx * dx + dy * dy);
					if (magnitude > 0)
					{
						dx /= magnitude;
						dy /= magnitude;
					}

					//okreslanie szybkosci ruchu
					if(!hp.getIsInKnockback())
					{
						velo.setVels(dx * speed.getSpeed() * deltaTime, dy * speed.getSpeed() * deltaTime);
						hitbox.setPosition(velo.getXVel(), velo.getYVel());
					}

					//robocza zmiana textury podczas ruchu -> mapa z vectorami na klatki
					if (direction == "-1-1")
					{
						//e->getComponent<SpriteComponent>().setSurface(IMG_Load("assets/ruch w lewo gora.png"));
					}
					else if (direction == "-10")
					{
						anim.changeAsset("left", 4, 250, ren);
					}
					else if (direction == "-11")
					{
						//e->getComponent<SpriteComponent>().setSurface(IMG_Load("assets/ruch w lewo dol.png"));
					}
					else if (direction == "0-1")
					{
						anim.changeAsset("up", 4, 250, ren);
					}
					else if (direction == "00")
					{
						anim.changeAsset("no-move", 4, 500, ren);
					}
					else if (direction == "01")
					{
						anim.changeAsset("down", 4, 250, ren);
					}
					else if (direction == "1-1")
					{
						//e->getComponent<SpriteComponent>().setSurface(IMG_Load("assets/ruch w prawo gora.png"));
					}
					else if (direction == "10")
					{
						anim.changeAsset("right", 4, 250, ren);
					}
					else if (direction == "11")
					{
						//e->getComponent<SpriteComponent>().setSurface(IMG_Load("assets/ruch w prawo dol.png"));
					}

					if (!sprite.getTexture() && !anim.getTexture())
					{
						cout << "Failed to create texture!" << endl;
					}
				}
		}
	}
}

void Systems::enemyMovementSystem(Manager& manager, float deltaTime, const Uint8* keys, SDL_Renderer* ren)
{
	for(auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<HealthComponent>() && e->hasComponent<HitboxComponent>() && e->hasComponent<VelocityComponent>() && e->hasComponent<DetectedRectComponent>() && e->hasComponent<SpeedComponent>() && e->getIsEnemy())
		{
			auto& detectedRect = e->getComponent<DetectedRectComponent>();
			auto& velo = e->getComponent<VelocityComponent>();
			auto& hitbox = e->getComponent<HitboxComponent>();
			auto& speed = e->getComponent<SpeedComponent>();
			auto& hp = e->getComponent<HealthComponent>();

			for (auto& en : manager.getVectorOfEntities())
			{
				if (en->getIsPlayer())
				{
					auto& hitboxSc = en->getComponent<HitboxComponent>();
					//wykrywanie i podarzanie z graczem
					if (SDL_HasIntersectionF(detectedRect.getHitboxReference(), hitboxSc.getHitboxReference()))
					{
						//cout << "Wykryto" << endl;
						detectedRect.setLastDetectionTime(SDL_GetTicks());
						detectedRect.setHasSthDetected(true);
					}
					else
					{
						Uint32 currentTime = SDL_GetTicks();
						int cooldown = 3000;
						if (detectedRect.getHasSthDetected() && currentTime - detectedRect.getLastDetectionTime() >= cooldown)
						{
							//cout << "Koniec \n";
							detectedRect.setHasSthDetected(false);
						}
					}

					//poruszanie bota
					if (detectedRect.getHasSthDetected())
					{
							int dx = 0, dy = 0;

							if (hitbox.getX() < hitboxSc.getX() - 10) { dx = 1; }
							else if (hitbox.getX() > hitboxSc.getX() + 10) { dx = -1; }
							else if (hitbox.getX() == hitboxSc.getX() + 10 ||
									hitbox.getX() == hitboxSc.getX() - 10
							) {
								dx = 0;
							}

							if (hitbox.getY() < hitboxSc.getY() - 10) { dy = 1; }
							else if (hitbox.getY() > hitboxSc.getY() + 10) { dy = -1; }
							else if (hitbox.getY() == hitboxSc.getY() + 10 ||
									hitbox.getY() == hitboxSc.getY() - 10
						) {
							dy = 0;
						}

						if (!hp.getIsInKnockback())
						{ 
							velo.setVels(dx * speed.getSpeed() * deltaTime, dy * speed.getSpeed() * deltaTime); 
						}
						//zmiana pozycji
						hitbox.setPosition(velo.getXVel(), velo.getYVel());
						detectedRect.setPosition(velo.getXVel(), velo.getYVel());
						if(e->hasComponent<AttackRectComponent>()) e->getComponent<AttackRectComponent>().setPosition(velo.getXVel(), velo.getYVel());
						if(e->hasComponent<ShootingRectComponent>()) e->getComponent<ShootingRectComponent>().setPosition(velo.getXVel(), velo.getYVel());
					}
				}
			}
		}
	}
}

void Systems::renderingSystem(Manager& manager, SDL_Renderer* ren, float deltaTime)
{
	for (int i = 0; i < 30; i++)
	{
		SDL_Rect mapSrcRect = { 0,0,32,32 };
		SDL_Rect mapDestRect = { 0,0,64,64 };
		for (int j = 0; j < 17; j++)
		{
			mapDestRect = { i * 64, j * 64,64,64 };
			switch (mapRuinBossFight[j][i]) {
			case 1:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(0), &mapSrcRect, &mapDestRect);
				break;
			case 2:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(1), &mapSrcRect, &mapDestRect);
				break;
			case 3:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(2), &mapSrcRect, &mapDestRect);
				break;
			case 4:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(3), &mapSrcRect, &mapDestRect);
				break;
			case 5:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(4), &mapSrcRect, &mapDestRect);
				break;
			case 6:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(5), &mapSrcRect, &mapDestRect);
				break;
			case 7:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(6), &mapSrcRect, &mapDestRect);
				break;
			case 8:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(7), &mapSrcRect, &mapDestRect);
				break;
			case 9:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(8), &mapSrcRect, &mapDestRect);
				break;
			case 10:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(9), &mapSrcRect, &mapDestRect);
				break;
			case 11:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(10), &mapSrcRect, &mapDestRect);
				break;
			case 12:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(11), &mapSrcRect, &mapDestRect);
				break;
			case 13:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(12), &mapSrcRect, &mapDestRect);
				break;
			case 14:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(13), &mapSrcRect, &mapDestRect);
				break;
			case 15:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(14), &mapSrcRect, &mapDestRect);
				break;
			case 16:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(15), &mapSrcRect, &mapDestRect);
				break;
			case 17:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(16), &mapSrcRect, &mapDestRect);
				break;
			case 18:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(17), &mapSrcRect, &mapDestRect);
				break;
			case 19:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(18), &mapSrcRect, &mapDestRect);
				break;
			case 20:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(19), &mapSrcRect, &mapDestRect);
				break;
			case 21:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(20), &mapSrcRect, &mapDestRect);
				break;
			case 22:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(21), &mapSrcRect, &mapDestRect);
				break;
			case 23:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(22), &mapSrcRect, &mapDestRect);
				break;
			case 24:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(23), &mapSrcRect, &mapDestRect);
				break;
			case 25:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(24), &mapSrcRect, &mapDestRect);
				break;
			case 26:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(25), &mapSrcRect, &mapDestRect);
				break;
			case 27:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(26), &mapSrcRect, &mapDestRect);
				break;
			case 28:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(27), &mapSrcRect, &mapDestRect);
				break;
			case 29:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(28), &mapSrcRect, &mapDestRect);
				break;
			case 30:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(29), &mapSrcRect, &mapDestRect);
				break;
			case 31:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(30), &mapSrcRect, &mapDestRect);
				break;
			case 32:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(31), &mapSrcRect, &mapDestRect);
				break;
			case 33:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(32), &mapSrcRect, &mapDestRect);
				break;
			case 34:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(33), &mapSrcRect, &mapDestRect);
				break;
			case 35:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(34), &mapSrcRect, &mapDestRect);
				break;
			}
		}
	}

	for (int i = 0; i < 15; i++)
	{
		SDL_Rect mapSrcRect = { 0,0,32,32 };
		SDL_Rect mapDestRect = { 0,0,128,128 };
		for (int j = 0; j < 8; j++)
		{
			mapDestRect = { i * 128 - 32, j * 128 + 32,128,128 };

			switch (mapRuinBossFight2[j][i]) {
			case 1:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(35), &mapSrcRect, &mapDestRect);
				break;
			case 2:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(36), &mapSrcRect, &mapDestRect);
				break;
			case 3:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(37), &mapSrcRect, &mapDestRect);
				break;
			case 4:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(38), &mapSrcRect, &mapDestRect);
				break;
			case 5:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(39), &mapSrcRect, &mapDestRect);
				break;
			case 6:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(40), &mapSrcRect, &mapDestRect);
				break;
			case 7:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(41), &mapSrcRect, &mapDestRect);
				break;
			case 8:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(42), &mapSrcRect, &mapDestRect);
				break;
			case 9:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(43), &mapSrcRect, &mapDestRect);
				break;
			case 10:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(44), &mapSrcRect, &mapDestRect);
				break;
			case 11:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(45), &mapSrcRect, &mapDestRect);
				break;
			case 12:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(46), &mapSrcRect, &mapDestRect);
				break;
			case 13:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(47), &mapSrcRect, &mapDestRect);
				break;
			case 14:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(48), &mapSrcRect, &mapDestRect);
				break;
			case 15:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(49), &mapSrcRect, &mapDestRect);
				break;
			case 16:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(50), &mapSrcRect, &mapDestRect);
				break;
			case 17:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(51), &mapSrcRect, &mapDestRect);
				break;
			case 18:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(52), &mapSrcRect, &mapDestRect);
				break;
			case 19:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(53), &mapSrcRect, &mapDestRect);
				break;
			case 20:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(54), &mapSrcRect, &mapDestRect);
				break;
			case 21:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(55), &mapSrcRect, &mapDestRect);
				break;
			case 22:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(56), &mapSrcRect, &mapDestRect);
				break;
			case 23:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(57), &mapSrcRect, &mapDestRect);
				break;
			case 24:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(58), &mapSrcRect, &mapDestRect);
				break;
			case 25:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(59), &mapSrcRect, &mapDestRect);
				break;
			case 26:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(60), &mapSrcRect, &mapDestRect);
				break;
			case 27:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(61), &mapSrcRect, &mapDestRect);
				break;
			case 28:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(62), &mapSrcRect, &mapDestRect);
				break;
			case 29:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(63), &mapSrcRect, &mapDestRect);
				break;
			case 30:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(64), &mapSrcRect, &mapDestRect);
				break;
			case 31:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(65), &mapSrcRect, &mapDestRect);
				break;
			case 32:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(66), &mapSrcRect, &mapDestRect);
				break;
			case 33:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(67), &mapSrcRect, &mapDestRect);
				break;
			case 34:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(68), &mapSrcRect, &mapDestRect);
				break;
			case 35:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(69), &mapSrcRect, &mapDestRect);
				break;
			case 36:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(70), &mapSrcRect, &mapDestRect);
				break;
			case 37:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(71), &mapSrcRect, &mapDestRect);
				break;
			case 38:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(72), &mapSrcRect, &mapDestRect);
				break;
			case 39:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(73), &mapSrcRect, &mapDestRect);
				break;
			case 40:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(74), &mapSrcRect, &mapDestRect);
				break;
			case 41:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(75), &mapSrcRect, &mapDestRect);
				break;
			case 42:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(76), &mapSrcRect, &mapDestRect);
				break;
			case 43:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(77), &mapSrcRect, &mapDestRect);
				break;
			case 44:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(78), &mapSrcRect, &mapDestRect);
				break;
			case 45:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(79), &mapSrcRect, &mapDestRect);
				break;
			case 46:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(80), &mapSrcRect, &mapDestRect);
				break;
			case 47:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(81), &mapSrcRect, &mapDestRect);
				break;
			case 48:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(82), &mapSrcRect, &mapDestRect);
				break;
			case 49:
				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(83), &mapSrcRect, &mapDestRect);
				break;
			case 50:
				mapDestRect = { i * 128, j * 128 + 32,64,64 };

				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(84), &mapSrcRect, &mapDestRect);
				break;
			case 51:
				mapDestRect = { i * 128, j * 128 + 32,64,64 };

				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(85), &mapSrcRect, &mapDestRect);
				break;
			case 52:
				mapDestRect = { i * 128, j * 128 + 32,64,64 };

				SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(86), &mapSrcRect, &mapDestRect);
				break;
			}
		}
	}

	for (auto& e : manager.getVectorOfObstacles())
	{
		//if (!e->hasComponent<SpriteComponent>() && e->hasComponent<HitboxComponent>()) e->getComponent<HitboxComponent>().drawHitbox(ren, e->getComponent<HitboxComponent>().getHitbox(), 0, 0, 255);
	}

	for (auto& e : manager.getVectorOfEntities())
	{
		//rysowanie hitboxa jesli nie ma sprite'a
		if (!e->hasComponent<SpriteComponent>() && e->hasComponent<HitboxComponent>()) e->getComponent<HitboxComponent>().drawHitbox(ren, e->getComponent<HitboxComponent>().getHitbox(), 0, 0, 255, 100);
		if (!e->hasComponent<SpriteComponent>() && e->hasComponent<DetectedRectComponent>()) e->getComponent<DetectedRectComponent>().drawHitbox(ren, e->getComponent<DetectedRectComponent>().getHitbox(), 255, 255, 0, 100);
		if (!e->hasComponent<SpriteComponent>() && e->hasComponent<AttackRectComponent>()) e->getComponent<AttackRectComponent>().drawHitbox(ren, e->getComponent<AttackRectComponent>().getHitbox(), 255, 0, 0, 130);
		if (!e->hasComponent<SpriteComponent>() && e->hasComponent<ShootingRectComponent>()) e->getComponent<ShootingRectComponent>().drawHitbox(ren, e->getComponent<ShootingRectComponent>().getHitbox(), 200, 0, 0, 130);

		if (e->hasComponent<AnimationComponent>() && e->hasComponent<HitboxComponent>())
		{
			auto& hitbox = e->getComponent<HitboxComponent>();
			auto& anim = e->getComponent<AnimationComponent>();

			//ustawianie srcrect'ow i destrect'ow na texture
			anim.setRects(hitbox.getHitbox());

			//rysowanie
			if (!anim.getTexture())
			{
				cout << "Texture is NULL, cannot render!" << endl;
				return;
			}

			if (SDL_RenderCopy(ren, anim.getTexture(), anim.getSrcRectReference(), anim.getDestRectReference()) != 0) {
				cout << "Error during rendering texture: %s\n" << SDL_GetError() << endl;
			}
		}

		if (e->hasComponent<HitboxComponent>())
		{
			auto& hitbox = e->getComponent<HitboxComponent>();

			if (e->hasComponent<AttackComponent>() && e->hasComponent<AttackSpriteComponent>())
			{
				auto& attack = e->getComponent<AttackComponent>();
				auto& attackSpr = e->getComponent<AttackSpriteComponent>();
				
				attackSpr.setRects({
					hitbox.getX() - 32.f,
					hitbox.getY() + hitbox.getHeight(),
					128,64
					});

				//rysowanie ataku
				if (SDL_GetTicks() - attack.getLastHitTime() <= 408 && e->hasComponent<RotatedRectComponent>())
				{
					auto& rotatedRect = e->getComponent<RotatedRectComponent>();
					//kolor okreslajacy rogi (do debbugu)
					SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // Czarny kolor

					//robocze okreslenie zdjecia ataku -> animacja
					attackSpr.changeAsset("attack", 8, 51, ren);

					//dobry punkt obrotu textury (jest dobrze dla rogow ale nie dla textury xd)
					SDL_Point centerOfAPlayerWithAttackOffset =
					{
						rotatedRect.getCenter().x - attackSpr.getDestRect().x,
						rotatedRect.getCenter().y - attackSpr.getDestRect().y
					};

					//obracanie ataku wzgledem myszy
					if (SDL_RenderCopyEx(ren, attackSpr.getTexture(), attackSpr.getSrcRectReference(), attackSpr.getDestRectReference(), (attack.getAngle() * 180 / M_PI) + 90, &centerOfAPlayerWithAttackOffset, SDL_FLIP_NONE) != 0) {
						cout << "Error during rendering texture: %s\n" << SDL_GetError() << endl;
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

			//rysowanie strzaly
			if (e->hasComponent<ShootingComponent>())
			{
				SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // Czarny kolor

				if (e->getComponent<ShootingComponent>().getHasShooted() && e->hasComponent<ShootingSpriteComponent>() && e->hasComponent<RotatedRectComponent>())
				{
					auto& shoot = e->getComponent<ShootingComponent>();
					auto& shootSpr = e->getComponent<ShootingSpriteComponent>(); 
					auto& rotatedRect = e->getComponent<RotatedRectComponent>();

					float arrowSpeed = shoot.getSpeed() * deltaTime * 10;

					shoot.setReducedDistance(shoot.getReducedDistance() - arrowSpeed);

					shootSpr.setRects({
						shoot.getStarterPos().x,
						shoot.getReducedDistance(),
						16,48
						});

					SDL_Point centerOfAPlayerWithAttackOffset =
					{
						rotatedRect.getCenter().x - shootSpr.getDestRect().x,
						rotatedRect.getCenter().y - shootSpr.getDestRect().y
					};

					//obracanie ataku wzgledem myszy
					if (SDL_RenderCopyEx(ren, shootSpr.getTexture(), shootSpr.getSrcRectReference(), shootSpr.getDestRectReference(), (shoot.getAngle() * 180 / M_PI) - 90, &centerOfAPlayerWithAttackOffset, SDL_FLIP_NONE) != 0) {
						cout << "Error during rendering texture: %s\n" << SDL_GetError() << endl;
					}

					if (shoot.getStarterPos().y - shoot.getReducedDistance() >= shoot.getRange())
					{
						cout << "Koniec\n";
						shoot.setHasShooted(false);
					}

					/*SDL_FPoint* attackCorners = e->getComponent<ShootingComponent>().getCorners();
					for (int i = 0; i < 4; i++) {
						SDL_Rect pointRect = {
							(int)(attackCorners[i].x - 2),
							(int)(attackCorners[i].y - 2),
							4, 4
						};
						SDL_RenderFillRect(ren, &pointRect);*/
				}
			}
		}
	}
}

void Systems::playerAttackSystem(Manager& manager, Uint32 mouseButtons, float mouseX, float mouseY)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<AttackComponent>() && e->hasComponent<ShootingComponent>() && e->getIsPlayer())
		{
			//zmienne potrzebne do limitowania atkow (brak spamienia co klatke)
			Uint32 cooldown = 700;
			
			auto& attack = e->getComponent<AttackComponent>();
			auto& shoot = e->getComponent<ShootingComponent>();

			if ((mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT))
				&& (SDL_GetTicks() - attack.getLastHitTime() >= cooldown)
				&& !attack.getHasBeenPressed()
				&& !shoot.getHasShooted()) // Przyciœniêcie przycisku
			{
				// Oznaczenie, ¿e gracz zaatakowa³
				attack.setHasBeenPressed(true);
				cout << "ATAK" << endl;
				attack.setLastHitTime(SDL_GetTicks());
			}
			else if (!(mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)))
			{
				attack.setHasBeenPressed(false);
			}

			handleCalculationOfAttacking(e, mouseX, mouseY);

		}
	}
}

void Systems::enemyAttackSystem(Manager& manager, Uint32 mouseButtons, float mouseX, float mouseY)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<AttackRectComponent>() && e->hasComponent<AttackComponent>() && e->getIsEnemy())
		{
			auto& attack = e->getComponent<AttackComponent>();
			auto& attackRect = e->getComponent<AttackRectComponent>();

			for (auto& en : manager.getVectorOfEntities())
			{
				if (en->getIsPlayer() && en->hasComponent<HitboxComponent>())
				{
					auto& hitboxSc = en->getComponent<HitboxComponent>();

					Uint32 cooldown = 700;
					//wykrywanie i atakowanie gracza
					if (SDL_HasIntersectionF(attackRect.getHitboxReference(), hitboxSc.getHitboxReference())
						&& !e->hasComponent<ShootingComponent>()
						&& (SDL_GetTicks() - attack.getLastHitTime() >= cooldown))
					{
						cout << "atak" << endl;
						attack.setLastHitTime(SDL_GetTicks());
					}

					handleCalculationOfAttacking(e, hitboxSc.getX() + (hitboxSc.getWidth() / 2), hitboxSc.getY() + (hitboxSc.getHeight() / 2));
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
		if (e->hasComponent<DamageComponent>() && e->hasComponent<AttackComponent>())
		{
			//okreslenie cooldown'u i pobranie rogow ataku
			Uint32 cooldown = 400;

			auto& dmg = e->getComponent<DamageComponent>();
			auto& attack = e->getComponent<AttackComponent>();

			//for na drugi obiekt aby sprawdzac kolizje miedzy dwoma obiektami
			for (auto& en : manager.getVectorOfEntities())
			{
				if (e != en && en->hasComponent<HitboxComponent>() && en->hasComponent<HealthComponent>())
				{
					auto& hpSc = en->getComponent<HealthComponent>();
					auto& hitboxSc = en->getComponent<HitboxComponent>();

					SDL_FPoint* attackCorners = attack.getCorners(); // 4 rogi po obrocie

					//sprawdzanie kolizji po obrotu strict'e pod atak
					if (rotatedRectCollides(attackCorners, hitboxSc.getHitbox()) &&
						SDL_GetTicks() - attack.getLastHitTime() <= 408 &&
						!hpSc.getGetHit())
					{
						cout << "Zadano obrazenia!" << endl;

						currentTime = SDL_GetTicks();

						hpSc.subtractHp(dmg.getMeleeDmg());
						cout << hpSc.getHp() << endl;

						if (hpSc.getHp() <= 0)
						{
							if (en->getIsPlayer()) cout << "Zginales \n";
							else if (en->getIsEnemy()) cout << "Zabiles \n";

							en->destroy();
						}
						else
						{
							hpSc.setIsInKnockback(true);
						}

						hpSc.setGetHit(true);
					}

					if (e->hasComponent<ShootingComponent>())
					{
						auto& shoot = e->getComponent<ShootingComponent>();
						auto& hpSc = en->getComponent<HealthComponent>();
						auto& hitboxSc = en->getComponent<HitboxComponent>();

						SDL_FPoint* arrowCorners = shoot.getCorners(); // 4 rogi po obrocie

						if (rotatedRectCollides(arrowCorners, hitboxSc.getHitbox()) &&
							shoot.getHasShooted())
						{
							cout << "Zadano obrazenia!" << endl;

							shoot.setHasShooted(false);
							shoot.setLastShootTime(currentTime);

							currentTime = SDL_GetTicks();

							hpSc.subtractHp(dmg.getArrowDmg());
							cout << hpSc.getHp() << endl;

							if (hpSc.getHp() <= 0)
							{
								if (en->getIsPlayer()) cout << "Zginales \n";
								else if (en->getIsEnemy()) cout << "Zabiles \n";

								en->destroy();
							}
							else
							{
								hpSc.setGetShooted(true);
								hpSc.setIsInKnockback(true);
							}
						}
					}
				}
			}
		}

		//jest to odzielone od tamtego warunku poniewaz moga byc obiekty bez ataku, zycia, obrazen lub jednej z tych 
		//np zwierzeta beda mialy tylko zycie i przez to bede one przechodzic przez sciany jesli nie bylo tego rozdzielenia
		if (e->hasComponent<HitboxComponent>() && e->hasComponent<VelocityComponent>())
		{
			auto& hitbox = e->getComponent<HitboxComponent>();
			auto& velo = e->getComponent<VelocityComponent>();

			for (auto& en : manager.getVectorOfObstacles())
			{
				auto& hitboxSc = en->getComponent<HitboxComponent>();
				
				if (SDL_HasIntersectionF(hitbox.getHitboxReference(), hitboxSc.getHitboxReference()))
				{
					//cout << "Sciana\n";
					hitbox.setPosition(-velo.getXVel(), -velo.getYVel());
					if (e->hasComponent<DetectedRectComponent>())
					{
						e->getComponent<DetectedRectComponent>().setPosition(-velo.getXVel(), -velo.getYVel());
						if(e->hasComponent<AttackRectComponent>()) e->getComponent<AttackRectComponent>().setPosition(-velo.getXVel(), -velo.getYVel());
						if(e->hasComponent<ShootingRectComponent>()) e->getComponent<ShootingRectComponent>().setPosition(-velo.getXVel(), -velo.getYVel());
					}
				}
			}
		}
	}
}

void Systems::knockbackSystem(Manager& manager)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if(e->hasComponent<DamageComponent>())
		{
			auto& dmg = e->getComponent<DamageComponent>();

			for (auto& en : manager.getVectorOfEntities())
			{
				if (en->hasComponent<HitboxComponent>() && en->hasComponent<HealthComponent>() && e->hasComponent<AttackComponent>())
				{
					auto& hpSc = en->getComponent<HealthComponent>();
					auto& hitboxSc = en->getComponent<HitboxComponent>();
					auto& attack = e->getComponent<AttackComponent>();

					if (e != en && hpSc.getIsInKnockback())
					{
						float valueOfKnonckback = dmg.getKnockbackPower();
						float angle = 0;

						if (hpSc.getGetHit())
						{
							angle = (attack.getAngle() * 180 / M_PI) + 180;
						}

						if (hpSc.getGetShooted() && e->hasComponent<ShootingComponent>())
						{
							angle = (e->getComponent<ShootingComponent>().getAngle() * 180 / M_PI) + 180;
						}

						float x = cos(angle * M_PI / 180);
						float y = sin(angle * M_PI / 180);

						hitboxSc.setPosition(x * valueOfKnonckback, y * valueOfKnonckback);

						if (en->getIsEnemy() && en->hasComponent<DetectedRectComponent>())
						{
							if (en->hasComponent<AttackRectComponent>())
							{
								en->getComponent<AttackRectComponent>().setPosition(x * valueOfKnonckback, y * valueOfKnonckback);
							}

							en->getComponent<DetectedRectComponent>().setPosition(x * valueOfKnonckback, y * valueOfKnonckback);

							if (en->hasComponent<ShootingRectComponent>())
							{
								en->getComponent<ShootingRectComponent>().setPosition(x * valueOfKnonckback, y * valueOfKnonckback);
							}
						}
						
						hpSc.setIsInKnockback(false);
					}
				}
			}
		}
	}
}

void Systems::dashSystem(Manager& manager)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->getIsPlayer() && e->hasComponent<DashComponent>() && e->hasComponent<HitboxComponent>() && e->getComponent<DashComponent>().getIsDashing())
		{
			auto& dash = e->getComponent<DashComponent>();
			auto& hitbox = e->getComponent<HitboxComponent>();

			float lenght = dash.getLenghtOfDash();

			static int actualCount = 0;
			int maxCount = 15;

			hitbox.setPosition(dash.getDx() * lenght / maxCount, dash.getDy() * lenght / maxCount);
			actualCount++;

			if (actualCount >= maxCount)
			{
				actualCount = 0;
				dash.setIsDashing(false);
			}
		}
	}
}

void Systems::playerShootingSystem(Manager& manager, Uint32 mouseButtons, float mouseX, float mouseY)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		Uint32 cooldown = 2000;
		if (e->hasComponent<ShootingComponent>() && e->hasComponent<HitboxComponent>() && e->getIsPlayer())
		{
			auto& shoot = e->getComponent<ShootingComponent>();
			auto& hitbox = e->getComponent<HitboxComponent>();

			if ((mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT))
				&& !shoot.getHasBeenPressed()
				&& (SDL_GetTicks() - shoot.getLastShootTime() >= cooldown))
			{
				shoot.setStarterPos(hitbox.getX() + 24, hitbox.getY() - 48);

				shoot.setHasBeenPressed(true);
				shoot.setHasShooted(true);
				shoot.setLastShootTime(SDL_GetTicks());

				shoot.setReducedDistance(shoot.getStarterPos().y);

				handleCalculationOfAttacking(e, mouseX, mouseY);

				shoot.setAngle();

				cout << "Strzal \n";
			}
			else if (!(mouseButtons && SDL_BUTTON(SDL_BUTTON_RIGHT)))
			{
				shoot.setHasBeenPressed(false);
			}
		}
	}
}

void Systems::enemyShootingSystem(Manager& manager, Uint32 mouseButtons, float mouseX, float mouseY)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<HitboxComponent>() && e->hasComponent<ShootingComponent>() && e->hasComponent<ShootingRectComponent>() && e->getIsEnemy())
		{
			for (auto& en : manager.getVectorOfEntities())
			{
				if (en->hasComponent<HitboxComponent>() && en->getIsPlayer())
				{
					auto& hitboxSc = en->getComponent<HitboxComponent>();
					auto& hitbox = e->getComponent<HitboxComponent>();
					auto& shootingRect = e->getComponent<ShootingRectComponent>();
					auto& shoot = e->getComponent<ShootingComponent>();

					handleCalculationOfAttacking(e, hitboxSc.getX() + (hitboxSc.getWidth() / 2), hitboxSc.getY() + (hitboxSc.getHeight() / 2));

					Uint32 cooldown = 2100;
					if (SDL_HasIntersectionF(shootingRect.getHitboxReference(), hitboxSc.getHitboxReference())
						&& !shoot.getHasShooted()
						&& (SDL_GetTicks() - shoot.getLastShootTime() >= cooldown))
					{
						cout << "fire" << endl;
						shoot.setStarterPos(hitbox.getX() + 24, hitbox.getY() - 48);

						shoot.setHasShooted(true);
						shoot.setLastShootTime(SDL_GetTicks());

						shoot.setReducedDistance(shoot.getStarterPos().y);

						shoot.setAngle();
					}
				}
			}
		}
	}
}

void Systems::healthSystem(Manager& manager)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<HealthComponent>())
		{
			e->getComponent<HealthComponent>().timerOfGetHit();
		}
	}
}