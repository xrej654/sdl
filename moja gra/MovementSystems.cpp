#include "ECS.h"
#include "Components.h"
#include <string>
#include "mapy.h"

template<typename E> bool tryMoveWithCollision(E& e, float dx, float dy, Manager& manager) {
	auto& hitbox = e->getComponent<HitboxComponent>();
	SDL_FRect predicted = hitbox.getHitbox();
	predicted.x += dx;
	predicted.y += dy;

	for (auto& en : manager.getVectorOfObstacles()) {
		auto& hitboxSc = en->getComponent<HitboxComponent>();
		if (SDL_HasIntersectionF(&predicted, hitboxSc.getHitboxReference())) {
			return false; // kolizja
		}
	}

	return true;
}

void Systems::playerMovementSystem(Manager& manager, float deltaTime, const Uint8* keys, SDL_Renderer* ren)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<HealthComponent>() && e->hasComponent<AttackComponent>() && e->hasComponent<ShootingComponent>() && e->hasComponent<DashComponent>() && e->hasComponent<VelocityComponent>() && e->hasComponent<HitboxComponent>() && e->hasComponent<SpeedComponent>() && e->hasComponent<SpriteComponent>() && e->hasComponent<AnimationComponent>() && e->getIsPlayer())
		{
			auto& dash = e->getComponent<DashComponent>();
			auto& velo = e->getComponent<VelocityComponent>();
			auto& hitbox = e->getComponent<HitboxComponent>();
			auto& anim = e->getComponent<AnimationComponent>();
			auto& sprite = e->getComponent<SpriteComponent>();
			auto& speed = e->getComponent<SpeedComponent>();
			auto& hp = e->getComponent<HealthComponent>();
			auto& attack = e->getComponent<AttackComponent>();
			auto& shoot = e->getComponent<ShootingComponent>();

			//okreslnie kierunku poruszania sie
			float dx = (keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A]);
			float dy = (keys[SDL_SCANCODE_S] - keys[SDL_SCANCODE_W]);

			if (keys[SDL_SCANCODE_SPACE] && SDL_GetTicks() - dash.getLastDashTime() >= dash.getDashCooldown())
			{
				dash.setIsDashing(true);
				dash.setLastDashTime(SDL_GetTicks());
				dash.setDirection(dx, dy);
				dash.setTimeLeft(1.0f);
			}

			if (!dash.getIsDashing())
			{
				//uzywany do okreslenie kierunku

				velo.setDirection(to_string((int)dx) + to_string((int)dy));

				//okreslenie ruchu po skosie, unikniecie szybszego ruchu po skosie
				float magnitude = sqrt(dx * dx + dy * dy);
				if (magnitude > 0)
				{
					dx /= magnitude;
					dy /= magnitude;
				}

				//okreslanie szybkosci ruchu
				if (!hp.getIsInKnockback()) //blokada podczas knockbacku
				{
					velo.setVels(dx * speed.getSpeed() * deltaTime, dy * speed.getSpeed() * deltaTime);
					if (tryMoveWithCollision(e, velo.getXVel(), velo.getYVel(), manager))
					{
						hitbox.setPosition(velo.getXVel(), velo.getYVel());
					}
				}

				if (SDL_GetTicks() - attack.getLastHitTime() > 408)
				{
					//robocza zmiana textury podczas ruchu -> mapa z vectorami na klatki
					if (velo.getDirection() == "-1-1")
					{
						anim.changeAsset("up", 4, 250, ren);
						attack.setBorders(225 + 67.5, 225 - 67.5);
						shoot.setBorders(225 + 67.5, 225 - 67.5);
					}
					else if (velo.getDirection() == "-10")
					{
						anim.changeAsset("left", 4, 250, ren);
						attack.setBorders(180 + 67.5, 180 - 67.5);
						shoot.setBorders(180 + 67.5, 180 - 67.5);
					}
					else if (velo.getDirection() == "-11")
					{
						anim.changeAsset("down", 4, 250, ren);
						attack.setBorders(135 + 67.5, 135 - 67.5);
						shoot.setBorders(135 + 67.5, 135 - 67.5);
					}
					else if (velo.getDirection() == "0-1")
					{
						anim.changeAsset("up", 4, 250, ren);
						attack.setBorders(270 + 67.5, 270 - 67.5);
						shoot.setBorders(270 + 67.5, 270 - 67.5);
					}
					else if (velo.getDirection() == "00")
					{
						anim.changeAsset("no-move", 4, 500, ren);
						attack.setBorders(360, 0);
						shoot.setBorders(360, 0);
					}
					else if (velo.getDirection() == "01")
					{
						anim.changeAsset("down", 4, 250, ren);
						attack.setBorders(90 + 67.5, 90 - 67.5);
						shoot.setBorders(90 + 67.5, 90 - 67.5);
					}
					else if (velo.getDirection() == "1-1")
					{
						anim.changeAsset("up", 4, 250, ren);
						attack.setBorders(315 + 67.5 - 360, 315 - 67.5);
						shoot.setBorders(315 + 67.5 - 360, 315 - 67.5);
					}
					else if (velo.getDirection() == "10")
					{
						anim.changeAsset("right", 4, 250, ren);
						attack.setBorders(0 + 67.5, 360 - 67.5);
						shoot.setBorders(0 + 67.5, 360 - 67.5);
					}
					else if (velo.getDirection() == "11")
					{
						anim.changeAsset("down", 4, 250, ren);
						attack.setBorders(45 + 67.5, 45 - 67.5 + 360);
						shoot.setBorders(45 + 67.5, 45 - 67.5 + 360);
					}
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
	for (auto& e : manager.getVectorOfEntities())
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
						//przeciwnik "pamieta" gracza przez jakis czas
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

						//okreslenie kierunku poruszania sie
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

						velo.setDirection(to_string(dx)  + to_string(dy));

						velo.setVels(dx * speed.getSpeed() * deltaTime, dy * speed.getSpeed() * deltaTime);

						if (!hp.getIsInKnockback() && tryMoveWithCollision(e, velo.getXVel(), velo.getYVel(), manager)) //blokada podczas knockbacku
						{
							//zmiana pozycji
							hitbox.setPosition(velo.getXVel(), velo.getYVel());
							detectedRect.setPosition(velo.getXVel(), velo.getYVel());
							if (e->hasComponent<AttackRectComponent>()) e->getComponent<AttackRectComponent>().setPosition(velo.getXVel(), velo.getYVel());
							if (e->hasComponent<ShootingRectComponent>()) e->getComponent<ShootingRectComponent>().setPosition(velo.getXVel(), velo.getYVel());
						}
					}

					if (e->hasComponent<AnimationComponent>())
					{
						cout << velo.getDirection() << endl;

						auto& anim = e->getComponent<AnimationComponent>();
						if (e->hasComponent<ShootingComponent>() && SDL_GetTicks() - e->getComponent<ShootingComponent>().getLastShootTime() <= 601)
						{
							if (velo.getDirection() == "-1-1")
							{
								anim.changeAsset("up", 4, 104, ren);
							}
							else if (velo.getDirection() == "-10")
							{
								anim.changeAsset("left", 4, 104, ren);
							}
							else if (velo.getDirection() == "-11")
							{
								anim.changeAsset("down", 4, 104, ren);
							}
							else if (velo.getDirection() == "0-1")
							{
								anim.changeAsset("up", 4, 104, ren);
							}
							else if (velo.getDirection() == "00")
							{
								anim.changeAsset("no-move", 4, 104, ren);
							}
							else if (velo.getDirection() == "01")
							{
								anim.changeAsset("down", 4, 104, ren);
							}
							else if (velo.getDirection() == "1-1")
							{
								anim.changeAsset("up", 4, 104, ren);
							}
							else if (velo.getDirection() == "10")
							{
								anim.changeAsset("right", 4, 104, ren);
							}
							else if (velo.getDirection() == "11")
							{
								anim.changeAsset("down", 4, 104, ren);
							}
						}
						else if (e->hasComponent<AttackComponent>() && SDL_GetTicks() - e->getComponent<AttackComponent>().getLastHitTime() > 408)
						{
							if (velo.getDirection() == "-1-1")
							{
								anim.changeAsset("up", 4, 104, ren);
							}
							else if (velo.getDirection() == "-10")
							{
								anim.changeAsset("left", 4, 104, ren);
							}
							else if (velo.getDirection() == "-11")
							{
								anim.changeAsset("down", 4, 104, ren);
							}
							else if (velo.getDirection() == "0-1")
							{
								anim.changeAsset("up", 4, 104, ren);
							}
							else if (velo.getDirection() == "00")
							{
								anim.changeAsset("no-move", 4, 104, ren);
							}
							else if (velo.getDirection() == "01")
							{
								anim.changeAsset("down", 4, 104, ren);
							}
							else if (velo.getDirection() == "1-1")
							{
								anim.changeAsset("up", 4, 104, ren);
							}
							else if (velo.getDirection() == "10")
							{
								anim.changeAsset("right", 4, 104, ren);
							}
							else if (velo.getDirection() == "11")
							{
								anim.changeAsset("down", 4, 104, ren);
							}
						}
					}
				}
			}
		}
	}
}

void Systems::knockbackSystem(Manager& manager, float deltaTime)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<DamageComponent>())
		{
			auto& dmg = e->getComponent<DamageComponent>();

			for (auto& en : manager.getVectorOfEntities())
			{
				if (en->hasComponent<HitboxComponent>() && en->hasComponent<HealthComponent>() && e->hasComponent<KnockbackComponent>())
				{
					auto& hpSc = en->getComponent<HealthComponent>();
					auto& hitboxSc = en->getComponent<HitboxComponent>();
					auto& knock = e->getComponent<KnockbackComponent>();

					if (e != en && hpSc.getIsInKnockback() && dmg.getAttacked())
					{
						float valueOfKnonckback = dmg.getKnockbackPower();

						knock.changeTimeLeft(deltaTime);

						float factor = knock.getFactor();

						if (factor <= 0)
						{
							factor = 0;
							hpSc.setIsInKnockback(false);
						}

						float step = valueOfKnonckback * factor * deltaTime;

						if (tryMoveWithCollision(en, knock.getDx() * step, knock.getDy() * step, manager))
						{
							hitboxSc.setPosition(knock.getDx() * step, knock.getDy() * step);

							if (en->getIsEnemy() && en->hasComponent<DetectedRectComponent>())
							{
								if (en->hasComponent<AttackRectComponent>())
								{
									en->getComponent<AttackRectComponent>().setPosition(knock.getDx() * step, knock.getDy() * step);
								}

								en->getComponent<DetectedRectComponent>().setPosition(knock.getDx() * step, knock.getDy() * step);

								if (en->hasComponent<ShootingRectComponent>())
								{
									en->getComponent<ShootingRectComponent>().setPosition(knock.getDx() * step, knock.getDy() * step);
								}
							}
						}

					}
				}
			}
		}
	}
}

void Systems::dashSystem(Manager& manager, float deltaTime)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->getIsPlayer() && e->hasComponent<DashComponent>() && e->hasComponent<HitboxComponent>() && e->getComponent<DashComponent>().getIsDashing())
		{
			auto& dash = e->getComponent<DashComponent>();
			auto& hitbox = e->getComponent<HitboxComponent>();

			float lenght = dash.getLenghtOfDash();

			dash.changeTimeLeft(deltaTime);

			float factor = dash.getFactor();

			if (factor <= 0)
			{
				factor = 0;
				dash.setIsDashing(false);
			}

			float step = lenght * factor * deltaTime;
			if (tryMoveWithCollision(e, dash.getDx() * step, dash.getDy() * step, manager))
			{
				hitbox.setPosition(dash.getDx() * step, dash.getDy() * step);
			}
		}
	}
}
