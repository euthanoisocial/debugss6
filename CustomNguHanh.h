#include "user.h"
#include "Protocol.h"
#pragma once
#if(NGU_HANH_NEW)
// -------------------------------------------------------------------------------
struct BUY_NGU_HANH_REQ
{
	PSBMSG_HEAD h;
	int			Number;
};
// -------------------------------------------------------------------------------
class NguHanh
{
public:
	// ----
	NguHanh();
	virtual ~NguHanh();
	// ----
	void		BUY_NGU_HANH(LPOBJ lpObj, BUY_NGU_HANH_REQ *aRecv);

	void		NGU_HANH_OPTION(LPOBJ lpObj, bool flag);


	//void		BUY_NGU_HANH_1(LPOBJ lpObj);
	// ----
	void	    LOAD_TXT();
	// ----
	int NGU_HANH_MAX;
	int NGU_HANH_WCOINC;

	int Kim;
	int Moc;
	int Thuy;
	int Hoa;
	int Tho;

}; extern NguHanh gNguHanh;

#endif
// -------------------------------------------------------------------------------