#include "stdafx.h"
#include "Util.h"
#include "User.h"
#include "CustomStartItemDame.h"
#include "CashShop.h"
#include "MemScript.h"
#include "Notice.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "ServerInfo.h"
#include "SocketItemType.h"
#include "Message.h"
#include "ObjectManager.h"
#include "Item.h"
#include "ItemManager.h"

CCustomStartItemDame gCustomStartItemDame;

CCustomStartItemDame::CCustomStartItemDame() // OK
{
	//this->Init();
}

CCustomStartItemDame::~CCustomStartItemDame() // OK
{

}

void CCustomStartItemDame::Init() // OK
{
	for (int n = 0; n < MAX_CLASS; n++)
	{
		this->m_CustomStartItemDameInfo[n].Class = -1;
		this->m_CustomStartItemDameInfo[n].ItemDameStart.clear();
	}
}

void CCustomStartItemDame::LoadItemDame(char* path) // OK
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

	//this->Init();

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
					CUSTOM_START_ITEM_DAME_INFO Items;

					int Class = lpMemScript->GetNumber();

					Items.ItemType = lpMemScript->GetAsNumber();

					Items.ItemIndex = lpMemScript->GetAsNumber();

					Items.ItemLevel = lpMemScript->GetAsNumber();

					Items.ItemLuck = lpMemScript->GetAsNumber();

					Items.ItemDurability = lpMemScript->GetAsNumber();

					Items.ItemSkill = lpMemScript->GetAsNumber();

					Items.ItemOption = lpMemScript->GetAsNumber();

					Items.ItemExcellent = lpMemScript->GetAsNumber();

					//this->SetInfo(info);

					//Items.TypeBuff = lpMemScript->GetAsNumber();

					Items.EffectBuff = lpMemScript->GetAsNumber();

					//Items.DupeDame = lpMemScript->GetAsNumber();

					//Items.TripDame = lpMemScript->GetAsNumber();

					Items.DameRate = lpMemScript->GetAsNumber();

					//Items.DefDameRate = lpMemScript->GetAsNumber();

					Items.Power1Buff = lpMemScript->GetAsNumber();

					Items.Power2Buff = lpMemScript->GetAsNumber();

					Items.TimeBuff = lpMemScript->GetAsNumber();

					this->m_CustomStartItemDameInfo[Class].ItemDameStart.push_back(Items);
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


