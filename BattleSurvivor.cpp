#include "stdafx.h"
#include "BattleSurvivor.h"
#include "Guild.h"
#include "EffectManager.h"
#include "ItemBagManager.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "CashShop.h"
#include "Gate.h"
#include "ScheduleManager.h"
#include "MemScript.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "Message.h"
#include "ServerInfo.h"
#include "Util.h"

CBsVEvent gBsVEvent;

CBsVEvent::CBsVEvent()
{
	this->m_TickCount = GetTickCount();
	this->m_State = 0;
	this->m_RemainTime = 0;
	this->m_StandTime = 0;
	this->m_CloseTime = 0;
	this->m_TickCount = 0;
	this->m_WarningTime = 0;
	this->m_EventTime = 0;


	this->ScoreBlue = 0;
	this->ScoreRed = 0;
	this->TotalPlayer = 0;

	this->Coin1 = 0;
	this->Coin2 = 0;
	this->Coin3 = 0;

	this->CleanUser();

}

CBsVEvent::~CBsVEvent()
{
	
}

void CBsVEvent::Init()
{
	if(gServerInfo.m_BsVEventSwitch == 0)
	{
		this->SetState(BSV_EVENT_STATE_BLANK);
	}
	else
	{
		this->SetState(BSV_EVENT_STATE_EMPTY);
	}
}

void CBsVEvent::Clear()
{
	this->ScoreBlue = 0;
	this->ScoreRed = 0;
	this->TotalPlayer = 0;

	this->CleanUser();
}

void CBsVEvent::Load(char* path)
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

	this->m_BSVStartTime.clear();

	this->Clear();

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
					this->m_WarningTime = lpMemScript->GetNumber();

					this->m_StandTime = lpMemScript->GetAsNumber();

					this->m_EventTime = lpMemScript->GetAsNumber();

					this->m_CloseTime = lpMemScript->GetAsNumber();

				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					BSV_TIME info;

					info.Year = lpMemScript->GetNumber();

					info.Month = lpMemScript->GetAsNumber();

					info.Day = lpMemScript->GetAsNumber();

					info.DayOfWeek = lpMemScript->GetAsNumber();

					info.Hour = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					this->m_BSVStartTime.push_back(info);

				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}
					this->ReqItemCount = lpMemScript->GetNumber();

					this->ReqItemIndex = lpMemScript->GetAsNumber();

					this->ReqItemLevel = lpMemScript->GetAsNumber();

					this->EventMap = lpMemScript->GetAsNumber();

					this->WaitingGate = lpMemScript->GetAsNumber();

					this->BluTeamGate = lpMemScript->GetAsNumber();

					this->RedTeamGate = lpMemScript->GetAsNumber();

					this->SetSkin = lpMemScript->GetAsNumber();

					this->MinLevel = lpMemScript->GetAsNumber();

					this->MaxLevel = lpMemScript->GetAsNumber();

					this->MinReset = lpMemScript->GetAsNumber();

					this->MaxReset = lpMemScript->GetAsNumber();

					this->MinMasterReset = lpMemScript->GetAsNumber();

					this->MaxMasterReset = lpMemScript->GetAsNumber();

				}
				else if(section == 3)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}
					this->Coin1 = lpMemScript->GetNumber();

					this->Coin2 = lpMemScript->GetAsNumber();

					this->Coin3 = lpMemScript->GetAsNumber();
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

void CBsVEvent::MainProc()
{
	if((GetTickCount()-this->m_TickCount) >= 1000)
	{
		this->m_TickCount = GetTickCount();

		this->m_RemainTime = (int)difftime(this->TargetTime,time(0));

	if(gServerInfo.m_BsVEventSwitch == 0)
	{
		if (gServerDisplayer.EventBsV != -1)
		{
			gServerDisplayer.EventBsV = -1;
		}
	}
	else 
	{
		if (this->m_State == BSV_EVENT_STATE_EMPTY)
		{
			gServerDisplayer.EventBsV = this->m_RemainTime;
		}
		else 
		{
			if (gServerDisplayer.EventBsV != 0)
			{
				gServerDisplayer.EventBsV = 0;
			}
		}
	}

		switch(this->m_State)
		{
			case BSV_EVENT_STATE_BLANK:
				this->ProcState_BLANK();
				break;
			case BSV_EVENT_STATE_EMPTY:
				this->ProcState_EMPTY();
				break;
			case BSV_EVENT_STATE_STAND:
				this->ProcState_STAND();
				break;
			case BSV_EVENT_STATE_START:
				this->ProcState_START();
				break;
			case BSV_EVENT_STATE_CLEAN:
				this->ProcState_CLEAN();
				break;
		}
	}
}

void CBsVEvent::ProcState_BLANK() // OK
{

}

void CBsVEvent::ProcState_EMPTY() // OK
{
	if(this->m_RemainTime > 0 && this->m_RemainTime <= (this->m_WarningTime*60))
	{
		this->CheckUser();

		this->EnterEnabled = 1;

		if((this->AlarmMinSave=(((this->m_RemainTime%60)==0)?((this->m_RemainTime/60)-1):(this->m_RemainTime/60))) != this->AlarmMinLeft)
		{
			this->AlarmMinLeft = this->AlarmMinSave;

			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(991),(this->AlarmMinLeft+1));
			//gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(826));
		}
	}

	if(this->m_RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(993));
		this->NoticeSendToAll(1,gMessage.GetMessage(994),this->m_StandTime);
		this->SetState(BSV_EVENT_STATE_STAND);
		
	}
}

void CBsVEvent::ProcState_STAND() // OK
{
	this->CheckUser();

	if(this->GetUserCount() < gServerInfo.m_BsVEventMinUsers)
	{
		this->NoticeSendToAll(0,gMessage.GetMessage(830));
		this->SetState(BSV_EVENT_STATE_EMPTY);
		return;
	}

	if(this->m_RemainTime <= 0)
	{
		if (this->TotalPlayer >= gServerInfo.m_BsVEventMinUsers)
		{
			this->DivisionTeam();
			this->NoticeSendToAll(0,gMessage.GetMessage(995));
			this->SetState(BSV_EVENT_STATE_START);
		}
		else
		{
			this->NoticeSendToAll(0,gMessage.GetMessage(996));
			this->SetState(BSV_EVENT_STATE_EMPTY);
		}
	}
}

void CBsVEvent::ProcState_START() // OK
{
	this->CheckUser();

	if(this->GetUserCount() == 0)
	{
		this->SetState(BSV_EVENT_STATE_EMPTY);
		return;
	}

	if(this->GetUserCount() == 1)
	{
		this->SetState(BSV_EVENT_STATE_CLEAN);
		return;
	}

	if(this->m_RemainTime <= 0)
	{
		this->NoticeSendToAll(0,gMessage.GetMessage(996));
		this->SetState(BSV_EVENT_STATE_CLEAN);
	}
}

void CBsVEvent::ProcState_CLEAN() // OK
{
	this->CheckUser();

	if(this->m_RemainTime <= 0)
	{
		this->SetState(BSV_EVENT_STATE_EMPTY);
	}
}

void CBsVEvent::SetState(int state) // OK
{
	switch((this->m_State=state))
	{
		case BSV_EVENT_STATE_BLANK:
			this->SetState_BLANK();
			break;
		case BSV_EVENT_STATE_EMPTY:
			this->SetState_EMPTY();
			break;
		case BSV_EVENT_STATE_STAND:
			this->SetState_STAND();
			break;
		case BSV_EVENT_STATE_START:
			this->SetState_START();
			break;
		case BSV_EVENT_STATE_CLEAN:
			this->SetState_CLEAN();
			break;
	}
}

void CBsVEvent::SetState_BLANK() // OK
{

}

void CBsVEvent::SetState_EMPTY() // OK
{
	this->EnterEnabled = 0;
	this->AlarmMinSave = -1;
	this->AlarmMinLeft = -1;

	this->ClearUser();
	this->ScoreUser = 0;
	//this->ScoreRed = 0;

	this->CheckSync();
}

void CBsVEvent::SetState_STAND() // OK
{
	this->EnterEnabled = 0;
	this->AlarmMinSave = -1;
	this->AlarmMinLeft = -1;

	this->m_RemainTime = this->m_StandTime*60;

	this->TargetTime = (int)(time(0)+this->m_RemainTime);
}

void CBsVEvent::SetState_START() // OK
{
	this->EnterEnabled = 0;
	this->AlarmMinSave = -1;
	this->AlarmMinLeft = -1;

	this->m_RemainTime = this->m_EventTime*60;
	this->TargetTime = (int)(time(0)+this->m_RemainTime);

}

void CBsVEvent::SetState_CLEAN() // OK
{
	this->EnterEnabled = 0;
	this->AlarmMinSave = -1;
	this->AlarmMinLeft = -1;


	for(int n=0;n < MAX_BSVEVENT_USER;n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) != 0)
		{
			gObjViewportListProtocolCreate(&gObj[this->User[n].Index]);
			gObjectManager.CharacterUpdateMapEffect(&gObj[this->User[n].Index]);

			LPOBJ lpObj = &gObj[this->User[n].Index];
			LPOBJ lpMsG = &gObj[this->User[n].Index];

			

			if (this->Coin1 > 0 || this->Coin2 > 0 || this->Coin3 > 0)
			{	
				GDSetCoinSend(lpObj->Index, this->Coin1, this->Coin2, this->Coin3,"BsVEvent");
				//gItemBagManager.DropItemBySpecialValue(ITEM_BAG_BSV_EVENT,&gObj[lpObj->Index],0xEB,0,0);
			}

			GDRankingBsVEventSaveSend(lpObj->Index, this->User[n].Kills, this->User[n].Deaths);
		}
	}

	this->m_RemainTime = this->m_CloseTime*60;

	this->TargetTime = (int)(time(0)+this->m_RemainTime);
}

void CBsVEvent::CheckSync() // OK
{
	if(this->m_BSVStartTime.empty() != 0)
	{
		this->SetState(BSV_EVENT_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<BSV_TIME>::iterator it=this->m_BSVStartTime.begin();it != this->m_BSVStartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(BSV_EVENT_STATE_BLANK);
		return;
	}

	this->m_RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	this->TargetTime = (int)ScheduleTime.GetTime();
}

int CBsVEvent::GetState() // OK
{
	return this->m_State;
}

bool CBsVEvent::Dialog(LPOBJ lpObj, LPOBJ lpNpc)
{
	if (!gServerInfo.m_BsVEventSwitch) return false;

	if (lpNpc->Class == gServerInfo.m_BsVEventNPC &&
		lpNpc->Map == gServerInfo.m_BsVEventNPCMap &&
		lpNpc->X == gServerInfo.m_BsVEventNPCX &&
		lpNpc->Y == gServerInfo.m_BsVEventNPCY)
	{
		GCChatTargetSend(lpObj, lpNpc->Index, gMessage.GetMessage(992));
		this->CheckEnterEnabled(lpObj);
		return 1;
	}

	return false;
}

bool CBsVEvent::CheckEnterEnabled(LPOBJ lpObj)
{
	
	if(OBJECT_RANGE(lpObj->PartyNumber) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(1038));
		return 0;
	}

	if(lpObj->Guild != 0 && lpObj->Guild->WarState == 1)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(997));
		return 0;
	}

	if(OBJECT_RANGE(lpObj->DuelUser) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(997));
		return 0;
	}

	if(lpObj->PShopOpen != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(997));
		return 0;
	}

	if(this->GetEnterEnabled() == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(998));
		return 0;
	}

	if (this->CheckReqItems(lpObj) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(999));
		return 0;
	}

	if (this->GetUserCount() >= gServerInfo.m_BsVEventMaxUsers)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(997));
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

	if(this->MinReset != -1 && this->MinReset > lpObj->Reset)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(231),this->MinReset);
		return 0;
	}

	if(this->MaxReset != -1 && this->MaxReset < lpObj->Reset)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(232),this->MaxReset);
		return 0;
	}

	if(this->MinMasterReset != -1 && this->MinMasterReset > lpObj->MasterReset)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(818),this->MinMasterReset);
		return 0;
	}

	if(this->MaxMasterReset != -1 && this->MaxMasterReset < lpObj->MasterReset)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(819),this->MaxMasterReset);
		return 0;
	}

	gEffectManager.ClearAllEffect(lpObj);

	return this->AddUser(lpObj->Index);
}

bool CBsVEvent::CheckReqItems(LPOBJ lpObj)
{
	lpObj->ChaosLock = 1;
	int count = gItemManager.GetInventoryItemCount(lpObj,this->ReqItemIndex, this->ReqItemLevel);

	if (count < this->ReqItemCount)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(999));
		return false;
	}

	gItemManager.DeleteInventoryItemCount(lpObj,this->ReqItemIndex,this->ReqItemLevel,this->ReqItemCount);

	lpObj->ChaosLock = 0;
	return true;
}

int CBsVEvent::GetEnterEnabled() // OK
{
	return this->EnterEnabled;
}

int CBsVEvent::GetEnteredUserCount() // OK
{
	return this->GetUserCount();
}

bool CBsVEvent::CheckEnteredUser(int aIndex) // OK
{
	return ((this->GetUser(aIndex)==0)?0:1);
}

bool CBsVEvent::CheckPlayerTarget(LPOBJ lpObj) // OK
{
		if(this->GetState() == BSV_EVENT_STATE_START)
		{
			if (this->EventMap == lpObj->Map)
			{
				if(this->CheckEnteredUser(lpObj->Index) != 0)
				{
					return 1;
				}
			}
		}
	return 0;
}


bool CBsVEvent::CheckPlayerJoined(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(this->GetState() != BSV_EVENT_STATE_BLANK)
	{
		if (this->EventMap == lpObj->Map)
		{
			if(this->CheckEnteredUser(lpObj->Index) != 0)
			{
				return 1;
			}
		}
	}
	return 0;
}

bool CBsVEvent::CheckSelfTeam(LPOBJ lpObj,LPOBJ lpTarget)
{
	if(this->GetState() == BSV_EVENT_STATE_START)
	{
		BSV_EVENT_USER* lpUserA = this->GetUser(lpObj->Index);
		BSV_EVENT_USER* lpUserB = this->GetUser(lpTarget->Index);

		if (lpUserA == 0 || lpUserB == 0)
		{
			return 1;
		}

		if (lpUserA->Index == lpUserB->Index)
		{
			return 1;
		}
	}
	return 0;
}

bool CBsVEvent::AddUser(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(this->GetUser(aIndex) != 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_BSVEVENT_USER;n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) != 0)
		{
			continue;
		}

		this->User[n].Index = aIndex;
		this->User[n].Available = 1;
		this->TotalPlayer++;
		gObjMoveGate(aIndex, this->WaitingGate);
		return 1;
	}
	return 0;
}

bool CBsVEvent::DelUser(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	BSV_EVENT_USER* lpUser = this->GetUser(aIndex);

	if(lpUser == 0)
	{
		return 0;
	}

	lpUser->Reset();
	this->TotalPlayer--;
	return 1;
}

BSV_EVENT_USER* CBsVEvent::GetUser(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}


	for(int n=0;n < MAX_BSVEVENT_USER;n++)
	{
		if(this->User[n].Index == aIndex)
		{
			return &this->User[n];
		}
		
	}
	return 0;
}

void CBsVEvent::CleanUser() // OK
{
	for(int n=0;n < MAX_BSVEVENT_USER;n++)
	{
		this->User[n].Reset();
		this->TotalPlayer = 0;
	}
}

void CBsVEvent::ClearUser() // OK
{
	for(int n=0;n < MAX_BSVEVENT_USER;n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) == 0)
		{
			continue;
		}

		gObjMoveGate(this->User[n].Index,17);

		if (this->SetSkin == 1)
		{
			gObj[this->User[n].Index].Change = -1;
		}

		gObjViewportListProtocolCreate(&gObj[this->User[n].Index]);
		gObjectManager.CharacterUpdateMapEffect(&gObj[this->User[n].Index]);

		this->User[n].Reset();

		this->TotalPlayer--;
	}
}

void CBsVEvent::CheckUser() // OK
{

	for(int n=0;n < MAX_BSVEVENT_USER;n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) == 0)
		{
			continue;
		}

		if(gObjIsConnected(this->User[n].Index) == 0)
		{
			this->DelUser(this->User[n].Index);
			continue;
		}

		if (gObj[this->User[n].Index].Map != this->EventMap)
		{
			this->DelUser(this->User[n].Index);
			continue;
		}
	}


	
}

int CBsVEvent::GetUserCount() // OK
{
	int count = 0;

	for(int n=0;n < MAX_BSVEVENT_USER;n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) != 0)
		{
			count++;
		}

		if (gObj[n].PartyNumber >= 0)
		{
			PMSG_PARTY_DEL_MEMBER_RECV pMsg;
			int pindex = gParty.GetMemberIndex(gObj[n].PartyNumber, pMsg.number);
			if (pindex >= 0)
			{
				pMsg.number = pindex;
				gParty.CGPartyDelMemberRecv(&pMsg, gObj[n].Index);
			}
		}
	}

	return count;
}

bool CBsVEvent::GetUserRespawnLocation(LPOBJ lpObj,int* gate,int* map,int* x,int* y,int* dir,int* level) // OK
{
	if(gServerInfo.m_BsVEventSwitch == 0)
	{
		return 0;
	}

	if(this->GetState() != BSV_EVENT_STATE_START)
	{
		return 0;
	}


	BSV_EVENT_USER* lpUser = this->GetUser(lpObj->Index);

	if(lpUser == 0)
	{
		return 0;
	}

	if (lpUser->Index)
	{

		if(gGate.GetGate(this->BluTeamGate,gate,map,x,y,dir,level) != 0)
		{
			return 1;
		}
	}
	else
	{
		if(gGate.GetGate(this->WaitingGate,gate,map,x,y,dir,level) != 0)
		{
			return 1;
		}
	}

	return 0;
}

void CBsVEvent::DivisionTeam()
{
	int iTeam = 1;
	int Index = 0;

	for(int i=0;i<MAX_BSVEVENT_USER;i++)
	{
		Index = this->ReturnMaxReset();
		this->User[i].Team = iTeam;
		if(iTeam == 1) iTeam = 2;
		else iTeam = 1;
	}
	
	for(int n=0;n<MAX_BSVEVENT_USER;n++)
	{
		
		if (this->User[n].Team == 1)
		{
			gObjMoveGate(this->User[n].Index, this->BluTeamGate);
			gObjViewportListProtocolCreate(&gObj[this->User[n].Index]);
			gObjectManager.CharacterUpdateMapEffect(&gObj[this->User[n].Index]);
		}
		else if (this->User[n].Team == 2)
		{
			gObjMoveGate(this->User[n].Index, this->RedTeamGate);
			gObjViewportListProtocolCreate(&gObj[this->User[n].Index]);
			gObjectManager.CharacterUpdateMapEffect(&gObj[this->User[n].Index]);
		}
	}
}

int CBsVEvent::ReturnMaxReset()
{
	int MaxRes = 0;
	int Index = 0;
	int UserIndex = 0;

	for(int n=0;n < MAX_BSVEVENT_USER;n++)
	{
		LPOBJ lpObj = &gObj[this->User[n].Index];
		if(OBJECT_RANGE(lpObj->Index) == 0)
		{
			continue;
		}

		if (!this->User[n].Available)
		{
			continue;
		}

		if(lpObj->Reset >= MaxRes)
		{
			MaxRes = lpObj->Reset;
			Index = lpObj->Index;
			UserIndex = n;
		}
	}
	this->User[UserIndex].Available = 0;
	return Index;
}

void CBsVEvent::UserDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if (gServerInfo.m_BsVEventSwitch == 0)
	{
		return;
	}

	if(this->GetState() != BSV_EVENT_STATE_START)
	{
		return;
	}

	BSV_EVENT_USER* lpUserA = this->GetUser(lpObj->Index);

	if(lpUserA == 0)
	{
		return;
	}

	BSV_EVENT_USER* lpUserB = this->GetUser(lpTarget->Index);

	if(lpUserB == 0)
	{
		return;
	}

	if(lpObj->Index)
	{
		this->ScoreUser++;
	}
	else if(lpTarget->Index) 
	{
		this->ScoreUser++;
	}

	lpUserA->Deaths++;
	lpUserB->Kills++;

	gNotice.GCNoticeSend(lpUserA->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(1035), lpUserA->Deaths);
	gNotice.GCNoticeSend(lpUserB->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(1036), lpUserB->Kills);

	for (int i=0;i<MAX_BSVEVENT_USER;i++)
	{
		if(OBJECT_RANGE(this->User[i].Index) == 0)
		{
			continue;
		}

		if(this->User[i].Index)
		{
			gNotice.GCNoticeSend(this->User[i].Index,0,0,0,0,0,0,gMessage.GetMessage(1037),lpObj->Name,lpTarget->Name,lpUserB->Kills);
		}
	}
}

void CBsVEvent::NoticeSendToAll(int type,char* message,...) // OK
{
	char buff[256];

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	for(int n=0;n < MAX_BSVEVENT_USER;n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) != 0)
		{
			gNotice.GCNoticeSend(this->User[n].Index,type,0,0,0,0,0,buff);
		}
	}
}

void CBsVEvent::StartBSV()
{
	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);

	//int day = aTime->tm_mday;
	//int month = aTime->tm_mon + 1;
	//int year = aTime->tm_year + 1900;

	int hour	= aTime->tm_hour;
	int minute	= aTime->tm_min+5;


	if (minute >= 60)
	{
		hour++;
		minute = minute-60; 
	}

	BSV_TIME info;

	info.Year = -1;

	info.Month = -1;

	info.Day = -1;

	info.DayOfWeek = -1;

	info.Hour = hour;

	info.Minute = minute;

	info.Second = 0;

	this->m_BSVStartTime.push_back(info);

	LogAdd(LOG_EVENT,"[Set BsV Start] At %2d:%2d:00",hour,minute);

	this->Init();
}
