#include "user.h"
#include "Protocol.h"
#pragma once
#if(QUAN_HAM_NEW)
// -------------------------------------------------------------------------------
struct QUAN_HAM_OPTION_DATA
{
	int			Index;

	int			QUAN_HAM_SAT_THUONG;
	int			QUAN_HAM_SAT_THUONG_X2;
	int			QUAN_HAM_PHONG_THU;
	int			QUAN_HAM_THE_LUC;
	int			QUAN_HAM_SD;
};
// -------------------------------------------------------------------------------
struct BUY_QUAN_HAM_REQ
{
	PSBMSG_HEAD h;
	int			Number;
};
// -------------------------------------------------------------------------------
class QuanHam
{
public:
	// ----
	void		Init();
	// ----
	QuanHam();
	virtual ~QuanHam();
	// ----
	void		BUY_QUAN_HAM(LPOBJ lpObj, BUY_QUAN_HAM_REQ *aRecv);
	// ----
	void	    Load();
	void		Load(char* path);
	void		SET_QUAN_HAM_INFO(QUAN_HAM_OPTION_DATA info);
	void		QUAN_HAM_OPTION(LPOBJ lpObj, bool flag);
	// ----

	int WCOIC_01;
	int WCOIC_02;
	int WCOIC_03;
	
	//QUAN_HAM_OPTION_DATA M_OPTION_QUAN_HAM[22];
	QUAN_HAM_OPTION_DATA M_OPTION_QUAN_HAM[20000];
	// ----
}; extern QuanHam gQuanHam;

#endif
// -------------------------------------------------------------------------------