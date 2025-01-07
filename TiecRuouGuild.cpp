// CustomTiecRuou.cpp: implementation of the CCustomTiecRuou class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TiecRuouGuild.h"
#include "Guild.h"
#include "DSProtocol.h"
#include "Map.h"
#include "MapManager.h"
#include "CashShop.h"
#include "MemScript.h"
#include "Notice.h"
#include "ScheduleManager.h"
#include "User.h"
#include "Util.h"
#include "Viewport.h"
#include "GuildClass.h"
#include "Message.h"
#include "ServerInfo.h"
#include "ESProtocol.h"

//--------------------------------------------------------------------

#if(EVENT_TIEC_RUOU_GUILD)

CCustomTiecRuou gTiecRuouEvent;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomTiecRuou::CCustomTiecRuou() // OK
{
	for(int n=0;n < MAX_CUSTOM_TIECRUOU;n++)
	{
		CUSTOM_TIECRUOU_INFO* lpInfo = &this->m_CustomTiecRuouInfo[n];

		lpInfo->Index = n;
		lpInfo->State = CUSTOM_TIECRUOU_STATE_BLANK;
		lpInfo->RemainTime = 0;
		lpInfo->TargetTime = 0;
		lpInfo->TickCount = GetTickCount();
		lpInfo->AlarmMinSave = -1;
		lpInfo->AlarmMinLeft = -1;
	}
}

CCustomTiecRuou::~CCustomTiecRuou() // OK
{

}

void CCustomTiecRuou::Init() // OK
{
	for(int n=0;n < MAX_CUSTOM_TIECRUOU;n++)
	{
		if(this->m_CustomTiecRuouSwitch == 0)
		{
			this->SetState(&this->m_CustomTiecRuouInfo[n],CUSTOM_TIECRUOU_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_CustomTiecRuouInfo[n],CUSTOM_TIECRUOU_STATE_EMPTY);
		}
	}
}

void CCustomTiecRuou::ReadCustomTiecRuouInfo(char* section,char* path) // OK
{
	this->m_CustomTiecRuouSwitch = GetPrivateProfileInt(section,"CustomTiecRuouSwitch",0,path);

	GetPrivateProfileString(section,"CustomTiecRuouText1","",this->m_CustomTiecRuouText1,sizeof(this->m_CustomTiecRuouText1),path);

	GetPrivateProfileString(section,"CustomTiecRuouText2","",this->m_CustomTiecRuouText2,sizeof(this->m_CustomTiecRuouText2),path);

	GetPrivateProfileString(section,"CustomTiecRuouText3","",this->m_CustomTiecRuouText3,sizeof(this->m_CustomTiecRuouText3),path);

	this->m_CustomTiecRuouNPC = GetPrivateProfileInt(section,"CustomTiecRuouNPC",0,path);

	this->m_CustomTiecRuouNPCMap = GetPrivateProfileInt(section,"CustomTiecRuouNPCMap",0,path);

	this->m_CustomTiecRuouNPCX = GetPrivateProfileInt(section,"CustomTiecRuouNPCX",0,path);

	this->m_CustomTiecRuouNPCY = GetPrivateProfileInt(section,"CustomTiecRuouNPCY",0,path);

	this->WaitingGate = GetPrivateProfileInt(section,"WaitingGate",0,path);

	this->MinLevel = GetPrivateProfileInt(section,"MinLevel",0,path);

	this->MaxLevel = GetPrivateProfileInt(section,"MaxLevel",0,path);

	this->MinReset = GetPrivateProfileInt(section, "MinReset", 0, path);

	this->MaxReset = GetPrivateProfileInt(section, "MaxReset", 0, path);
}

void CCustomTiecRuou::Load(char* path) // OK
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

	for(int n=0;n < MAX_CUSTOM_TIECRUOU;n++)
	{
		this->m_CustomTiecRuouInfo[n].StartTime.clear();
		//this->m_CustomTiecRuouInfo[n].RuleInfo.RewardCoin.clear();
	}

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

					CUSTOM_TIECRUOU_START_TIME info;

					int index = lpMemScript->GetNumber();

					info.Year = lpMemScript->GetAsNumber();

					info.Month = lpMemScript->GetAsNumber();

					info.Day = lpMemScript->GetAsNumber();

					info.DayOfWeek = lpMemScript->GetAsNumber();

					info.Hour = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					this->m_CustomTiecRuouInfo[index].StartTime.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int index = lpMemScript->GetNumber();

					strcpy_s(this->m_CustomTiecRuouInfo[index].RuleInfo.Name,lpMemScript->GetAsString());

					this->m_CustomTiecRuouInfo[index].RuleInfo.AlarmTime = lpMemScript->GetAsNumber();

					this->m_CustomTiecRuouInfo[index].RuleInfo.EventTime = lpMemScript->GetAsNumber();

					this->m_CustomTiecRuouInfo[index].RuleInfo.CongHien = lpMemScript->GetAsNumber();

					this->m_CustomTiecRuouInfo[index].RuleInfo.TimeDelay = lpMemScript->GetAsNumber();
					//--
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}
					int index = lpMemScript->GetNumber();

					this->CongHienMoi = lpMemScript->GetAsNumber();

					this->ZenMoi = lpMemScript->GetAsNumber();

					this->WcMoi = lpMemScript->GetAsNumber();

					//this->MaxLanMoi = lpMemScript->GetAsNumber();

					//this->MaxCongHien = lpMemScript->GetAsNumber();
				}
				else
				{
					break;
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

void CCustomTiecRuou::MainProc() // OK
{
	for(int n=0;n < MAX_CUSTOM_TIECRUOU;n++)
	{
		CUSTOM_TIECRUOU_INFO* lpInfo = &this->m_CustomTiecRuouInfo[n];

		if((GetTickCount()-lpInfo->TickCount) >= 1000)
		{
			lpInfo->TickCount = GetTickCount();

			lpInfo->RemainTime = (int)difftime(lpInfo->TargetTime,time(0));

			if(this->m_CustomTiecRuouSwitch == 0)
			{
				if (gServerDisplayer.EventTiecRuou != -1)
				{
					gServerDisplayer.EventTiecRuou = -1;
				}
			}
			else 
			{
				if (lpInfo->RemainTime >= 0 && (lpInfo->RemainTime < gServerDisplayer.EventTiecRuou || gServerDisplayer.EventTiecRuou <= 0))
				{
					if (lpInfo->State == CUSTOM_TIECRUOU_STATE_EMPTY)
					{
						gServerDisplayer.EventTiecRuou = lpInfo->RemainTime;
					}
					else 
					{
						if (gServerDisplayer.EventTiecRuou != 0)
						{
							gServerDisplayer.EventTiecRuou = 0;
						}
					}
				}
			}

			switch(lpInfo->State)
			{
				case CUSTOM_TIECRUOU_STATE_BLANK:
					this->ProcState_BLANK(lpInfo);
					break;
				case CUSTOM_TIECRUOU_STATE_EMPTY:
					this->ProcState_EMPTY(lpInfo);
					break;
				case CUSTOM_TIECRUOU_STATE_START:
					this->ProcState_START(lpInfo);
					break;
			}
		}
	}
}

void CCustomTiecRuou::ProcState_BLANK(CUSTOM_TIECRUOU_INFO* lpInfo) // OK
{

}

void CCustomTiecRuou::ProcState_EMPTY(CUSTOM_TIECRUOU_INFO* lpInfo) // OK
{
	if(lpInfo->RemainTime > 0 && lpInfo->RemainTime <= (lpInfo->RuleInfo.AlarmTime*60))
	{
		if((lpInfo->AlarmMinSave=(((lpInfo->RemainTime%60)==0)?((lpInfo->RemainTime/60)-1):(lpInfo->RemainTime/60))) != lpInfo->AlarmMinLeft)
		{
			lpInfo->AlarmMinLeft = lpInfo->AlarmMinSave;

			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,this->m_CustomTiecRuouText1,lpInfo->RuleInfo.Name,(lpInfo->AlarmMinLeft+1));
		}
	}

	if(lpInfo->RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,this->m_CustomTiecRuouText2,lpInfo->RuleInfo.Name);
		this->SetState(lpInfo,CUSTOM_TIECRUOU_STATE_START);
	}
}

void CCustomTiecRuou::ProcState_START(CUSTOM_TIECRUOU_INFO* lpInfo) // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[n];

		lpObj->WCoinCTick++;

		if(lpObj->WCoinCTick == lpInfo->RuleInfo.TimeDelay)
		{
			if((((lpInfo->RuleInfo.EventTime*60)-lpInfo->RemainTime) >= lpObj->WCoinCTick) && lpObj->Map == 105 && lpObj->GuildNumber > 0)
			{
				lpObj->WCoinCTick = 0;
				gCashShop.GDCashShopAddPointSaveSend(lpObj->Index, 0, 0, 0, 0, lpInfo->RuleInfo.CongHien);
				gCashShop.CGCashShopPointRecv(lpObj->Index);
				GCEffectInfoSend(lpObj->Index,16);
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(1022), lpInfo->RuleInfo.CongHien);
			}
			
		}
	}

	if(lpInfo->RemainTime <= 0)
	{
		//lpObj->MoiRuouCount = 0;
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,this->m_CustomTiecRuouText3,lpInfo->RuleInfo.Name);
		this->SetState(lpInfo,CUSTOM_TIECRUOU_STATE_EMPTY);
	}
}

void CCustomTiecRuou::SetState(CUSTOM_TIECRUOU_INFO* lpInfo,int state) // OK
{
	switch((lpInfo->State=state))
	{
		case CUSTOM_TIECRUOU_STATE_BLANK:
			this->SetState_BLANK(lpInfo);
			break;
		case CUSTOM_TIECRUOU_STATE_EMPTY:
			this->SetState_EMPTY(lpInfo);
			break;
		case CUSTOM_TIECRUOU_STATE_START:
			this->SetState_START(lpInfo);
			break;
	}
}

void CCustomTiecRuou::SetState_BLANK(CUSTOM_TIECRUOU_INFO* lpInfo) // OK
{

}

void CCustomTiecRuou::SetState_EMPTY(CUSTOM_TIECRUOU_INFO* lpInfo) // OK
{
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	this->CheckSync(lpInfo);
}

void CCustomTiecRuou::SetState_START(CUSTOM_TIECRUOU_INFO* lpInfo) // OK
{
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	lpInfo->RemainTime = lpInfo->RuleInfo.EventTime*60;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void CCustomTiecRuou::CheckSync(CUSTOM_TIECRUOU_INFO* lpInfo) // OK
{
	if(lpInfo->StartTime.empty() != 0)
	{
		this->SetState(lpInfo,CUSTOM_TIECRUOU_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<CUSTOM_TIECRUOU_START_TIME>::iterator it=lpInfo->StartTime.begin();it != lpInfo->StartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpInfo,CUSTOM_TIECRUOU_STATE_BLANK);
		return;
	}

	lpInfo->RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	lpInfo->TargetTime = (int)ScheduleTime.GetTime();
}

LONG CCustomTiecRuou::GetDummyUserIndex() // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			return n;
		}
	}

	return OBJECT_START_USER;
}

bool CCustomTiecRuou::Dialog(LPOBJ lpObj, LPOBJ lpNpc)
{
	//LogAdd(LOG_RED,"CCustomTiecRuou::Dialog 1");
	//if (!this->m_CustomTiecRuouSwitch) return false;

	if (lpNpc->Class == 380 &&
		lpNpc->Map == 0 &&
		lpNpc->X == 123 &&
		lpNpc->Y == 139)
	{
		this->GCSendData(lpObj->Index);
		return 1;
	}

	return false;
}

void CCustomTiecRuou::GCSendData(int aIndex)
{

	PMSG_TIECRUOU_REQ pRequest;
	pRequest.Head.set(0xFB,0x60, sizeof(pRequest)); // can thay doi

	DataSend(aIndex, (LPBYTE)&pRequest, sizeof(pRequest));
}

void CCustomTiecRuou::RecvTiecRuouPanel(PMSG_TIECRUOU_RECV* Data, int aIndex)
{
	this->CheckEnterEnabled(&gObj[aIndex]);
}

void CCustomTiecRuou::RecvTiecRuouZen(PMSG_TIECRUOU_ZEN_RECV* Data, int aIndex)
{
	this->MuaRuouZen(&gObj[aIndex]);
}

void CCustomTiecRuou::RecvTiecRuouWC(PMSG_TIECRUOU_WC_RECV* Data, int aIndex)
{
	this->MuaRuouWC(&gObj[aIndex]);
}

bool CCustomTiecRuou::CheckEnterEnabled(LPOBJ lpObj)
{
	if(lpObj->Interface.use != 0 || lpObj->Teleport != 0 || lpObj->DieRegen != 0 || lpObj->PShopOpen != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(350));
		return 0;
	}

	if(lpObj->Guild != 0 && lpObj->Guild->WarState == 1)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(1023));
		return 0;
	}
	GUILD_INFO_STRUCT * lpGuildInfo = lpObj->Guild;

	if (lpGuildInfo == NULL)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(506));
		return 0;
	}

	if (lpObj->GuildNumber == 0) //Verifica se o personagem faz parte de uma guild
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(506));
		return 0;
	}

	if(OBJECT_RANGE(lpObj->DuelUser) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(1024));
		return 0;
	}

	if(lpObj->PShopOpen != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(1025));
		return 0;
	}

	if(this->MinLevel != -1 && this->MinLevel > lpObj->Level)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(224),this->MinLevel);
		return 0;
	}

	if(this->MaxLevel != -1 && this->MaxLevel < lpObj->Level)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(230),this->MaxLevel);
		return 0;
	}

	if (this->MinReset != -1 && this->MinReset > lpObj->Reset)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(231), this->MinReset);
		return 0;
	}

	if (this->MaxReset != -1 && this->MaxReset < lpObj->Reset)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(232), this->MaxReset);
		return 0;
	}

	gObjMoveGate(lpObj->Index,this->WaitingGate);
	GCNewCharacterInfoSend(lpObj);
}


void CCustomTiecRuou::MuaRuouZen(LPOBJ lpObj)	//OK
{
	if(this->ZenMoi > lpObj->Money)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(1026), this->ZenMoi);
		return;
	}

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0 && gObj[n].Type == OBJECT_USER)
		{
			if (strcmp(gObj[n].GuildName,lpObj->Guild->Name) == 0)
			{
				gCashShop.GDCashShopAddPointSaveSend(gObj[n].Index, 0, 0, 0, 0, this->CongHienMoi);
				gCashShop.CGCashShopPointRecv(gObj[n].Index);
				GCEffectInfoSend(gObj[n].Index,16);
				gNotice.GCNoticeSend(gObj[n].Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(1027), lpObj->Name, this->CongHienMoi);
			}
		}
	}
	//
	lpObj->Money -= this->ZenMoi;
	GCMoneySend(lpObj->Index, lpObj->Money);
	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(1028), this->ZenMoi);
}

void CCustomTiecRuou::MuaRuouWC(LPOBJ lpObj)	//OK
{
	if(this->WcMoi > lpObj->Coin1)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(1029), this->WcMoi);
		return;
	}

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0 && gObj[n].Type == OBJECT_USER)
		{
			if (strcmp(gObj[n].GuildName,lpObj->Guild->Name) == 0)
			{
				gCashShop.GDCashShopAddPointSaveSend(gObj[n].Index, 0, 0, 0, 0, this->CongHienMoi);
				gCashShop.CGCashShopPointRecv(gObj[n].Index);
				GCEffectInfoSend(gObj[n].Index,16);
				gNotice.GCNoticeSend(gObj[n].Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(1027), lpObj->Name, this->CongHienMoi);
			}
		}
	}

	gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WcMoi, 0, 0, 0);

	gCashShop.CGCashShopPointRecv(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(1030), this->WcMoi);
}


void CCustomTiecRuou::StartTiecRuou()
{
	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);

	int hour	= aTime->tm_hour;
	int minute	= aTime->tm_min+2;


	if (minute >= 60)
	{
		hour++;
		minute = minute-60; 
	}

	CUSTOM_TIECRUOU_START_TIME info;

	info.Year = -1;

	info.Month = -1;

	info.Day = -1;

	info.DayOfWeek = -1;

	info.Hour = hour;

	info.Minute = minute;

	info.Second = 0;

	this->m_CustomTiecRuouInfo[0].StartTime.push_back(info);

	LogAdd(LOG_EVENT,"[Set Tiec Ruou Start] At %2d:%2d:00",hour,minute);

	this->Init();
}

#endif