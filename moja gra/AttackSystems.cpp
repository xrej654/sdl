#include "ECS.h"
#include "Components.h"
#include <string>
#include "mapy.h"

// Funkcja wspólna dla obliczania rogow (corners) ataku i strzały.
// Dzięki temu kod się nie powtarza w playerAttackSystem i shootingSystem.
template<typename E> static void handleCalculationOfAttacking(E& e, float targetX, float targetY) {
	if (e->hasComponent<RotatedRectComponent>() && e->hasComponent<HitboxComponent>())
	{
		if (e->hasComponent<AttackComponent>() && e->hasComponent<AttackSpriteComponent>())
		{
			auto& attack = e->getComponent<AttackComponent>();

			if (SDL_GetTicks() - attack.getLastHitTime() <= 408 && attack.getIsAngleGood())
			{
				auto& rotatedRect = e->getComponent<RotatedRectComponent>();
				auto& attackSpr = e->getComponent<AttackSpriteComponent>();
				auto& hitbox = e->getComponent<HitboxComponent>();

				//okreslenia srodka gracza potrzebne do rogow ataku i obrotu 
				rotatedRect.setCenter(
					hitbox.getX() + (hitbox.getWidth() / 2),
					hitbox.getY() + (hitbox.getHeight() / 2));

				//strona w ktora jest zwrocony atak 
				attack.setDxAndDy(hitbox.getHitbox(), targetX, targetY);

				float angleF = attack.getAngleFirst();
				float angleSc = attack.getAngleSecond();
				float angleDeg = (attack.getAngle() * 180 / M_PI) + 180;

				if (angleF > angleSc)
				{
					if (angleDeg >= angleSc && angleDeg <= angleF)
					{
						attack.setIsAngleGood(true);
					}
					else
					{
						attack.setIsAngleGood(false);
					}
				}
				else
				{
					if (angleDeg >= angleSc || angleDeg <= angleF)
					{
						attack.setIsAngleGood(true);
					}
					else
					{
						attack.setIsAngleGood(false);
					}
				}

				//zmiana kata na radian 
				rotatedRect.setRad(attack.getAngle() + M_PI / 2.0);

				//okreslanie rogow 
				attack.setCorners(
					rotatedRect.rotate(
						attackSpr.getDestRect().x,
						attackSpr.getDestRect().y),

					rotatedRect.rotate(
						attackSpr.getDestRect().x + attackSpr.getDestRect().w,
						attackSpr.getDestRect().y),

					rotatedRect.rotate(
						attackSpr.getDestRect().x + attackSpr.getDestRect().w,
						attackSpr.getDestRect().y + attackSpr.getDestRect().h),

					rotatedRect.rotate(
						attackSpr.getDestRect().x,
						attackSpr.getDestRect().y + attackSpr.getDestRect().h)

				);
			}
		}

		if (e->hasComponent<ShootingComponent>() && e->hasComponent<ShootingSpriteComponent>())
		{
			auto& shoot = e->getComponent<ShootingComponent>();

			if (e->getComponent<ShootingComponent>().getHasShooted() && shoot.getIsAngleGood())
			{
				auto& rotatedRect = e->getComponent<RotatedRectComponent>();
				auto& shootSpr = e->getComponent<ShootingSpriteComponent>();
				auto& hitbox = e->getComponent<HitboxComponent>();
				//okreslenia srodka gracza potrzebne do rogow ataku i obrotu 

				rotatedRect.setCenter(
					shoot.getStarterPos().x - 24 + (hitbox.getWidth() / 2),
					shoot.getStarterPos().y + 48 + (hitbox.getHeight() / 2)
				);

				//strona w ktora jest zwrocony atak
				shoot.setDxAndDy(hitbox.getHitbox(), targetX, targetY);

				float angleF = shoot.getAngleFirst();
				float angleSc = shoot.getAngleSecond();
				float angleDeg = (shoot.getAngle() * 180 / M_PI) + 180;

				if (angleF > angleSc)
				{
					if (angleDeg >= angleSc && angleDeg <= angleF)
					{
						shoot.setIsAngleGood(true);
					}
					else
					{
						shoot.setIsAngleGood(false);
						shoot.setHasShooted(false);
					}
				}
				else
				{
					if (angleDeg >= angleSc || angleDeg <= angleF)
					{
						shoot.setIsAngleGood(true);
					}
					else
					{
						shoot.setIsAngleGood(false);
						shoot.setHasShooted(false);
					}
				}

				//zmiana kata na radian
				rotatedRect.setRad(shoot.getAngle() + 3 * M_PI / 2.0);

				//okreslanie rogow 
				shoot.setCorners(
					rotatedRect.rotate(
						shootSpr.getDestRect().x,
						shootSpr.getDestRect().y),

					rotatedRect.rotate(
						shootSpr.getDestRect().x + shootSpr.getDestRect().w,
						shootSpr.getDestRect().y),

					rotatedRect.rotate(
						shootSpr.getDestRect().x + shootSpr.getDestRect().w,
						shootSpr.getDestRect().y + shootSpr.getDestRect().h),

					rotatedRect.rotate(
						shootSpr.getDestRect().x,
						shootSpr.getDestRect().y + shootSpr.getDestRect().h)

				);
			}
		}
	}
}

void Systems::playerAttackSystem(Manager& manager, Uint32 mouseButtons, float mouseX, float mouseY, SDL_Renderer* ren)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<AttackComponent>() && e->hasComponent<AnimationComponent>() && e->hasComponent<ShootingComponent>() && e->getIsPlayer())
		{
			//zmienne potrzebne do limitowania atkow (brak spamienia co klatke)
			Uint32 cooldown = 700;

			auto& attack = e->getComponent<AttackComponent>();
			auto& shoot = e->getComponent<ShootingComponent>();
			auto& anim = e->getComponent<AnimationComponent>();

			if ((mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT))
				&& (SDL_GetTicks() - attack.getLastHitTime() >= cooldown)
				&& !attack.getHasBeenPressed()
				&& !shoot.getHasShooted()) // Przyciœniêcie przycisku
			{
				// Oznaczenie, ¿e gracz zaatakowa³
				attack.setHasBeenPressed(true);
				cout << "ATAK" << endl;
				attack.setLastHitTime(SDL_GetTicks());
				attack.setIsAngleGood(true);
				anim.resetCurrentFrame();
			}
			else if (!(mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)))
			{
				attack.setHasBeenPressed(false);
			}

			handleCalculationOfAttacking(e, mouseX, mouseY);

			if (e->hasComponent<VelocityComponent>())
			{
				auto& velo = e->getComponent<VelocityComponent>();

				if (SDL_GetTicks() - attack.getLastHitTime() <= 408 && SDL_GetTicks() > 408)
				{
					if (velo.getDirection() == "-1-1")
					{
						anim.changeAsset("attack-up", 4, 104, ren);
					}
					else if (velo.getDirection() == "-10")
					{
						anim.changeAsset("attack-left", 4, 104, ren);
					}
					else if (velo.getDirection() == "-11")
					{
						anim.changeAsset("attack-down", 4, 104, ren);
					}
					else if (velo.getDirection() == "0-1")
					{
						anim.changeAsset("attack-up", 4, 104, ren);
					}
					else if (velo.getDirection() == "00")
					{
						anim.changeAsset("attack-no-move", 4, 104, ren);
					}
					else if (velo.getDirection() == "01")
					{
						anim.changeAsset("attack-down", 4, 104, ren);
					}
					else if (velo.getDirection() == "1-1")
					{
						anim.changeAsset("attack-up", 4, 104, ren);
					}
					else if (velo.getDirection() == "10")
					{
						anim.changeAsset("attack-right", 4, 104, ren);
					}
					else if (velo.getDirection() == "11")
					{
						anim.changeAsset("attack-down", 4, 104, ren);
					}
				}
			}
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
			auto en = manager.getPlayer();

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

void Systems::playerShootingSystem(Manager& manager, Uint32 mouseButtons, float mouseX, float mouseY, SDL_Renderer* ren)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		Uint32 cooldown = 2000;
		if (e->hasComponent<ShootingComponent>() && e->hasComponent<ShootingSpriteComponent>() && e->hasComponent<HitboxComponent>() && e->hasComponent<AnimationComponent>() && e->getIsPlayer())
		{
			auto& shoot = e->getComponent<ShootingComponent>();
			auto& shootSpr = e->getComponent<ShootingSpriteComponent>();
			auto& hitbox = e->getComponent<HitboxComponent>();
			auto& anim = e->getComponent<AnimationComponent>();

			if ((mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT))
				&& !shoot.getHasBeenPressed()
				&& (SDL_GetTicks() - shoot.getLastShootTime() >= cooldown))
			{
				shoot.setStarterPos(hitbox.getX() + 24, hitbox.getY() - 48);

				shoot.setHasBeenPressed(true);
				shoot.setHasShooted(true);
				shoot.setLastShootTime(SDL_GetTicks());

				anim.resetCurrentFrame();

				shoot.setIsAngleGood(true);

				shoot.setReducedDistance(shoot.getStarterPos().y);

				shootSpr.setRects({
					shoot.getStarterPos().x,
					shoot.getReducedDistance(),
					16,48
					});

				handleCalculationOfAttacking(e, mouseX, mouseY);

				shoot.setAngle();

				cout << "Strzal \n";
			}
			else if (!(mouseButtons && SDL_BUTTON(SDL_BUTTON_RIGHT)))
			{
				shoot.setHasBeenPressed(false);
			}

			if (e->hasComponent<VelocityComponent>())
			{
				auto& velo = e->getComponent<VelocityComponent>();

				if (SDL_GetTicks() - shoot.getLastShootTime() <= 601 && SDL_GetTicks() > 601)
				{
					cout << "s\n";
					if (velo.getDirection() == "-1-1")
					{
						anim.changeAsset("shoot-up", 4, 150, ren);
					}
					else if (velo.getDirection() == "-10")
					{
						anim.changeAsset("shoot-left", 4, 150, ren);
					}
					else if (velo.getDirection() == "-11")
					{
						anim.changeAsset("shoot-down", 4, 150, ren);
					}
					else if (velo.getDirection() == "0-1")
					{
						anim.changeAsset("shoot-up", 4, 150, ren);
					}
					else if (velo.getDirection() == "00")
					{
						anim.changeAsset("shoot-no-move", 4, 150, ren);
					}
					else if (velo.getDirection() == "01")
					{
						anim.changeAsset("shoot-down", 4, 150, ren);
					}
					else if (velo.getDirection() == "1-1")
					{
						anim.changeAsset("shoot-up", 4, 150, ren);
					}
					else if (velo.getDirection() == "10")
					{
						anim.changeAsset("shoot-right", 4, 150, ren);
					}
					else if (velo.getDirection() == "11")
					{
						anim.changeAsset("shoot-down", 4, 150, ren);
					}
				}
			}
		}
	}
}

void Systems::enemyShootingSystem(Manager& manager, Uint32 mouseButtons, float mouseX, float mouseY)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<HitboxComponent>() && e->hasComponent<ShootingComponent>() && e->hasComponent<ShootingSpriteComponent>() && e->hasComponent<ShootingRectComponent>() && e->getIsEnemy())
		{
			auto en = manager.getPlayer();
			if (en->hasComponent<HitboxComponent>() && en->getIsPlayer())
			{
				auto& hitboxSc = en->getComponent<HitboxComponent>();
				auto& hitbox = e->getComponent<HitboxComponent>();
				auto& shootingRect = e->getComponent<ShootingRectComponent>();
				auto& shoot = e->getComponent<ShootingComponent>();
				auto& shootSpr = e->getComponent<ShootingSpriteComponent>();

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

					shootSpr.setRects({
						shoot.getStarterPos().x,
						shoot.getReducedDistance(),
						16,48
						});

					shoot.setAngle();
				}

				handleCalculationOfAttacking(e, hitboxSc.getX() + (hitboxSc.getWidth() / 2), hitboxSc.getY() + (hitboxSc.getHeight() / 2));
			}
		}
	}
}