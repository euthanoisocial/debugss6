#include "StdAfx.h"
#include "user.h"
#include "CustomDanhHieu.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "Path.h"
#include "MemScript.h"
#include "Util.h"
#include "CashShop.h"
#include "Message.h"
#include "Notice.h"
#include "CustomRankUser.h"
#include "ObjectManager.h"

#if(DANH_HIEU_NEW)

DanhHieu gDanhHieu;
// -------------------------------------------------------------------------------
DanhHieu::DanhHieu() // OK
{
	this->Init();
}
// -------------------------------------------------------------------------------
DanhHieu::~DanhHieu() // OK
{
}
// -------------------------------------------------------------------------------
void DanhHieu::Init()
{
	for (int n = 0; n < 21; n++)
	{
		this->M_OPTION_DANH_HIEU[n].Index = 0;
	}
}
// -------------------------------------------------------------------------------
void DanhHieu::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->Init();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if (section == 0)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					DANH_HIEU_OPTION_DATA info;

					memset(&info, 0, sizeof(info));

					info.Index = lpMemScript->GetNumber();

					info.DANH_HIEU_SAT_THUONG = lpMemScript->GetAsNumber();

					info.DANH_HIEU_SAT_THUONG_X2 = lpMemScript->GetAsNumber();

					info.DANH_HIEU_PHONG_THU = lpMemScript->GetAsNumber();

					info.DANH_HIEU_THE_LUC = lpMemScript->GetAsNumber();

					info.DANH_HIEU_SD = lpMemScript->GetAsNumber();

					this->SET_DANH_HIEU_INFO(info);

				}
				else

				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}
// -------------------------------------------------------------------------------
void DanhHieu::SET_DANH_HIEU_INFO(DANH_HIEU_OPTION_DATA info) // OK
{
	if (info.Index < 0 || info.Index >= 21)
	{
		return;
	}

	this->M_OPTION_DANH_HIEU[info.Index] = info;
}
// -------------------------------------------------------------------------------
void DanhHieu::DANH_HIEU_OPTION(LPOBJ lpObj, bool flag)
{
	/*if (flag != 0)
	{
		return;
	}*/
	//tue fix /////////
	if (lpObj == nullptr || flag != 0 || OBJECT_RANGE(lpObj->Index) == 0 || gObjIsConnected(lpObj->Index) == false)
	{
		return;
	}
	//////////////////////
	if (lpObj->rDanhHieu < 0 || lpObj->rDanhHieu >= sizeof(this->M_OPTION_DANH_HIEU) / sizeof(this->M_OPTION_DANH_HIEU[0]))
	{
		// Xử lý lỗi, ví dụ: ghi log lỗi hoặc trả về
		return;
	}
	// =======================
	// Add Damage		
	lpObj->PhysiDamageMinLeft += this->M_OPTION_DANH_HIEU[lpObj->rDanhHieu].DANH_HIEU_SAT_THUONG;
	lpObj->PhysiDamageMinRight += this->M_OPTION_DANH_HIEU[lpObj->rDanhHieu].DANH_HIEU_SAT_THUONG;
	lpObj->PhysiDamageMaxLeft += this->M_OPTION_DANH_HIEU[lpObj->rDanhHieu].DANH_HIEU_SAT_THUONG;
	lpObj->PhysiDamageMaxRight += this->M_OPTION_DANH_HIEU[lpObj->rDanhHieu].DANH_HIEU_SAT_THUONG;
	lpObj->MagicDamageMin += this->M_OPTION_DANH_HIEU[lpObj->rDanhHieu].DANH_HIEU_SAT_THUONG;
	lpObj->MagicDamageMax += this->M_OPTION_DANH_HIEU[lpObj->rDanhHieu].DANH_HIEU_SAT_THUONG;
	// Add Double Damage
	lpObj->DoubleDamageRate += this->M_OPTION_DANH_HIEU[lpObj->rDanhHieu].DANH_HIEU_SAT_THUONG_X2;
	// Add Defense
	lpObj->Defense += this->M_OPTION_DANH_HIEU[lpObj->rDanhHieu].DANH_HIEU_PHONG_THU;
	lpObj->MagicDefense += this->M_OPTION_DANH_HIEU[lpObj->rDanhHieu].DANH_HIEU_PHONG_THU;
	// Add HP
	lpObj->AddLife += this->M_OPTION_DANH_HIEU[lpObj->rDanhHieu].DANH_HIEU_THE_LUC;
	// Add SD
	lpObj->AddShield += this->M_OPTION_DANH_HIEU[lpObj->rDanhHieu].DANH_HIEU_SD;

}
// -------------------------------------------------------------------------------
void DanhHieu::Load()
{
	this->WCOIC_01 = GetPrivateProfileInt("CONFIG", "WCOIC_01", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_02 = GetPrivateProfileInt("CONFIG", "WCOIC_02", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_03 = GetPrivateProfileInt("CONFIG", "WCOIC_03", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_04 = GetPrivateProfileInt("CONFIG", "WCOIC_04", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_05 = GetPrivateProfileInt("CONFIG", "WCOIC_05", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_06 = GetPrivateProfileInt("CONFIG", "WCOIC_06", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_07 = GetPrivateProfileInt("CONFIG", "WCOIC_07", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_08 = GetPrivateProfileInt("CONFIG", "WCOIC_08", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_09 = GetPrivateProfileInt("CONFIG", "WCOIC_09", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_10 = GetPrivateProfileInt("CONFIG", "WCOIC_10", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_11 = GetPrivateProfileInt("CONFIG", "WCOIC_11", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_12 = GetPrivateProfileInt("CONFIG", "WCOIC_12", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_13 = GetPrivateProfileInt("CONFIG", "WCOIC_13", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_14 = GetPrivateProfileInt("CONFIG", "WCOIC_14", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_15 = GetPrivateProfileInt("CONFIG", "WCOIC_15", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_16 = GetPrivateProfileInt("CONFIG", "WCOIC_16", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_16 = GetPrivateProfileInt("CONFIG", "WCOIC_17", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_16 = GetPrivateProfileInt("CONFIG", "WCOIC_18", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_16 = GetPrivateProfileInt("CONFIG", "WCOIC_19", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
	this->WCOIC_16 = GetPrivateProfileInt("CONFIG", "WCOIC_20", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomDanhHieu.ini"));
}
// -------------------------------------------------------------------------------
void DanhHieu::BUY_DANH_HIEU(LPOBJ lpObj, BUY_DANH_HIEU_REQ *aRecv)
{
	//-----------------------------------------------
	if (aRecv->Number == 1)
	{
		if (lpObj->rDanhHieu < 1)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_01)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_01);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_01, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
		//-----------------------------------------------
		else if (lpObj->rDanhHieu < 2)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_02)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_02);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_02, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}

		//-----------------------------------------------
			else if (lpObj->rDanhHieu < 3)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_03)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_03);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_03, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
		//-----------------------------------------------
			else if (lpObj->rDanhHieu < 4)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_04)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_04);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_04, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 5)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_05)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_05);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_05, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 6)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_06)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_06);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_06, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 7)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_07)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_07);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_07, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 8)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_08)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_08);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_08, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 9)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_09)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_09);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_09, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 10)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_10)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_10);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_10, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 11)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_11)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_11);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_11, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 12)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_12)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_12);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_12, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 13)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_13)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_13);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_13, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 14)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_14)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_14);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_14, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 15)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_15)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_15);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_15, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 16)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_16)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_16);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_16, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 17)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_17)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_17);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_17, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 18)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_18)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_18);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_18, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 19)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_19)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_19);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_19, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
			//-----------------------------------------------
			else if (lpObj->rDanhHieu < 20)
		{
		if (lpObj->rDanhHieu > 20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2008));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_20)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2006), this->WCOIC_20);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_20, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rDanhHieu += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2007), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
	}


}
#endif
