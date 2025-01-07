#include "user.h"
#include "stdafx.h"

#define MAX_BOTALCHEMIST	30

struct BotAlchemistBodyItems
{
	int num;
	int level;
	int opt;
	bool Enabled;
};

struct botAlchemistStruct
{
	int index;
	BYTE Class;
	BOOL OnlyVip;
	int VipMoney;
	int Zen;
	int PCPoint;
	int GensFamily;
	BYTE Rate;
	BOOL OnlySameType;
	BOOL OnlyLowerIndex;
	BOOL AcceptAncient;
	int ChangeColorName;
	BYTE MaxLevel;
	BYTE MaxExc;
	char Name[11];
	BYTE Map;
	BYTE X;
	BYTE Y;
	BYTE Dir;
	bool Enabled;
	BotAlchemistBodyItems body[9];
	//-
	BOOL AllowLevel;
	BOOL AllowOpt;
	BOOL AllowLuck;
	BOOL AllowSkill;
	BOOL AllowExc;
	BOOL AllowFFFF;
};

class ObjBotAlchemist
{
public:
	void Read(char * FilePath);
	void MakeBot();
	int GetBotIndex(int aIndex);
	BOOL IsInTrade(int aIndex);
	BOOL TradeOpen(int index, int nindex);
	void TradeOk(int aIndex);
	void TradeCancel(int aIndex);
	BYTE gObjInventoryInsertItemPos(int aIndex, CItem item, int pos, BOOL RequestCheck);
private:
	bool Enabled;
	int	getNumberOfExcOptions(int checksum);
	bool AllowExc(BYTE BotNum, BYTE ExcOpt);
	BYTE Alchemy(int aIndex, int BotNum);
	botAlchemistStruct bot[MAX_BOTALCHEMIST];
};
extern ObjBotAlchemist BotAlchemist;
