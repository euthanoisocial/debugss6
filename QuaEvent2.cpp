#include "stdafx.h"
#include "Util.h"
#include "User.h"
#include "QuaEvent2.h"
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

CQuaTop3 gQuaTop3;

CQuaTop3::CQuaTop3() // OK
{
	this->Init();
}

CQuaTop3::~CQuaTop3() // OK
{

}

void CQuaTop3::Init() // OK
{
	for (int n = 0; n < MAX_CLASS; n++)
	{
		this->m_QuaTop3Info[n].Class1 = -1;
		
		this->m_QuaTop3Info[n].QUATOP3.clear(); // ok
	
	}
}

void CQuaTop3::Load(char* path) // OK
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
					CUSTOM_QUA_TOP3_INFO Items;

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

					this->m_QuaTop3Info[Class1].QUATOP3.push_back(Items);
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
void CQuaTop3::GDSaveData(int aIndex)
{
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	QUATOP3_GD_SAVE_DATA pRequest;
	pRequest.header.set(0xB0, 0x20, sizeof(pRequest));
	memcpy(pRequest.Name, lpUser->Name, 11);
	pRequest.index = aIndex;
	//--
	pRequest.QUATOP3 = lpUser->QUATOPEVENT3.QUATOP3;

	//--
	gDataServerConnection.DataSend((BYTE*)&pRequest, pRequest.header.size);
}
// -------------------------------------------------------------------------------
void CQuaTop3::GDReqData(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	QUATOP3_GD_REQ_DATA pMsg;
	pMsg.header.set(0xB0, 0x19, sizeof(pMsg));
	pMsg.index = aIndex;
	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));
	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}
// -------------------------------------------------------------------------------
void CQuaTop3::GDSaveUserInfo(int aIndex)	//-> Complete
{
	this->GDSaveData(aIndex);
}
// -------------------------------------------------------------------------------
void CQuaTop3::GDReqUserInfo(int aIndex)	//-> Complete
{
	// ----
	this->GDReqData(aIndex);
}
// -------------------------------------------------------------------------------
void CQuaTop3::DGGetData(QUATOP3_DG_GET_DATA * aRecv)
{
	// ----
	LPOBJ lpUser = &gObj[aRecv->index];
	// ----
	if (aRecv->QUATOP3 < 0)
	{
		aRecv->QUATOP3 = 0;
	}
	// ----
	lpUser->QUATOPEVENT3.QUATOP3 = aRecv->QUATOP3;
}
// -------------------------------------------------------------------------------
//===============================================================================================================================

struct QUATOP3_UPDATE  // ok
{
	PBMSG_HEAD3 h;
	int QuaTop3Check1;
	//---
	int	QUATOP3COIN;
	int	QUATOP3;

};
void QuaTop3DataSend(int aIndex) // ok
{
	QUATOP3_UPDATE rSend;
	LPOBJ lpObj = &gObj[aIndex];
	rSend.h.set((LPBYTE)&rSend, 0xF3, 107, sizeof(rSend));
	rSend.QuaTop3Check1 = gQuaTop3.QuaTop3Check1;
	//---
	rSend.QUATOP3COIN = lpObj->QUATOP3COIN;
	rSend.QUATOP3 = lpObj->QUATOPEVENT3.QUATOP3;

	DataSend(aIndex, (LPBYTE)&rSend, sizeof(rSend));
}

void CQuaTop3::Load()
{
	this->QuaTop3Check1 = GetPrivateProfileInt("Config", "QuaTop3Check1", 0, gPath.GetFullPath("Custom\\QuaEvent\\QuaTop3SendClient.ini"));

}

void CQuaTop3::QuaTop3(LPOBJ lpObj)  // ok
{
	int SetQuaTop3 = 0;
	if (lpObj->QUATOP3COIN >= this->QuaTop3Check1)
	{
		if (lpObj->QUATOPEVENT3.QUATOP3 == 1)
		{	
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn đã nhận quà không thể nhận lại!");
			return;
		}
		if (gItemManager.CheckItemInventorySpace(lpObj, 4, 4) == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Hòm đồ đã đầy !");
			return;
		}
		QUATOP3_INFO* lpInfo = &this->m_QuaTop3Info[lpObj->Class];
		for (std::vector<CUSTOM_QUA_TOP3_INFO>::iterator it = lpInfo->QUATOP3.begin(); it != lpInfo->QUATOP3.end(); it++)
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
			SetQuaTop3++;
			GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, it->ItemIndex, it->ItemLevel, 0, it->ItemSkill, it->ItemLuck, it->ItemOption, -1, it->ItemExcellent, it->ItemAncient, it->ItemJOH, it->ItemOpEx, ItemSocketOption, 0xFF, 0);
		}
		if (SetQuaTop3 != 0)
		{
			lpObj->QUATOPEVENT3.QUATOP3 = 1;
			this->GDSaveUserInfo(lpObj->Index);
		}
	}
}
#endif