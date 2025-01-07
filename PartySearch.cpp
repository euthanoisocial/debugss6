#include "stdafx.h"
#include "PartySearch.h"
#include "Util.h"
#include "Party.h"
#include "DefaultClassInfo.h"
#include "Notice.h"
#include "Message.h"
#include "ServerInfo.h"
#include "Map.h"
#include "MapManager.h"
#include "MapServerManager.h"
#include "MapItem.h"
#include "Map.h"
#include "Log.h"
#include "Guild.h"

#if(PARTYSEARCH==1)

cPartySearch gPartySearch;

cPartySearch::cPartySearch(){
	this->m_Registered.clear();
}

cPartySearch::~cPartySearch(){

}

void cPartySearch::Load(char* section, char* path){
	this->NpcID = GetPrivateProfileInt(section, "NpcID", 0, path);
	this->NpcMap = GetPrivateProfileInt(section, "NpcMap", 0, path);
	this->NpcX = GetPrivateProfileInt(section, "NpcX", 0, path);
	this->NpcY = GetPrivateProfileInt(section, "NpcY", 0, path);
	this->DelAfterRelog = GetPrivateProfileInt(section, "DelAfterRelog", 0, path);
}

void cPartySearch::PartySearchAddToList(PMSG_RECV_PARTYSEARCH_ADD* lpMsg, int aIndex){
	REGISTERED_INFO info;
	memcpy(info.Name, gObj[aIndex].Name, sizeof(info.Name));
	info.aIndex = aIndex;
	info.DarkKnight = lpMsg->DarkKnight;
	info.DarkWizard = lpMsg->DarkWizard;
	info.Elf = lpMsg->Elf;
	info.MagicGladiator = lpMsg->MagicGladiator;
	info.DarkLord = lpMsg->DarkLord;
	info.Summoner = lpMsg->Summoner;
	info.RageFighter = lpMsg->RageFighter;
	info.OnlyGuild = lpMsg->OnlyGuild;
	info.OnlyAlliance = lpMsg->OnlyAlliance;
	info.OneClass = lpMsg->OneClass;
	info.Level = lpMsg->Level;
	info.RequirePassword = lpMsg->RequirePassword;
	if(info.RequirePassword)
	{
		memcpy(info.Password,(lpMsg->Password == "")?"":lpMsg->Password,sizeof(info.Password));
		//LogAdd(LOG_RED,"CHECK PARTY [%s]",info.Password);
	}
	
	//strcpy_s(info.Password,lpMsg->Password);
	

	if (lpMsg->SystemActive == true){
		std::string nme(info.Name);
		std::transform(nme.begin(), nme.end(), nme.begin(), tolower);
		std::map<std::string, REGISTERED_INFO>::iterator it = this->m_Registered.find(nme);
		if (it == this->m_Registered.end()){
			this->m_Registered.insert(std::pair<std::string, REGISTERED_INFO>(nme, info));
			//gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(634),gObj[aIndex].Name, g_szMapName[gObj[aIndex].Map],gObj[aIndex].X,gObj[aIndex].Y);
			if (info.RequirePassword)
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"New Private Auto Party created [%s] %s %d %d",gObj[aIndex].Name, gMapManager.GetMapName(gObj[aIndex].Map),gObj[aIndex].X,gObj[aIndex].Y);
			else
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(986),gObj[aIndex].Name, gMapManager.GetMapName(gObj[aIndex].Map),gObj[aIndex].X,gObj[aIndex].Y);
		}else{
			it->second = info;
		}
		info.IsOnline = true;
	}else{
		std::string nme(info.Name);
		std::transform(nme.begin(), nme.end(), nme.begin(), tolower);
		std::map<std::string, REGISTERED_INFO>::iterator it = this->m_Registered.find(nme);
		if (it != this->m_Registered.end()){
			this->m_Registered.erase(it);
			return;
		}
	}
}

void cPartySearch::PartySearchDelFromList(char* Name){
	if (this->DelAfterRelog == false){
		return;
	}
	std::string nme(Name);
	std::transform(nme.begin(), nme.end(), nme.begin(), tolower);
	std::map<std::string, REGISTERED_INFO>::iterator it = this->m_Registered.find(nme);
	if (it != this->m_Registered.end()){
		this->m_Registered.erase(it);
	}
}

void cPartySearch::GCPartyListSend(int aIndex){

	BYTE send[4096];
	PMSG_SEND_PARTYLIST pMsg;
	pMsg.header.set(0xF3, 0xF0, sizeof(pMsg));
	int size = sizeof(pMsg);
	pMsg.Count = 0;
	PMSG_PARTYLIST info;
	for (std::map<std::string, REGISTERED_INFO>::iterator it = this->m_Registered.begin(); it != this->m_Registered.end(); it++){
		LPOBJ lpObj = gObjFind(it->second.Name);
		if (lpObj == 0){
			it->second.IsOnline = false;
		}else{
			std::string name(it->second.Name);
			std::transform(name.begin(), name.end(), name.begin(), tolower);
			std::map<std::string, REGISTERED_INFO>::iterator iter = this->m_Registered.find(name);
			iter->second.aIndex = lpObj->Index;
			iter->second.IsOnline = true;
		}
		if (it->second.IsOnline == true){
			memcpy(info.Name, it->second.Name, sizeof(info.Name));
			info.Map = lpObj->Map;
			info.X = lpObj->X;
			info.Y = lpObj->Y;
			info.Level = it->second.Level;
			info.OnlyGuild = it->second.OnlyGuild;

			info.OnlyAlliance = it->second.OnlyAlliance;

			info.RequirePassword = it->second.RequirePassword;
			//LogAdd(LOG_RED,"CHECK RequirePassword [%d]",info.RequirePassword);
			info.IsSameGuild = false;
			info.IsSameAlliance = false;

			if (info.OnlyGuild == true && (lpObj->GuildNumber == gObj[aIndex].GuildNumber)){
				info.IsSameGuild = true;
			}

			if (info.OnlyAlliance == true)
			{
				if ((lpObj->Guild != 0 && lpObj->Guild->GuildUnion != 0) && (gObj[aIndex].Guild != 0 && gObj[aIndex].Guild->GuildUnion != 0))
				{
					if (lpObj->Guild->GuildUnion == gObj[aIndex].Guild->GuildUnion)
					{
						info.IsSameAlliance = true;
					}
				}
			}

			info.Count = 1;
			if (OBJECT_RANGE(lpObj->PartyNumber) != 0){
				info.Count = gParty.GetMemberCount(lpObj->PartyNumber);
			}
			bool HaveDW = true;
			bool HaveDK = true;
			bool HaveFE = true;
			bool HaveMG = true;
			bool HaveDL = true;
			bool HaveSU = true;
			bool HaveRF = true;
			if (it->second.OneClass == true){
				if (info.Count != 1){
					for (int i = 0; i<info.Count; i++){
						LPOBJ lpTarget = &gObj[gParty.m_PartyInfo[lpObj->PartyNumber].Index[i]];
						if (lpTarget->Class == CLASS_DW){
							HaveDW = false;
						}else if (lpTarget->Class == CLASS_DK){
							HaveDK = false;
						}else if (lpTarget->Class == CLASS_FE){
							HaveFE = false;
						}else if (lpTarget->Class == CLASS_MG){
							HaveMG = false;
						}else if (lpTarget->Class == CLASS_DL){
							HaveDL = false;
						}else if (lpTarget->Class == CLASS_SU){
							HaveSU = false;
						}else if (lpTarget->Class == CLASS_RF){
							HaveRF = false;
						}
					}
				}else{
					if (lpObj->Class == CLASS_DW){
						HaveDW = false;
					}else if (lpObj->Class == CLASS_DK){
						HaveDK = false;
					}else if (lpObj->Class == CLASS_FE){
						HaveFE = false;
					}else if (lpObj->Class == CLASS_MG){
						HaveMG = false;
					}else if (lpObj->Class == CLASS_DL){
						HaveDL = false;
					}else if (lpObj->Class == CLASS_SU){
						HaveSU = false;
					}else if (lpObj->Class == CLASS_RF){
						HaveRF = false;
					}
				}
				info.DarkWizard = HaveDW;
				info.DarkKnight = HaveDK;
				info.Elf = HaveFE;
				info.MagicGladiator = HaveMG;
				info.DarkLord = HaveDL;
				info.Summoner = HaveSU;
				info.RageFighter = HaveRF;
			}else{
				info.DarkWizard = it->second.DarkWizard;
				info.DarkKnight = it->second.DarkKnight;
				info.Elf = it->second.Elf;
				info.MagicGladiator = it->second.MagicGladiator;
				info.DarkLord = it->second.DarkLord;
				info.Summoner = it->second.Summoner;
				info.RageFighter = it->second.RageFighter;
			}
			memcpy(&send[size], &info, sizeof(info));
			size += sizeof(info);
			pMsg.Count++;
		}
	}
	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, sizeof(pMsg));
	DataSend(aIndex, send, size);

	gLog.Output(LOG_GENERAL,"PARTY SEARCH LIST SEND");
}

bool cPartySearch::Dialog(LPOBJ lpObj, LPOBJ lpNpc){
	
	if (lpNpc->Class == this->NpcID
		&&	lpNpc->Map == this->NpcMap 
		&&	lpNpc->X == this->NpcX 
		&&	lpNpc->Y == this->NpcY){
		this->GCPartyListSend(lpObj->Index);
		return true;
	}
	return false;
}

void cPartySearch::RequestParty(PMSG_PARTY_REQ_REQ* lpMsg, int aIndex){
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTarget = gObjFind(lpMsg->Name);

	if(BATTLE_SURVIVOR_BSV(lpObj->Map) != 0 || BATTLE_SURVIVOR_BSV(lpTarget->Map) != 0){
		return;
	}
	
	if (lpTarget == NULL){
		return;
	}
	std::string name(lpTarget->Name);
	std::transform(name.begin(), name.end(), name.begin(), tolower);
	std::map<std::string, REGISTERED_INFO>::iterator info = this->m_Registered.find(name);

	if (info->second.RequirePassword)
	{
		if (!lpMsg->needPassword)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0, 0, 0, 0, 0, "Lien he Administrator de bao loi ban dang gap phai xin cam on");
			return;
		}
	
		if (strcmp(info->second.Password,lpMsg->Password) != 0)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0, 0, 0, 0, 0, "Ban nhap sai mat khau vui long nhat lai cho dung|hoac hoi lai LeadTeam");
			return;
		}
	}

	if (lpObj->PartyNumber >= 0){
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(610));
		return;
	}
	if (!strcmp(lpObj->Name, lpTarget->Name)){
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(611));
		return;
	}
	if (lpTarget->PartyNumber >= 0){
		if (lpObj->PartyNumber == lpTarget->PartyNumber){
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(612));
			return;
		}
	}
	if (info->second.OnlyGuild == true && (lpObj->GuildNumber != lpTarget->GuildNumber)){
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(613));
		return;
	}
	if (gParty.Create(lpTarget->Index) == 0){
		if (gParty.AddMember(lpTarget->PartyNumber, lpObj->Index) == 0){
			gParty.GCPartyResultSend(lpObj->Index, 2);
			gParty.GCPartyResultSend(lpTarget->Index, 2);
		}
	}else{
		if (gParty.AddMember(lpTarget->PartyNumber, lpObj->Index) == 0){
			gParty.GCPartyResultSend(lpObj->Index, 2);
			gParty.GCPartyResultSend(lpTarget->Index, 2);
			gParty.Destroy(lpTarget->PartyNumber);
		}
	}
	gLog.Output(LOG_GENERAL,"PARTY SEARCH ADD TEAM");
}

void cPartySearch::SendPartySettings(LPOBJ lpObj){
	std::string nme(lpObj->Name);
	std::transform(nme.begin(), nme.end(), nme.begin(), tolower);
	std::map<std::string, REGISTERED_INFO>::iterator it = this->m_Registered.find(nme);
	if (it == this->m_Registered.end()){
		return;
	}
	PMSG_PARTYSETTINGS_SEND pMsg;
	pMsg.header.set(0xF3, 0xF5, sizeof(pMsg));
	pMsg.SystemActive = true;
	pMsg.OnlyGuild = it->second.OnlyGuild;
	pMsg.OneClass = it->second.OneClass;
	pMsg.Level = it->second.Level;
	pMsg.DarkWizard = it->second.DarkWizard;
	pMsg.DarkKnight = it->second.DarkKnight;
	pMsg.Elf = it->second.Elf;
	pMsg.MagicGladiator = it->second.MagicGladiator;
	pMsg.DarkLord = it->second.DarkLord;
	pMsg.Summoner = it->second.Summoner;
	pMsg.RageFighter = it->second.RageFighter;

	//pMsg.RequiredPassword = it->second.RequirePassword;
	//memcpy(pMsg.Password,it->second.Password,sizeof(pMsg.Password));


	pMsg.RequiredPassword = it->second.RequirePassword;
	if(pMsg.RequiredPassword){
		memcpy(pMsg.Password,(it->second.Password == "")?"":it->second.Password,sizeof(pMsg.Password));
		//LogAdd(LOG_RED,"CHECK RequiredPassword [%d]",it->second.RequirePassword);
		//LogAdd(LOG_RED,"pMsg.Password [%s]",pMsg.Password);
	}
	
	DataSend(lpObj->Index, (LPBYTE)&pMsg, sizeof(pMsg));
}
#endif