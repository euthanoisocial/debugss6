#include "StdAfx.h"
#include "user.h"
#include "CustomQuanHam.h"
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

#if(QUAN_HAM_NEW)

QuanHam gQuanHam;
// -------------------------------------------------------------------------------
QuanHam::QuanHam() // OK
{
	this->Init();
}
// -------------------------------------------------------------------------------
QuanHam::~QuanHam() // OK
{
}
// -------------------------------------------------------------------------------
void QuanHam::Init()
{
	for (int n = 0; n < 4; n++)
	{
		this->M_OPTION_QUAN_HAM[n].Index = 0;
	}
}
// -------------------------------------------------------------------------------
void QuanHam::Load(char* path) // OK
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

					QUAN_HAM_OPTION_DATA info;

					memset(&info, 0, sizeof(info));

					info.Index = lpMemScript->GetNumber();

					info.QUAN_HAM_SAT_THUONG = lpMemScript->GetAsNumber();

					info.QUAN_HAM_SAT_THUONG_X2 = lpMemScript->GetAsNumber();

					info.QUAN_HAM_PHONG_THU = lpMemScript->GetAsNumber();

					info.QUAN_HAM_THE_LUC = lpMemScript->GetAsNumber();

					info.QUAN_HAM_SD = lpMemScript->GetAsNumber();

					this->SET_QUAN_HAM_INFO(info);

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
void QuanHam::SET_QUAN_HAM_INFO(QUAN_HAM_OPTION_DATA info) // OK
{
	if (info.Index < 0 || info.Index >= 23)
	{
		return;
	}

	this->M_OPTION_QUAN_HAM[info.Index] = info;
}
// -------------------------------------------------------------------------------
void QuanHam::QUAN_HAM_OPTION(LPOBJ lpObj, bool flag)
{
	
	if (lpObj == nullptr || flag != 0 || OBJECT_RANGE(lpObj->Index) == 0 || gObjIsConnected(lpObj->Index) == false)
	{
		return;
	}
	//////////////////////
	if (lpObj->rQuanHam < 0 || lpObj->rQuanHam >= sizeof(this->M_OPTION_QUAN_HAM) / sizeof(this->M_OPTION_QUAN_HAM[0]))
	{
		// Xử lý lỗi, ví dụ: ghi log lỗi hoặc trả về
		return;
	}
	// Add Damage		
	lpObj->PhysiDamageMinRight += this->M_OPTION_QUAN_HAM[lpObj->rQuanHam].QUAN_HAM_SAT_THUONG;
	lpObj->PhysiDamageMaxRight += this->M_OPTION_QUAN_HAM[lpObj->rQuanHam].QUAN_HAM_SAT_THUONG;
	lpObj->PhysiDamageMinLeft += this->M_OPTION_QUAN_HAM[lpObj->rQuanHam].QUAN_HAM_SAT_THUONG;
	lpObj->PhysiDamageMaxLeft += this->M_OPTION_QUAN_HAM[lpObj->rQuanHam].QUAN_HAM_SAT_THUONG;
	lpObj->MagicDamageMin += this->M_OPTION_QUAN_HAM[lpObj->rQuanHam].QUAN_HAM_SAT_THUONG;
	lpObj->MagicDamageMax += this->M_OPTION_QUAN_HAM[lpObj->rQuanHam].QUAN_HAM_SAT_THUONG;

	//// Add Double Damage
	lpObj->DoubleDamageRate += this->M_OPTION_QUAN_HAM[lpObj->rQuanHam].QUAN_HAM_SAT_THUONG_X2;
	//// Add Defense
	lpObj->Defense += this->M_OPTION_QUAN_HAM[lpObj->rQuanHam].QUAN_HAM_PHONG_THU;
	lpObj->MagicDefense += this->M_OPTION_QUAN_HAM[lpObj->rQuanHam].QUAN_HAM_PHONG_THU;
	//// Add HP
	lpObj->AddLife += this->M_OPTION_QUAN_HAM[lpObj->rQuanHam].QUAN_HAM_THE_LUC;
	//// Add SD
	lpObj->AddShield += this->M_OPTION_QUAN_HAM[lpObj->rQuanHam].QUAN_HAM_SD;

}
// -------------------------------------------------------------------------------

void QuanHam::Load()
{
	this->WCOIC_01 = GetPrivateProfileInt("CONFIG", "WCOIC_01", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomQuanHam.ini"));
	this->WCOIC_02 = GetPrivateProfileInt("CONFIG", "WCOIC_02", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomQuanHam.ini"));
	this->WCOIC_03 = GetPrivateProfileInt("CONFIG", "WCOIC_03", 0, gPath.GetFullPath("Custom\\DanhHieu\\CustomQuanHam.ini"));
}
void QuanHam::BUY_QUAN_HAM(LPOBJ lpObj, BUY_QUAN_HAM_REQ *aRecv)
{
	//-----------------------------------------------
	if (aRecv->Number == 1)
	{
		if (lpObj->rQuanHam < 1)
		{
		if (lpObj->rQuanHam > 3)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2011));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_01)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2009), this->WCOIC_01);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_01, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rQuanHam += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2010), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
		//-----------------------------------------------
		else if (lpObj->rQuanHam < 2)
		{
		if (lpObj->rQuanHam > 3)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2011));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_02)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2009), this->WCOIC_02);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_02, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rQuanHam += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2010), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}

		//-----------------------------------------------
			else if (lpObj->rQuanHam < 3)
		{
		if (lpObj->rQuanHam > 3)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2011));
			return;
		}
		if (lpObj->Coin1 < this->WCOIC_03)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2009), this->WCOIC_03);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->WCOIC_03, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rQuanHam += 1; // Thay Đổi
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2010), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
		
	}
}
#endif
