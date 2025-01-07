#include "StdAfx.h"
#include "user.h"
#include "CustomRingPedan.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "Path.h"
#include "MemScript.h"
#include "Util.h"
#include "CashShop.h"
#include "Message.h"
#include "Notice.h"
#include "ItemBagManager.h"
#include "ItemManager.h"
#include "ObjectManager.h"
#include "CustomSachThuocTinh.h"

#if(RINGPEDAN_NEW)

RingPedan gRingPedan;

RingPedan::RingPedan() // OK
{
	this->Init();
}

RingPedan::~RingPedan() // OK
{

}

void RingPedan::Init()
{

}

void RingPedan::Load()
{

}
///===============================================================
void RingPedan::GDSaveData(int aIndex)
{
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	RINGPEDAN_GD_SAVE_DATA pRequest;
	pRequest.header.set(0xB0, 0x10, sizeof(pRequest));
	memcpy(pRequest.Name, lpUser->Name, 11);
	pRequest.index = aIndex;
	//--
	pRequest.RINGPEDAN1 = lpUser->RINGPEDAN.RINGPEDAN1;
	pRequest.RINGPEDAN2 = lpUser->RINGPEDAN.RINGPEDAN2;
	pRequest.RINGPEDAN3 = lpUser->RINGPEDAN.RINGPEDAN3;
	pRequest.RINGPEDAN4 = lpUser->RINGPEDAN.RINGPEDAN4;
	//--
	gDataServerConnection.DataSend((BYTE*)&pRequest, pRequest.header.size);
}
// -------------------------------------------------------------------------------
void RingPedan::GDReqData(int aIndex)
{

	LPOBJ lpObj = &gObj[aIndex];
	RINGPEDAN_GD_REQ_DATA pMsg;
	pMsg.header.set(0xB0, 0x09, sizeof(pMsg));
	pMsg.index = aIndex;
	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));
	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);

}
// -------------------------------------------------------------------------------
void RingPedan::GDSaveUserInfo(int aIndex)	//-> Complete
{
	this->GDSaveData(aIndex);
}
// -------------------------------------------------------------------------------
void RingPedan::GDReqUserInfo(int aIndex)	//-> Complete
{
	// ----
	this->GDReqData(aIndex);
}
// -------------------------------------------------------------------------------
void RingPedan::DGGetData(RINGPEDAN_DG_GET_DATA * aRecv)
{
	// ----
	LPOBJ lpUser = &gObj[aRecv->index];
	// ----

	if (aRecv->RINGPEDAN1 < 0)
	{
		aRecv->RINGPEDAN1 = 0;
	}
	else if (aRecv->RINGPEDAN2 < 0)
	{
		aRecv->RINGPEDAN2 = 0;
	}
	else if (aRecv->RINGPEDAN3 < 0)
	{
		aRecv->RINGPEDAN3 = 0;
	}
	else if (aRecv->RINGPEDAN4 < 0)
	{
		aRecv->RINGPEDAN4 = 0;
	}
	// ----
	lpUser->RINGPEDAN.RINGPEDAN1 = aRecv->RINGPEDAN1;
	lpUser->RINGPEDAN.RINGPEDAN2 = aRecv->RINGPEDAN2;
	lpUser->RINGPEDAN.RINGPEDAN3 = aRecv->RINGPEDAN3;
	lpUser->RINGPEDAN.RINGPEDAN4 = aRecv->RINGPEDAN4;
}
// -------------------------------------------------------------------------------
//===============================================================================================================================
void RingPedan::Load_Text()
{
	this->MAX_RINGPEDAN_01 = GetPrivateProfileInt("Config", "MAX_RINGPEDAN_01", 0, gPath.GetFullPath("Custom\\CustomRingPedan.ini"));
	this->MAX_RINGPEDAN_02 = GetPrivateProfileInt("Config", "MAX_RINGPEDAN_02", 0, gPath.GetFullPath("Custom\\CustomRingPedan.ini"));
	this->MAX_RINGPEDAN_03 = GetPrivateProfileInt("Config", "MAX_RINGPEDAN_03", 0, gPath.GetFullPath("Custom\\CustomRingPedan.ini"));
	this->MAX_RINGPEDAN_04 = GetPrivateProfileInt("Config", "MAX_RINGPEDAN_04", 0, gPath.GetFullPath("Custom\\CustomRingPedan.ini"));

	GetPrivateProfileString("Text", "MSG_01", "BAN DA DAT CAP DO TOI DA", MSG_01, 1024, gPath.GetFullPath("Custom\\CustomRingPedan.ini"));
	GetPrivateProfileString("Text", "MSG_02", "BAN CAN 1 %s SACH THUOC TINH", MSG_02, 1024, gPath.GetFullPath("Custom\\CustomRingPedan.ini")); 

}
//===============================================================================================================================
void RingPedan::BUG_RINGPEDAN(LPOBJ lpObj, BUY_RINGPEDAN_REQ *aRecv)
{
	//-----------------------------------------------
	if (aRecv->Number == 1)
	{

		//----
		if (lpObj->RINGPEDAN.RINGPEDAN1 >  this->MAX_RINGPEDAN_01) // < 30
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->MSG_01);
			return;
		}

		int RingPedan01 = 0;
		for (int x = 12; x < INVENTORY_MAIN_SIZE; x++)
		{
			if (lpObj->Inventory[x].IsItem() == TRUE)
			{
				if (lpObj->Inventory[x].m_Index == GET_ITEM(14, 457))
				{
					RingPedan01 = 1;
					gItemManager.InventoryDelItem(lpObj->Index, x);
					gItemManager.GCItemDeleteSend(lpObj->Index, x, 1);
					lpObj->RINGPEDAN.RINGPEDAN1 += 1; // Thay Đổi
					this->GDSaveUserInfo(lpObj->Index);
					gObjectManager.CharacterCalcAttribute(lpObj->Index);
					break;
				}
			}
		}
		if (RingPedan01 == 0)
		{
			char text[100];
			wsprintf(text, this->MSG_02, gItemManager.GetItemName(GET_ITEM(14, 457)));
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, text);
		}
	}
	if (aRecv->Number == 2)
	{
		//----
		if (lpObj->RINGPEDAN.RINGPEDAN2 >  this->MAX_RINGPEDAN_02) // < 30
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->MSG_01);
			return;
		}

		int RingPedan02 = 0;
		for (int x = 12; x < INVENTORY_MAIN_SIZE; x++)
		{
			if (lpObj->Inventory[x].IsItem() == TRUE)
			{
				if (lpObj->Inventory[x].m_Index == GET_ITEM(14, 459))
				{
					RingPedan02 = 1;
					gItemManager.InventoryDelItem(lpObj->Index, x);
					gItemManager.GCItemDeleteSend(lpObj->Index, x, 1);
					lpObj->RINGPEDAN.RINGPEDAN2 += 1; // Thay Đổi
					this->GDSaveUserInfo(lpObj->Index);
					gObjectManager.CharacterCalcAttribute(lpObj->Index);
					break;
				}
			}
		}
		if (RingPedan02 == 0)
		{
			char text[100];
			wsprintf(text, this->MSG_02, gItemManager.GetItemName(GET_ITEM(14, 459)));
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, text);
		}
	}
	if (aRecv->Number == 3)
	{
		//----
		if (lpObj->RINGPEDAN.RINGPEDAN3 >  this->MAX_RINGPEDAN_03) // < 30
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->MSG_01);
			return;
		}

		int RingPedan03 = 0;
		for (int x = 12; x < INVENTORY_MAIN_SIZE; x++)
		{
			if (lpObj->Inventory[x].IsItem() == TRUE)
			{
				if (lpObj->Inventory[x].m_Index == GET_ITEM(14, 458))
				{
					RingPedan03 = 1;
					gItemManager.InventoryDelItem(lpObj->Index, x);
					gItemManager.GCItemDeleteSend(lpObj->Index, x, 1);
					lpObj->RINGPEDAN.RINGPEDAN3 += 1; // Thay Đổi
					this->GDSaveUserInfo(lpObj->Index);
					gObjectManager.CharacterCalcAttribute(lpObj->Index);
					break;
				}
			}
		}
		if (RingPedan03 == 0)
		{
			char text[100];
			wsprintf(text, this->MSG_02, gItemManager.GetItemName(GET_ITEM(14, 458)));
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, text);
		}
	}
	if (aRecv->Number == 4)
	{
		//----
		if (lpObj->RINGPEDAN.RINGPEDAN4 >  this->MAX_RINGPEDAN_04) // < 30
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, this->MSG_01);
			return;
		}

		int RingPedan04 = 0;
		for (int x = 12; x < INVENTORY_MAIN_SIZE; x++)
		{
			if (lpObj->Inventory[x].IsItem() == TRUE)
			{
				if (lpObj->Inventory[x].m_Index == GET_ITEM(14, 460))
				{
					RingPedan04 = 1;
					gItemManager.InventoryDelItem(lpObj->Index, x);
					gItemManager.GCItemDeleteSend(lpObj->Index, x, 1);
					lpObj->RINGPEDAN.RINGPEDAN4 += 1; // Thay Đổi
					this->GDSaveUserInfo(lpObj->Index);
					gObjectManager.CharacterCalcAttribute(lpObj->Index);
					break;
				}
			}
		}
		if (RingPedan04 == 0)
		{
			char text[100];
			wsprintf(text, this->MSG_02, gItemManager.GetItemName(GET_ITEM(14, 460)));
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, text);
		}
	}
}
//===============================================================================================================================
void RingPedan::Load_Option(LPOBJ lpObj, bool flag)
{
	if (flag != 0)
	{
		return;
	}

	CItem* Right = &lpObj->Inventory[0];

	////--------------------------------------------------------------------------------
	if (lpObj->RINGPEDAN.RINGPEDAN1 >= 5)
	{
		lpObj->AddLife += (int)((lpObj->MaxLife * 4) / 100);
		lpObj->AddMana += (int)((lpObj->MaxMana * 4) / 100);
	}
	if (lpObj->RINGPEDAN.RINGPEDAN1 >= 10)
	{
		lpObj->DamageReflect += (int)((lpObj->DamageReflect * 4) / 100); // Phản sát thương
		lpObj->DamageReduction[5] += (int)((lpObj->DamageReduction[5] * 5) / 100); // Giảm sát thương
	}
	if (lpObj->RINGPEDAN.RINGPEDAN1 >= 15)
	{
		lpObj->Defense += 10; // Sức chống đỡ
		lpObj->SkillDamageBonus += 50; // Tấn công của tuyệt chiêu
		lpObj->ExcellentDamage += 5; // Sát thương hoàn hảo
	}
	////--------------------------------------------------------------------------------
	if (lpObj->RINGPEDAN.RINGPEDAN2 >= 5)
	{
		lpObj->AddLife += (int)((lpObj->MaxLife * 4) / 100);
		lpObj->AddMana += (int)((lpObj->MaxMana * 4) / 100);
	}
	if (lpObj->RINGPEDAN.RINGPEDAN2 >= 10)
	{
		lpObj->DamageReflect += (int)((lpObj->DamageReflect * 4) / 100); // Phản sát thương
		lpObj->DamageReduction[5] += (int)((lpObj->DamageReduction[5] * 5) / 100); // Giảm sát thương
	}
	if (lpObj->RINGPEDAN.RINGPEDAN2 >= 15)
	{
		lpObj->Defense += 10; // Sức chống đỡ
		lpObj->SkillDamageBonus += 50; // Tấn công của tuyệt chiêu
		lpObj->ExcellentDamage += 5; // Sát thương hoàn hảo
	}
	////--------------------------------------------------------------------------------
	if (lpObj->RINGPEDAN.RINGPEDAN3 >= 5)
	{
		lpObj->AddLife += (int)((lpObj->MaxLife * 4) / 100);
		lpObj->AddMana += (int)((lpObj->MaxMana * 4) / 100);
	}
	if (lpObj->RINGPEDAN.RINGPEDAN3 >= 10)
	{
		lpObj->DamageReflect += (int)((lpObj->DamageReflect * 4) / 100); // Phản sát thương
		lpObj->DamageReduction[5] += (int)((lpObj->DamageReduction[5] * 5) / 100); // Giảm sát thương
	}
	if (lpObj->RINGPEDAN.RINGPEDAN3 >= 15)
	{
		lpObj->Defense += 10; // Sức chống đỡ
		lpObj->SkillDamageBonus += 50; // Tấn công của tuyệt chiêu
		lpObj->ExcellentDamage += 5; // Sát thương hoàn hảo
	}
	////--------------------------------------------------------------------------------
	if (lpObj->RINGPEDAN.RINGPEDAN4 >= 5)
	{
		lpObj->AddLife += (int)((lpObj->MaxLife * 4) / 100);
		lpObj->AddMana += (int)((lpObj->MaxMana * 4) / 100);
	}
	if (lpObj->RINGPEDAN.RINGPEDAN4 >= 10)
	{
		lpObj->DamageReflect += (int)((lpObj->DamageReflect * 4) / 100); // Phản sát thương
		lpObj->DamageReduction[5] += (int)((lpObj->DamageReduction[5] * 5) / 100); // Giảm sát thương
	}
	if (lpObj->RINGPEDAN.RINGPEDAN4 >= 15)
	{
		lpObj->Defense += 10; // Sức chống đỡ
		lpObj->SkillDamageBonus += 50; // Tấn công của tuyệt chiêu
		lpObj->ExcellentDamage += 5; // Sát thương hoàn hảo
	}
	////--------------------------------------------------------------------------------

	////--------------------------------------------------------------------------------
	//if (lpObj->Class == CLASS_DW)
	//{
	//	lpObj->PhysiDamageMinLeft += lpObj->RingPedan.RingPedan1;	 //	Sát Thương
	//	lpObj->PhysiDamageMinRight += lpObj->RingPedan.RingPedan1; //	Sát Thương
	//	lpObj->PhysiDamageMaxLeft += lpObj->RingPedan.RingPedan1;	 //	Sát Thương
	//	lpObj->PhysiDamageMaxRight += lpObj->RingPedan.RingPedan1; //	Sát Thương

	//	lpObj->MagicDamageMax += (lpObj->MagicDamageMin*lpObj->RingPedan.RingPedan1) / 100;		 //	 Sát thương phép thuật
	//	lpObj->CurseDamageMax += (lpObj->MagicDamageMax*lpObj->RingPedan.RingPedan1) / 100;		 //	 Sát thương phép thuật
	//}
	//else if (lpObj->Class == CLASS_DK)
	//{
	//	lpObj->PhysiDamageMinLeft += lpObj->RingPedan.RingPedan1;	 //	Sát Thương
	//	lpObj->PhysiDamageMinRight += lpObj->RingPedan.RingPedan1; //	Sát Thương
	//	lpObj->PhysiDamageMaxLeft += lpObj->RingPedan.RingPedan1;	 //	Sát Thương
	//	lpObj->PhysiDamageMaxRight += lpObj->RingPedan.RingPedan1; //	Sát Thương
	//}
	//else if (lpObj->Class == CLASS_FE)
	//{
	//	lpObj->PhysiDamageMinLeft += lpObj->RingPedan.RingPedan1;	 //	Sát Thương
	//	lpObj->PhysiDamageMinRight += lpObj->RingPedan.RingPedan1; //	Sát Thương
	//	lpObj->PhysiDamageMaxLeft += lpObj->RingPedan.RingPedan1;	 //	Sát Thương
	//	lpObj->PhysiDamageMaxRight += lpObj->RingPedan.RingPedan1; //	Sát Thương

	//	lpObj->MagicDamageMax += lpObj->RingPedan.RingPedan1;		 //	 Sát thương phép thuật
	//	lpObj->CurseDamageMax += lpObj->RingPedan.RingPedan1;		 //	 Sát thương phép thuật
	//}
	//else if (lpObj->Class == CLASS_MG)
	//{
	//	lpObj->PhysiDamageMinLeft += lpObj->RingPedan.RingPedan1;	 //	Sát Thương
	//	lpObj->PhysiDamageMinRight += lpObj->RingPedan.RingPedan1; //	Sát Thương
	//	lpObj->PhysiDamageMaxLeft += lpObj->RingPedan.RingPedan1;	 //	Sát Thương
	//	lpObj->PhysiDamageMaxRight += lpObj->RingPedan.RingPedan1; //	Sát Thương

	//	// Fix ST Danh Hieu
	//	if (Right->m_Index >= GET_ITEM(5, 0))
	//	{
	//		lpObj->MagicDamageMax += lpObj->RingPedan.RingPedan1;		 //	 Sát thương phép thuật
	//		lpObj->CurseDamageMax += lpObj->RingPedan.RingPedan1;		 //	 Sát thương phép thuật
	//	}
	//}
	//else if (lpObj->Class == CLASS_DL)
	//{
	//	lpObj->PhysiDamageMinLeft += lpObj->RingPedan.RingPedan1;	 //	Sát Thương
	//	lpObj->PhysiDamageMinRight += lpObj->RingPedan.RingPedan1; //	Sát Thương
	//	lpObj->PhysiDamageMaxLeft += lpObj->RingPedan.RingPedan1;	 //	Sát Thương
	//	lpObj->PhysiDamageMaxRight += lpObj->RingPedan.RingPedan1; //	Sát Thương

	//	lpObj->MagicDamageMax += (lpObj->MagicDamageMin*lpObj->RingPedan.RingPedan1) / 100;		 //	 Sát thương phép thuật
	//	lpObj->CurseDamageMax += (lpObj->MagicDamageMax*lpObj->RingPedan.RingPedan1) / 100;		 //	 Sát thương phép thuật
	//}
	//else if (lpObj->Class == CLASS_SU)
	//{
	//	lpObj->PhysiDamageMinLeft += lpObj->RingPedan.RingPedan1;	 //	Sát Thương
	//	lpObj->PhysiDamageMinRight += lpObj->RingPedan.RingPedan1; //	Sát Thương
	//	lpObj->PhysiDamageMaxLeft += lpObj->RingPedan.RingPedan1;	 //	Sát Thương
	//	lpObj->PhysiDamageMaxRight += lpObj->RingPedan.RingPedan1; //	Sát Thương

	//	lpObj->MagicDamageMax += (lpObj->MagicDamageMin*lpObj->RingPedan.RingPedan1) / 100;		 //	 Sát thương phép thuật
	//	lpObj->CurseDamageMax += (lpObj->MagicDamageMax*lpObj->RingPedan.RingPedan1) / 100;		 //	 Sát thương phép thuật

	//}
	//else if (lpObj->Class == CLASS_RF)
	//{
	//	lpObj->PhysiDamageMinLeft += lpObj->RingPedan.RingPedan1;	 //	Sát Thương
	//	lpObj->PhysiDamageMinRight += lpObj->RingPedan.RingPedan1; //	Sát Thương
	//	lpObj->PhysiDamageMaxLeft += lpObj->RingPedan.RingPedan1;	 //	Sát Thương
	//	lpObj->PhysiDamageMaxRight += lpObj->RingPedan.RingPedan1; //	Sát Thương

	//	lpObj->MagicDamageMax += (lpObj->MagicDamageMin*lpObj->RingPedan.RingPedan1) / 100;		 //	 Sát thương phép thuật
	//	lpObj->CurseDamageMax += (lpObj->MagicDamageMax*lpObj->RingPedan.RingPedan1) / 100;		 //	 Sát thương phép thuật
	//}
	////--------------------------------------------------------------------------------
	//lpObj->AttackSuccessRate += (lpObj->AttackSuccessRate*lpObj->RingPedan.RingPedan2) / 100;    // Khả năng tấn công
	////--------------------------------------------------------------------------------
	//lpObj->Defense += lpObj->RingPedan.RingPedan3; // Sức chống đỡ
	////--------------------------------------------------------------------------------
	//lpObj->SkillDamageBonus += lpObj->RingPedan.RingPedan4; // Tấn công của tuyệt chiêu
	////--------------------------------------------------------------------------------
}
//===============================================================================================================================
#endif