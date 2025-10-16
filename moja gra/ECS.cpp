#include "ECS.h"
#include "Components.h"
#include <string>
#include "mapy.h"

//systemy polegaja na petli we vector'ze z entity'ami
//po tym sa if'y sprawdzajacy czy sa komponenty aby nie bylo bledu ze wykorzystuje komponenty na obikie ktory nie ma tych komponentow

void Systems::collisionSystem(Manager& manager)
{
	Uint32 currentTime = SDL_GetTicks();

	if (manager.getVectorOfEntities().size() == 1)
	{
		manager.setEndScreen(true);
	}

	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<DamageComponent>() && e->hasComponent<KnockbackComponent>())
		{
			//okreslenie cooldown'u i pobranie rogow ataku
			Uint32 cooldown = 400;
			float angle = 0;

			auto& dmg = e->getComponent<DamageComponent>();

			//for na drugi obiekt aby sprawdzac kolizje miedzy dwoma obiektami
			for (auto& en : manager.getVectorOfEntities())
			{
				if (e != en && en->hasComponent<HitboxComponent>() && en->hasComponent<HealthComponent>())
				{
					auto& hpSc = en->getComponent<HealthComponent>();
					auto& knock = e->getComponent<KnockbackComponent>();
					auto& hitboxSc = en->getComponent<HitboxComponent>();

					if(e->hasComponent<AttackComponent>())
					{
						auto& attack = e->getComponent<AttackComponent>();
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
								if (en->getIsPlayer()) { cout << "Zginales \n"; manager.setEndScreen(true); manager.playerAlive = false; }
								else if (en->getIsEnemy()) cout << "Zabiles \n";

								en->destroy();
							}
							else
							{
								angle = (attack.getAngle() * 180 / M_PI) + 180;

								hpSc.setGetHit(true);
								knock.setDxAndDy(angle);
								knock.setTimeLeft(1.0f);
								hpSc.setIsInKnockback(true);
								dmg.setAttacked(true);
							}

						}
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
								if (en->getIsPlayer()) { cout << "Zginales \n"; manager.setEndScreen(true); manager.playerAlive = false; }
								else if (en->getIsEnemy()) cout << "Zabiles \n";

								en->destroy();
							}
							else
							{
								angle = (shoot.getAngle() * 180 / M_PI) + 180; hpSc.setGetShooted(true);
								knock.setDxAndDy(angle);
								knock.setTimeLeft(1.0f); 
								hpSc.setIsInKnockback(true);
								dmg.setAttacked(true);
							}
						}
					}
				}
			}
		}
	}
}

//system potrzebny do zmiany flagi getHit
void Systems::healthSystem(Manager& manager,const Uint8* keys)
{
	for (auto& e : manager.getVectorOfEntities())
	{
		if (e->hasComponent<HealthComponent>())
		{
			auto& hp = e->getComponent<HealthComponent>();
			hp.timerOfGetHit();

			if (SDL_GetTicks() - hp.getHealTime() >= hp.getPotionCooldown())
			{
				hp.setHasUsedPotion(false);
			}


			if (e->getIsPlayer() && keys[SDL_SCANCODE_H] && !hp.getHasUsedPotion())
			{
				hp.addHp(50.f);
				hp.setHasUsedPotion(true);
				hp.setHealTime();
			}
		}
	}
}