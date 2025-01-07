#include "StdAfx.h"
#include "HuyHieu.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "Util.h"

HuyHieu gHuyHieu;

HuyHieu::HuyHieu(void)
{
}


HuyHieu::~HuyHieu(void)
{
}

void HuyHieu::Init()
{
	/*this->m_HuyHieu.Index = 0;
	this->m_HuyHieu.Enabled = 0;
	this->m_HuyHieu.dong1	= 0;
	this->m_HuyHieu.dong2   = 0;
	this->m_HuyHieu.dong3	= 0;
	this->m_HuyHieu.dong4	= 0;
	this->m_HuyHieu.dong5	= 0;*/

	m_HuyHieuInfo.clear();

}

void HuyHieu::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}
	this->m_HuyHieuInfo.clear();
	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}
		
			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}
					
					HUYHIEU m_HuyHieu;

				     m_HuyHieu.Index = lpMemScript->GetNumber();

			         m_HuyHieu.Enabled = lpMemScript->GetAsNumber();
	               
			         m_HuyHieu.dong1	  = lpMemScript->GetAsNumber();
			         
			         m_HuyHieu.dong2   = lpMemScript->GetAsNumber();
			         
			         m_HuyHieu.dong3   = lpMemScript->GetAsNumber();
			         
			         m_HuyHieu.dong4	  = lpMemScript->GetAsNumber();
			         
			         m_HuyHieu.dong5	  = lpMemScript->GetAsNumber();
					
					 LogAdd(LOG_BLUE,"%d/%d/%d/%d/%d/%d/%d",m_HuyHieu.Index, m_HuyHieu.Enabled,m_HuyHieu.dong1,m_HuyHieu.dong2,m_HuyHieu.dong3,m_HuyHieu.dong4,m_HuyHieu.dong5 );

					this->m_HuyHieuInfo.push_back(m_HuyHieu);
				}	
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void HuyHieu::Option(LPOBJ lpObj, bool flag)
{

	//if (!this->m_HuyHieu.Enabled) return;

	if (flag != 0)
	{
		return;
	}

	int huyhieu = 0;


	if(this->CountItem(lpObj,11) >= 5)
	{
		huyhieu = 1;
	}

	if(this->CountItem(lpObj,12) >= 5)
	{
		huyhieu = 2;
	}
	
	if(this->CountItem(lpObj,13) >= 5)
	{
		huyhieu = 3;
	}

	if(this->CountItem(lpObj,14) >= 5)
	{
		huyhieu = 4;
	}

	if(this->CountItem(lpObj,15) >= 5)
	{
		huyhieu = 5;
	}

	PMSG_HUYHIEU_REQ pRequest;
	pRequest.Head.set(0xFB,0xE8, sizeof(pRequest));
	pRequest.huyhieu = huyhieu;

	for(std::vector<HUYHIEU>::iterator it=this->m_HuyHieuInfo.begin();it != this->m_HuyHieuInfo.end();it++)
	{
		if(it->Index == huyhieu)
		{
			pRequest.dong1 = it->dong1;
			pRequest.dong2 = it->dong2;
			pRequest.dong3 = it->dong3;
			pRequest.dong4 = it->dong4;
			pRequest.dong5 = it->dong5;

			switch(huyhieu)
			{
			case 1:
				lpObj->IgnoreDefenseRate		+= it->dong5;
				lpObj->DoubleDamageRate			+= it->dong4;
				lpObj->PhysiDamageMinLeft		+= it->dong3;
				lpObj->PhysiDamageMinRight		+= it->dong3;
				lpObj->PhysiDamageMaxLeft		+= it->dong3;
				lpObj->PhysiDamageMaxRight		+= it->dong3;
				lpObj->Defense					+= it->dong2;
				lpObj->AddLife					+= it->dong1;
				break;
			case 2:
				lpObj->IgnoreDefenseRate		+= it->dong5;
				lpObj->DoubleDamageRate			+= it->dong4;
				lpObj->PhysiDamageMinLeft		+= it->dong3;
				lpObj->PhysiDamageMinRight		+= it->dong3;
				lpObj->PhysiDamageMaxLeft		+= it->dong3;
				lpObj->PhysiDamageMaxRight		+= it->dong3;
				lpObj->Defense					+= it->dong2;
				lpObj->AddLife					+= it->dong1;
				break;
			case 3:
				lpObj->IgnoreDefenseRate		+= it->dong5;
				lpObj->DoubleDamageRate			+= it->dong4;
				lpObj->PhysiDamageMinLeft		+= it->dong3;
				lpObj->PhysiDamageMinRight		+= it->dong3;
				lpObj->PhysiDamageMaxLeft		+= it->dong3;
				lpObj->PhysiDamageMaxRight		+= it->dong3;
				lpObj->Defense					+= it->dong2;
				lpObj->AddLife					+= it->dong1;
				break;
			case 4:
				lpObj->IgnoreDefenseRate		+= it->dong5;
				lpObj->DoubleDamageRate			+= it->dong4;
				lpObj->PhysiDamageMinLeft		+= it->dong3;
				lpObj->PhysiDamageMinRight		+= it->dong3;
				lpObj->PhysiDamageMaxLeft		+= it->dong3;
				lpObj->PhysiDamageMaxRight		+= it->dong3;
				lpObj->Defense					+= it->dong2;
				lpObj->AddLife					+= it->dong1;
				break;
			case 5:
				lpObj->IgnoreDefenseRate		+= it->dong5;
				lpObj->DoubleDamageRate			+= it->dong4;
				lpObj->PhysiDamageMinLeft		+= it->dong3;
				lpObj->PhysiDamageMinRight		+= it->dong3;
				lpObj->PhysiDamageMaxLeft		+= it->dong3;
				lpObj->PhysiDamageMaxRight		+= it->dong3;
				lpObj->Defense					+= it->dong2;
				lpObj->AddLife					+= it->dong1;
				break;

			default:
				break;
			}
		}
	}

	if(huyhieu == 0)
	{
		for(std::vector<HUYHIEU>::iterator it=this->m_HuyHieuInfo.begin();it != this->m_HuyHieuInfo.end();it++)
		{
			if(it->Index == 1)
			{
				pRequest.dong1 = it->dong1;
				pRequest.dong2 = it->dong2;
				pRequest.dong3 = it->dong3;
				pRequest.dong4 = it->dong4;
				pRequest.dong5 = it->dong5;
			}
		}
	}

	DataSend(lpObj->Index, (LPBYTE)&pRequest, sizeof(pRequest));

	
}

int HuyHieu::CountItem(LPOBJ lpObj, int level)
{
	int count = 0;

	CItem* VK1 = &lpObj->Inventory[0];
	CItem* VK2 = &lpObj->Inventory[1];
	CItem* Helm = &lpObj->Inventory[2];
	CItem* Armor = &lpObj->Inventory[3];
	CItem* Pants = &lpObj->Inventory[4];
	CItem* Gloves = &lpObj->Inventory[5];
	CItem* Boots = &lpObj->Inventory[6];

	if(lpObj->DBClass == 48 || lpObj->DBClass == 49 || lpObj->DBClass == 50)
	{
		if((VK1->m_Level >= level && VK1->m_NewOption) || (VK2->m_Level >= level && VK2->m_NewOption))
		{
			count+=2;
		}
	}
	else
	{
		if(VK1->m_Level >= level && VK1->m_NewOption)
		{
			count++;;
		}

		if(VK2->m_Level >= level && VK2->m_NewOption)
		{
			count++;;
		}

	}

	if(Helm->m_Level >= level && Helm->m_NewOption)
	{
			count++;;
	}
	if(Armor->m_Level >= level && Armor->m_NewOption)
	{
			count++;;
	}
	if(Pants->m_Level >= level && Pants->m_NewOption)
	{
			count++;;
	}
	if(Gloves->m_Level >= level && Gloves->m_NewOption)
	{
			count++;;
	}
	if(Boots->m_Level >= level && Boots->m_NewOption)
	{
			count++;;
	}

	return count;
}