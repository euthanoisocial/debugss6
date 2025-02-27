#include "stdafx.h"
#include "HackCheck.h"
#include "Protect.h"
#include "ServerInfo.h"
#include "ThemidaSDK.h"

BYTE EncDecKey1;
BYTE EncDecKey2;
BYTE MHPEncDecKey1;
BYTE MHPEncDecKey2;
char MHPCustomerName[32];

void DecryptData(BYTE* lpMsg,int size) // OK
{
	if(MHPEncDecKey1 != 0 || MHPEncDecKey2 != 0)
	{
		MHPDecryptData(lpMsg,size);
	}

	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]^EncDecKey1)-EncDecKey2;
	}
}

void EncryptData(BYTE* lpMsg,int size) // OK
{
	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]+EncDecKey2)^EncDecKey1;
	}

	if(MHPEncDecKey1 != 0 || MHPEncDecKey2 != 0)
	{
		MHPEncryptData(lpMsg,size);
	}
}

void MHPDecryptData(BYTE* lpMsg,int size) // OK
{
	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]^MHPEncDecKey1)-MHPEncDecKey2;
	}
}

void MHPEncryptData(BYTE* lpMsg,int size) // OK
{
	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]+MHPEncDecKey2)^MHPEncDecKey1;
	}
}

void InitHackCheck() // OK
{
	VM_START

	WORD EncDecKey = 0;

	for(int n=0;n < sizeof(gServerInfo.m_CustomerName);n++)
	{
		EncDecKey += (BYTE)(gServerInfo.m_CustomerName[n]^gServerInfo.m_ServerSerial[(n%sizeof(gServerInfo.m_ServerSerial))]);
	}

#if (PROTECT_STATE == 1) // || GAMESERVER_UPDATE >= 801
	EncDecKey1 = (BYTE)0xB3;
	EncDecKey2 = (BYTE)0x8C; // Default is 0x76 [ ON THIS I MODIFIC THE 0x76 TO 0xFF ] BECAUSE THE PROBLEM DISCONNECT....
#else
	EncDecKey1 = (BYTE)0xE2;
	EncDecKey2 = (BYTE)0x77; // Default is 0x76
#endif
	
   //EncDecKey2 = gProtect.m_EncKeyNew; [ THIS NEED REMOVE - BECAUSE THIS I ADD THIS // ] BECAUSE THE PROBLEM DISCONNECT....

	EncDecKey1 += LOBYTE(EncDecKey);
	EncDecKey2 += HIBYTE(EncDecKey);

	GetPrivateProfileString("MHPServerInfo","CustomerName","",MHPCustomerName,sizeof(MHPCustomerName),"..\\Data\\Hack\\MHPServer.ini");

	MHPEncDecKey1 = GetPrivateProfileInt("MHPServerInfo","EncDecKey1",0,"..\\Data\\Hack\\MHPServer.ini");

	MHPEncDecKey2 = GetPrivateProfileInt("MHPServerInfo","EncDecKey2",0,"..\\Data\\Hack\\MHPServer.ini");

	if(MHPEncDecKey1 != 0 || MHPEncDecKey2 != 0)
	{
		WORD MHPEncDecKey = 0;

		for(int n=0;n < sizeof(MHPCustomerName);n++)
		{
			MHPEncDecKey += MHPCustomerName[n];
		}

		MHPEncDecKey1 += LOBYTE(MHPEncDecKey);
		MHPEncDecKey2 += HIBYTE(MHPEncDecKey);
	}

	VM_END
}
