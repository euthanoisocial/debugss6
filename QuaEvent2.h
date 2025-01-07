// MocNap.h: interface for the MocNap class.
//
//////////////////////////////////////////////////////////////////////
#include "Protocol.h"
#include "user.h"

#pragma once

#define MAX_CLASS 7

#if(QUAEVENT == 1)

struct QUATOP3_GD_SAVE_DATA
{
	PSBMSG_HEAD header;
	// ----
	WORD	index;
	char	Name[11];
	// ----
	int		QUATOP3;
	// ----
};

// -------------------------------------------------------------------------------
struct QUATOP3_GD_REQ_DATA
{
	PSBMSG_HEAD	header;
	WORD index;
	char name[11];
};
// -------------------------------------------------------------------------------

struct QUATOP3_DG_GET_DATA
{
	PSBMSG_HEAD	header;
	// ----
	WORD	index;
	// ----
	int		QUATOP3;
	// ----
};

struct CUSTOM_QUA_TOP3_INFO
{
	int ItemIndex;
	int ItemLevel;
	int ItemDurability;
	int ItemSkill;
	int ItemLuck;
	int ItemOption;
	int ItemExcellent;
	int ItemAncient;
	int ItemJOH;
	int ItemOpEx;
	int ItemSocket1;
	int ItemSocket2;
	int ItemSocket3;
	int ItemSocket4;
	int ItemSocket5;
};

struct QUATOP3_INFO
{
	int Class1;


	std::vector<CUSTOM_QUA_TOP3_INFO> QUATOP3; // ok

};



void QuaTop3DataSend(int aIndex);

class CQuaTop3
{

public:
	CQuaTop3();
	virtual ~CQuaTop3();
	void Init();
	void Load();

	void Load(char* path);

	void SetInfo(QUATOP3_INFO info);

	void QuaTop3(LPOBJ lpObj);

	int QuaTop3Check1;


	void		GDSaveData(int aIndex);
	void		GDReqData(int aIndex);
	void		GDSaveUserInfo(int aIndex);
	void		GDReqUserInfo(int aIndex);
	void		DGGetData(QUATOP3_DG_GET_DATA * aRecv);

public:

	QUATOP3_INFO m_QuaTop3Info[MAX_CLASS];
};

extern CQuaTop3 gQuaTop3;

#endif