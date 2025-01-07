#include "user.h"
#include "Protocol.h"
#pragma once
#if(THUE_VIP_NEW)
// -------------------------------------------------------------------------------
struct BUY_THUE_VIP_REQ
{
	PSBMSG_HEAD h;
	int			Number;
};
// -------------------------------------------------------------------------------
class ThueVip
{
public:
	// ----
	void		Init();
	// ----
	ThueVip();
	virtual ~ThueVip();
	// ----
	void		BUY_THUE_VIP(LPOBJ lpObj, BUY_THUE_VIP_REQ *aRecv);
	void		THUE_VIP_OPTION(LPOBJ lpObj, bool flag);
	// ----
	void	    LOAD_TXT();
	// ----
	int         VIP_DONG_WCOINC;
	int         VIP_BAC_WCOINC;
	int         VIP_VANG_WCOINC;
	int         VIP_KIMCUONG_WCOINC;

	int         VIP_DONG_TIME;
	int         VIP_BAC_TIME;
	int         VIP_VANG_TIME;
	int         VIP_KIMCUONG_TIME;

	int         THUE_VIP_SAT_THUONG_HOAN_HAO_DONG;
	int         THUE_VIP_SAT_THUONG_CHI_MANG_DONG;
	int         THUE_VIP_SAT_THUONG_X2_DONG;
	int         THUE_VIP_SAT_THUONG_DONG;
	int         THUE_VIP_THE_LUC_DONG;
	int         THUE_VIP_SD_DONG;

	int         THUE_VIP_SAT_THUONG_HOAN_HAO_BAC;
	int         THUE_VIP_SAT_THUONG_CHI_MANG_BAC;
	int         THUE_VIP_SAT_THUONG_X2_BAC;
	int         THUE_VIP_SAT_THUONG_BAC;
	int         THUE_VIP_THE_LUC_BAC;
	int         THUE_VIP_SD_BAC;

	int         THUE_VIP_SAT_THUONG_HOAN_HAO_VANG;
	int         THUE_VIP_SAT_THUONG_CHI_MANG_VANG;
	int         THUE_VIP_SAT_THUONG_X2_VANG;
	int         THUE_VIP_SAT_THUONG_VANG;
	int         THUE_VIP_THE_LUC_VANG;
	int         THUE_VIP_SD_VANG;

	int         THUE_VIP_SAT_THUONG_HOAN_HAO_KIMCUONG;
	int         THUE_VIP_SAT_THUONG_CHI_MANG_KIMCUONG;
	int         THUE_VIP_SAT_THUONG_X2_KIMCUONG;
	int         THUE_VIP_SAT_THUONG_KIMCUONG;
	int         THUE_VIP_THE_LUC_KIMCUONG;
	int         THUE_VIP_SD_KIMCUONG;




	// ----
}; extern ThueVip gThueVip;

#endif
// -------------------------------------------------------------------------------