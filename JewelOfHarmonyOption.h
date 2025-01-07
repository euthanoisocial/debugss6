// JewelOfHarmonyOption.h: interface for the CJewelOfHarmonyOption class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Item.h"
#include "User.h"

#define MAX_JEWEL_OF_HARMONY_OPTION_TYPE 4
#define MAX_JEWEL_OF_HARMONY_OPTION 16
#define MAX_JEWEL_OF_HARMONY_OPTION_TABLE 16

enum eJewelOfHarmonyItemType
{
	JEWEL_OF_HARMONY_ITEM_OPTION_TYPE_NONE = 0,
	JEWEL_OF_HARMONY_ITEM_OPTION_TYPE_WEAPON = 1,
	JEWEL_OF_HARMONY_ITEM_OPTION_TYPE_STAFF = 2,
	JEWEL_OF_HARMONY_ITEM_OPTION_TYPE_ARMOR = 3,
};

enum eJewelOfHarmonyOption
{
	JEWEL_OF_HARMONY_OPTION_NONE = 0,
	JEWEL_OF_HARMONY_WEAPON_OPTION_ADD_MIN_PHYSI_DAMAGE = 1,
	JEWEL_OF_HARMONY_WEAPON_OPTION_ADD_MAX_PHYSI_DAMAGE = 2,
	JEWEL_OF_HARMONY_WEAPON_OPTION_SUB_REQUIRE_STRENGTH = 3,
	JEWEL_OF_HARMONY_WEAPON_OPTION_SUB_REQUIRE_DEXTERITY = 4,
	JEWEL_OF_HARMONY_WEAPON_OPTION_ADD_PHYSI_DAMAGE = 5,
	JEWEL_OF_HARMONY_WEAPON_OPTION_ADD_CRITICAL_DAMAGE = 6,
	JEWEL_OF_HARMONY_WEAPON_OPTION_ADD_SKILL_DAMAGE = 7,
	JEWEL_OF_HARMONY_WEAPON_OPTION_ADD_ATTACK_SUCCESS_RATE_PVP = 8,
	JEWEL_OF_HARMONY_WEAPON_OPTION_SUB_SD_RATE = 9,
	JEWEL_OF_HARMONY_WEAPON_OPTION_ADD_SD_IGNORE_RATE = 10,
	JEWEL_OF_HARMONY_STAFF_OPTION_ADD_MAGIC_DAMAGE = 1,
	JEWEL_OF_HARMONY_STAFF_OPTION_SUB_REQUIRE_STRENGTH = 2,
	JEWEL_OF_HARMONY_STAFF_OPTION_SUB_REQUIRE_DEXTERITY = 3,
	JEWEL_OF_HARMONY_STAFF_OPTION_ADD_SKILL_DAMAGE = 4,
	JEWEL_OF_HARMONY_STAFF_OPTION_ADD_CRITICAL_DAMAGE = 5,
	JEWEL_OF_HARMONY_STAFF_OPTION_SUB_SD_RATE = 6,
	JEWEL_OF_HARMONY_STAFF_OPTION_ADD_ATTACK_SUCCESS_RATE_PVP = 7,
	JEWEL_OF_HARMONY_STAFF_OPTION_ADD_SD_IGNORE_RATE = 8,
	JEWEL_OF_HARMONY_ARMOR_OPTION_ADD_DEFENSE = 1,
	JEWEL_OF_HARMONY_ARMOR_OPTION_ADD_MAX_BP = 2,
	JEWEL_OF_HARMONY_ARMOR_OPTION_ADD_MAX_HP = 3,
	JEWEL_OF_HARMONY_ARMOR_OPTION_ADD_HP_RECOVERY = 4,
	JEWEL_OF_HARMONY_ARMOR_OPTION_ADD_MP_RECOVERY = 5,
	JEWEL_OF_HARMONY_ARMOR_OPTION_ADD_DEFENSE_SUCCESS_RATE_PVP = 6,
	JEWEL_OF_HARMONY_ARMOR_OPTION_ADD_DAMAGE_REDUCTION = 7,
	JEWEL_OF_HARMONY_ARMOR_OPTION_ADD_SD_RATE = 8,
};

struct JEWEL_OF_HARMONY_OPTION_INFO
{
	int Index;
	char Name[32];
	int Rate;
	int Level;
	int ValueTable[MAX_JEWEL_OF_HARMONY_OPTION_TABLE];
	int MoneyTable[MAX_JEWEL_OF_HARMONY_OPTION_TABLE];
};

class CJewelOfHarmonyOption
{
public:
	CJewelOfHarmonyOption();
	virtual ~CJewelOfHarmonyOption();
	void Init();
	void Load(char* path);
	void SetInfo(int type,JEWEL_OF_HARMONY_OPTION_INFO info);
	JEWEL_OF_HARMONY_OPTION_INFO* GetInfo(int type,int index);
	bool IsJewelOfHarmonyItem(CItem* lpItem);
	int GetJewelOfHarmonyItemOptionType(CItem* lpItem);
	int GetJewelOfHarmonyItemRestoreMoney(CItem* lpItem);
	int GetJewelOfHarmonyRandomOption(CItem* lpItem);
	int GetJewelOfHarmonySubRequireStrOption(CItem* lpItem);
	int GetJewelOfHarmonySubRequireDexOption(CItem* lpItem);
	bool AddJewelOfHarmonyOption(LPOBJ lpObj,int SourceSlot,int TargetSlot);
	bool AddSmeltStoneOption(LPOBJ lpObj,int SourceSlot,int TargetSlot);
	bool AddJewelOfElevationOption(LPOBJ lpObj,int SourceSlot,int TargetSlot);
	void CalcJewelOfHarmonyOption(LPOBJ lpObj,bool flag);
	void InsertOption(LPOBJ lpObj,int type,int index,int value,bool flag);
private:
	JEWEL_OF_HARMONY_OPTION_INFO m_JewelOfHarmonyOptionInfo[MAX_JEWEL_OF_HARMONY_OPTION_TYPE][MAX_JEWEL_OF_HARMONY_OPTION];
};

extern CJewelOfHarmonyOption gJewelOfHarmonyOption;
