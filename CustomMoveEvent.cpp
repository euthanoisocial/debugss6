#include "StdAfx.h"
#include "user.h"
#include "CustomMoveEvent.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "Path.h"
#include "MemScript.h"
#include "Util.h"
#include "CashShop.h"
#include "Message.h"
#include "Notice.h"
#include "CommandManager.h"
#include "ObjectManager.h"
#include "ServerInfo.h"

#if(MOVE_EVENT_NEW)

MOVE_EVENT gMOVE_EVENT;
// -------------------------------------------------------------------------------
MOVE_EVENT::MOVE_EVENT() // OK
{
}
// -------------------------------------------------------------------------------
MOVE_EVENT::~MOVE_EVENT() // OK
{
}
// -------------------------------------------------------------------------------
void MOVE_EVENT::MOVE_EVENT_GAME(LPOBJ lpObj, MOVE_EVENT_REQ *aRecv)
{
	//if ((GetLargeRand() % 100))
	//{
	//	gObjMoveGate(lpObj->Index, 800);
	//}
	//-----------------------------------------------
	if (aRecv->Number == 1)
	{
		gObjMoveGate(lpObj->Index, 800);
	}
	if (aRecv->Number == 2)
	{
		gObjMoveGate(lpObj->Index, 801);
	}
	if (aRecv->Number == 3)
	{
		gObjMoveGate(lpObj->Index, 802);
	}
	if (aRecv->Number == 4)
	{
		gObjMoveGate(lpObj->Index, 803);
	}
	if (aRecv->Number == 5)
	{
		gObjMoveGate(lpObj->Index, 804);
	}
	if (aRecv->Number == 6)
	{
		gObjMoveGate(lpObj->Index, 805);
	}
	if (aRecv->Number == 7)
	{
		gObjMoveGate(lpObj->Index, 806);
	}
	if (aRecv->Number == 8)
	{
		gObjMoveGate(lpObj->Index, 807);
	}
	if (aRecv->Number == 9)
	{
		gObjMoveGate(lpObj->Index, 808);
	}
	if (aRecv->Number == 10)
	{
		gObjMoveGate(lpObj->Index, 809);
	}
	if (aRecv->Number == 11)
	{
		gObjMoveGate(lpObj->Index, 810);
	}
	if (aRecv->Number == 12)
	{
		gObjMoveGate(lpObj->Index, 811);
	}
	if (aRecv->Number == 13)
	{
		gObjMoveGate(lpObj->Index, 812);
	}
	if (aRecv->Number == 14)
	{
		gObjMoveGate(lpObj->Index, 813);
	}
	if (aRecv->Number == 15)
	{
		gObjMoveGate(lpObj->Index, 814);
	}
	if (aRecv->Number == 16)
	{
		gObjMoveGate(lpObj->Index, 815);
	}
	if (aRecv->Number == 17)
	{
		gObjMoveGate(lpObj->Index, 816);
	}
	if (aRecv->Number == 18)
	{
		gObjMoveGate(lpObj->Index, 817);
	}
	if (aRecv->Number == 19)
	{
		gObjMoveGate(lpObj->Index, 818);
	}
	if (aRecv->Number == 20)
	{
		gObjMoveGate(lpObj->Index, 819);
	}
	if (aRecv->Number == 21)
	{
		gObjMoveGate(lpObj->Index, 820);
	}
	if (aRecv->Number == 22)
	{
		gObjMoveGate(lpObj->Index, 821);
	}
	if (aRecv->Number == 23)
	{
		gObjMoveGate(lpObj->Index, 822);
	}
	if (aRecv->Number == 24)
	{
		gObjMoveGate(lpObj->Index, 823);
	}
	if (aRecv->Number == 25)
	{
		gObjMoveGate(lpObj->Index, 824);
	}
	if (aRecv->Number == 26)
	{
		gObjMoveGate(lpObj->Index, 825);
	}

}
#endif
