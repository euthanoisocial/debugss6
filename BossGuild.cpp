// CTCMini.cpp: Evento CTC Mini by CuongBeo
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DSProtocol.h"
#include "GuildClass.h"
#include "Map.h"
#include "MapServerManager.h"
#include "MemScript.h"
#include "Message.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "ScheduleManager.h"
#include "Notice.h"
#include "Util.h"
#include "CommandManager.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "Guild.h"
#include "BossGuild.h"
#include "CashShop.h"
#include "Monster.h"
#include "ItemBagManager.h"
#include "Gate.h"

#if(BOSS_GUILD == 1)
CBossGuild gBossGuild;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBossGuild::CBossGuild() // OK
{

	this->m_State = BOSSGUILD_STATE_BLANK;
	this->m_RemainTime = 0;
	this->m_TargetTime = 0;
	this->m_TickCount = GetTickCount();
	this->MinutesLeft = -1;

	this->m_RemainTimeWinner = 0;

	ZeroMemory(&this->GateMove, sizeof(this->GateMove));
	this->m_BossGuildStartTime.clear();
	this->Class = -1;
	this->Map = 0;
	this->X = 0;
	this->Y = 0;
	
	this->GuildMinPlayer = 0;

	this->Clear();
}


void CBossGuild::Clear()
{
	for (int n = 0; n < MAX_GUILD; n++)
	{
		this->Guild[n].Reset();
	}

	for (int n = 0; n < MAX_CHAR; n++)
	{
		this->Char[n].Reset();
	}

	this->m_RemainTimeWinner = 0;

	this->m_Active = 0;

}
CBossGuild::~CBossGuild() // OK
{

}

void CBossGuild::Init() // OK
{
	if (this->m_Enabled == 0)
	{
		this->SetState(BOSSGUILD_STATE_BLANK);
	}
	else
	{
		this->SetState(BOSSGUILD_STATE_EMPTY);
	}
}
void CBossGuild::Load(char* path) // OK
{
	this->m_BossGuildStartTime.clear();

	this->Clear();

	this->m_Enabled = 0;
	this->m_EventTime = 0;
	this->m_WarningTime = 0;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);
	if (res.status != pugi::status_ok){
		ErrorMessageBox("File %s load fail. Error: %s", path, res.description());
		return;
	}

	//--
	pugi::xml_node oBossGuild = file.child("BossGuild");
	//--
	pugi::xml_node eBossGuild = oBossGuild.child("EventTime");
	for (pugi::xml_node rBossGuild = eBossGuild.child("Time"); rBossGuild; rBossGuild = rBossGuild.next_sibling()){
		BOSSGUILD_START_TIME info;
		info.Year = rBossGuild.attribute("Year").as_int();
		info.Month = rBossGuild.attribute("Month").as_int();
		info.Day = rBossGuild.attribute("Day").as_int();
		info.DayOfWeek = rBossGuild.attribute("DayOfWeek").as_int();
		info.Hour = rBossGuild.attribute("Hour").as_int();
		info.Minute = rBossGuild.attribute("Minute").as_int();
		info.Second = rBossGuild.attribute("Second").as_int();
		this->m_BossGuildStartTime.push_back(info);
	}
	//--
	pugi::xml_node eNPCTalk = oBossGuild.child("NPCTalk");
	for (pugi::xml_node rNPCTalk = eNPCTalk.child("NPC"); rNPCTalk; rNPCTalk = rNPCTalk.next_sibling()){
		this->Class = rNPCTalk.attribute("Class").as_int();
		this->Map = rNPCTalk.attribute("Map").as_int();
		this->X = rNPCTalk.attribute("X").as_int();
		this->Y = rNPCTalk.attribute("Y").as_int();
	}
	//--
	pugi::xml_node eCauHinhTime = oBossGuild.child("CauHinhTime");
	for (pugi::xml_node rCTCConfig = eCauHinhTime.child("Config"); rCTCConfig; rCTCConfig = rCTCConfig.next_sibling())
	{
		this->m_Enabled = rCTCConfig.attribute("Enabled").as_int();
		this->m_WarningTime = rCTCConfig.attribute("TimeChuanBi").as_int();
		this->m_EventTime = rCTCConfig.attribute("TimeSuKien").as_int();
		this->GuildMinPlayer = rCTCConfig.attribute("GuildMinPlayer").as_int();
		//LogAdd(LOG_EVENT,"Debug  m_Enabled %d m_WarningTime %d m_EventTime %d WinnerTime %d", this->m_Enabled,this->m_WarningTime,this->m_EventTime,this->WinnerTime);
	}
	//====
	pugi::xml_node eCauHinhGate = oBossGuild.child("CauHinhGate");
	for (pugi::xml_node rGateConfig = eCauHinhGate.child("Config"); rGateConfig; rGateConfig = rGateConfig.next_sibling())
	{
		this->GateMove[0] = rGateConfig.attribute("Gate1").as_int();
		this->GateMove[1] = rGateConfig.attribute("Gate2").as_int();
		this->GateMove[2] = rGateConfig.attribute("Gate3").as_int();
		this->GateMove[3] = rGateConfig.attribute("Gate4").as_int();
		//LogAdd(LOG_EVENT,"Debug  Gate1 %d, Gate2 %d, Gate3 %d, Gate4 %d, Gate5 %d,",this->GateMove[0],this->GateMove[1],this->GateMove[2],this->GateMove[3],this->GateMove[4]);
	}
	pugi::xml_node eKillBoss = oBossGuild.child("KillBoss");
	for (pugi::xml_node rKillBoss = eKillBoss.child("Config"); rKillBoss; rKillBoss = rKillBoss.next_sibling()){
	   
	    b_KillBoss.WCoin = rKillBoss.attribute("WCoin").as_int();
		b_KillBoss.WCoinP = rKillBoss.attribute("WCoinP").as_int();
		b_KillBoss.GobinP = rKillBoss.attribute("GobinP").as_int();
		b_KillBoss.Ruud = rKillBoss.attribute("Ruud").as_int();
	}
	pugi::xml_node eSetBoss = oBossGuild.child("SetBoss");
	for (pugi::xml_node rSetBoss = eSetBoss.child("Config"); rSetBoss; rSetBoss = rSetBoss.next_sibling()){
	   
		b_SetBoss.Boss1 = rSetBoss.attribute("Boss1").as_int();
		//b_SetBoss.Boss2 = rSetBoss.attribute("Boss2").as_int();
		
	}
}

void CBossGuild::MainProc() // OK
{
	DWORD elapsed = GetTickCount() - this->m_TickCount;

	if (elapsed < 1000)
	{
		return;
	}

	this->m_TickCount = GetTickCount();

	this->m_RemainTime = (int)difftime(this->m_TargetTime, time(0));


	if (this->m_Enabled == 0)
	{
		if (gServerDisplayer.EventBossGuild != -1)
		{
			gServerDisplayer.EventBossGuild = -1;
		}
	}
	else
	{
		if (this->m_State == BOSSGUILD_STATE_EMPTY)
		{
			gServerDisplayer.EventBossGuild = this->m_RemainTime;
		}
		else
		{
			if (gServerDisplayer.EventBossGuild != 0)
			{
				gServerDisplayer.EventBossGuild = 0;
			}
		}
	}

	//LogAdd(LOG_EVENT,"Debug  MainProc %d (State %d)",this->m_RemainTime,this->m_State);

	switch (this->m_State)
	{
	case BOSSGUILD_STATE_BLANK:
		this->ProcState_BLANK();
		break;
	case BOSSGUILD_STATE_EMPTY:
		this->ProcState_EMPTY();
		break;
	case BOSSGUILD_STATE_START:
		this->ProcState_START();
		break;
	}
}

void CBossGuild::ProcState_BLANK() // OK
{

}

void CBossGuild::ProcState_EMPTY() // OK
{

	if (this->m_RemainTime > 0 && this->m_RemainTime <= (this->m_WarningTime * 60))
	{
		int minutes = this->m_RemainTime / 60;

		if ((this->m_RemainTime % 60) == 0)
		{
			minutes--;
		}

		if (this->MinutesLeft != minutes)
		{
			this->MinutesLeft = minutes;

			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "[BossGuild] Bắt đầu sau %d phút !", (MinutesLeft + 1));
		}
		if (this->m_Active == 0) { this->m_Active = 1; }
	}

	if (this->m_RemainTime > 0 && this->m_RemainTime <= 5)
	{
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "[BossGuild] Bắt đầu sau %d giây !", m_RemainTime);
	}

	if (this->m_RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "[BossGuild] Sự kiện bắt đầu !"); //Bat Dau Su Kien

		this->SetState(BOSSGUILD_STATE_START);
	}
}

void CBossGuild::ProcState_START() // OK
{
	if (this->m_RemainTime > 0 && this->m_RemainTime <= 300)
	{
		int minutes = this->m_RemainTime / 60;

		if (this->MinutesLeft != minutes)
		{
			this->MinutesLeft = minutes;

			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "[BossGuild] Sẽ kết thúc sau %d phút !", (MinutesLeft + 1));
		}
	}

	if (this->m_RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "[BossGuild] Đã kết thúc !");

		this->m_RemainTimeWinner = 40; //Thoi Gian dem nguoc trao thuong

		this->m_Active = 0;

		this->SetState(BOSSGUILD_STATE_EMPTY);



		//for (int aIndex = OBJECT_START_USER; aIndex < MAX_OBJECT; aIndex++)
		//{
		//	LPOBJ lpUser = &gObj[aIndex];

		//	if (gObjIsConnectedGP(aIndex) == 0)
		//	{
		//		lpUser->PKLevel = 0;
		//	}

		for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
		{
			if (gObjIsConnectedGP(n) == 0)
			{
				continue;
			}

			LPOBJ lpObj = &gObj[n];

			lpObj->PKLevel = 0;
		}

		// ok ne
		for (int n = 0; n < MAX_CHAR; n++)
		{
			if (this->Char[n].Index == -1)
			{
				continue;
			}
			gObjMoveGate(this->Char[n].Index, 17); //Move tat ca cac thanh nien ve 
		}
		this->Clear();
	}
}

void CBossGuild::SetState(int state) // OK
{
	this->m_State = state;

	switch (this->m_State)
	{
	case BOSSGUILD_STATE_BLANK:
		this->SetState_BLANK();
		break;
	case BOSSGUILD_STATE_EMPTY:
		this->SetState_EMPTY();
		break;
	case BOSSGUILD_STATE_START:
		this->SetState_START();
		break;
	}
}

void CBossGuild::SetState_BLANK() // OK
{

}

void CBossGuild::SetState_EMPTY() // OK
{
	this->CheckSync();
}

void CBossGuild::SetState_START() // OK
{
	this->m_Active = 2;

	this->m_RemainTime = this->m_EventTime * 60;

	this->m_TargetTime = (int)(time(0) + this->m_RemainTime);

	this->StartMonsterBossGuild(); // add boss

	//this->StartMonsterBossGuild1(); // add boss

	this->SendTimeBossGuild(0, 0);

	LogAdd(LOG_EVENT, "[BossGuild] Bat Dau Event Boss Guild");

}

void CBossGuild::CheckSync() // OK
{

	if (this->m_BossGuildStartTime.empty() != 0)
	{
		this->SetState(BOSSGUILD_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for (std::vector<BOSSGUILD_START_TIME>::iterator it = this->m_BossGuildStartTime.begin(); it != this->m_BossGuildStartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year, it->Month, it->Day, it->Hour, it->Minute, it->Second, it->DayOfWeek);
	}

	if (ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(BOSSGUILD_STATE_BLANK);
		return;
	}

	this->m_RemainTime = (int)difftime(ScheduleTime.GetTime(), time(0));

	this->m_TargetTime = (int)ScheduleTime.GetTime();

}
void CBossGuild::CGPacketBossGuild(BOSSGUILD_CGPACKET* aRecv, int aIndex)
{
	if (!this->m_Enabled)
	{
		return;
	}
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}
	LPOBJ lpObj = &gObj[aIndex];

	if ((GetTickCount() - lpObj->ClickClientSend) < 2000) {
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Bấm Nhanh VKL Đợi Chút Đi !");
		return;
	}

	GUILD_INFO_STRUCT * lpGuildInfo = lpObj->Guild;

	if (this->m_Active == 0 || this->m_Active == 2) //Verifica se o evento está em andamento
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Chưa Đến Thời Gian Diễn Ra Sự Kiện");
		return;
	}

	if (lpGuildInfo == NULL)
	{

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "[BossGuild] Không thấy thông tin Guild của bạn");
		return;
	}

	if (lpObj->GuildNumber == 0) //Verifica se o personagem faz parte de uma guild
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "[BossGuild] Bạn chưa vào Guild nên không thể tham gia !");
		return;
	}

	if (lpGuildInfo->TotalCount < this->GuildMinPlayer) //So Thanh Vien Toi Thieu Tham Gia
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "[BossGuild] Guild bạn không đủ người than gia !");
		return;
	}


	lpObj->PKLevel = 6;

	int CongVao = aRecv->CongVao;
	this->SetChar(lpObj->Index, CongVao);
	gObjMoveGate(lpObj->Index, this->GateMove[CongVao]);
	this->SendTimeBossGuild(lpObj->Index, 1);

}

void CBossGuild::SendTimeBossGuild(int aIndex, int Type) //Gui thong tin ve CLient
{
	BOSSGUILDTIME_SENDCLIENT pMsg;
	pMsg.header.set(0xF3, 0x36, sizeof(pMsg));
	if (Type == 1) 
	{
		//=== Send 1 Client===//
		if (gObjIsConnectedGP(aIndex) == 0)
		{
			return;
		}
		LPOBJ lpObj = &gObj[aIndex];
		pMsg.TimeBossGuild = this->m_RemainTime;
			//==============
					memset(&pMsg.GNameBoss1,0,sizeof(pMsg.GNameBoss1));
					memset(&pMsg.GNameBoss2,0,sizeof(pMsg.GNameBoss2));
					memset(&pMsg.GNameBoss3,0,sizeof(pMsg.GNameBoss3));
					//==============
					for(int b=0;b < 3;b++) //Them Cong
					{
						if(IndexBoss[b] != -1 && gObj[IndexBoss[b]].GuildNumber == lpObj->GuildNumber) {
							pMsg.Boss[b] = 1;
						} 
						else if(IndexBoss[b] != -1 &&  gObj[IndexBoss[b]].GuildNumber != 0 && gObj[IndexBoss[b]].GuildNumber != lpObj->GuildNumber) {
							pMsg.Boss[b] = 2;
						}
						else  {
							pMsg.Boss[b] = 0;
						}
					
					}
					if(strlen(gObj[IndexBoss[0]].GuildName) > 1){
						memcpy(pMsg.GNameBoss1, gObj[IndexBoss[0]].GuildName, 9); //MAX_GUILD_NAME define
					}
					if(strlen(gObj[IndexBoss[1]].GuildName) > 1){
						memcpy(pMsg.GNameBoss2, gObj[IndexBoss[1]].GuildName, 9); //MAX_GUILD_NAME define
					}
					if(strlen(gObj[IndexBoss[2]].GuildName) > 1){
						memcpy(pMsg.GNameBoss3, gObj[IndexBoss[2]].GuildName, 9); //MAX_GUILD_NAME define
					}
		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);

	}
	else {
		//=== Send All ==//
		for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
		{
			if (gObjIsConnectedGP(n) != 0 && gObj[n].Map == MapBossGuild)
			{
				pMsg.TimeBossGuild = this->m_RemainTime;
				memset(&pMsg.GNameBoss1,0,sizeof(pMsg.GNameBoss1));
					memset(&pMsg.GNameBoss2,0,sizeof(pMsg.GNameBoss2));
					memset(&pMsg.GNameBoss3,0,sizeof(pMsg.GNameBoss3));
					for(int b=0;b < 3;b++) //Them Cong
					{
						//========================
						if(IndexBoss[b] != -1 &&  gObj[IndexBoss[b]].GuildNumber != 0 && gObj[IndexBoss[b]].GuildNumber == gObj[n].GuildNumber) {
							pMsg.Boss[b] = 1;
						} 
						else if(IndexBoss[b] != -1 &&  gObj[IndexBoss[b]].GuildNumber != 0 && gObj[IndexBoss[b]].GuildNumber != gObj[n].GuildNumber) {
							pMsg.Boss[b] = 2;
						}
						else  {
							pMsg.Boss[b] = 0;
						}
					
					}
					if(strlen(gObj[IndexBoss[0]].GuildName) > 1){
						memcpy(pMsg.GNameBoss1, gObj[IndexBoss[0]].GuildName, 9); //MAX_GUILD_NAME define
					}
					if(strlen(gObj[IndexBoss[1]].GuildName) > 1){
						memcpy(pMsg.GNameBoss2, gObj[IndexBoss[1]].GuildName, 9); //MAX_GUILD_NAME define
					}
					if(strlen(gObj[IndexBoss[2]].GuildName) > 1){
						memcpy(pMsg.GNameBoss3, gObj[IndexBoss[2]].GuildName, 9); //MAX_GUILD_NAME define
					}
				DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
			}
		}
	}
}


void SendInfoBossGuild(int aIndex) // ok
{
	// Gửi Lên Client
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	INFOBOSSGUILD_SENDCLIENT pMsg;

	pMsg.header.set(0xF3, 0x35, sizeof(pMsg)); // sửa lại

	pMsg.OpenWindow = 1;

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
}

bool CBossGuild::Dialog(int aIndex, int aNpcIndex) // ok
{
	// Click NPC
	if (!this->m_Enabled)
	{
		return false;
	}
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return false;
	}
	LPOBJ lpUser = &gObj[aIndex];
	LPOBJ lpNpc = &gObj[aNpcIndex];

	if (lpNpc->Class == this->Class
		&&	lpNpc->Map == this->Map
		&&	lpNpc->X == this->X
		&&	lpNpc->Y == this->Y)
	{
		//=== Gui Info ve CLient
		SendInfoBossGuild(aIndex);
		return true;
	}
	// ----
	return false;
}



void CBossGuild::AddGuild(int gIndex) // Set Guild tham gia Su Kien
{

	LPOBJ lpObj = &gObj[gIndex];
	for (int n = 0; n < MAX_GUILD; n++)
	{
		if (this->Guild[n].Number >= 0)
		{
			continue;
		}

		this->Guild[n].Number = lpObj->GuildNumber;
		this->Guild[n].Owner = 0;
		memcpy(this->Guild[n].GuildOwner, lpObj->GuildName, sizeof(this->Guild[n].GuildOwner));
		return;
	}
}

bool CBossGuild::GetGuild(int gIndex) // OK
{
	for (int n = 0; n < MAX_GUILD; n++)
	{
		if (this->Guild[n].Number == gIndex)
		{
			return 1;
		}
	}
	return 0;
}


void CBossGuild::SetChar(int cIndex, int CongVao) // OK
{

	if (this->GetChar(cIndex) == 0)
	{
		this->AddChar(cIndex, CongVao);
	}

}

void CBossGuild::AddChar(int cIndex, int CongVao) // OK
{
	for (int n = 0; n < MAX_CHAR; n++)
	{
		if (this->Char[n].Index >= 0)
		{
			continue;
		}

		this->Char[n].Index = cIndex;
		memcpy(this->Char[n].Name, gObj[cIndex].Name, sizeof(gObj[cIndex].Name));
		this->Char[n].CongVao = CongVao;
		LogAdd(LOG_RED, "Add Nhan Vat %s Cong %d", this->Char[n].Name, CongVao);
		return;
	}
}

int CBossGuild::GetCongVao(int cIndex) // OK
{
	for (int n = 0; n < MAX_CHAR; n++)
	{
		if (this->Char[n].Index == cIndex)
		{
			return this->Char[n].CongVao;
		}
	}
	return 0;
}

bool CBossGuild::GetChar(int cIndex) // OK
{
	for (int n = 0; n < MAX_CHAR; n++)
	{
		if (this->Char[n].Index == cIndex)
		{
			return 1;
		}
	}
	return 0;
}

void CBossGuild::CheckChar() // OK
{
	for (int n = 0; n < MAX_CHAR; n++)
	{
		if (OBJECT_RANGE(this->Char[n].Index) == 0)
		{
			continue;
		}
		if (strcmp(this->Char[n].Name, gObj[this->Char[n].Index].Name) == 0)
		{
			this->Char[n].Reset();
			return;
		}
	}
	return;
}


//=================================================================================================================

bool CBossGuild::CheckPlayerTarget(LPOBJ lpObj, LPOBJ lpTarget, int Type) // OK
{
	if (Type == 1) //Attack
	{
		if (lpObj->Map == MapBossGuild)
		{
		///	LogAdd(LOG_RED, "DEBUG 1 Attack %d", lpObj->GuildNumber);
			if (lpObj->GuildNumber == 0 && lpObj->Type == OBJECT_USER)
			{
				gObjMoveGate(lpObj->Index, 17); //Move tat ca cac thanh nien ve 
				return 1;
			}
			if (lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_MONSTER)
			{
				if (lpObj->GuildNumber == lpTarget->GuildNumber &&  lpObj->GuildNumber)
				{
					return 1;
				}
			}

		}
	}
	else //View PK
	{
		if (lpObj->Map == MapBossGuild)
		{
			if (lpTarget->GuildNumber != lpObj->GuildNumber)
			{
				return 1;
			}
		}

	}
	return 0;
}
bool CBossGuild::GetUserRespawnLocation(LPOBJ lpObj, int* gate, int* map, int* x, int* y, int* dir, int* level) // OK
{

	if (this->m_Enabled == 0)
	{
		return 0;
	}

	if (this->GetChar(lpObj->Index) == 0)
	{
		return 0;
	}

	int CongVao = this->GetCongVao(lpObj->Index);

	{
		if (gGate.GetGate(this->GateMove[CongVao], gate, map, x, y, dir, level) != 0)
		{
			return 1;
		}
	}

	return 0;
}


void CBossGuild::StartMonsterBossGuild() // OK
{
	LogAdd(LOG_RED, "[BossGuild] Add Monster 1");

	int qtd = 1;
	
	qtd = (qtd > 0) ? qtd : 1;

	for (int n = 0; n < qtd; n++)
	{

		int index = gObjAddMonster(MapBossGuild);

		if (OBJECT_RANGE(index) == 0)
		{
			return;
		}

		LPOBJ lpMonster = &gObj[index];

		int px = 68;
		int py = 160;

		lpMonster->PosNum = -1;
		lpMonster->X = px;
		lpMonster->Y = py;
		lpMonster->TX = px;
		lpMonster->TY = py;
		lpMonster->OldX = px;
		lpMonster->OldY = py;
		lpMonster->StartX = px;
		lpMonster->StartY = py;
		lpMonster->Dir = 1;
		lpMonster->Map = MapBossGuild;
		lpMonster->MonsterDeleteTime = GetTickCount() + 1800000;

		if (gObjSetMonster(index, b_SetBoss.Boss1)== 0)
		{
			gObjDel(index);
			return;
		}
	}
}

//void CBossGuild::StartMonsterBossGuild1() // OK
//{
//	LogAdd(LOG_RED, "[BossGuild] Add Monster 2");
//
//	int qtd = 1;
//
//	qtd = (qtd > 0) ? qtd : 1;
//
//	for (int n = 0; n < qtd; n++)
//	{
//
//		int index = gObjAddMonster(MapBossGuild);
//
//		if (OBJECT_RANGE(index) == 0)
//		{
//			return;
//		}
//
//		LPOBJ lpMonster = &gObj[index];
//
//		int px = 65;
//		int py = 146;
//
//		lpMonster->PosNum = -1;
//		lpMonster->X = px;
//		lpMonster->Y = py;
//		lpMonster->TX = px;
//		lpMonster->TY = py;
//		lpMonster->OldX = px;
//		lpMonster->OldY = py;
//		lpMonster->StartX = px;
//		lpMonster->StartY = py;
//		lpMonster->Dir = 1;
//		lpMonster->Map = MapBossGuild;
//		lpMonster->MonsterDeleteTime = GetTickCount() + 1800000;
//
//		if (gObjSetMonster(index, b_SetBoss.Boss2) == 0)
//		{
//			gObjDel(index);
//			return;
//		}
//	}
//}

void CBossGuild::MonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget)
{
	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	if(lpObj->Class == b_SetBoss.Boss1  && lpObj->Map == 105)
	{
		//gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "Nhan vat %s tieu diet boss guild", lpTarget->Name);
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[%s] Tiêu Diệt Boss Guild - Guild[%s] Được + 100 Điểm Boss Guild ",lpTarget->Name, lpTarget->GuildName);
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"Các Thành Viên Trong Guild [%s] Tham Gia Event Nhận Thưởng 30k Wcoin", lpTarget->GuildName);
		this->BossReward(&gObj[aIndex]);
	}

}


void CBossGuild::BossReward(LPOBJ lpObj)
{


	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0 && gObj[n].Type == OBJECT_USER)
		{
			if (strcmp(gObj[n].GuildName,lpObj->Guild->Name) == 0)
			{
				gCashShop.GDCashShopAddPointSaveSend(gObj[n].Index,0, b_KillBoss.WCoin, b_KillBoss.WCoinP, b_KillBoss.GobinP , b_KillBoss.Ruud);
				
				gCashShop.CGCashShopPointRecv(gObj[n].Index);

			}
		}
	}
}


void CBossGuild::StartBossGuild() // ok
{
	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);

	int hour = aTime->tm_hour;
	int minute = aTime->tm_min + 2;

	if (minute >= 60)
	{
		hour++;
		minute = minute - 60;
	}

	BOSSGUILD_START_TIME info;

	info.Year = -1;

	info.Month = -1;

	info.Day = -1;

	info.DayOfWeek = -1;

	info.Hour = hour;

	info.Minute = minute;

	info.Second = 0;

	this->m_BossGuildStartTime.push_back(info);

	LogAdd(LOG_EVENT, "[Boss Guild] Bắt Đầu Sau %2d:%2d:00", hour, minute);

	this->Init();
}


#endif