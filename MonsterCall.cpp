#include "stdafx.h"
#include "MonsterCall.h"
#include "Monster.h"
#include "EffectManager.h"
#include "Kalima.h"
#include "Crywolf.h"
#include "CrywolfSync.h"
#include "MemScript.h"
#include "Util.h"
#include "MasterSkillTree.h"
#include "Notice.h"


cMonsterCall gMonsterCall;

void cMonsterCall::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_MonsterCallInfo.clear();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			CALL_INFO info;

			info.ItemID = lpMemScript->GetNumber();

			info.MonsterID = lpMemScript->GetAsNumber();

			info.Count = lpMemScript->GetAsNumber();

			info.Time = lpMemScript->GetAsNumber();

			info.Renewal = lpMemScript->GetAsNumber();

			info.AllowParty = lpMemScript->GetAsNumber();

			info.AllowGuild = lpMemScript->GetAsNumber();

			this->m_MonsterCallInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int cMonsterCall::SetMonster(LPOBJ lpObj, CItem* lpItem)
{
	//LogAdd(LOG_TEST, "CheckMonster");

	std::vector<CALL_INFO>::iterator it = this->m_MonsterCallInfo.begin();

	for (; it != this->m_MonsterCallInfo.end(); it++)
	{
		if (it->ItemID == lpItem->m_Index)
	{
		int index;
		for (int i = 1; i <= it->Count; i++)
		{
			index = gObjAddSummon();

			if (OBJECT_RANGE(index) == 0)
			{
				return 0;
			}

			lpObj->SummonIndex = index;

			LPOBJ lpSummon = &gObj[index];

			lpSummon->X = lpObj->X - 1;
			lpSummon->Y = lpObj->Y + i;
			lpSummon->MTX = lpSummon->X;
			lpSummon->MTY = lpSummon->Y;
			lpSummon->Dir = 2;
			lpSummon->Map = lpObj->Map;

			gObjSetMonster(index, it->MonsterID);

#if(GAMESERVER_UPDATE>=602)
			lpSummon->Life += (float)((__int64)lpSummon->Life*gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_LIFE)) / 100;
			lpSummon->MaxLife += (float)((__int64)lpSummon->MaxLife*gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_LIFE)) / 100;
			lpSummon->ScriptMaxLife += (float)((__int64)lpSummon->ScriptMaxLife*gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_LIFE)) / 100;
			lpSummon->Defense += ((__int64)lpSummon->Defense*gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_DEFENSE)) / 100;
			lpSummon->PhysiDamageMin += ((__int64)lpSummon->PhysiDamageMin*gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_DAMAGE)) / 100;
			lpSummon->PhysiDamageMax += ((__int64)lpSummon->PhysiDamageMax*gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_SUMMON_DAMAGE)) / 100;
#endif
			lpSummon->SummonIndex = lpObj->Index;
			lpSummon->Attribute = 100;
			lpSummon->TargetNumber = -1;
			lpSummon->ActionState.Attack = 0;
			lpSummon->ActionState.Emotion = 0;
			lpSummon->ActionState.EmotionCount = 0;
			lpSummon->PathCount = 0;
			lpSummon->MoveRange = 1; // 15

			// new
			lpSummon->SpotRenewal = (bool)it->Renewal;
			lpSummon->SpotAllowParty = (bool)it->AllowParty;
			lpSummon->SpotAllowGuild = (bool)it->AllowGuild;

			GCSummonLifeSend(lpSummon->SummonIndex, (int)lpSummon->Life, (int)lpSummon->MaxLife);

			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "Địt vào lồn");
		}
	}
	else
	{
		for (int n = MAX_OBJECT_MONSTER; n < OBJECT_START_USER; n++)
		{
			if (gObj[n].SummonIndex == lpObj->Index)
			{
				if (gObjIsConnected(gObj[n].Index) != 0)
				{
					gObj[n].SummonIndex = -1;
				}
				gObjDel(n);
				gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "XOA BOSS NE");
			}
		}
			return TRUE;
		}
	}

	return FALSE;
}

int cMonsterCall::RespawnMonster(LPOBJ lpObj)
{
	if (lpObj->CreatorIndex != -1)
	{
		if (time(NULL) >= lpObj->SpotTime/* ||  (lpObj->SpotRenewal == false && lpObj->SpotKillCount > 0)*/)
		{
			gObjDel(lpObj->Index);
			return 0;
		}

		if (lpObj->SpotRenewal == false && lpObj->SpotKillCount > 0)
		{
			gObjMonsterDieGiveItem(lpObj, &gObj[lpObj->CreatorIndex]);
			gObjDel(lpObj->Index);
			return 0;
		}

		//gObjMonsterDieGiveItem(lpObj,lpTarget);

		lpObj->Teleport = ((lpObj->Teleport == 3) ? 0 : lpObj->Teleport);

		if (lpObj->State == OBJECT_CREATE)
		{
			lpObj->State = OBJECT_PLAYING;
			lpObj->RegenOk = ((lpObj->RegenOk == 3) ? 0 : lpObj->RegenOk);
		}

		if (lpObj->DieRegen != 2)
		{
			return 0;
		}

		lpObj->AttackerKilled = 0;

		if (lpObj->CurrentAI != 0 && lpObj->RegenType != 0)
		{
			return 0;
		}

		lpObj->Live = 1;
		lpObj->ViewState = 0;
		lpObj->Teleport = 0;
		lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
		lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;

		gEffectManager.ClearAllEffect(lpObj);

		if (this->gObjMonsterRegen(lpObj, &gObj[lpObj->CreatorIndex]) == 0)
		{
			return 0;
		}

		lpObj->DieRegen = 0;
		lpObj->State = OBJECT_CREATE;

		gObjViewportListProtocolCreate(lpObj);
		return TRUE;
	}

	return 0;
}

bool cMonsterCall::gObjMonsterRegen(LPOBJ lpObj, LPOBJ Player) // OK
{
	gObjClearViewport(lpObj);

	lpObj->TargetNumber = -1;
	lpObj->LastAttackerID = -1;
	lpObj->NextActionTime = 5000;
	lpObj->ActionState.Rest = 0;
	lpObj->ActionState.Attack = 0;
	lpObj->ActionState.Move = 0;
	lpObj->ActionState.Escape = 0;
	lpObj->ActionState.Emotion = 0;
	lpObj->ActionState.EmotionCount = 0;

	int px = lpObj->SpotX;
	int py = lpObj->SpotY;

	gObjGetRandomFreeLocation(lpObj->Map, &px, &py, 1, 1, 30);

	lpObj->X = px;
	lpObj->Y = py;

	lpObj->MTX = px;
	lpObj->MTY = py;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->StartX = (BYTE)lpObj->X;
	lpObj->StartY = (BYTE)lpObj->Y;
	lpObj->PathCur = 0;
	lpObj->PathCount = 0;
	lpObj->PathStartEnd = 0;

	gObjMonsterInitHitDamage(lpObj);

	if (lpObj->Map != MAP_CRYWOLF || (gCrywolfSync.GetCrywolfState() != CRYWOLF_STATE_NOTIFY2 && gCrywolfSync.GetCrywolfState() != CRYWOLF_STATE_READY && gCrywolfSync.GetCrywolfState() != CRYWOLF_STATE_START && gCrywolfSync.GetCrywolfState() != CRYWOLF_STATE_END))
	{
		if (gCrywolfSync.CheckApplyBenefit() != 0 && gCrywolfSync.GetOccupationState() == 0)
		{
			lpObj->Life = (lpObj->ScriptMaxLife*gCrywolfSync.GetMonHPBenefitRate()) / 100;
			lpObj->MaxLife = (lpObj->ScriptMaxLife*gCrywolfSync.GetMonHPBenefitRate()) / 100;
		}
	}

	return 1;
}

bool cMonsterCall::CheckAttack(LPOBJ lpObj, LPOBJ lpMonster)
{
	if (lpMonster->CreatorIndex == -1)
	{
		return 1;
	}
	if (lpMonster->CreatorIndex != -1)
	{
		LPOBJ lpCreator = gObjFind(lpMonster->CreatorName);
		if (lpCreator == 0)
		{
			return 0;
		}

		if (lpMonster->CreatorIndex != lpCreator->Index)
		{
			lpMonster->CreatorIndex = lpCreator->Index;
		}
	}

	if (lpMonster->CreatorIndex != -1 && lpMonster->CreatorIndex == lpObj->Index)
	{
		//LogAdd(LOG_TEST,"kek1 oO");
		return 1;
	}

	if (gObj[lpMonster->CreatorIndex].GuildNumber != -1 && gObj[lpMonster->CreatorIndex].GuildNumber == lpObj->GuildNumber)
	{
		//LogAdd(LOG_TEST,"[%s] - [%s] kek2 oO",gObj[lpMonster->CreatorIndex].Name,lpObj->Name);
		return 1;
	}

	if (gObj[lpMonster->CreatorIndex].PartyNumber != -1 && (gObj[lpMonster->CreatorIndex].PartyNumber == lpObj->PartyNumber))
	{
		//LogAdd(LOG_TEST,"[%d] - [%d] kek3 oO",gObj[lpMonster->CreatorIndex].PartyNumber,lpObj->PartyNumber);
		return 1;
	}

	return 0;
}