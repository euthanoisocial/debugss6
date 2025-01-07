// LeoThap.h: interface for the CLeoThap class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "Protocol.h"
#include "User.h"

#define MAX_LT_LEVEL 7
#define MAX_LT_USER 50
#define MAX_LT_MONSTER 200
#define MAX_LT_RANK 10

#define LT_LEVEL_RANGE(x) (((x)<0)?0:((x)>=MAX_LT_LEVEL)?0:1)

enum eLeoThapState
{
	LT_STATE_BLANK = 0,
	LT_STATE_EMPTY = 1,
	LT_STATE_STAND = 2,
	LT_STATE_START = 3,
	LT_STATE_CLEAN = 4,
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_LEO_THAP_ENTER_RECV
{
	PBMSG_HEAD header; // C1:90
	BYTE level;
	BYTE slot;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_LEOTHAP_TIME
{
	PSBMSG_HEAD h;
	int TimeCount;
	int Map;
	int State;
	int eventTime;
};

struct PMSG_LEOTHAP_TIME2
{
	PSBMSG_HEAD h;
	int TimeCount;
	int Map;
	int State;
};

struct PMSG_LEO_THAP_ENTER_SEND
{
	PBMSG_HEAD header; // C1:90
	BYTE result;
};

struct PMSG_LEO_THAP_SCORE_SEND
{
	PBMSG_HEAD header; // C1:93
	BYTE rank;
	BYTE count;
};

struct PMSG_LEO_THAP_SCORE
{
	char name[10];
	DWORD score;
	DWORD RewardExperience;
	DWORD RewardMoney;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct LEO_THAP_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

struct LEO_THAP_USER
{
	int Index;
	int Gate;
};

struct LEO_THAP_LEVEL
{
	int Level;
	int State;
	int Map;
	int RemainTime;
	int TargetTime;
	int TickCount;
	int EnterEnabled;
	int MinutesLeft;
	int TimeCount;
	int Stage;
	int moveG1;
	int monterG1;
	int moveG2;
	int moveG3;
	int monterG2;
	int monterG3;
	LEO_THAP_USER User[MAX_LT_USER];
	int MonsterIndex[MAX_LT_MONSTER];
};

struct PMSG_LEOTHAP_REQ
{
	PSBMSG_HEAD	Head;
	// ----
	bool isOpenWindow;
};

struct PMSG_LEOTHAP_RECV
{
	PSBMSG_HEAD header;
	int Number;
};

struct LEO_THAP_THUONG
{
	int Monter;
	int WC;
	int WP;
	int GP;
	int Chao;
	int Cre;
	int Life;
	int Bles;
	int Sould;
	int Boss;
	int Time;
};

struct LEO_THAP_NPC
{
	int Class;
	int Map;
	int X;
	int Y;
};

struct LEO_THAP_LEPHI
{
	int WC;
	int WP;
	int GP;
};

class CLeoThap
{
public:
	CLeoThap();
	virtual ~CLeoThap();
	void Init();
	void Load(char* path);
	void MainProc();
	void ProcState_BLANK(LEO_THAP_LEVEL* lpLevel);
	void ProcState_EMPTY(LEO_THAP_LEVEL* lpLevel);
	void ProcState_STAND(LEO_THAP_LEVEL* lpLevel);
	void ProcState_START(LEO_THAP_LEVEL* lpLevel);
	void ProcState_CLEAN(LEO_THAP_LEVEL* lpLevel);
	void SetState(LEO_THAP_LEVEL* lpLevel,int state);
	void SetState_BLANK(LEO_THAP_LEVEL* lpLevel);
	void SetState_EMPTY(LEO_THAP_LEVEL* lpLevel);
	void SetState_STAND(LEO_THAP_LEVEL* lpLevel);
	void SetState_START(LEO_THAP_LEVEL* lpLevel);
	void SetState_CLEAN(LEO_THAP_LEVEL* lpLevel);
	void SetStage0(LEO_THAP_LEVEL* lpLevel);
	void SetStage1(LEO_THAP_LEVEL* lpLevel);
	void SetStage2(LEO_THAP_LEVEL* lpLevel);
	void CheckSync(LEO_THAP_LEVEL* lpLevel);
	int GetState(int level);
	int GetRemainTime(int level);
	int GetEnterEnabled(int level);
	int GetEnteredUserCount(int level);
	bool AddUser(LEO_THAP_LEVEL* lpLevel,int aIndex, int gate);
	bool DelUser(LEO_THAP_LEVEL* lpLevel,int aIndex);
	LEO_THAP_USER* GetUser(LEO_THAP_LEVEL* lpLevel,int aIndex);
	void CleanUser(LEO_THAP_LEVEL* lpLevel);
	void ClearUser(LEO_THAP_LEVEL* lpLevel);
	void CheckUser(LEO_THAP_LEVEL* lpLevel);
	int GetUserCount(LEO_THAP_LEVEL* lpLevel);
	int GetUserAbleLevel(LPOBJ lpObj);
	bool AddMonster(LEO_THAP_LEVEL* lpLevel,int aIndex);
	bool DelMonster(LEO_THAP_LEVEL* lpLevel,int aIndex);
	int* GetMonster(LEO_THAP_LEVEL* lpLevel,int aIndex);
	void CleanMonster(LEO_THAP_LEVEL* lpLevel);
	void ClearMonster(LEO_THAP_LEVEL* lpLevel);
	int GetMonsterCount(LEO_THAP_LEVEL* lpLevel);
	void SetMonster(LEO_THAP_LEVEL* lpLevel,int MonsterClass, int Time);
	void RecvLeoThap(PMSG_LEOTHAP_RECV* Data, int aIndex);
	bool Dialog(LPOBJ lpObj, LPOBJ lpNpc);
	void UserDieProc(LPOBJ lpObj,LPOBJ lpTarget);
	void MonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget);
	void NoticeSendToAll(LEO_THAP_LEVEL* lpLevel,int type,char* message,...);
	bool CheckPlayerTarget(LPOBJ lpObj);
	void checkUserNone(LEO_THAP_LEVEL* lpLevel);
	void SendTime(int Index, int time, int state, int eventTime);
	void SendTime2(int Index, int time, int State);
	void StartLT();
	DWORD getTime();
	bool GetRandomItemDropLocation(int map,int* ox,int* oy,int tx,int ty,int count); // OK
private:
	LEO_THAP_LEVEL m_LeoThapLevel[MAX_LT_LEVEL];
	int m_WarningTime;
	int m_NotifyTime;
	int m_EventTime;
	int m_CloseTime;
	int m_luot;
	std::vector<LEO_THAP_START_TIME> m_LeoThapStartTime;
	std::vector<LEO_THAP_THUONG> m_LeoThapTang1;
	std::vector<LEO_THAP_THUONG> m_LeoThapTang2;
	std::vector<LEO_THAP_THUONG> m_LeoThapTang3;
	LEO_THAP_NPC NPC;
	LEO_THAP_LEPHI LP;
};

extern CLeoThap gLeoThap;