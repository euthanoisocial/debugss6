//////////////////////////////////////////////////////////////////////

#pragma once

#if(BOSS_GUILD == 1)

#include "User.h"
#include "Protocol.h"

#define MAX_GUILD 20
#define MAX_CHAR 100


struct BOSSGUILD_CGPACKET
{
	PSBMSG_HEAD header; // C3:F3:03
	BYTE CongVao;

};

struct INFOBOSSGUILD_SENDCLIENT
{
	PSBMSG_HEAD header; // C3:F3:03
	BYTE OpenWindow;
};

struct BOSSGUILDTIME_SENDCLIENT
{
	PSBMSG_HEAD header; // C3:F3:03
	int	TimeBossGuild;
	BYTE Boss[3];
	char GNameBoss1[9];
	char GNameBoss2[9];
	char GNameBoss3[9];

};

enum eBOSGUILDState
{
	BOSSGUILD_STATE_BLANK = 0,
	BOSSGUILD_STATE_EMPTY = 1,
	BOSSGUILD_STATE_START = 2,
};

struct BOSSGUILD_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};


//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

//struct SDHP_RANKING_CTC_GUILD_SAVE_SEND
//{
//	PBMSG_HEAD header; // C1:74
//	char name[9];
//	DWORD score;
//};
//
//struct SDHP_RANKING_CTC_PLAYER_SAVE_SEND
//{
//	PBMSG_HEAD header; // C1:75
//	WORD index;
//	char account[11];
//	char name[11];
//	DWORD score;
//};

struct BONUS_POINT_KillBoss
{
	int WCoin;
	int WCoinP;
	int GobinP;
	int Ruud;
	int IndexBonus;
};

struct SET_BOSS
{
	int Boss1;
	//int Boss2;
};

struct BOSSGUILD_GUILD
{
	void Reset() // OK
	{
		this->Number = -1;
		this->Owner = 0; //Chiem tru
		ZeroMemory(GuildOwner, sizeof(GuildOwner));
	}

	int Number;
	int Owner;
	char GuildOwner[9];
};

struct BOSSGUILD_CHAR
{
	void Reset() // OK
	{
		memset(this->Name, 0, sizeof(this->Name));
		this->Index = -1;
		this->CongVao = 0;
	}

	int Index;
	char Name[11];
	int CongVao;
};
//---
class CBossGuild
{
public:
	CBossGuild();
	virtual ~CBossGuild();
	//---

	void Init();
	void Load(char* path);
	void MainProc();
	void ProcState_BLANK();
	void ProcState_EMPTY();
	void ProcState_START();
	void SetState(int state);
	void SetState_BLANK();
	void SetState_EMPTY();
	void SetState_START();
	void CheckSync();
	void Clear();
	bool Dialog(int aIndex, int aNpcIndex);
	bool CheckPlayerTarget(LPOBJ lpObj, LPOBJ lpTarget, int Type);
	void BossReward(LPOBJ lpObj);
	void MonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget);
	void AddGuild(int gIndex);
	bool GetGuild(int gIndex);
	void SetChar(int cIndex, int CongVao);
	void AddChar(int cIndex, int CongVao);
	int GetCongVao(int cIndex);
	bool GetChar(int cIndex);
	void CheckChar();
	bool MonsterDie(int aIndexMonster, int aIndexUser);
	int m_Active;

	int m_RemainTimeWinner;

	void StartBossGuild(); // ok

	void StartMonsterBossGuild(); // ok

	//void StartMonsterBossGuild1(); // ok

	int m_Enabled;

	int GateMove[4];

	bool GetUserRespawnLocation(LPOBJ lpObj, int* gate, int* map, int* x, int* y, int* dir, int* level);
	void CGPacketBossGuild(BOSSGUILD_CGPACKET* aRecv, int aIndex);

	void SendTimeBossGuild(int aIndex, int Type);
	BONUS_POINT_KillBoss b_KillBoss;
	SET_BOSS b_SetBoss;
private:
	int m_State;
	int m_RemainTime;
	int m_TargetTime;
	int m_TickCount;
	int m_WarningTime;
	int m_EventTime;
	int MinutesLeft;
	//---
	std::vector<BOSSGUILD_START_TIME> m_BossGuildStartTime;

	BOSSGUILD_GUILD Guild[MAX_GUILD];
	BOSSGUILD_CHAR Char[MAX_CHAR];

	int Class;
	int Map;
	int X;
	int Y;
	int GuildMinPlayer;
	int IndexBoss[3];
};

extern CBossGuild gBossGuild;

static const BYTE MapBossGuild = 105;
static const int ClassBoss = 727;

#endif

//static const int ClassCong = 525;
//static const int ClassTru = 908;
////Tru 1 : 96, 165 / Tru 2: 128,165 / Tru 3: 159,165
//static const BYTE XoaTru[3][4] = { { 96, 165, 96, 165 }, { 128, 165, 128, 165 }, { 159, 165, 159, 165 } };  //att = 6
//static const BYTE ThemTru[3][2] = { { 96, 165 }, { 128, 165 }, { 159, 165 } };
//
//static const BYTE XoaCong[3][4] = { { 94, 131, 98, 133 }, { 126, 131, 130, 133 }, { 159, 131, 163, 133 } };
//static const BYTE ThemCong[3][2] = { { 96, 131 }, { 128, 131 }, { 161, 131 } };
//
//
//static const BYTE XoaDuongVao[5][4] = { { 79, 31, 85, 33 }, { 108, 31, 114, 33 }, { 138, 31, 144, 332 }, { 171, 31, 176, 33 }, { 122, 231, 136, 233 } }; //att = 16
//
