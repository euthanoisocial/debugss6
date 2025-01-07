#include "user.h"
#include "Protocol.h"
#pragma once
#if(MOVE_EVENT_NEW)
// -------------------------------------------------------------------------------
struct MOVE_EVENT_REQ
{
	PSBMSG_HEAD h;
	int			Number;
};
// -------------------------------------------------------------------------------
class MOVE_EVENT
{
public:
	// ----
	MOVE_EVENT();
	virtual ~MOVE_EVENT();
	void		MOVE_EVENT_GAME(LPOBJ lpObj, MOVE_EVENT_REQ *aRecv);
	int MoveBossVang;
	// ----
}; extern MOVE_EVENT gMOVE_EVENT;

#endif
// -------------------------------------------------------------------------------