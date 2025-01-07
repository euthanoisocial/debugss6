#include "user.h"
#include "Protocol.h"

#if(RINGPEDAN_NEW)

struct RINGPEDAN_GD_SAVE_DATA
{
	PSBMSG_HEAD header;
	// ----
	WORD	index;
	char	Name[11];
	// ----
	int		RINGPEDAN1;
	int		RINGPEDAN2;
	int		RINGPEDAN3;
	int		RINGPEDAN4;
	// ----
};

// -------------------------------------------------------------------------------
struct RINGPEDAN_GD_REQ_DATA
{
	PSBMSG_HEAD	header;
	WORD index;
	char name[11];
};
// -------------------------------------------------------------------------------


struct RINGPEDAN_DG_GET_DATA
{
	PSBMSG_HEAD	header;
	// ----
	WORD	index;
	// ----
	int		RINGPEDAN1;
	int		RINGPEDAN2;
	int		RINGPEDAN3;
	int		RINGPEDAN4;
	// ----
};

// -------------------------------------------------------------------------------
struct ACTIVE_RINGPEDAN_REQ
{
	PSBMSG_HEAD h;
	int			Type;
	int			Number;
};

//  ------------------------------------------------------------------------------
struct BUY_RINGPEDAN_REQ
{
	PSBMSG_HEAD h;
	int			Number;
};
// -------------------------------------------------------------------------------s
class RingPedan
{
public:
	// ----
	RingPedan();
	virtual ~RingPedan();

	void	Init();
	void	Load();

	void		GDSaveData(int aIndex);
	void		GDReqData(int aIndex);
	void		GDSaveUserInfo(int aIndex);
	void		GDReqUserInfo(int aIndex);
	void		DGGetData(RINGPEDAN_DG_GET_DATA * aRecv);

	void		BUG_RINGPEDAN(LPOBJ lpObj, BUY_RINGPEDAN_REQ *aRecv);

	void		Load_Option(LPOBJ lpObj, bool flag);

	void	    Load_Text();

	int		MAX_RINGPEDAN_01;
	int		MAX_RINGPEDAN_02;
	int		MAX_RINGPEDAN_03;
	int		MAX_RINGPEDAN_04;

	char MSG_01[1024];
	char MSG_02[1024];

	// ----
	RINGPEDAN_GD_REQ_DATA RINGPEDAN;
	// ----

private:

}; extern RingPedan gRingPedan;

#endif
// -------------------------------------------------------------------------------