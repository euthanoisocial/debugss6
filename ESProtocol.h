#pragma once

#include "Protocol.h"
#include "User.h"

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_GUILDCREATE_RESULT
{
	PBMSG_HEAD h; // C1:30
	BYTE Result;
	BYTE Flag;
	DWORD GuildNumber;
	BYTE NumberH;
	BYTE NumberL;
	char Master[11];
	char GuildName[9];
	BYTE Mark[32];
	BYTE btGuildType;
};

struct SDHP_GUILDDESTROY_RESULT
{
	PBMSG_HEAD h; // C1:31
	BYTE Result;
	BYTE Flag;
	BYTE NumberH;
	BYTE NumberL;
	char GuildName[9];
	char Master[11];
};

struct SDHP_GUILDMEMBERADD_RESULT
{
	PBMSG_HEAD h; // C1:32
	BYTE Result;
	BYTE Flag;
	BYTE NumberH;
	BYTE NumberL;
	char GuildName[9];
	char MemberID[11];
	short pServer;
};

struct SDHP_GUILDMEMBERDEL_RESULT
{
	PBMSG_HEAD h; // C1:33
	BYTE Result;
	BYTE Flag;
	BYTE NumberH;
	BYTE NumberL;
	char GuildName[9];
	char MemberID[11];
};

struct SDHP_GUILDMEMBER_INFO
{
	PBMSG_HEAD h; // C1:35
	char GuildName[9];
	char MemberID[11];
	BYTE btGuildStatus;
	BYTE btGuildType;
	short pServer;
};

struct SDHP_GUILDALL_COUNT
{
	PWMSG_HEAD h; // C2:36
	int Number;
	char GuildName[9];
	char Master[11];
	BYTE Mark[32];
	int score;
	BYTE btGuildType;
	int iGuildUnion;
	int iGuildRival;
	char szGuildRivalName[9];
	BYTE Count;
};

struct SDHP_GUILDALL
{
	char MemberID[11];
	BYTE btGuildStatus;
	short pServer;
};

struct EXSDHP_SERVERGROUP_GUILD_CHATTING_RECV
{
	PBMSG_HEAD h; // C1:50
	int iGuildNum;
	char szCharacterName[11];
	char szChattingMsg[60];
};

struct EXSDHP_SERVERGROUP_UNION_CHATTING_RECV
{
	PBMSG_HEAD h; // C1:51
	int iUnionNum;
	char szCharacterName[11];
	char szChattingMsg[60];
};

struct EXSDHP_GUILD_ASSIGN_STATUS_RESULT
{
	PBMSG_HEAD h; // C1:E1
	BYTE btFlag;
	WORD wUserIndex;
	BYTE btType;
	BYTE btResult;
	BYTE btGuildStatus;
	char szGuildName[9];
	char szTargetName[11];
};

struct EXSDHP_GUILD_ASSIGN_TYPE_RESULT
{
	PBMSG_HEAD h; // C1:E2
	BYTE btFlag;
	WORD wUserIndex;
	BYTE btGuildType;
	BYTE btResult;
	char szGuildName[9];
};

struct EXSDHP_RELATIONSHIP_JOIN_RESULT
{
	PBMSG_HEAD h; // C1:E5
	BYTE btFlag;
	WORD wRequestUserIndex;
	WORD wTargetUserIndex;
	BYTE btResult;
	BYTE btRelationShipType;
	int iRequestGuildNum;
	int iTargetGuildNum;
	char szRequestGuildName[9];
	char szTargetGuildName[9];
};

struct EXSDHP_RELATIONSHIP_BREAKOFF_RESULT
{
	PBMSG_HEAD h; // C1:E6
	BYTE btFlag;
	WORD wRequestUserIndex;
	WORD wTargetUserIndex;
	BYTE btResult;
	BYTE btRelationShipType;
	int iRequestGuildNum;
	int iTargetGuildNum;
};

struct EXSDHP_UNION_RELATIONSHIP_LIST
{
	PWMSG_HEAD h; // C1:E7
	BYTE btFlag;
	BYTE btRelationShipType;
	BYTE btRelationShipMemberCount;
	char szUnionMasterGuildName[9];
	int iUnionMasterGuildNumber;
	int iRelationShipMember[100];
};

struct EXSDHP_NOTIFICATION_RELATIONSHIP
{
	PWMSG_HEAD h; // C1:E8
	BYTE btFlag;
	BYTE btUpdateFlag;
	BYTE btGuildListCount;
	int iGuildList[100];
};

struct EXSDHP_UNION_LIST_COUNT
{
	PWMSG_HEAD h; // C1:E9
	BYTE btCount;
	BYTE btResult;
	WORD wRequestUserIndex;
	int iTimeStamp;
	BYTE btRivalMemberNum;
	BYTE btUnionMemberNum;
};

struct EXSDHP_UNION_LIST
{
	BYTE btMemberNum;
	BYTE Mark[32];
	char szGuildName[9];
};

struct EXSDHP_KICKOUT_UNIONMEMBER_RESULT
{
	PSBMSG_HEAD h; // C1:EB:01
	BYTE btFlag;
	WORD wRequestUserIndex;
	BYTE btRelationShipType;
	BYTE btResult;
	char szUnionMasterGuildName[9];
	char szUnionMemberGuildName[9];
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_USERCLOSE
{
	PBMSG_HEAD h; // C1:02 [RECV/SEND]
	char CharName[11];
	char GuildName[9];
	BYTE Type;
};

struct SDHP_GUILDCREATE
{
	PBMSG_HEAD h; // C1:30
	char GuildName[9];
	char Master[11];
	BYTE Mark[32];
	BYTE NumberH;
	BYTE NumberL;
	BYTE btGuildType;
};

struct SDHP_GUILDDESTROY
{
	PBMSG_HEAD h; // C1:31
	BYTE NumberH;
	BYTE NumberL;
	char GuildName[9];
	char Master[11];
};

struct SDHP_GUILDMEMBERADD
{
	PBMSG_HEAD h; // C1:32
	char GuildName[9];
	char MemberID[11];
	BYTE NumberH;
	BYTE NumberL;
};

struct SDHP_GUILDMEMBERDEL
{
	PBMSG_HEAD h; // C1:33
	BYTE NumberH;
	BYTE NumberL;
	char GuildName[9];
	char MemberID[11];
};

struct SDHP_GUILDMEMBER_INFO_REQUEST
{
	PBMSG_HEAD h; // C1:35
	BYTE NumberH;
	BYTE NumberL;
	char MemberID[11];
};

struct SDHP_GUILDSCOREUPDATE
{
	PBMSG_HEAD h; // C1:37 [RECV/SEND]
	char GuildName[9];
	int Score;
};

struct SDHP_GUILDNOTICE
{
	PBMSG_HEAD h; // C1:38 [RECV/SEND]
	char GuildName[9];
	char szGuildNotice[60];
};

struct EXSDHP_SERVERGROUP_GUILD_CHATTING_SEND
{
	PBMSG_HEAD h; // C1:50
	int iGuildNum;
	char szCharacterName[11];
	char szChattingMsg[60];
};

struct EXSDHP_SERVERGROUP_UNION_CHATTING_SEND
{
	PBMSG_HEAD h; // C1:51
	int iUnionNum;
	char szCharacterName[11];
	char szChattingMsg[60];
};

struct EXSDHP_GUILD_ASSIGN_STATUS_REQ
{
	PBMSG_HEAD h; // C1:E1
	WORD wUserIndex;
	BYTE btType;
	BYTE btGuildStatus;
	char szGuildName[9];
	char szTargetName[11];
};

struct EXSDHP_GUILD_ASSIGN_TYPE_REQ
{
	PBMSG_HEAD h; // C1:E2
	WORD wUserIndex;
	BYTE btGuildType;
	char szGuildName[9];
};

struct EXSDHP_RELATIONSHIP_JOIN_REQ
{
	PBMSG_HEAD h; // C1:E5
	WORD wRequestUserIndex;
	WORD wTargetUserIndex;
	BYTE btRelationShipType;
	int iRequestGuildNum;
	int iTargetGuildNum;
};

struct EXSDHP_RELATIONSHIP_BREAKOFF_REQ
{
	PBMSG_HEAD h; // C1:E6
	WORD wRequestUserIndex;
	WORD wTargetUserIndex;
	BYTE btRelationShipType;
	int iRequestGuildNum;
	int iTargetGuildNum;
};

struct EXSDHP_UNION_LIST_REQ
{
	PBMSG_HEAD h;
	WORD wRequestUserIndex;
	int iUnionMasterGuildNumber;
};

struct EXSDHP_KICKOUT_UNIONMEMBER_REQ
{
	PSBMSG_HEAD h; // C1:EB:01
	WORD wRequestUserIndex;
	BYTE btRelationShipType;
	char szUnionMasterGuildName[9];
	char szUnionMemberGuildName[9];
};

//**********************************************//
//**********************************************//
//**********************************************//

void ESDataSend(BYTE* lpMsg,int size);
void ESDataRecv(BYTE head,BYTE* lpMsg,int size);
void ExDBServerProtocolCore(BYTE head,BYTE* lpMsg,int size);
void GDCharClose(int type, LPSTR GuildName, LPSTR Name);
void GDCharCloseRecv( SDHP_USERCLOSE* lpMsg);
void GDGuildCreateSend(int aIndex, LPSTR Name, LPSTR Master, LPBYTE Mark, int iGuildType);
void GDGuildCreateRecv( SDHP_GUILDCREATE_RESULT* lpMsg);
void GDGuildDestroySend(int aIndex, LPSTR Name, LPSTR Master);
void GDGuildDestroyRecv( SDHP_GUILDDESTROY_RESULT* lpMsg);
void GDGuildMemberAdd(int aIndex, LPSTR Name, LPSTR MemberId);
void GDGuildMemberAddResult( SDHP_GUILDMEMBERADD_RESULT* lpMsg);
void GDGuildMemberDel(int aIndex, LPSTR Name, LPSTR MemberId);
void GDGuildMemberDelResult( SDHP_GUILDMEMBERDEL_RESULT* lpMsg);
void GDGuildUpdate(LPSTR Name, LPSTR Master, LPBYTE Mark,  int score, BYTE count);
void DGGuildMemberInfoRequest(int aIndex);
void DGGuildMemberInfo( SDHP_GUILDMEMBER_INFO* lpMsg);
void DGGuildMasterListRecv(LPBYTE lpData);
void DGGuildScoreUpdate(LPSTR guildname, int score);
void GDGuildScoreUpdateRecv( SDHP_GUILDSCOREUPDATE* lpMsg);
void GDGuildNoticeSave(LPSTR guild_name, LPSTR guild_notice);
void DGGuildNoticeRecv( SDHP_GUILDNOTICE* lpMsg);
void DGGuildListRequest();
void GDGuildServerGroupChattingSend(int iGuildNum, char* szCharacterName, char* szChattingMsg);
void DGGuildServerGroupChattingRecv( EXSDHP_SERVERGROUP_GUILD_CHATTING_RECV* lpMsg);
void GDUnionServerGroupChattingSend(int iUnionNum, char* szCharacterName, char* szChattingMsg);
void DGUnionServerGroupChattingRecv( EXSDHP_SERVERGROUP_UNION_CHATTING_RECV* lpMsg);
void GDGuildReqAssignStatus(int aIndex, int iAssignType, LPSTR szTagetName, int iGuildStatus);
void DGGuildAnsAssignStatus( EXSDHP_GUILD_ASSIGN_STATUS_RESULT* lpMsg);
void GDGuildReqAssignType(int aIndex, int iGuildType);
void DGGuildAnsAssignType( EXSDHP_GUILD_ASSIGN_TYPE_RESULT* lpMsg);
void GDRelationShipReqJoin(int aIndex, int iTargetUserIndex, int iRelationShipType);
void DGRelationShipAnsJoin( EXSDHP_RELATIONSHIP_JOIN_RESULT* lpMsg);
void GDUnionBreakOff(int aIndex, int iUnionNumber);
void DGUnionBreakOff( EXSDHP_RELATIONSHIP_BREAKOFF_RESULT* lpMsg);
void GDRelationShipReqBreakOff(int aIndex, int iTargetUserIndex, int iRelationShipType);
void DGRelationShipAnsBreakOff( EXSDHP_RELATIONSHIP_BREAKOFF_RESULT* lpMsg);
void DGRelationShipNotificationRecv( EXSDHP_NOTIFICATION_RELATIONSHIP* lpMsg);
void DGRelationShipListRecv( EXSDHP_UNION_RELATIONSHIP_LIST* lpMsg);
void GDUnionListSend(int iUserIndex, int iUnionMasterGuildNumber);
void DGUnionListRecv(LPBYTE aRecv);
void GDRelationShipReqKickOutUnionMember(int aIndex, LPSTR szTargetGuildName);
void DGRelationShipAnsKickOutUnionMember( EXSDHP_KICKOUT_UNIONMEMBER_RESULT* lpMsg);
