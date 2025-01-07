#include "StdAfx.h"
#include "user.h"
#include "CustomNguHanh.h"
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
#include "CustomVKR.h"

#if(NGU_HANH_NEW)

NguHanh gNguHanh;
// -------------------------------------------------------------------------------
NguHanh::NguHanh() // OK
{
	//this->LOAD_TXT ();
}
// -------------------------------------------------------------------------------
NguHanh::~NguHanh() // OK
{
}
// -------------------------------------------------------------------------------
void NguHanh::LOAD_TXT()
{
	this->NGU_HANH_MAX = GetPrivateProfileInt("CONFIG", "NGU_HANH_MAX", 0, gPath.GetFullPath("Custom\\NguHanh\\CustomNguHanh.ini"));
	this->NGU_HANH_WCOINC = GetPrivateProfileInt("CONFIG", "NGU_HANH_WCOINC", 0, gPath.GetFullPath("Custom\\NguHanh\\CustomNguHanh.ini"));

	this->Kim = GetPrivateProfileInt("CONFIG", "Kim", 0, gPath.GetFullPath("Custom\\NguHanh\\CustomNguHanh.ini"));
	this->Moc = GetPrivateProfileInt("CONFIG", "Moc", 0, gPath.GetFullPath("Custom\\NguHanh\\CustomNguHanh.ini"));
	this->Thuy = GetPrivateProfileInt("CONFIG","Thuy", 0, gPath.GetFullPath("Custom\\NguHanh\\CustomNguHanh.ini"));
	this->Hoa = GetPrivateProfileInt("CONFIG", "Hoa", 0, gPath.GetFullPath("Custom\\NguHanh\\CustomNguHanh.ini"));
	this->Tho = GetPrivateProfileInt("CONFIG", "Tho", 0, gPath.GetFullPath("Custom\\NguHanh\\CustomNguHanh.ini"));
}
// -------------------------------------------------------------------------------
void NguHanh::BUY_NGU_HANH(LPOBJ lpObj, BUY_NGU_HANH_REQ *aRecv)
{
	if (aRecv->Number == 1)
	{

		if (lpObj->rNguHanh < 1)
		{
			if (lpObj->rNguHanh > this->NGU_HANH_MAX)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2020));
				return;
			}

			int NguyenLieu = (
				  gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,19),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,51),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,56),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,105),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,106),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,110),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,13),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,25),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,31),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,18),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,30),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,33),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,10),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,49),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,65),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,36),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,50),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,66),13));


			if (NguyenLieu < 1)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2019));
				return;
			}

			//--
			lpObj->rNguHanh += 1; // Thay Đổi
			//--
			GCFireworksSend(lpObj, lpObj->X, lpObj->Y);
			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2018), lpObj->Name, lpObj->rNguHanh);
			gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
			GDCharacterInfoSaveSend(lpObj->Index);
			GetInfoNguHanh(lpObj);
			gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
		else if (lpObj->rNguHanh < 2)
		{
			if (lpObj->rNguHanh > this->NGU_HANH_MAX)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2020));
				return;
			}

				int NguyenLieu = (
				  gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,19),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,51),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,56),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,105),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,106),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,110),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,13),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,25),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,31),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,18),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,30),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,33),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,10),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,49),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,65),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,36),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,50),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,66),13));

			if (NguyenLieu < 1)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2019));
				return;
			}

			//--
			lpObj->rNguHanh += 1; // Thay Đổi
			//--
			GCFireworksSend(lpObj, lpObj->X, lpObj->Y);
			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2018), lpObj->Name, lpObj->rNguHanh);
			gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
			GDCharacterInfoSaveSend(lpObj->Index);
			GetInfoNguHanh(lpObj);
			gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
		else if (lpObj->rNguHanh < 3)
		{
			if (lpObj->rNguHanh > this->NGU_HANH_MAX)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2020));
				return;
			}

				int NguyenLieu = (
				  gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,19),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,51),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,56),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,105),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,106),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,110),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,13),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,25),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,31),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,18),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,30),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,33),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,10),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,49),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,65),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,36),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,50),13)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,66),13));

			if (NguyenLieu < 1)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2019));
				return;
			}

			//--
			lpObj->rNguHanh += 1; // Thay Đổi
			//--
			GCFireworksSend(lpObj, lpObj->X, lpObj->Y);
			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2018), lpObj->Name, lpObj->rNguHanh);
			gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
			GDCharacterInfoSaveSend(lpObj->Index);
			GetInfoNguHanh(lpObj);
			gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
		else if (lpObj->rNguHanh < 4)
		{
			if (lpObj->rNguHanh > this->NGU_HANH_MAX)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2020));
				return;
			}

				int NguyenLieu = (
				  gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,19),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,51),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,56),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,105),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,106),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,110),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,13),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,25),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,31),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,18),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,30),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,33),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,10),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,49),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,65),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,36),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,50),14)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,66),14));

			if (NguyenLieu < 1)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2019));
				return;
			}

			//--
			lpObj->rNguHanh += 1; // Thay Đổi
			//--
			GCFireworksSend(lpObj, lpObj->X, lpObj->Y);
			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2018), lpObj->Name, lpObj->rNguHanh);
			gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
			GDCharacterInfoSaveSend(lpObj->Index);
			GetInfoNguHanh(lpObj);
			gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
		else if (lpObj->rNguHanh < 5)
		{
			if (lpObj->rNguHanh > this->NGU_HANH_MAX)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2020));
				return;
			}

				int NguyenLieu = (
				  gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,19),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,51),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,56),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,105),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,106),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(0,110),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,13),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,25),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(2,31),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,18),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,30),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(4,33),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,10),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,49),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,65),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,36),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,50),15)
				||gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(5,66),15));

			if (NguyenLieu < 1)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(2019));
				return;
			}

			//--
			lpObj->rNguHanh += 1; // Thay Đổi
			//--
			GCFireworksSend(lpObj, lpObj->X, lpObj->Y);
			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(2018), lpObj->Name, lpObj->rNguHanh);
			gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);
			GDCharacterInfoSaveSend(lpObj->Index);
			GetInfoNguHanh(lpObj);
			gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
	}
	if (aRecv->Number == 2)
	{
		GetInfoNguHanh(lpObj);
	}

}
#endif
