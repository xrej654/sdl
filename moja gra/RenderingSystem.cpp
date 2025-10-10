#include "ECS.h"
#include "Components.h"
#include <string>
#include "mapy.h"

void Systems::renderingSystem(Manager& manager, SDL_Renderer* ren, float deltaTime, int screenWitdh, int screenHeight)
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
		/*if (!e->hasComponent<SpriteComponent>() && e->hasComponent<DetectedRectComponent>()) e->getComponent<DetectedRectComponent>().drawHitbox(ren, e->getComponent<DetectedRectComponent>().getHitbox(), 255, 255, 0, 100);
		if (!e->hasComponent<SpriteComponent>() && e->hasComponent<AttackRectComponent>()) e->getComponent<AttackRectComponent>().drawHitbox(ren, e->getComponent<AttackRectComponent>().getHitbox(), 255, 0, 0, 130);
		if (!e->hasComponent<SpriteComponent>() && e->hasComponent<ShootingRectComponent>()) e->getComponent<ShootingRectComponent>().drawHitbox(ren, e->getComponent<ShootingRectComponent>().getHitbox(), 200, 0, 0, 130);*/

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
				if (SDL_GetTicks() - attack.getLastHitTime() <= 408 && e->hasComponent<RotatedRectComponent>() && attack.getIsAngleGood())
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
				auto& shoot = e->getComponent<ShootingComponent>();

				if (e->getComponent<ShootingComponent>().getHasShooted() && e->hasComponent<ShootingSpriteComponent>() && e->hasComponent<RotatedRectComponent>() && shoot.getIsAngleGood())
				{
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

					if (shoot.getStarterPos().y - shoot.getReducedDistance() > shoot.getRange())
					{
						cout << "Koniec\n";
						shoot.setHasShooted(false);
					}

					/*SDL_FPoint* attackCorners = e->getComponent<ShootingComponent>().getCorners();
					for (int i = 0; i < 4; i++) {
						SDL_Rect pointRect = {
							(int)(attackCorners[i].x - 16),
							(int)(attackCorners[i].y - 16),
							32, 32
						};
						SDL_RenderFillRect(ren, &pointRect);
					}*/
				}
			}
		

		if (e->getIsPlayer())
		{
			SDL_FRect rect = { 20,20,300,20 };

			SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
			SDL_RenderFillRectF(ren, &rect);

			float newWitdh = 0;
			if (e->hasComponent<HealthComponent>())
			{
				auto& hp = e->getComponent<HealthComponent>();
				newWitdh = (hp.getHp() / hp.getMaxHp()) * rect.w;
			}

			rect = { 20,20, newWitdh ,20 };

			SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
			SDL_RenderFillRectF(ren, &rect);

			SDL_Rect srcRect = { 0,0,16,16 };
			SDL_Rect destRect = { screenWitdh - 160, screenHeight - 112,80,80 };

			if (e->hasComponent<HealthComponent>())
			{
				auto& hp = e->getComponent<HealthComponent>();

				if (SDL_GetTicks() - hp.getHealTime() <= hp.getPotionCooldown())
				{
					SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(88), &srcRect, &destRect);
				}
				else
				{
					SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(87), &srcRect, &destRect);
				}
			}

			destRect = { screenWitdh - 160, screenHeight - 208,80,80 };
			if (e->hasComponent<DashComponent>())
			{
				auto& dash = e->getComponent<DashComponent>();

				if (dash.getIsDashing())
				{
					SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(90), &srcRect, &destRect);
				}
				else
				{
					SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(89), &srcRect, &destRect);
				}
			}

			srcRect = { 0,0,32,48 };
			destRect = { 64, screenHeight - 192,128,128 };
			if (e->hasComponent<ShootingComponent>())
			{
				auto& shoot = e->getComponent<ShootingComponent>();

				if (shoot.getHasShooted())
				{
					SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(92), &srcRect, &destRect);
				}
				else
				{
					SDL_RenderCopy(ren, manager.getVectorOfBaseTextures().at(91), &srcRect, &destRect);

				}
			}
		}
		else if (e->getIsEnemy())
		{
			SDL_FRect rect = { e->getComponent<HitboxComponent>().getX() + 8,e->getComponent<HitboxComponent>().getY() - 16,e->getComponent<HitboxComponent>().getWidth() - 16,10 };

			SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
			SDL_RenderFillRectF(ren, &rect);

			float newWitdh = (e->getComponent<HealthComponent>().getHp() / e->getComponent<HealthComponent>().getMaxHp()) * rect.w;

			rect = { e->getComponent<HitboxComponent>().getX() + 8,e->getComponent<HitboxComponent>().getY() - 16,newWitdh,10 };

			SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
			SDL_RenderFillRectF(ren, &rect);
		}


	}
}