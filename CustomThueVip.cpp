#include "StdAfx.h"
#include "user.h"
#include "CustomThueVip.h"
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

#if(THUE_VIP_NEW)

ThueVip gThueVip;
// -------------------------------------------------------------------------------
ThueVip::ThueVip() // OK
{
}
// -------------------------------------------------------------------------------
ThueVip::~ThueVip() // OK
{
}
// -------------------------------------------------------------------------------
void ThueVip::THUE_VIP_OPTION(LPOBJ lpObj, bool flag)
{
	if (flag != 0)
	{
		return;
	}
	if (lpObj->rThueVip == 1)
	{
		// SÁT THƯƠNG HOÀN HẢO
		lpObj->ExcellentDamage += this->THUE_VIP_SAT_THUONG_HOAN_HAO_DONG;
		// SÁT THƯƠNG CHÍ MẠNG
		lpObj->CriticalDamage += this->THUE_VIP_SAT_THUONG_CHI_MANG_DONG;
		// X2 SÁT THƯƠNG
		lpObj->DoubleDamageRate += this->THUE_VIP_SAT_THUONG_X2_DONG;
		// SÁT THƯƠNG		
		lpObj->PhysiDamageMinLeft += this->THUE_VIP_SAT_THUONG_DONG;
		lpObj->PhysiDamageMinRight += this->THUE_VIP_SAT_THUONG_DONG;
		lpObj->PhysiDamageMaxLeft += this->THUE_VIP_SAT_THUONG_DONG;
		lpObj->PhysiDamageMaxRight += this->THUE_VIP_SAT_THUONG_DONG;
		// THỂ LỰC
		lpObj->AddLife += this->THUE_VIP_THE_LUC_DONG;
		// SD
		lpObj->AddShield += this->THUE_VIP_SD_DONG;
	}
	if (lpObj->rThueVip == 2)
	{
		// SÁT THƯƠNG HOÀN HẢO
		lpObj->ExcellentDamage += this->THUE_VIP_SAT_THUONG_HOAN_HAO_BAC;
		// SÁT THƯƠNG CHÍ MẠNG
		lpObj->CriticalDamage += this->THUE_VIP_SAT_THUONG_CHI_MANG_BAC;
		// X2 SÁT THƯƠNG
		lpObj->DoubleDamageRate += this->THUE_VIP_SAT_THUONG_X2_BAC;
		// SÁT THƯƠNG		
		lpObj->PhysiDamageMinLeft += this->THUE_VIP_SAT_THUONG_BAC;
		lpObj->PhysiDamageMinRight += this->THUE_VIP_SAT_THUONG_BAC;
		lpObj->PhysiDamageMaxLeft += this->THUE_VIP_SAT_THUONG_BAC;
		lpObj->PhysiDamageMaxRight += this->THUE_VIP_SAT_THUONG_BAC;
		// THỂ LỰC
		lpObj->AddLife += this->THUE_VIP_THE_LUC_BAC;
		// SD
		lpObj->AddShield += this->THUE_VIP_SD_BAC;
	}
	if (lpObj->rThueVip == 3)
	{
		// SÁT THƯƠNG HOÀN HẢO
		lpObj->ExcellentDamage += this->THUE_VIP_SAT_THUONG_HOAN_HAO_VANG;
		// SÁT THƯƠNG CHÍ MẠNG
		lpObj->CriticalDamage += this->THUE_VIP_SAT_THUONG_CHI_MANG_VANG;
		// X2 SÁT THƯƠNG
		lpObj->DoubleDamageRate += this->THUE_VIP_SAT_THUONG_X2_VANG;
		// SÁT THƯƠNG		
		lpObj->PhysiDamageMinLeft += this->THUE_VIP_SAT_THUONG_VANG;
		lpObj->PhysiDamageMinRight += this->THUE_VIP_SAT_THUONG_VANG;
		lpObj->PhysiDamageMaxLeft += this->THUE_VIP_SAT_THUONG_VANG;
		lpObj->PhysiDamageMaxRight += this->THUE_VIP_SAT_THUONG_VANG;
		// THỂ LỰC
		lpObj->AddLife += this->THUE_VIP_THE_LUC_VANG;
		// SD
		lpObj->AddShield += this->THUE_VIP_SD_VANG;
	}
	if (lpObj->rThueVip == 4)
	{
		// SÁT THƯƠNG HOÀN HẢO
		lpObj->ExcellentDamage += this->THUE_VIP_SAT_THUONG_HOAN_HAO_KIMCUONG;
		// SÁT THƯƠNG CHÍ MẠNG
		lpObj->CriticalDamage += this->THUE_VIP_SAT_THUONG_CHI_MANG_KIMCUONG;
		// X2 SÁT THƯƠNG
		lpObj->DoubleDamageRate += this->THUE_VIP_SAT_THUONG_X2_KIMCUONG;
		// SÁT THƯƠNG		
		lpObj->PhysiDamageMinLeft += this->THUE_VIP_SAT_THUONG_KIMCUONG;
		lpObj->PhysiDamageMinRight += this->THUE_VIP_SAT_THUONG_KIMCUONG;
		lpObj->PhysiDamageMaxLeft += this->THUE_VIP_SAT_THUONG_KIMCUONG;
		lpObj->PhysiDamageMaxRight += this->THUE_VIP_SAT_THUONG_KIMCUONG;
		// THỂ LỰC
		lpObj->AddLife += this->THUE_VIP_THE_LUC_KIMCUONG;
		// SD
		lpObj->AddShield += this->THUE_VIP_SD_KIMCUONG;
	}
}
// -------------------------------------------------------------------------------
void ThueVip::LOAD_TXT()
{
	this->VIP_DONG_WCOINC = GetPrivateProfileInt("CONFIG", "VIP_DONG_WCOINC", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->VIP_BAC_WCOINC = GetPrivateProfileInt("CONFIG", "VIP_BAC_WCOINC", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->VIP_VANG_WCOINC = GetPrivateProfileInt("CONFIG", "VIP_VANG_WCOINC", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->VIP_KIMCUONG_WCOINC = GetPrivateProfileInt("CONFIG", "VIP_KIMCUONG_WCOINC", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));

	this->VIP_DONG_TIME = GetPrivateProfileInt("CONFIG", "VIP_DONG_TIME", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->VIP_BAC_TIME = GetPrivateProfileInt("CONFIG", "VIP_BAC_TIME", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->VIP_VANG_TIME = GetPrivateProfileInt("CONFIG", "VIP_VANG_TIME", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->VIP_KIMCUONG_TIME = GetPrivateProfileInt("CONFIG", "VIP_KIMCUONG_TIME", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));

	this->THUE_VIP_SAT_THUONG_HOAN_HAO_DONG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_HOAN_HAO_DONG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SAT_THUONG_CHI_MANG_DONG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_CHI_MANG_DONG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SAT_THUONG_X2_DONG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_X2_DONG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SAT_THUONG_DONG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_DONG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_THE_LUC_DONG = GetPrivateProfileInt("CONFIG", "THUE_VIP_THE_LUC_DONG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SD_DONG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SD_DONG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));

	this->THUE_VIP_SAT_THUONG_HOAN_HAO_BAC = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_HOAN_HAO_BAC", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SAT_THUONG_CHI_MANG_BAC = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_CHI_MANG_BAC", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SAT_THUONG_X2_BAC = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_X2_BAC", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SAT_THUONG_BAC = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_BAC", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_THE_LUC_BAC = GetPrivateProfileInt("CONFIG", "THUE_VIP_THE_LUC_BAC", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SD_BAC = GetPrivateProfileInt("CONFIG", "THUE_VIP_SD_BAC", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));

	this->THUE_VIP_SAT_THUONG_HOAN_HAO_VANG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_HOAN_HAO_VANG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SAT_THUONG_CHI_MANG_VANG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_CHI_MANG_VANG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SAT_THUONG_X2_VANG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_X2_VANG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SAT_THUONG_VANG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_VANG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_THE_LUC_VANG = GetPrivateProfileInt("CONFIG", "THUE_VIP_THE_LUC_VANG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SD_VANG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SD_VANG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));

	this->THUE_VIP_SAT_THUONG_HOAN_HAO_KIMCUONG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_HOAN_HAO_KIMCUONG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SAT_THUONG_CHI_MANG_KIMCUONG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_CHI_MANG_KIMCUONG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SAT_THUONG_X2_KIMCUONG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_X2_KIMCUONG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SAT_THUONG_KIMCUONG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SAT_THUONG_KIMCUONG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_THE_LUC_KIMCUONG = GetPrivateProfileInt("CONFIG", "THUE_VIP_THE_LUC_KIMCUONG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));
	this->THUE_VIP_SD_KIMCUONG = GetPrivateProfileInt("CONFIG", "THUE_VIP_SD_KIMCUONG", 0, gPath.GetFullPath("Custom\\VipNhanVat\\CustomThueVip.ini"));



}
// -------------------------------------------------------------------------------
void ThueVip::BUY_THUE_VIP(LPOBJ lpObj, BUY_THUE_VIP_REQ *aRecv)
{
	if (aRecv->Number == 1)
	{
		if (lpObj->rThueVip > 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2005));
			return;
		}
		if (lpObj->Coin1 < this->VIP_DONG_WCOINC)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2003), this->VIP_DONG_WCOINC);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->VIP_DONG_WCOINC, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rThueVip = 1; // Thay Đổi
		lpObj->rThueVipTime = this->VIP_DONG_TIME; // 1 Ngay
		GCFireworksSend(lpObj, lpObj->X, lpObj->Y);
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2004), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
	}
	//-----------------------------------------------
	else if (aRecv->Number == 2)
	{
		if (lpObj->rThueVip > 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2005));
			return;
		}
		if (lpObj->Coin1 < this->VIP_BAC_WCOINC)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2003), this->VIP_BAC_WCOINC);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->VIP_BAC_WCOINC, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rThueVip = 2; // Thay Đổi
		lpObj->rThueVipTime = this->VIP_BAC_TIME; // 1 Ngay
		GCFireworksSend(lpObj, lpObj->X, lpObj->Y);
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2004), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
	}
	//-----------------------------------------------
	else if (aRecv->Number == 3)
	{
		if (lpObj->rThueVip > 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2005));
			return;
		}
		if (lpObj->Coin1 <  this->VIP_VANG_WCOINC)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2003), this->VIP_VANG_WCOINC);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->VIP_VANG_WCOINC, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rThueVip = 3; // Thay Đổi
		lpObj->rThueVipTime = this->VIP_VANG_TIME; // 1 Ngay
		GCFireworksSend(lpObj, lpObj->X, lpObj->Y);
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2004), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
	}
	//-----------------------------------------------
	else if (aRecv->Number == 4)
	{
		if (lpObj->rThueVip > 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2005));
			return;
		}
		if (lpObj->Coin1 <  this->VIP_KIMCUONG_WCOINC)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2003), this->VIP_KIMCUONG_WCOINC);
			return;
		}
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->VIP_KIMCUONG_WCOINC, 0, 0, 0);
		gCashShop.CGCashShopPointRecv(lpObj->Index);
		lpObj->rThueVip = 4; // Thay Đổi
		lpObj->rThueVipTime = this->VIP_KIMCUONG_TIME; // 1 Ngay
		GCFireworksSend(lpObj, lpObj->X, lpObj->Y);
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2004), lpObj->Name);
		gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
		GDCharacterInfoSaveSend(lpObj->Index);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
	}
	//-----------------------------------------------

}
#endif
