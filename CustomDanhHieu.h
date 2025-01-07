#include "user.h"
#include "Protocol.h"
#pragma once
#if(DANH_HIEU_NEW)
// -------------------------------------------------------------------------------
struct DANH_HIEU_OPTION_DATA
{
	int			Index;

	int			DANH_HIEU_SAT_THUONG;
	int			DANH_HIEU_SAT_THUONG_X2;
	int			DANH_HIEU_PHONG_THU;
	int			DANH_HIEU_THE_LUC;
	int			DANH_HIEU_SD;
};
struct BUY_DANH_HIEU_REQ
{
	PSBMSG_HEAD h;
	int			Number;
};
// -------------------------------------------------------------------------------
class DanhHieu
{
public:
		// ----
	void		Init();
	// ----
	DanhHieu();
	virtual ~DanhHieu();
	// ----
	void		BUY_DANH_HIEU(LPOBJ lpObj, BUY_DANH_HIEU_REQ *aRecv);

	void	    Load();
	// ----
	void		Load(char* path);
	void		SET_DANH_HIEU_INFO(DANH_HIEU_OPTION_DATA info);
	void		DANH_HIEU_OPTION(LPOBJ lpObj, bool flag);


	int WCOIC_01;
	int WCOIC_02;
	int WCOIC_03;
	int WCOIC_04;
	int WCOIC_05;
	int WCOIC_06;
	int WCOIC_07;
	int WCOIC_08;
	int WCOIC_09;
	int WCOIC_10;
	int WCOIC_11;
	int WCOIC_12;
	int WCOIC_13;
	int WCOIC_14;
	int WCOIC_15;
	int WCOIC_16;
	int WCOIC_17;
	int WCOIC_18;
	int WCOIC_19;
	int WCOIC_20;



	// ----
	//DANH_HIEU_OPTION_DATA M_OPTION_DANH_HIEU[20];
	DANH_HIEU_OPTION_DATA M_OPTION_DANH_HIEU[220];
	// ----
}; extern DanhHieu gDanhHieu;

#endif
// -------------------------------------------------------------------------------