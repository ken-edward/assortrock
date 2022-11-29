#include "stdafx.h"
#include "Card.h"
#include "Hero.h"
#include "ResMgr.h"
#include "Hero.h"
#include "CardHeader.h"


Card::Card()
	: m_CardFileName(nullptr)
	, m_UnitFileName(nullptr)
{
	ImageX = 138;
	ImageY = 196;
	SizeX = 50;
	SizeY = 72;
	scale = 0.36f;

	order = OnHand;
}


Card::~Card()
{
}


void Card::OnPlay()
{
	if (CardPlaySound != nullptr)
	{
		ResMgr::Inst().PlaySound(CardPlaySound);
	}


	if (Name == L"Gnome" || Name == L"Novice")
	{
		pHero->Draw();
	}
}


void Card::OnDeath()
{
	if (CardDeathSound != nullptr)
	{
		ResMgr::Inst().PlaySound(CardDeathSound);
	}

	if (Name == L"Loot")
	{
		pHero->Draw();
	}
}


void Card::AttackSound()
{
	if (CardAttackSound != nullptr)
	{
		ResMgr::Inst().PlaySound(CardAttackSound);
	}
}





void Card::Update()
{
	Drawing();
	

	if (m_Type == Weapon)
	{
		WeaponPlaying();

		if (wielded == false)
		{
			if (visible == true)
			{
				if (opp == false || (opp == true && burning == true))
				{
					RENDINST.ShowImageByScale(x - ImageX * scale, y - ImageY * scale, m_CardFileName, scale);
				}
				else if (opp == true)
				{
					RENDINST.ShowImageByScale(x - ImageX * scale, y - ImageY * scale, L"Card_Back.bmp", scale);
				}
			}
		}
		else // 무기 착용 중
		{
			if (Hero::MyTurnFlag == true)
			{
				RENDINST.ShowImageByScale(x - 80 * scale, y - 80 * scale, m_WeaponName, scale);
			}
			else
			{
				RENDINST.ShowImageByScale(x - 80 * scale, y - 80 * scale, L"Weapon_Closed.bmp", scale);
			}

			// 스탯 표시
			int type = 0;
			
			if (hp < Maxhp)
			{
				type = 1;
			}

			RENDINST.ShowNumber(x - 33, y + 26, atk);
			RENDINST.ShowNumber(x + 26, y + 26, hp, type);
		}

	
	}



	if (m_Type == Minion)
	{
		Playing();

		if (onfield == false)
		{
			if (visible == true)
			{
				if (opp == false || (opp == true && burning == true))
				{
					RENDINST.ShowImageByScale(x - ImageX * scale, y - ImageY * scale, m_CardFileName, scale);
				}
				else if (opp == true)
				{
					RENDINST.ShowImageByScale(x - ImageX * scale, y - ImageY * scale, L"Card_Back.bmp", scale);
				}
			}
		}
		if (onfield == true && poscertain == true)
		{
			Attack();
			BoomDying();

			if (TauntShow == true)
			{
				RENDINST.ShowImageByScale(x - ImageX * scale, y + (3 - ImageY) * scale, L"Taunt.bmp", scale);
			}

			if (INPUTINST.CursorTest(this)
				&& Hero::victory == 0)
			{
				RENDINST.ShowImage(x - ImageX, y - ImageY, L"Unit_Selected.bmp");
			}

			RENDINST.ShowImageByScale(x - ImageX * scale, y - ImageY * scale, m_UnitFileName, scale);

			if (DeathRattle == true)
			{
				float dScale = 0.4f;
				RENDINST.ShowImageByScale((x - 47 * dScale) + 3, (y - 38 * dScale) + 48 * scale, L"DeathRattle.bmp", dScale);
			}

			if (legendary == true)
			{
				float lScale = 0.44f * scale;
				RENDINST.ShowImageByScale((x - 116 * lScale) + 14, (y - 94 * lScale) - 28 * scale, L"legendary.bmp", lScale);
			}

			int pivotx = 27;
			int pivoty = 29;
			int adjusthp = pivotx - 2;
			RENDINST.ShowNumber(x - pivotx * scale, y + pivoty * scale, atk);
			if (hp >= Maxhp)
			{
				RENDINST.ShowNumber(x + adjusthp * scale, y + pivoty * scale, hp);
			}
			else if (hp < Maxhp)
			{
				RENDINST.ShowNumber(x + adjusthp * scale, y + pivoty * scale, hp, 1);
			}


			showdmg();
			if (hp + armor < prevhp + prevarmor)
			{
				dmg = prevhp + prevarmor - hp + armor;
				damaged = true;
				dmgcount = 0.0f;
			}
			prevhp = hp;
			prevarmor = armor;

			
		}

		OnTurnEnd();
	}
	if (m_Type == Spell)
	{
		CastingSpell();

		if (m_SpellType == DealCard)
		{
			if (playmode == false)
			{
				if (visible == true)
				{
					RENDINST.ShowImageByScale(x - ImageX * scale, y - ImageY * scale, m_CardFileName, scale);
				}
				
			}
			else if (playmode == true)
			{
				RENDINST.ShowImage(x - 30, y - 30, L"target.bmp");
			}
		}
		else 
		{
			if (visible == true)
			{
				RENDINST.ShowImageByScale(x - ImageX * scale, y - ImageY * scale, m_CardFileName, scale);
			}	
		}
	}
}


void Card::Attack()
{
	if (atkstart == true)
	{
		atkon = true;
		atkstart = false;
		attackable = false;
		pHero->Busy = true;
		StartX = x;
		StartY = y;
		TargetX = target->x;
		TargetY = target->y;
		order = AttackingOrder;
		scale = 1.1f;

		if (target->hp <= atk)
		{
			target->ActualDeath = true;
		}
	}

	if (atkon == true)
	{
		atkcount += DELTATIME;

		float standby = 0.3f;
		float reaching = 0.2f;

		if (atkcount <= standby)
		{
			// 아무것도 안 함
		}
		else if (atkcount <= standby + reaching) // 상대 자리에 가서 때림
		{
			float targetplusX = TargetX - StartX;
			float targetplusY = TargetY - StartY;
			float changerate = DELTATIME / reaching;
			x += targetplusX * changerate;
			y += targetplusY * changerate;
		}
		else if (atkcount <= standby + reaching + reaching) // 다시 돌아옴
		{
			if (attacked == false)
			{
				target->TakingDamage(atk);
				TakingDamage(target->atk);
				attacked = true;

				x = TargetX;
				y = TargetY;

				if (atk < 6)
				{
					ResMgr::Inst().PlaySound(L"3attack.mp3");
				}
				else if (atk >= 6)
				{
					ResMgr::Inst().PlaySound(L"6attack.mp3");
				}
			}

			float targetplusX = TargetX - StartX;
			float targetplusY = TargetY - StartY;
			float targetplusScale = 0.1f;
			float changerate = DELTATIME / reaching;
			x -= targetplusX * changerate;
			y -= targetplusY * changerate;
			scale -= targetplusScale * changerate;
		}
		else // 마무리
		{
			order = OnField;
			atkon = false;
			atkcount = 0.0f;
			x = StartX;
			y = StartY;
			scale = 1.0f;
			attacked = false;
			pHero->Busy = false;

			target = nullptr;
		}
	}
}


void Card::Playing()
{
	if (playstart == true)
	{
		playstart = false;
		playon = true;
		
		playcount = 0.0f;


		pHero->Busy = true;

		ResMgr::Inst().PlaySound(L"summon_sound.ogg");
	}
	
	if (playon == false)
	{
		return;
	}

	playcount += DELTATIME;
	
	float moving = 0.3f;
	float standby = 0.2f;
	float putting = 0.2f;
	float waiting = 0.4f;

	if (playcount <= moving)
	{
		float changerate = DELTATIME / moving;
		float targetplusX = playtargetX - playstartX;
		float targetplusY = playtargetY - playstartY;

		x += targetplusX * changerate;
		y += targetplusY * changerate;
	}
	else if (playcount <= moving + standby)
	{
		x = playtargetX;
		y = playtargetY;
	}
	else if (playcount <= moving + standby + putting)
	{
		if (playchange == false)
		{
			scale = 1.2f;
			playchange = true;
			order = OnField;
			SizeX = 45;
			SizeY = 50;
			ImageX = 47;
			ImageY = 64;
			onfield = true;

			
		}
		float targetplusScale = -0.2f;
		float changerate = DELTATIME / moving;
		scale += targetplusScale * changerate;
	}
	else if (playcount <= moving + standby + putting + waiting)
	{
		scale = 1.0f;
		// 아무것도 안 함.
	}
	else // 마무리
	{
		if (Name == L"DrBoom")
		{
			ResMgr::Inst().PlaySound(L"BoomBot_Summon.mp3");
			pHero->Summon(new BoomBot, fieldpos + 1);
			pHero->Summon(new BoomBot, fieldpos);
		}

		playon = false;
		playcount = 0.0f;
		scale = 1.0f;

		pHero->Busy = false;

		CardDisplayOn = false;

		if (Taunt == true)
		{
			TauntShow = true;
			ResMgr::Inst().PlaySound(L"Taunt.ogg");
		}
	}
}


void Card::Drawing()
{
	if (drawstart == true && pHero->Busy == false && pHero->enemy->Busy == false)
	{
		drawstart = false;
		drawon = true;
		drawcount = 0.0f;
		pHero->Busy = true;
		visible = true;
		drawspeed = 15;
		scale = 0.5f;
	}

	if (drawon == true)
	{
		drawcount += DELTATIME;

		if (opp == false) // 나일 때 카드 뽑기
		{
			float preShowing = 1;
			float adding = 0.3f;

			if (drawcount < preShowing)
			{
				float changerate = DELTATIME / preShowing;
				drawspeed -= 15 * changerate;
				x -= drawspeed;

				if (x < 880)
				{
					x = 880;
				}
			}
			else if (drawcount < preShowing + adding)
			{
				if (drawchange == false)
				{
					if (pHero->Hand.size() >= 10)
					{
						m_bDeath = true;
						ResMgr::Inst().PlaySound(L"Card_Burn.ogg");
						pHero->Busy = false;
						return;
					}


					drawchange = true;
					pHero->Hand.push_back(this);

					drawstartX = x;
					drawstartY = y;
					


					int HandSize = pHero->Hand.size();

					int CardGap = 88;

					int StartPointX = 640 + CardGap / 2; // 반 칸 더해진 분량

					if (HandSize <= 4) // 4장 이하
					{
						StartPointX -= CardGap * (HandSize / 2);

						if (HandSize % 2 == 1) // 홀수면 반 칸 더 뺌
						{
							StartPointX -= CardGap / 2;
						}
					}
					else if (HandSize > 4) // 4장 초과
					{
						StartPointX = 640 - CardGap * 1.5;
						CardGap = CardGap * 3 / (HandSize - 1);
					}


					for (int i = 0; i < HandSize; i++)
					{
						if (pHero->Hand[i]->drawon == true)
						{
							pHero->Hand[i]->drawtargetX = StartPointX + CardGap * i;
							pHero->Hand[i]->drawtargetY = 690;
							break;
						}
					}

				} // drawchange

				float changerate = DELTATIME / adding;
				float targetplusX = drawtargetX - drawstartX;
				float targetplusY = drawtargetY - drawstartY;
				
				float targetminusScale = 0.5f - 0.36f;
				scale -= targetminusScale * changerate;

				x += targetplusX * changerate;
				y += targetplusY * changerate;
				
			}
			else
			{
				scale = 0.36f;
				drawcount = 0.0f;
				drawon = false;
				pHero->Busy = false;
			}
		}
		else // 상대
		{
			float adding = 0.6f;
		
			if (pHero->Hand.size() >= 10 && justadded == false) // 카드 공개하면서 태움
			{
				burning = true;
				float preShowing = 1;
				float changerate = DELTATIME / preShowing;
				if (drawcount < preShowing)
				{
					drawspeed -= 15 * changerate;
					x -= drawspeed;
					return;
				}
				else
				{
					m_bDeath = true;
					ResMgr::Inst().PlaySound(L"Card_Burn.ogg");
					pHero->Busy = false;
					return;
				}
			}
			else if (drawcount < adding)
			{
				if (drawchange == false)
				{
					justadded = true;
					drawchange = true;
					pHero->Hand.push_back(this);

					drawstartX = x;
					drawstartY = y;

					int HandSize = pHero->Hand.size();

					int CardGap = 88;

					int StartPointX = 640 + CardGap / 2; // 반 칸 더해진 분량

					if (HandSize <= 4) // 4장 이하
					{
						StartPointX -= CardGap * (HandSize / 2);

						if (HandSize % 2 == 1) // 홀수면 반 칸 더 뺌
						{
							StartPointX -= CardGap / 2;
						}
					}
					else if (HandSize > 4) // 4장 초과
					{
						StartPointX = 640 - CardGap * 1.5;
						CardGap = CardGap * 3 / (HandSize - 1);
					}


					for (int i = 0; i < HandSize; i++)
					{
						if (pHero->Hand[i]->drawon == true)
						{
							pHero->Hand[i]->drawtargetX = StartPointX + CardGap * i;
							pHero->Hand[i]->drawtargetY = -30;
						}
					}

				} // drawchange

				float changerate = DELTATIME / adding;
				float targetplusX = drawtargetX - drawstartX;
				float targetplusY = drawtargetY - drawstartY;
				float targetminusScale = 0.5f - 0.36f;
				scale -= targetminusScale * changerate;

				x += targetplusX * changerate;
				y += targetplusY * changerate;
			}
			else
			{
				scale = 0.36f;
				drawcount = 0.0f;
				drawon = false;
				pHero->Busy = false;
			}
		}
	}
}


Character* Card::GetRandomTarget()
{
	int EnemyCount = 1 + pHero->enemy->Field.size();

	int RandNum = rand() % EnemyCount;

	if (RandNum == 0)
	{
		return pHero->enemy;
	}
	else
	{
		return pHero->enemy->Field[RandNum - 1];
	}

	return nullptr;
}



void Card::OnTurnEnd() // 일단은 라그 전용
{
	if (TurnEndStart == true && pHero->Busy == false)
	{
		TurnEndStart = false;
		TurnEndOn = true;
		TurnEndCount = 0.0f;
		pHero->Busy = true;
		pTarget = GetRandomTarget();
		targetplusX = pTarget->x - x;
		targetplusY = pTarget->y - y;
		fireX = x;
		fireY = y;
		order = PlayingOrder;
	}

	if (TurnEndOn == true)
	{
		TurnEndCount += DELTATIME;
		float waiting = 1.0f;
		float shooting = waiting + 1.0f;
		float hitting = shooting + 1.0f;

		if (TurnEndCount < waiting) // 죽어라
		{
			if (phase[0] == false)
			{
				phase[0] = true;

				ResMgr::Inst().PlaySound(L"Rag_TurnEnd.mp3");
			}

		}
		else if (TurnEndCount < shooting) // 벌레같은 놈들
		{
			
			float changerate = DELTATIME / (shooting - waiting);
			fireX += changerate * targetplusX;
			fireY += changerate * targetplusY;

			float fscale = 0.6f;
			RENDINST.ShowImageByScale(
				fireX - 100 * fscale,
				fireY - 190 * fscale,
				L"fire.bmp", fscale,
				4, 3, rand() % 4, rand() % 3
			);
		}
		else if (TurnEndCount < hitting)
		{
			if (phase[1] == false)
			{
				phase[1] = true;
				pTarget->TakingDamage(8);
			}
		}
		else
		{
			order = OnField;
			TurnEndCount = 0.0f;
			TurnEndOn = false;
			pHero->Busy = false;

			for (size_t i = 0; i < 5; i++)
			{
				phase[i] = false;
			}
		}
	}
}




void Card::BoomDying()
{
	if (deathstart == true
		&& pHero->Busy == false
		&& pHero->enemy->Busy == false)
	{
		deathstart = false;
		deathon = true;
		anicount = 0;
		pHero->Busy = true;
	}

	if (deathon == true)
	{
		anicount += DELTATIME;
		float bombflying = 0.8f;
		float givingdmg = bombflying + 0.4f;

		if (anicount < bombflying)
		{
			if (phase[0] == false)
			{
				phase[0] = true;
				ResMgr::Inst().PlaySound(L"BoomBot_Explode.mp3");
			}
		}
		else if (anicount < givingdmg)
		{
			if (phase[1] == false)
			{
				phase[1] = true;
				Character* target = GetRandomTarget();
				int dmg = rand() % 4 + 1; // 1~4
				target->TakingDamage(dmg);
			}
		}
		else
		{
			deathon = false;
			pHero->Busy = false;
			m_bDeath = true;
			died = true;
			for (size_t i = 0; i < 5; i++)
			{
				phase[i] = false;
			}
		}
	}
}


void Card::WeaponPlaying()
{
	if (WeaponPlayStart == true && pHero->Busy == false)
	{
		WeaponPlayStart = false;
		WeaponPlayOn = true;
		pHero->Busy = true;
		anicount = 0.0f;
		ResMgr::Inst().PlaySound(L"hero_weapon_draw.ogg");
	}

	if (WeaponPlayOn == true)
	{
		anicount += DELTATIME;

		float approaching = 0.8f;

		if (anicount < approaching)
		{
			if (phase[0] == false)
			{
				phase[0] = true;
				playstartX = x;
				playstartY = y;
				playtargetX = 515;
				playtargetY = 560;
			}

			float targetplusX = playtargetX - playstartX;
			float targetplusY = playtargetY - playstartY;
			float changerate = DELTATIME / approaching;

			x += targetplusX * changerate;
			y += targetplusY * changerate;
		}
		else
		{
			if (pHero->WieldingWeapon != nullptr) // 무기 교체
			{
				Card* tempweapon = pHero->WieldingWeapon;

				pHero->tempatk -= tempweapon->atk;
				pHero->atk -= tempweapon->atk;

				tempweapon->m_bDeath = true;

				ResMgr::Inst().PlaySound(L"Weapon_Destruction_01.ogg");
			}

			pHero->WieldingWeapon = this;

			for (size_t i = 0; i < 5; i++)
			{
				phase[i] = false;
			}
			
			scale = 0.7f;

			x = playtargetX;
			y = playtargetY; // 보정

			pHero->atk +=atk;
			pHero->tempatk += atk;

			WeaponPlayOn = false;
			order = OnField;
			pHero->Busy = false;
			anicount = 0.0f;
			wielded = true;
		}
	}
}


void Card::CastingSpell()
{
	if (caststart == true)
	{
		caststart = false;
		caston = true;
		pHero->Busy = true;
		anicount = 0;
		visible = false;
	}

	if (caston == true)
	{
		anicount += DELTATIME;

		float waiting = 0.6f;

		if (anicount < waiting)
		{
			// 아무것도 안 함
		}
		else
		{
			pHero->Busy = false;
			caston = false;
			m_bDeath = true;
		}
	}
}