// MocNap.h: interface for the MocNap class.
//
//////////////////////////////////////////////////////////////////////
#include "Protocol.h"
#include "user.h"

#pragma once

#define MAX_CLASS 7

#if(QUAEVENT == 1)

struct QUATOP1_GD_SAVE_DATA
{
	PSBMSG_HEAD header;
	// ----
	WORD	index;
	char	Name[11];
	// ----
	int		QUATOP1;
	// ----
};

// -------------------------------------------------------------------------------
struct QUATOP1_GD_REQ_DATA
{
	PSBMSG_HEAD	header;
	WORD index;
	char name[11];
};
// -------------------------------------------------------------------------------

struct QUATOP1_DG_GET_DATA
{
	PSBMSG_HEAD	header;
	// ----
	WORD	index;
	// ----
	int		QUATOP1;
	// ----
};

struct CUSTOM_QUA_TOP1_INFO
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

struct QUATOP1_INFO
{
	int Class1;


	std::vector<CUSTOM_QUA_TOP1_INFO> QUATOP1; // ok

};



void QuaTop1DataSend(int aIndex);

class CQuaTop1
{

public:
	CQuaTop1();
	virtual ~CQuaTop1();
	void Init();
	void Load();

	void Load(char* path);

	void SetInfo(QUATOP1_INFO info);

	void QuaTop1(LPOBJ lpObj);

	int QuaTop1Check1;


	void		GDSaveData(int aIndex);
	void		GDReqData(int aIndex);
	void		GDSaveUserInfo(int aIndex);
	void		GDReqUserInfo(int aIndex);
	void		DGGetData(QUATOP1_DG_GET_DATA * aRecv);

public:

	QUATOP1_INFO m_QuaTop1Info[MAX_CLASS];
};

extern CQuaTop1 gQuaTop1;

#endif