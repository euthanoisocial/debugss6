#include "stdafx.h"
#include "DSProtocol.h"
#include "Util.h"
#include "Message.h"
#include "User.h"
#include "Path.h"
#include "ServerInfo.h"
#include "MemScript.h"
#include "CustomRankUser.h"
#include "Notice.h"

CCustomRankUser gCustomRankUser;

void CCustomRankUser::Load(char* path)
{

	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	for(int n=0;n < MAX_RANK_USER;n++)
	{
		this->m_Data[n].Clear();
	}

	this->m_count = 0;

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int index = lpMemScript->GetNumber();

					strcpy_s(this->m_Data[index].m_Name,lpMemScript->GetAsString());

					this->m_Data[index].m_iResetMin = lpMemScript->GetAsNumber();

					this->m_Data[index].m_iResetMax = lpMemScript->GetAsNumber();

					this->m_Data[index].m_Coin1 = lpMemScript->GetAsNumber();

					this->m_Data[index].m_Coin2 = lpMemScript->GetAsNumber();

					this->m_Data[index].m_Coin3 = lpMemScript->GetAsNumber();

					this->m_count++;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomRankUser::GCReqRankLevelUser(int aIndex, int aTargetIndex)
{
	if(gServerInfo.m_CustomRankUserSwitch == 0)
	{
		return;
	}

	if(gObj[aTargetIndex].Type != OBJECT_USER && gObj[aTargetIndex].Type != OBJECT_BOTS || gObj[aIndex].Type != OBJECT_USER && gObj[aIndex].Type != OBJECT_BOTS) {
		return;
	}

	if(gObjIsConnected(aIndex) == false || gObj[aTargetIndex].IsBot >= 1 && gObj[aTargetIndex].IsBot < 0xA)
	{
		return;
	}
	
	// ---
	PMSG_CUSTOM_RANKUSER pMsg = {0};
	// ---
	pMsg.h.set(0xF3, 0xE5, sizeof(pMsg));
	// ---
	int iRankIndex = this->GetRankIndex(aTargetIndex);
	// ---
	pMsg.iReset		= gObj[aTargetIndex].Reset;
	pMsg.iMReset	= gObj[aTargetIndex].MasterReset;
	pMsg.iKills		= gObj[aTargetIndex].Kills;
	pMsg.iDeads		= gObj[aTargetIndex].Deads;
	pMsg.iLevel		= gObj[aTargetIndex].Level;
	pMsg.iIndex		= aTargetIndex;
	pMsg.iFor		= gObj[aTargetIndex].Strength;
	pMsg.iAgi		= gObj[aTargetIndex].Dexterity;
	pMsg.iEne		= gObj[aTargetIndex].Energy;
	pMsg.iVit		= gObj[aTargetIndex].Vitality;
	pMsg.iCom		= gObj[aTargetIndex].Leadership;
	pMsg.iRank		= iRankIndex;
	// ---
	pMsg.irDanhHieu = gObj[aTargetIndex].rDanhHieu;
	pMsg.irQuanHam = gObj[aTargetIndex].rQuanHam;
	/*pMsg.irTuLuyen = gObj[aTargetIndex].rTuLuyen;
	pMsg.irThueVip = gObj[aTargetIndex].rThueVip;*/
	pMsg.irTuLuyen = gObj[aTargetIndex].rTuLuyen;
	pMsg.irNguHanh = gObj[aTargetIndex].rNguHanh;
	pMsg.iVipIcon = gObj[aTargetIndex].AccountLevel;
#if(HT_THUE_FLAG)
	pMsg.m_RankTitle6 = gObj[aTargetIndex].rQuanHam;
#endif
	// ---
	//tue add Tu Chan///
	#if TU_LUYEN_NEW
	pMsg.irRankTitle3 = gObj[aTargetIndex].rTuLuyen;
	LogAdd(LOG_BLUE, "[C.RankUser g.tới Client] gia tri tu luyen cua %s la %d", gObj[aTargetIndex].Name, gObj[aTargetIndex].rTuLuyen);
	LogAdd(LOG_BLUE, "[C.RankUser g.tới Client] Tu luyen RankTitle3 cua %s là %d", gObj[aTargetIndex].Name, pMsg.irRankTitle3);
	#endif
	////////////////
	pMsg.iClasse	= gObj[aTargetIndex].DBClass;
	memcpy(pMsg.szName, this->m_Data[iRankIndex].m_Name, sizeof(pMsg.szName));
	memcpy(pMsg.iVip, gMessage.GetMessage(32 + gObj[aTargetIndex].AccountLevel), sizeof(pMsg.iVip));
	// ---
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	DataSend(aTargetIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


int CCustomRankUser::GetRankIndex(int aIndex)
{
	int Valor;

	if (gServerInfo.m_CustomRankUserType == 1)
	{
		Valor = gObj[aIndex].MasterReset;
	}
	else if (gServerInfo.m_CustomRankUserType == 2)
	{
		Valor = (gObj[aIndex].Level + gObj[aIndex].MasterLevel);
	}
	else
	{
		Valor = gObj[aIndex].Reset;
	}

	for(int i = 0; i < this->m_count; i++)
	{
		if(Valor >= this->m_Data[i].m_iResetMin && Valor <= this->m_Data[i].m_iResetMax ||
		   Valor >= this->m_Data[i].m_iResetMin && this->m_Data[i].m_iResetMax == -1)
		{
			return i;
		}
	}
	// ---
	return -1;
}

void CCustomRankUser::CheckUpdate(LPOBJ lpObj)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 4)

	if(gServerInfo.m_CustomRankUserRewardSwitch == 0)
	{
		return;
	}

	int Valor;

	if (gServerInfo.m_CustomRankUserType == 1)
	{
		Valor = lpObj->MasterReset;
	}
	else if (gServerInfo.m_CustomRankUserType == 2)
	{
		Valor = lpObj->Level + lpObj->MasterLevel;
	}
	else
	{
		Valor = lpObj->Reset;
	}

	for(int i = 0; i < this->m_count; i++)
	{
		if(this->m_Data[i].m_iResetMin == Valor)
		{
			if(gServerInfo.m_CustomRankUserNoticeToAll == 1)
			{
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(738),lpObj->Name,this->m_Data[i].m_Name);	
			}
			if(gServerInfo.m_CustomRankUserNoticeToUser == 1)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(739),this->m_Data[i].m_Name);	
			}

			//-- Reward
			GDSetCoinSend(lpObj->Index, this->m_Data[i].m_Coin1, this->m_Data[i].m_Coin2, this->m_Data[i].m_Coin3,"RankUser");

			this->GCReqRankLevelUser(lpObj->Index, lpObj->Index);

			return;
		}
	}
	return;

#endif
}