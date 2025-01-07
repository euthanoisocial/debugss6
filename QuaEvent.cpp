#include "stdafx.h"
#include "Util.h"
#include "User.h"
#include "QuaEvent.h"
#include "CashShop.h"
#include "MemScript.h"
#include "Notice.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "ServerInfo.h"
#include "SocketItemType.h"
#include "Path.h"
#include "GameMain.h"

#if(QUAEVENT == 1)

CQuaTop1 gQuaTop1;

CQuaTop1::CQuaTop1() // OK
{
	this->Init();
}

CQuaTop1::~CQuaTop1() // OK
{

}

void CQuaTop1::Init() // OK
{
	for (int n = 0; n < MAX_CLASS; n++)
	{
		this->m_QuaTop1Info[n].Class1 = -1;
		
		this->m_QuaTop1Info[n].QUATOP1.clear(); // ok
	
	}
}

void CQuaTop1::Load(char* path) // OK
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
					CUSTOM_QUA_TOP1_INFO Items;

					int Class1 = lpMemScript->GetNumber();

					Items.ItemIndex = lpMemScript->GetAsNumber();

					Items.ItemLevel = lpMemScript->GetAsNumber();

					Items.ItemLuck = lpMemScript->GetAsNumber();

					Items.ItemDurability = lpMemScript->GetAsNumber();

					Items.ItemSkill = lpMemScript->GetAsNumber();

					Items.ItemOption = lpMemScript->GetAsNumber();

					Items.ItemExcellent = lpMemScript->GetAsNumber();

					Items.ItemAncient = lpMemScript->GetAsNumber();

					Items.ItemJOH = lpMemScript->GetAsNumber();

					Items.ItemOpEx = lpMemScript->GetAsNumber();

					Items.ItemSocket1 = lpMemScript->GetAsNumber();

					Items.ItemSocket2 = lpMemScript->GetAsNumber();

					Items.ItemSocket3 = lpMemScript->GetAsNumber();

					Items.ItemSocket4 = lpMemScript->GetAsNumber();

					Items.ItemSocket5 = lpMemScript->GetAsNumber();

					this->m_QuaTop1Info[Class1].QUATOP1.push_back(Items);
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


///===============================================================
void CQuaTop1::GDSaveData(int aIndex)
{
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	QUATOP1_GD_SAVE_DATA pRequest;
	pRequest.header.set(0xB0, 0x16, sizeof(pRequest));
	memcpy(pRequest.Name, lpUser->Name, 11);
	pRequest.index = aIndex;
	//--
	pRequest.QUATOP1 = lpUser->QUATOPEVENT1.QUATOP1;

	//--
	gDataServerConnection.DataSend((BYTE*)&pRequest, pRequest.header.size);
}
// -------------------------------------------------------------------------------
void CQuaTop1::GDReqData(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	QUATOP1_GD_REQ_DATA pMsg;
	pMsg.header.set(0xB0, 0x15, sizeof(pMsg));
	pMsg.index = aIndex;
	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));
	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}
// -------------------------------------------------------------------------------
void CQuaTop1::GDSaveUserInfo(int aIndex)	//-> Complete
{
	this->GDSaveData(aIndex);
}
// -------------------------------------------------------------------------------
void CQuaTop1::GDReqUserInfo(int aIndex)	//-> Complete
{
	// ----
	this->GDReqData(aIndex);
}
// -------------------------------------------------------------------------------
void CQuaTop1::DGGetData(QUATOP1_DG_GET_DATA * aRecv)
{
	// ----
	LPOBJ lpUser = &gObj[aRecv->index];
	// ----
	if (aRecv->QUATOP1 < 0)
	{
		aRecv->QUATOP1 = 0;
	}
	// ----
	lpUser->QUATOPEVENT1.QUATOP1 = aRecv->QUATOP1;
}
// -------------------------------------------------------------------------------
//===============================================================================================================================

struct QUATOP1_UPDATE  // ok
{
	PBMSG_HEAD3 h;
	int QuaTop1Check1;
	//---
	int	QUATOP1COIN;
	int	QUATOP1;

};
void QuaTop1DataSend(int aIndex) // ok
{
	QUATOP1_UPDATE rSend;
	LPOBJ lpObj = &gObj[aIndex];
	rSend.h.set((LPBYTE)&rSend, 0xF3, 105, sizeof(rSend));
	rSend.QuaTop1Check1 = gQuaTop1.QuaTop1Check1;
	//---
	rSend.QUATOP1COIN = lpObj->QUATOP1COIN;
	rSend.QUATOP1 = lpObj->QUATOPEVENT1.QUATOP1;

	DataSend(aIndex, (LPBYTE)&rSend, sizeof(rSend));
}

void CQuaTop1::Load()
{
	this->QuaTop1Check1 = GetPrivateProfileInt("Config", "QuaTop1Check1", 0, gPath.GetFullPath("Custom\\QuaEvent\\QuaTop1SendClient.ini"));

}

void CQuaTop1::QuaTop1(LPOBJ lpObj)  // ok
{
	int SetQuaTop1 = 0;
	if (lpObj->QUATOP1COIN >= this->QuaTop1Check1)
	{
		if (lpObj->QUATOPEVENT1.QUATOP1 == 1)
		{	
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn đã nhận quà không thể nhận lại!");
			return;
		}
		if (gItemManager.CheckItemInventorySpace(lpObj, 4, 4) == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Hòm đồ đã đầy !");
			return;
		}
		QUATOP1_INFO* lpInfo = &this->m_QuaTop1Info[lpObj->Class];
		for (std::vector<CUSTOM_QUA_TOP1_INFO>::iterator it = lpInfo->QUATOP1.begin(); it != lpInfo->QUATOP1.end(); it++)
		{
			BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
			if (gSocketItemType.CheckSocketItemType(it->ItemIndex) == 1)
			{
				int qtd = gSocketItemType.GetSocketItemMaxSocket(it->ItemIndex);
				ItemSocketOption[0] = (BYTE)((qtd > 0) ? ((it->ItemSocket1 != 255) ? it->ItemSocket1 : 255) : 255);
				ItemSocketOption[1] = (BYTE)((qtd > 1) ? ((it->ItemSocket2 != 255) ? it->ItemSocket2 : 255) : 255);
				ItemSocketOption[2] = (BYTE)((qtd > 2) ? ((it->ItemSocket3 != 255) ? it->ItemSocket3 : 255) : 255);
				ItemSocketOption[3] = (BYTE)((qtd > 3) ? ((it->ItemSocket4 != 255) ? it->ItemSocket4 : 255) : 255);
				ItemSocketOption[4] = (BYTE)((qtd > 4) ? ((it->ItemSocket5 != 255) ? it->ItemSocket5 : 255) : 255);
			}
			SetQuaTop1++;
			GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, it->ItemIndex, it->ItemLevel, 0, it->ItemSkill, it->ItemLuck, it->ItemOption, -1, it->ItemExcellent, it->ItemAncient, it->ItemJOH, it->ItemOpEx, ItemSocketOption, 0xFF, 0);
		}
		if (SetQuaTop1 != 0)
		{
			lpObj->QUATOPEVENT1.QUATOP1 = 1;
			this->GDSaveUserInfo(lpObj->Index);
		}
	}
}
#endif