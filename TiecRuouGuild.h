// CustomTiecRuou.h: interface for the CCustomTiecRuou class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "User.h"
#include "Protocol.h"
#if(EVENT_TIEC_RUOU_GUILD)

#define MAX_CUSTOM_TIECRUOU 10

enum eCustomTiecRuouState
{
	CUSTOM_TIECRUOU_STATE_BLANK = 0,
	CUSTOM_TIECRUOU_STATE_EMPTY = 1,
	CUSTOM_TIECRUOU_STATE_START = 2,
};

struct CUSTOM_TIECRUOU_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

struct CUSTOM_TIECRUOU_RULE_INFO
{
	chr Name[32];
	int AlarmTime;
	int EventTime;
	int CongHien;
	DWORD TimeDelay;
};

struct CUSTOM_TIECRUOU_INFO
{
	int Index;
	int State;
	int RemainTime;
	int TargetTime;
	int TickCount;
	int AlarmMinSave;
	int AlarmMinLeft;
	CUSTOM_TIECRUOU_RULE_INFO RuleInfo;
	std::vector<CUSTOM_TIECRUOU_START_TIME> StartTime;
};

struct PMSG_TIECRUOU_REQ
{
	PSBMSG_HEAD	Head;
	// ----
};

struct PMSG_TIECRUOU_RECV
{
	PSBMSG_HEAD header;
	int Number;
};

struct PMSG_TIECRUOU_ZEN_RECV
{
	PSBMSG_HEAD header;
	int Number;
};

struct PMSG_TIECRUOU_WC_RECV
{
	PSBMSG_HEAD header;
	int Number;
};

class CCustomTiecRuou
{
public:
	CCustomTiecRuou();
	virtual ~CCustomTiecRuou();
	void Init();
	void ReadCustomTiecRuouInfo(char* section,char* path);
	void Load(char* path);
	void MainProc();
	void ProcState_BLANK(CUSTOM_TIECRUOU_INFO* lpInfo);
	void ProcState_EMPTY(CUSTOM_TIECRUOU_INFO* lpInfo);
	void ProcState_START(CUSTOM_TIECRUOU_INFO* lpInfo);
	void SetState(CUSTOM_TIECRUOU_INFO* lpInfo,int state);
	void SetState_BLANK(CUSTOM_TIECRUOU_INFO* lpInfo);
	void SetState_EMPTY(CUSTOM_TIECRUOU_INFO* lpInfo);
	void SetState_START(CUSTOM_TIECRUOU_INFO* lpInfo);
	void CheckSync(CUSTOM_TIECRUOU_INFO* lpInfo);
	LONG GetDummyUserIndex();
	bool Dialog(LPOBJ lpObj, LPOBJ lpNpc);
	bool CheckEnterEnabled(LPOBJ lpObj);
	void GCSendData(int aIndex);
	void RecvTiecRuouPanel(PMSG_TIECRUOU_RECV* Data, int aIndex);
	void RecvTiecRuouZen(PMSG_TIECRUOU_ZEN_RECV* Data, int aIndex);
	void RecvTiecRuouWC(PMSG_TIECRUOU_WC_RECV* Data, int aIndex);

	void MuaRuouZen(LPOBJ lpObj);
	void MuaRuouWC(LPOBJ lpObj);
	void StartTiecRuou();
public:
	int m_CustomTiecRuouSwitch;
	char m_CustomTiecRuouText1[128];
	char m_CustomTiecRuouText2[128];
	char m_CustomTiecRuouText3[128];
	int m_CustomTiecRuouNPCY;
	int m_CustomTiecRuouNPC;
	int m_CustomTiecRuouNPCMap;
	int m_CustomTiecRuouNPCX;
	int WaitingGate;
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
	//--Phan Moi Ruou
	int CongHienMoi;
	int ZenMoi;
	int WcMoi;
	CUSTOM_TIECRUOU_INFO m_CustomTiecRuouInfo[MAX_CUSTOM_TIECRUOU];
};

extern CCustomTiecRuou gTiecRuouEvent;
#endif