// MocNap.h: interface for the MocNap class.
//
//////////////////////////////////////////////////////////////////////
#include "Protocol.h"
#include "user.h"

#pragma once

#define MAX_CLASS 7

#if(QUAEVENT == 1)

struct QUATOP2_GD_SAVE_DATA
{
	PSBMSG_HEAD header;
	// ----
	WORD	index;
	char	Name[11];
	// ----
	int		QUATOP2;
	// ----
};

// -------------------------------------------------------------------------------
struct QUATOP2_GD_REQ_DATA
{
	PSBMSG_HEAD	header;
	WORD index;
	char name[11];
};
// -------------------------------------------------------------------------------

struct QUATOP2_DG_GET_DATA
{
	PSBMSG_HEAD	header;
	// ----
	WORD	index;
	// ----
	int		QUATOP2;
	// ----
};

struct CUSTOM_QUA_TOP2_INFO
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

struct QUATOP2_INFO
{
	int Class1;


	std::vector<CUSTOM_QUA_TOP2_INFO> QUATOP2; // ok

};



void QuaTop2DataSend(int aIndex);

class CQuaTop2
{

public:
	CQuaTop2();
	virtual ~CQuaTop2();
	void Init();
	void Load();

	void Load(char* path);

	void SetInfo(QUATOP2_INFO info);

	void QuaTop2(LPOBJ lpObj);

	int QuaTop2Check1;


	void		GDSaveData(int aIndex);
	void		GDReqData(int aIndex);
	void		GDSaveUserInfo(int aIndex);
	void		GDReqUserInfo(int aIndex);
	void		DGGetData(QUATOP2_DG_GET_DATA * aRecv);

public:

	QUATOP2_INFO m_QuaTop2Info[MAX_CLASS];
};

extern CQuaTop2 gQuaTop2;

#endif