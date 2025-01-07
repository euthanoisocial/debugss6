// LeoThap.cpp: implementation of the CLeoThap class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LeoThap.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "ItemManager.h"
#include "Map.h"
#include "MapServerManager.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "MonsterSetBase.h"
#include "Notice.h"
#include "NpcTalk.h"
#include "ObjectManager.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"
#include "Party.h"
CLeoThap gLeoThap;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLeoThap::CLeoThap() // OK
{
	for(int n=0;n < MAX_LT_LEVEL;n++)
	{
		LEO_THAP_LEVEL* lpLevel = &this->m_LeoThapLevel[n];

		lpLevel->Level = n;
		lpLevel->State = LT_STATE_BLANK;
		lpLevel->Map = -1;
		lpLevel->RemainTime = 0;
		lpLevel->TargetTime = 0;
		lpLevel->TickCount = GetTickCount();
		lpLevel->EnterEnabled = 0;
		lpLevel->MinutesLeft = -1;
		lpLevel->TimeCount = 0;
		lpLevel->Stage = 0;
		lpLevel->moveG1 = 0;
		lpLevel->monterG1 = 0;
		lpLevel->moveG2 = 0;
		lpLevel->monterG2 = 0;
		lpLevel->moveG3 = 0;
		lpLevel->monterG3 = 0;
		this->NPC.Class = -1;
		this->NPC.Map = -1;
		this->NPC.X = -1;
		this->NPC.Y = -1;
		this->CleanUser(lpLevel);

		this->CleanMonster(lpLevel);
	}
}

CLeoThap::~CLeoThap() // OK
{

}

void CLeoThap::Init() // OK
{
	for(int n=0;n < MAX_LT_LEVEL;n++)
	{
		if(gServerInfo.m_LeoThapEvent == 0)
		{
			this->SetState(&this->m_LeoThapLevel[n],LT_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_LeoThapLevel[n],LT_STATE_EMPTY);
		}
	}
}

void CLeoThap::Load(char* path) // OK
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

	this->m_LeoThapStartTime.clear();

	this->m_LeoThapTang1.clear();
	this->m_LeoThapTang2.clear();
	this->m_LeoThapTang3.clear();
	this->NPC.Class = -1;
	this->NPC.Map = -1;
	this->NPC.X = -1;
	this->NPC.Y = -1;
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

					this->m_NotifyTime = lpMemScript->GetAsNumber();

					this->m_EventTime = lpMemScript->GetAsNumber();

					this->m_CloseTime = lpMemScript->GetAsNumber();
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					LEO_THAP_START_TIME info;

					info.Year = lpMemScript->GetNumber();

					info.Month = lpMemScript->GetAsNumber();

					info.Day = lpMemScript->GetAsNumber();

					info.DayOfWeek = lpMemScript->GetAsNumber();

					info.Hour = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					this->m_LeoThapStartTime.push_back(info);
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					LEO_THAP_THUONG info;
					info.Monter = lpMemScript->GetNumber();
					
					info.WC = lpMemScript->GetAsNumber();

					info.WP = lpMemScript->GetAsNumber();

					info.GP = lpMemScript->GetAsNumber();

					info.Chao = lpMemScript->GetAsNumber();

					info.Cre = lpMemScript->GetAsNumber();

					info.Life = lpMemScript->GetAsNumber();

					info.Bles = lpMemScript->GetAsNumber();

					info.Sould = lpMemScript->GetAsNumber();

					info.Boss = lpMemScript->GetAsNumber();

					info.Time = lpMemScript->GetAsNumber();

					this->m_LeoThapTang1.push_back(info);
				}
				else if(section == 3)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					LEO_THAP_THUONG info;
					info.Monter = lpMemScript->GetNumber();
					
					info.WC = lpMemScript->GetAsNumber();

					info.WP = lpMemScript->GetAsNumber();

					info.GP = lpMemScript->GetAsNumber();

					info.Chao = lpMemScript->GetAsNumber();

					info.Cre = lpMemScript->GetAsNumber();

					info.Life = lpMemScript->GetAsNumber();

					info.Bles = lpMemScript->GetAsNumber();

					info.Sould = lpMemScript->GetAsNumber();

					info.Boss = lpMemScript->GetAsNumber();

					info.Time = lpMemScript->GetAsNumber();

					this->m_LeoThapTang2.push_back(info);

				}
				else if(section == 4)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					LEO_THAP_THUONG info;
					info.Monter = lpMemScript->GetNumber();
					
					info.WC = lpMemScript->GetAsNumber();

					info.WP = lpMemScript->GetAsNumber();

					info.GP = lpMemScript->GetAsNumber();

					info.Chao = lpMemScript->GetAsNumber();

					info.Cre = lpMemScript->GetAsNumber();

					info.Life = lpMemScript->GetAsNumber();

					info.Bles = lpMemScript->GetAsNumber();

					info.Sould = lpMemScript->GetAsNumber();

					info.Boss = lpMemScript->GetAsNumber();

					info.Time = lpMemScript->GetAsNumber();

					this->m_LeoThapTang3.push_back(info);
				}
				else if(section == 5)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->NPC.Class = lpMemScript->GetNumber();
					this->NPC.Map = lpMemScript->GetAsNumber();
					this->NPC.X = lpMemScript->GetAsNumber();
					this->NPC.Y = lpMemScript->GetAsNumber();
				}
				else if(section == 6)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_luot = lpMemScript->GetNumber();
				}
				else if(section == 7)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->LP.WC = lpMemScript->GetNumber();
					this->LP.WP = lpMemScript->GetAsNumber();
					this->LP.GP = lpMemScript->GetAsNumber();
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

void CLeoThap::MainProc() // OK
{
	for(int n=0;n < MAX_LT_LEVEL;n++)
	{
		LEO_THAP_LEVEL* lpLevel = &this->m_LeoThapLevel[n];

		DWORD elapsed = GetTickCount()-lpLevel->TickCount;

		if(elapsed < 1000)
		{
			continue;
		}

		lpLevel->TickCount = GetTickCount();

		lpLevel->RemainTime = (int)difftime(lpLevel->TargetTime,time(0));

		if (n==0)
		{
			if(gServerInfo.m_LeoThapEvent == 0)
			{
				if (gServerDisplayer.EventLT != -1)
				{
					gServerDisplayer.EventLT = -1;
				}
			}
			else 
			{
				if (lpLevel->State == LT_STATE_EMPTY)
				{
					gServerDisplayer.EventLT = lpLevel->RemainTime;
				}
				else 
				{
					if (gServerDisplayer.EventLT != 0)
					{
						gServerDisplayer.EventLT = 0;
					}
				}
			}
		}

		if(gServerDisplayer.EventLT > 0)
		{
			if (lpLevel->State != LT_STATE_EMPTY && lpLevel->State != LT_STATE_BLANK)
			{
				gServerDisplayer.EventLT = 0;
			}
		}

		switch(lpLevel->State)
		{
			case LT_STATE_BLANK:
				this->ProcState_BLANK(lpLevel);
				break;
			case LT_STATE_EMPTY:
				this->ProcState_EMPTY(lpLevel);
				break;
			case LT_STATE_STAND:
				this->ProcState_STAND(lpLevel);
				break;
			case LT_STATE_START:
				this->ProcState_START(lpLevel);
				break;
			case LT_STATE_CLEAN:
				this->ProcState_CLEAN(lpLevel);
				break;
		}
	}
}

void CLeoThap::ProcState_BLANK(LEO_THAP_LEVEL* lpLevel) // OK
{

}

void CLeoThap::ProcState_EMPTY(LEO_THAP_LEVEL* lpLevel) // OK
{
	if(lpLevel->RemainTime > 0 && lpLevel->RemainTime <= (this->m_WarningTime*60))
	{
		this->CheckUser(lpLevel);
		//this->checkUserNone(lpLevel);
		int minutes = lpLevel->RemainTime/60;

		if((lpLevel->RemainTime%60) == 0)
		{
			minutes--;
		}

		if(lpLevel->MinutesLeft != minutes)
		{
			lpLevel->MinutesLeft = minutes;

			if(lpLevel->Level == 0)
			{
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[Leo Tháp] sẽ đóng lại sau %d phút",(lpLevel->MinutesLeft+1));
			}
		}

		lpLevel->EnterEnabled = 1;
	}

	if(lpLevel->RemainTime >= 0 && lpLevel->RemainTime <= 60 && lpLevel->TimeCount == 0)
	{
		// Cổng vào leo tháp sẽ đóng lại sau 30s

		for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
		{
			if(gObjIsConnectedGP(n) != 0 && gObj[n].Type == OBJECT_USER)
			{
				this->SendTime2(gObj[n].Index,lpLevel->RemainTime, lpLevel->State);
			}
		} // end
	}

	if(lpLevel->RemainTime <= 0)
	{
		//if(lpLevel->Level == 0){gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"Leo tháp đóng cửa");}
		this->NoticeSendToAll(lpLevel,1,"[Leo Tháp] sẽ đóng lại sau %d phút",(lpLevel->Level+1),this->m_NotifyTime);
		this->SetState(lpLevel,LT_STATE_STAND);
	}
}

void CLeoThap::ProcState_STAND(LEO_THAP_LEVEL* lpLevel) // OK
{
	this->CheckUser(lpLevel);
	//this->checkUserNone(lpLevel);
	if(this->GetUserCount(lpLevel) == 0)
	{
		this->SetState(lpLevel,LT_STATE_EMPTY);
		return;
	}

	if(lpLevel->RemainTime >= 0 && lpLevel->RemainTime <= 60 && lpLevel->TimeCount == 0)
	{
		// Gửi thông báo còn 60S
		for(int n=0;n < MAX_LT_USER;n++)
		{
			if(lpLevel->User[n].Index > 0)
			{
				this->SendTime2(lpLevel->User[n].Index,lpLevel->RemainTime, lpLevel->State);
			}
		}
	}

	if(lpLevel->RemainTime <= 0)
	{
	
		this->NoticeSendToAll(lpLevel,0,"[Leo Tháp] bắt đầu");

		for(int n=0;n < MAX_LT_USER;n++)
		{
			if(lpLevel->User[n].Index > 0)
			{
				LPOBJ lpObj = &gObj[lpLevel->User[n].Index];
				lpObj->TowerCount ++;
				lpObj->TowerTime = this->getTime();
				GDCharacterInfoSaveSend(lpObj->Index);
			}
		}

		this->SetState(lpLevel,LT_STATE_START);
	}
}

void CLeoThap::ProcState_START(LEO_THAP_LEVEL* lpLevel) // OK
{
	this->CheckUser(lpLevel);

	this->checkUserNone(lpLevel);

	if(this->GetUserCount(lpLevel) == 0)
	{
		this->SetState(lpLevel,LT_STATE_EMPTY);
		return;
	}

	int time3 = (this->m_EventTime / 3)*60;
	int time2 = time3 * 2;

	if(lpLevel->RemainTime > time2)
	{
		lpLevel->Stage = 0;
	} else if(lpLevel->RemainTime > time3)
	{
		lpLevel->Stage = 1;
		if(lpLevel->moveG2 == 0)
		{
			// Move user lên tầng 2

		for(int n=0;n < MAX_LT_USER;n++)
		{
			if(lpLevel->User[n].Index > 0 && (lpLevel->User[n].Gate == 0))
			{
				// Move
				gObjMoveGate(lpLevel->User[n].Index,520);
			}

			if(lpLevel->User[n].Index > 0 && (lpLevel->User[n].Gate == 1))
			{
				// Move
				gObjMoveGate(lpLevel->User[n].Index,521);
			}

			if(lpLevel->User[n].Index > 0 && (lpLevel->User[n].Gate == 2))
			{
				// Move
				gObjMoveGate(lpLevel->User[n].Index,522);
			}


			if(lpLevel->User[n].Index > 0 && (lpLevel->User[n].Gate == 3))
			{
				// Move
				gObjMoveGate(lpLevel->User[n].Index,523);
			}

		}
		lpLevel->moveG2 = 1;

		}


	} else {
		lpLevel->Stage = 2;
		if(lpLevel->moveG3 == 0)
		{
			// Move user lên tầng 3
		for(int n=0;n < MAX_LT_USER;n++)
		{
				//if(lpLevel->User[n].Index > 0)
				//{
				//	// Move
				//	if(lpLevel->User[n].Index > 0)
				//	{
				//		// Move
				//		gObjMoveGate(lpLevel->User[n].Index,522);
				//	}
				//}
			if(lpLevel->User[n].Index > 0 && (lpLevel->User[n].Gate == 0 ))
			{
				// Move
				gObjMoveGate(lpLevel->User[n].Index,524);
			}

			if(lpLevel->User[n].Index > 0 && (lpLevel->User[n].Gate == 1 ))
			{
				// Move
				gObjMoveGate(lpLevel->User[n].Index,524);
			}

			if(lpLevel->User[n].Index > 0 && (lpLevel->User[n].Gate == 2 ))
			{
				// Move
				gObjMoveGate(lpLevel->User[n].Index,525);
			}


			if(lpLevel->User[n].Index > 0 && (lpLevel->User[n].Gate == 3))
			{
				// Move
				gObjMoveGate(lpLevel->User[n].Index,525);
			}
		}

		lpLevel->moveG3 = 1;
		}
	}

	// Gửi thời gian về Client
	for(int n=0;n < MAX_LT_USER;n++)
	{
		if(lpLevel->User[n].Index > 0)
		{
			this->SendTime(lpLevel->User[n].Index, lpLevel->RemainTime, lpLevel->Stage, this->m_EventTime);
		}
	}

	switch(lpLevel->Stage)
	{
		case 0:
			this->SetStage0(lpLevel);
			break;
		case 1:
			this->SetStage1(lpLevel);
			break;
		case 2:
			this->SetStage2(lpLevel);
			break;
	}

	if(lpLevel->RemainTime <= 0)
	{
		this->NoticeSendToAll(lpLevel,0, "[Leo Tháp] kết thúc");

		this->SetState(lpLevel,LT_STATE_CLEAN);
	}
}

void CLeoThap::ProcState_CLEAN(LEO_THAP_LEVEL* lpLevel) // OK
{
	this->CheckUser(lpLevel);
	//this->checkUserNone(lpLevel);
	if(lpLevel->RemainTime <= 0)
	{
		this->SetState(lpLevel,LT_STATE_EMPTY);
	}
}

void CLeoThap::SetState(LEO_THAP_LEVEL* lpLevel,int state) // OK
{
	lpLevel->State = state;

	switch(lpLevel->State)
	{
		case LT_STATE_BLANK:
			this->SetState_BLANK(lpLevel);
			break;
		case LT_STATE_EMPTY:
			this->SetState_EMPTY(lpLevel);
			break;
		case LT_STATE_STAND:
			this->SetState_STAND(lpLevel);
			break;
		case LT_STATE_START:
			this->SetState_START(lpLevel);
			break;
		case LT_STATE_CLEAN:
			this->SetState_CLEAN(lpLevel);
			break;
	}
}

void CLeoThap::SetState_BLANK(LEO_THAP_LEVEL* lpLevel) // OK
{
}

void CLeoThap::SetState_EMPTY(LEO_THAP_LEVEL* lpLevel) // OK
{
	lpLevel->EnterEnabled = 0;
	lpLevel->MinutesLeft = -1;
	lpLevel->TimeCount = 0;
	lpLevel->Stage = 0;
	lpLevel->moveG1 = 0;
	lpLevel->monterG1 = 0;
	lpLevel->moveG2 = 0;
	lpLevel->monterG2 = 0;
	lpLevel->moveG3 = 0;
	lpLevel->monterG3 = 0;
	this->ClearUser(lpLevel);

	this->ClearMonster(lpLevel);

	this->CheckSync(lpLevel);
}

void CLeoThap::SetState_STAND(LEO_THAP_LEVEL* lpLevel) // OK
{
	lpLevel->EnterEnabled = 0;
	lpLevel->MinutesLeft = -1;
	lpLevel->TimeCount = 0;

	lpLevel->RemainTime = this->m_NotifyTime*60;

	lpLevel->TargetTime = (int)(time(0)+lpLevel->RemainTime);
}

void CLeoThap::SetState_START(LEO_THAP_LEVEL* lpLevel) // OK
{
	lpLevel->EnterEnabled = 0;
	lpLevel->MinutesLeft = -1;
	lpLevel->TimeCount = 0;

	lpLevel->RemainTime = this->m_EventTime*60;

	lpLevel->TargetTime = (int)(time(0)+lpLevel->RemainTime);

}

void CLeoThap::SetState_CLEAN(LEO_THAP_LEVEL* lpLevel) // OK
{
	lpLevel->EnterEnabled = 0;
	lpLevel->MinutesLeft = -1;
	lpLevel->TimeCount = 0;
	lpLevel->moveG1 = 0;
	lpLevel->monterG1 = 0;
	lpLevel->moveG2 = 0;
	lpLevel->monterG2 = 0;
	lpLevel->moveG3 = 0;
	lpLevel->monterG3 = 0;

	this->ClearMonster(lpLevel);

	lpLevel->RemainTime = this->m_CloseTime*60;

	lpLevel->TargetTime = (int)(time(0)+lpLevel->RemainTime);
}

void CLeoThap::SetStage0(LEO_THAP_LEVEL* lpLevel) // OK
{
	if(lpLevel->monterG1 == 0)
	{
		for(std::vector<LEO_THAP_THUONG>::iterator it=this->m_LeoThapTang1.begin();it != this->m_LeoThapTang1.end();it++)
		{
			this->SetMonster(lpLevel,it->Monter, it->Time);
		}

		lpLevel->monterG1 = 1;
	}
}

void CLeoThap::SetStage1(LEO_THAP_LEVEL* lpLevel) // OK
{
	if(lpLevel->monterG2 == 0)
	{
		for(std::vector<LEO_THAP_THUONG>::iterator it=this->m_LeoThapTang2.begin();it != this->m_LeoThapTang2.end();it++)
		{
			this->SetMonster(lpLevel,it->Monter, it->Time);
		}
		lpLevel->monterG2 = 1;
	}
}

void CLeoThap::SetStage2(LEO_THAP_LEVEL* lpLevel) // OK
{
	if(lpLevel->monterG3 == 0)
	{
		for(std::vector<LEO_THAP_THUONG>::iterator it=this->m_LeoThapTang3.begin();it != this->m_LeoThapTang3.end();it++)
		{
			this->SetMonster(lpLevel,it->Monter, it->Time);
		}
		lpLevel->monterG3 = 1;
	}
}

void CLeoThap::CheckSync(LEO_THAP_LEVEL* lpLevel) // OK
{
	if(this->m_LeoThapStartTime.empty() != 0)
	{
		this->SetState(lpLevel,LT_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<LEO_THAP_START_TIME>::iterator it=this->m_LeoThapStartTime.begin();it != this->m_LeoThapStartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpLevel,LT_STATE_BLANK);
		return;
	}

	lpLevel->RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	lpLevel->TargetTime = (int)ScheduleTime.GetTime();

}

int CLeoThap::GetState(int level) // OK
{
	if(LT_LEVEL_RANGE(level) == 0)
	{
		return LT_STATE_BLANK;
	}

	return this->m_LeoThapLevel[level].State;
}

int CLeoThap::GetRemainTime(int level) // OK
{
	if(LT_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}

	if(this->m_LeoThapStartTime.empty() != 0)
	{
		return 0;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<LEO_THAP_START_TIME>::iterator it=this->m_LeoThapStartTime.begin();it != this->m_LeoThapStartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		return 0;
	}

	int RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	return (((RemainTime%60)==0)?(RemainTime/60):((RemainTime/60)+1));
}

int CLeoThap::GetEnterEnabled(int level) // OK
{
	if(LT_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}
	
	return this->m_LeoThapLevel[level].EnterEnabled;
}

int CLeoThap::GetEnteredUserCount(int level) // OK
{
	if(LT_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}

	return this->GetUserCount(&this->m_LeoThapLevel[level]);
}

bool CLeoThap::AddUser(LEO_THAP_LEVEL* lpLevel,int aIndex, int gate) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(this->GetUser(lpLevel,aIndex) != 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_LT_USER;n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) > 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[aIndex];

		// Kiểm tra lệ phí

		if(LP.WC > 0)
		{
			if(lpObj->Coin1 < LP.WC)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"Bạn không đủ %d WCoinC:", LP.WC);
				return 0;
			}
		}

		if(LP.WP > 0)
		{
			if(lpObj->Coin2 < LP.WP)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"Bạn không đủ %d WP:", LP.WP);
				return 0;
			}
		}

		if(LP.GP > 0)
		{
			if(lpObj->Coin3 < LP.GP)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"Bạn không đủ %d GP:", LP.GP);
				return 0;
			}
		}

		if(lpObj->TowerTime == this->getTime())
		{
			if(lpObj->TowerCount >= this->m_luot)
			{
				// Thông báo bạn hết lượt
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"Bạn hết lượt trong hôm nay!");
				return 0;
			}
		}

		lpLevel->User[n].Index = aIndex;
		lpLevel->User[n].Gate = gate;
		// Trừ tiền
		GDSetCoinSend(aIndex, -LP.WC, -LP.WP, -LP.GP, "Le Phi Leo Thap");

		return 1;
	}
	return 0;
}

bool CLeoThap::DelUser(LEO_THAP_LEVEL* lpLevel,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LEO_THAP_USER* lpUser = this->GetUser(lpLevel,aIndex);

	if(lpUser == 0)
	{
		return 0;
	}

	lpUser->Index = -1;
	lpUser->Gate = -1;
	return 1;
}

LEO_THAP_USER* CLeoThap::GetUser(LEO_THAP_LEVEL* lpLevel,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_LT_USER;n++)
	{
		if(lpLevel->User[n].Index == aIndex)
		{
			return &lpLevel->User[n];
		}
	}

	return 0;
}

void CLeoThap::CleanUser(LEO_THAP_LEVEL* lpLevel) // OK
{
	for(int n=0;n < MAX_LT_USER;n++)
	{
		lpLevel->User[n].Index = -1;
		lpLevel->User[n].Gate = -1;
	}
}

void CLeoThap::ClearUser(LEO_THAP_LEVEL* lpLevel) // OK
{
	for(int n=0;n < MAX_LT_USER;n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) == 0)
		{
			continue;
		}
		LPOBJ lpObj = &gObj[lpLevel->User[n].Index];
		gObjMoveGate(lpLevel->User[n].Index,17);
		gObjViewportListProtocolCreate(lpObj);
		gObjectManager.CharacterUpdateMapEffect(lpObj);
		lpLevel->User[n].Index = -1;
		lpLevel->User[n].Gate = -1;
	}
}

void CLeoThap::CheckUser(LEO_THAP_LEVEL* lpLevel) // OK
{
	for(int n=0;n < MAX_LT_USER;n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) == 0)
		{
			continue;
		}

		if(gObjIsConnected(lpLevel->User[n].Index) == 0)
		{
			this->DelUser(lpLevel,lpLevel->User[n].Index);
			continue;
		}

		if(gObj[lpLevel->User[n].Index].Map != gServerInfo.m_LeoThapMap)
		{
			this->DelUser(lpLevel,lpLevel->User[n].Index);
			continue;
		}
	}
}

int CLeoThap::GetUserCount(LEO_THAP_LEVEL* lpLevel) // OK
{
	int count = 0;

	for(int n=0;n < MAX_LT_USER;n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			count++;
		}
	}

	return count;
}

int CLeoThap::GetUserAbleLevel(LPOBJ lpObj) // OK
{
	return 0;
}

bool CLeoThap::AddMonster(LEO_THAP_LEVEL* lpLevel,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(this->GetMonster(lpLevel,aIndex) != 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_LT_MONSTER;n++)
	{
		if(OBJECT_RANGE(lpLevel->MonsterIndex[n]) != 0)
		{
			continue;
		}

		lpLevel->MonsterIndex[n] = aIndex;
		return 1;
	}

	return 0;
}

bool CLeoThap::DelMonster(LEO_THAP_LEVEL* lpLevel,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	int* index = this->GetMonster(lpLevel,aIndex);

	if(index == 0)
	{
		return 0;
	}

	(*index) = -1;
	return 1;
}

int* CLeoThap::GetMonster(LEO_THAP_LEVEL* lpLevel,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_LT_MONSTER;n++)
	{
		if(lpLevel->MonsterIndex[n] == aIndex)
		{
			return &lpLevel->MonsterIndex[n];
		}
	}

	return 0;
}

void CLeoThap::CleanMonster(LEO_THAP_LEVEL* lpLevel) // OK
{
	for(int n=0;n < MAX_LT_MONSTER;n++)
	{
		lpLevel->MonsterIndex[n] = -1;
	}
}

void CLeoThap::ClearMonster(LEO_THAP_LEVEL* lpLevel) // OK
{
	for(int n=0;n < MAX_LT_MONSTER;n++)
	{
		if(OBJECT_RANGE(lpLevel->MonsterIndex[n]) != 0)
		{
			gObjDel(lpLevel->MonsterIndex[n]);
			lpLevel->MonsterIndex[n] = -1;
		}
	}
}

int CLeoThap::GetMonsterCount(LEO_THAP_LEVEL* lpLevel) // OK
{
	int count = 0;

	for(int n=0;n < MAX_LT_MONSTER;n++)
	{
		if(OBJECT_RANGE(lpLevel->MonsterIndex[n]) != 0)
		{
			count++;
		}
	}

	return count;
}

void CLeoThap::SetMonster(LEO_THAP_LEVEL* lpLevel,int MonsterClass, int Time) // OK
{
	for(int n=0;n < gMonsterSetBase.m_count;n++)
	{
		MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[n];

		if(lpInfo->Type != 3 || lpInfo->MonsterClass != MonsterClass || lpInfo->Map != gServerInfo.m_LeoThapMap)
		{
			continue;
		}

		int index = gObjAddMonster(gServerInfo.m_LeoThapMap);

		if(OBJECT_RANGE(index) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[index];

		if(gObjSetPosMonster(index,n) == 0)
		{
			gObjDel(index);
			continue;
		}

		if(gObjSetMonster(index,lpInfo->MonsterClass) == 0)
		{
			gObjDel(index);
			continue;
		}

		if(Time > 0)
		{
			lpObj->MaxRegenTime = Time * 1000;
		}
		else 
		{
			lpObj->MaxRegenTime = 1000;
		}

		if(this->AddMonster(lpLevel,index) == 0)
		{
			gObjDel(index);
			continue;
		}
	}
}

bool CLeoThap::Dialog(LPOBJ lpObj, LPOBJ lpNpc) // OK
{
	if(gServerInfo.m_LeoThapEvent == 0)
	{
		return false;
	}

	if (lpNpc->Class == this->NPC.Class &&
		lpNpc->Map == this->NPC.Map &&
		lpNpc->X == this->NPC.X &&
		lpNpc->Y == this->NPC.Y)
	{
		int level = this->GetUserAbleLevel(lpObj);
		if(this->GetEnterEnabled(level) == 0)
		{
			GCChatTargetSend(lpObj, lpNpc->Index, "Leo tháp chưa mở bạn nhé!");
			return 0;
		}

		PMSG_LEOTHAP_REQ pRequest;
		pRequest.Head.set(0xFB,0x27, sizeof(pRequest));
		pRequest.isOpenWindow = true;
		DataSend(lpObj->Index, (LPBYTE)&pRequest, sizeof(pRequest));

		return 1;
	}
	return false;
}

void CLeoThap::RecvLeoThap(PMSG_LEOTHAP_RECV* Data, int aIndex)
{
	LEO_THAP_LEVEL* lpLevel = &this->m_LeoThapLevel[0];

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"Cong: %d",Data->Number );

	if(this->AddUser(lpLevel, aIndex, Data->Number))
	{
		if(Data->Number == 0)
		{
			gObjMoveGate(aIndex,516);
		}
		else if(Data->Number == 1)
		{
			gObjMoveGate(aIndex,517);
		}
		else if(Data->Number == 2)
		{
			gObjMoveGate(aIndex,518);
		}
		else if(Data->Number == 3)
		{
			gObjMoveGate(aIndex,519);
		}
		LPOBJ lpObj = &gObj[aIndex];

		gObjViewportListProtocolCreate(lpObj);
		gObjectManager.CharacterUpdateMapEffect(lpObj);
	}
}

void CLeoThap::UserDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	int level = this->GetUserAbleLevel(lpObj);

	if(LT_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	if(this->GetState(level) != LT_STATE_START)
	{
		return;
	}
	if(this->GetUser(&this->m_LeoThapLevel[level],lpObj->Index) == 0)
	{
		return;
	}
	this->DelUser(&this->m_LeoThapLevel[level],lpObj->Index);
	//GDCharacterInfoSaveSend(lpObj->Index);
	gObjMoveGate(lpObj->Index,17);

	//gObjViewportListProtocolCreate(lpObj);
	//gObjectManager.CharacterUpdateMapEffect(lpObj);

}

void CLeoThap::MonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	int level = this->GetUserAbleLevel(lpTarget);

	if(LT_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	if(this->GetState(level) != LT_STATE_START)
	{
		return;
	}

	if(this->GetMonster(&this->m_LeoThapLevel[level],lpObj->Index) == 0)
	{
		return;
	}

	if(lpTarget->Type != OBJECT_USER)
	{
		return;
	}

	LEO_THAP_USER* lpUser = this->GetUser(&this->m_LeoThapLevel[level],lpTarget->Index);

	if(lpUser == 0)
	{
		return;
	}

	// Thưởng điểm thành tựu
	
	for(std::vector<LEO_THAP_THUONG>::iterator it=this->m_LeoThapTang1.begin();it != this->m_LeoThapTang1.end();it++)
	{
		if(lpObj->Class == it->Monter && it->Boss == 1)
		{
			 if(it->Chao > 0)
			 {
				 int px = lpObj->X;
				 int py = lpObj->Y;
				 for(int i=0; i<it->Chao; i++)
				 {
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(12, 15), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				 }
			 }

			 if(it->Cre > 0)
			 {
				int px = lpObj->X;
				int py = lpObj->Y;
				for(int i=0; i<it->Cre; i++)
				{
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(14, 22), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				}
			 }

			 if(it->Life > 0)
			 {
				int px = lpObj->X;
				int py = lpObj->Y;
				for(int i=0; i<it->Life; i++)
				{
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(14, 16), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				}
			 }

			 if(it->Bles > 0)
			 {
				int px = lpObj->X;
				int py = lpObj->Y;
				for(int i=0; i<it->Bles; i++)
				{
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(14, 13), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				}
			 }

			 if(it->Sould > 0)
			 {
				int px = lpObj->X;
				int py = lpObj->Y;
				for(int i=0; i<it->Sould; i++)
				{
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(14, 14), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				}
			 }
			 // Thưởng cùng party
			 //lpTarget->PartyNumber
			 if(gParty.IsParty(lpTarget->PartyNumber) == 1) // kiem tra thang giet bos co PT ko
			 {
				 for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
				 {
			 		if(gObjIsConnectedGP(n) != 0 && gObj[n].Type == OBJECT_USER)
			 		{
						if(gParty.IsParty(gObj[n].PartyNumber) == 1 && gObj[n].PartyNumber == lpTarget->PartyNumber && gObj[n].Map == gServerInfo.m_LeoThapMap)
			 			{
							// Check tọa độ + tiền đánh boos +-30
							if(gObj[n].X <= lpTarget->X + 30 && gObj[n].X >= lpTarget->X - 30 && gObj[n].Y <= lpTarget->Y + 30 && gObj[n].Y >= lpTarget->Y - 30)
							{
								GDSetCoinSend(gObj[n].Index, it->WC, it->WP, it->GP, "Leo Thap");
							}
			 			}
			 		}
				 } // end
			 } else // Khong co PT thi thuong cho minh no
			 {
				 GDSetCoinSend(lpTarget->Index, it->WC, it->WP, it->GP, "Leo Thap");
			 }
		 }
		 else if(lpObj->Class == it->Monter) // Truong hop quai nho
		 {
			 // Check thuong quai nho cho PT
			if(gParty.IsParty(lpTarget->PartyNumber) == 1) // kiem tra thang giet quai co PT ko
			 {
				 for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
				 {
			 		if(gObjIsConnectedGP(n) != 0 && gObj[n].Type == OBJECT_USER)
			 		{
						if(gParty.IsParty(gObj[n].PartyNumber) == 1 && gObj[n].PartyNumber == lpTarget->PartyNumber && gObj[n].Map == gServerInfo.m_LeoThapMap)
			 			{
							// Check tọa độ + tiền đánh boos +-30
							if(gObj[n].X <= lpTarget->X + 30 && gObj[n].X >= lpTarget->X - 30 && gObj[n].Y <= lpTarget->Y + 30 && gObj[n].Y >= lpTarget->Y - 30)
							{
								GDSetCoinSend(gObj[n].Index, it->WC, it->WP, it->GP, "Leo Thap");
							}
			 			}
			 		}
				 } // end
			 } else // Khong co PT thi thuong cho minh no
			 {
				 GDSetCoinSend(lpTarget->Index, it->WC, it->WP, it->GP, "Leo Thap");
			 }
		 }
	}

	for(std::vector<LEO_THAP_THUONG>::iterator it=this->m_LeoThapTang2.begin();it != this->m_LeoThapTang2.end();it++)
	{
		if(lpObj->Class == it->Monter && it->Boss == 1)
		{
			 // Thưởng ngọc
			 if(it->Chao > 0)
			 {
				 int px = lpObj->X;
				 int py = lpObj->Y;
				 for(int i=0; i<it->Chao; i++)
				 {
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(12, 15), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				 }
			 }

			 if(it->Cre > 0)
			 {
				int px = lpObj->X;
				int py = lpObj->Y;
				for(int i=0; i<it->Cre; i++)
				{
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(14, 22), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				}
			 }

			 if(it->Life > 0)
			 {
				int px = lpObj->X;
				int py = lpObj->Y;
				for(int i=0; i<it->Life; i++)
				{
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(14, 16), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				}
			 }

			 if(it->Bles > 0)
			 {
				int px = lpObj->X;
				int py = lpObj->Y;
				for(int i=0; i<it->Bles; i++)
				{
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(14, 13), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				}
			 }

			 if(it->Sould > 0)
			 {
				int px = lpObj->X;
				int py = lpObj->Y;
				for(int i=0; i<it->Sould; i++)
				{
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(14, 14), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				}
			 }
			 // Thưởng cùng party
			 if(gParty.IsParty(lpTarget->PartyNumber) == 1) // kiem tra thang giet bos co PT ko
			 {
				 for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
				 {
			 		if(gObjIsConnectedGP(n) != 0 && gObj[n].Type == OBJECT_USER)
			 		{
						if(gParty.IsParty(gObj[n].PartyNumber) == 1 && gObj[n].PartyNumber == lpTarget->PartyNumber && gObj[n].Map == gServerInfo.m_LeoThapMap)
			 			{
							// Check tọa độ + tiền đánh boos +-30
							if(gObj[n].X <= lpTarget->X + 30 && gObj[n].X >= lpTarget->X - 30 && gObj[n].Y <= lpTarget->Y + 30 && gObj[n].Y >= lpTarget->Y - 30)
							{
								GDSetCoinSend(gObj[n].Index, it->WC, it->WP, it->GP, "Leo Thap");
							}
			 			}
			 		}
				 } // end
			 } else // Khong co PT thi thuong cho minh no
			 {
				 GDSetCoinSend(lpTarget->Index, it->WC, it->WP, it->GP, "Leo Thap");
			 }
		 }
		 else if(lpObj->Class == it->Monter) // Truong hop quai nho
		 {
			 // Check thuong quai nho cho PT
			if(gParty.IsParty(lpTarget->PartyNumber) == 1) // kiem tra thang giet quai co PT ko
			 {
				 for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
				 {
			 		if(gObjIsConnectedGP(n) != 0 && gObj[n].Type == OBJECT_USER)
			 		{
						if(gParty.IsParty(gObj[n].PartyNumber) == 1 && gObj[n].PartyNumber == lpTarget->PartyNumber && gObj[n].Map == gServerInfo.m_LeoThapMap)
			 			{
							// Check tọa độ + tiền đánh boos +-30
							if(gObj[n].X <= lpTarget->X + 30 && gObj[n].X >= lpTarget->X - 30 && gObj[n].Y <= lpTarget->Y + 30 && gObj[n].Y >= lpTarget->Y - 30)
							{
								GDSetCoinSend(gObj[n].Index, it->WC, it->WP, it->GP, "Leo Thap");
							}
			 			}
			 		}
				 } // end
			 } else // Khong co PT thi thuong cho minh no
			 {
				 GDSetCoinSend(lpTarget->Index, it->WC, it->WP, it->GP, "Leo Thap");
			 }
		 }
	}

	for(std::vector<LEO_THAP_THUONG>::iterator it=this->m_LeoThapTang3.begin();it != this->m_LeoThapTang3.end();it++)
	{
		if(lpObj->Class == it->Monter && it->Boss == 1)
		{
			 // Thưởng ngọc
			 if(it->Chao > 0)
			 {
				 int px = lpObj->X;
				 int py = lpObj->Y;
				 for(int i=0; i<it->Chao; i++)
				 {
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(12, 15), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				 }
			 }

			 if(it->Cre > 0)
			 {
				int px = lpObj->X;
				int py = lpObj->Y;
				for(int i=0; i<it->Cre; i++)
				{
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(14, 22), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				}
			 }

			 if(it->Life > 0)
			 {
				int px = lpObj->X;
				int py = lpObj->Y;
				for(int i=0; i<it->Life; i++)
				{
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(14, 16), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				}
			 }

			 if(it->Bles > 0)
			 {
				int px = lpObj->X;
				int py = lpObj->Y;
				for(int i=0; i<it->Bles; i++)
				{
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(14, 13), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				}
			 }

			 if(it->Sould > 0)
			 {
				int px = lpObj->X;
				int py = lpObj->Y;
				for(int i=0; i<it->Sould; i++)
				{
					 if(this->GetRandomItemDropLocation(gServerInfo.m_LeoThapMap,&px,&py,4,4,20) == 0)
					 {
						px = lpObj->X;
						py = lpObj->Y;
					 }
					 GDCreateItemSend(lpTarget->Index, lpObj->Map, px, py, GET_ITEM(14, 14), 0, 0, 0, 0, 0, lpTarget->Index, 0, 0, 0, 0, 0, 0xFF, 0);
				}
			 }
			 // Thưởng cùng party
			 if(gParty.IsParty(lpTarget->PartyNumber) == 1) // kiem tra thang giet bos co PT ko
			 {
				 for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
				 {
			 		if(gObjIsConnectedGP(n) != 0 && gObj[n].Type == OBJECT_USER)
			 		{
						if(gParty.IsParty(gObj[n].PartyNumber) == 1 && gObj[n].PartyNumber == lpTarget->PartyNumber && gObj[n].Map == gServerInfo.m_LeoThapMap)
			 			{
							// Check tọa độ + tiền đánh boos +-30
							if(gObj[n].X <= lpTarget->X + 30 && gObj[n].X >= lpTarget->X - 30 && gObj[n].Y <= lpTarget->Y + 30 && gObj[n].Y >= lpTarget->Y - 30)
							{
								GDSetCoinSend(gObj[n].Index, it->WC, it->WP, it->GP, "Leo Thap");
							}
			 			}
			 		}
				 } // end
			 } else // Khong co PT thi thuong cho minh no
			 {
				 GDSetCoinSend(lpTarget->Index, it->WC, it->WP, it->GP, "Leo Thap");
			 }
		 }
		 else if(lpObj->Class == it->Monter) // Truong hop quai nho
		 {
			 // Check thuong quai nho cho PT
			if(gParty.IsParty(lpTarget->PartyNumber) == 1) // kiem tra thang giet quai co PT ko
			 {
				 for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
				 {
			 		if(gObjIsConnectedGP(n) != 0 && gObj[n].Type == OBJECT_USER)
			 		{
						if(gParty.IsParty(gObj[n].PartyNumber) == 1 && gObj[n].PartyNumber == lpTarget->PartyNumber && gObj[n].Map == gServerInfo.m_LeoThapMap)
			 			{
							// Check tọa độ + tiền đánh boos +-30
							if(gObj[n].X <= lpTarget->X + 30 && gObj[n].X >= lpTarget->X - 30 && gObj[n].Y <= lpTarget->Y + 30 && gObj[n].Y >= lpTarget->Y - 30)
							{
								GDSetCoinSend(gObj[n].Index, it->WC, it->WP, it->GP, "Leo Thap");
							}
			 			}
			 		}
				 } // end
			 } else // Khong co PT thi thuong cho minh no
			 {
				 GDSetCoinSend(lpTarget->Index, it->WC, it->WP, it->GP, "Leo Thap");
			 }
		 }
	}

	
}

void CLeoThap::NoticeSendToAll(LEO_THAP_LEVEL* lpLevel,int type,char* message,...) // OK
{
	char buff[256];

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	for(int n=0;n < MAX_LT_USER;n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			gNotice.GCNoticeSend(lpLevel->User[n].Index,type,0,0,0,0,0,buff);
		}
	}
}

void CLeoThap::StartLT()
{
	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);

	int day = aTime->tm_mday;
	int month = aTime->tm_mon + 1;
	int year = aTime->tm_year + 1900;

	int hour	= aTime->tm_hour;
	int minute	= aTime->tm_min+2;

	if (minute >= 60)
	{
		hour++;
		minute = minute-60; 
	}

	LEO_THAP_START_TIME info;

	info.Year = -1;

	info.Month = -1;

	info.Day = -1;

	info.DayOfWeek = -1;

	info.Hour = hour;

	info.Minute = minute;

	info.Second = 0;

	this->m_LeoThapStartTime.push_back(info);

	LogAdd(LOG_EVENT,"[Leo Tháp] Sẽ bắt đầu vào %2d:%2d:00",hour,minute);

	this->Init();
}


DWORD CLeoThap::getTime()
{
	time_t now = time(0);
    tm *ltm = localtime(&now);
	return (ltm->tm_year + 1900) * 10000 + (ltm->tm_mon + 1) * 100 + ltm->tm_mday;
}

bool CLeoThap::CheckPlayerTarget(LPOBJ lpObj)
{
	LEO_THAP_LEVEL* lpLevel = &this->m_LeoThapLevel[0];

	if(lpLevel->State == 3)
	{
		for(int n=0;n < MAX_LT_USER;n++)
		{
			if(lpLevel->User[n].Index == lpObj->Index)
			{
				return 1;
			}
		}
	}
	return 0;
}


void CLeoThap::checkUserNone(LEO_THAP_LEVEL* lpLevel)
{
	for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if(gObjIsConnectedGP(n) != 0 && gObj[n].Type == OBJECT_USER)
		{
			if (gObj[n].Map == gServerInfo.m_LeoThapMap)
			{
				if(this->GetUser(lpLevel, gObj[n].Index) == 0)
				{
					gObjMoveGate(gObj[n].Index,17);
				}
			}
		}
	}
}

void CLeoThap::SendTime(int Index, int time, int state, int eventTime)
{
	PMSG_LEOTHAP_TIME pMsg;
	pMsg.h.set(0xFB, 0x28, sizeof(pMsg));
	int Map = gServerInfo.m_LeoThapMap;
	pMsg.TimeCount = time;
	pMsg.Map = Map;
	pMsg.State = state;
	pMsg.eventTime = eventTime;
	DataSend(Index, (LPBYTE)&pMsg, pMsg.h.size);
}

void CLeoThap::SendTime2(int Index, int time, int State)
{
	PMSG_LEOTHAP_TIME2 pMsg;
	pMsg.h.set(0xFB, 0x29, sizeof(pMsg));
	int Map = gServerInfo.m_LeoThapMap;
	pMsg.TimeCount = time;
	pMsg.State = State;
	DataSend(Index, (LPBYTE)&pMsg, pMsg.h.size);
}

bool::CLeoThap::GetRandomItemDropLocation(int map,int* ox,int* oy,int tx,int ty,int count) // OK
{
	int x = (*ox);
	int y = (*oy);

	tx = ((tx<1)?1:tx);
	ty = ((ty<1)?1:ty);

	for(int n=0;n < count;n++)
	{
		(*ox) = ((GetLargeRand()%(tx+1))*((GetLargeRand()%2==0)?-1:1))+x;
		(*oy) = ((GetLargeRand()%(ty+1))*((GetLargeRand()%2==0)?-1:1))+y;

		if(gMap[map].CheckAttr((*ox),(*oy),4) == 0 && gMap[map].CheckAttr((*ox),(*oy),8) == 0)
		{
			return 1;
		}
	}

	return 0;
}