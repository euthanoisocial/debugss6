#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#define GAMESERVER_STATUS "STANDBY MODE"

#define GAMESERVER_STATUS_MODE "ACTIVE MODE"

#define GAMESERVER_HOST 1

#define ISPREMIUN 1

extern int Conectar;

#define GAMESERVER_NOMBRE 1

#if(GAMESERVER_NOMBRE == 0)
	#define GAMESERVER_CLIENT "VIERJA&HIGHLEVEL"
#elif(GAMESERVER_NOMBRE == 1)
	#define GAMESERVER_CLIENT "GameServer"
#else
#define GAMESERVER_CLIENT "REAL GAMING"
#endif

#define UPDATE_GAMING 15
#define GAMESERVER_CLIENTE_UPDATE 17
#define GAMESERVER_CLIENTE_PREMIUM 1

#define VERSION "1.0.0.0"

#define PARTYSEARCH 1

#if(GAMESERVER_TYPE==0)
#define GAMESERVER_VERSION "Mu-Solar Team"
#else
#define GAMESERVER_VERSION "GameServer Siege"
#endif

#ifndef GAMESERVER_TYPE
#define GAMESERVER_TYPE 0
#endif

#ifndef GAMESERVER_EXTRA
#define GAMESERVER_EXTRA 1
#endif

#ifndef GAMESERVER_UPDATE
#define GAMESERVER_UPDATE 803
#endif

#if(GAMESERVER_UPDATE==401)
#define GAMESERVER_SEASON "SEASON 4"
#endif

#if(GAMESERVER_UPDATE==603)
#define GAMESERVER_SEASON "SEASON 6"
#endif

#if(GAMESERVER_UPDATE==803)
#define GAMESERVER_SEASON "SEASON 8"
#endif

#ifndef GAMESERVER_LANGUAGE
#define GAMESERVER_LANGUAGE 1
#endif

#ifndef PROTECT_STATE
#define PROTECT_STATE 1
#endif

#ifndef BOTCONGHUONG
#define BOTCONGHUONG 1
#endif

#ifndef BOTHUMUADORAC
#define BOTHUMUADORAC 1
#endif

#ifndef BOTSTATUE
#define BOTSTATUE 1
#endif

#ifndef ENCRYPT_STATE
#define ENCRYPT_STATE 1
#endif

#if(PROTECT_STATE==0)
#define GAMESERVER_NAME "Free"
#else
#define GAMESERVER_NAME "Premium"
#endif


#define MOCNAP           1 // ON
#define QUAEVENT         1 // ON
#define DANGKYTK         0 // OFF
#define DANGKY           1 // ON
#define DOIMK            1 // ON
#define SOIITEM          1 // ON
#define THANMA           1 // ON
#define HAISLOTRING      1 // ON
#define BOSS_GUILD       1 // ON
#define HIDE             1 // ON
#ifndef OANTUTI
#define OANTUTI          1 // ON
#endif
#define CUSTOM_PET					1 // ON
#define NEW_FENRIR					1 // ON
#define OFFLINE_MODE				1 // ON
#define CHANGE_FIX_POINT            0 // OFF
#define BUFFDAME_ICON       		1 // ON
#define VONGQUAY_NEW                1 // ON
#define NGANHANGNGOC_NEW            1 // ON 
#define MONSTERITEM                 1 // ON 
#define QUAN_HAM_NEW                1 // ON
#define DANH_HIEU_NEW	            1 // ON
#define THUE_VIP_NEW	            1 // ON
#define NGU_HANH_NEW                1 // ON
#define NHIEMVU_NEW                 1 // ON
#define BUY_TINH_NANG_NEW           1 // ON
#define SACHTHUOCTINH_NEW           1 // ON 
#define RINGPEDAN_NEW               1 // ON 
#define ENABLE_MUUNPET              1 // ON
#define USE_FAKE_ONLINE             TRUE
#define POINT_SAU_KHI_RESET         1 // OFF
#define RESETLIFE_NEW	            1 // ON
#define GHRS_TOP1_NEW               1 // ON 
//----------------------------
#define EXBEXO_LOAD_TIME_EVENT      1 // ON
//----------------------------
#define MOVE_EVENT_NEW              1 // ON
#define SWAMP_EVENT			        1 // ON
#define BOT_NV_NEW                  0 // OFF
#define BOT_STORE                   1 // ON
#define DEV_DAMAGE_TABLE	        1 // ON
#define CUSTOM_PERSONAL_SHOP        1 // ON
#define OFFLINE_MODE_NEW            1 // ON
#define CUSTOM_PARTY_LEADER		    1 // ON
#define MULTICHARACTER              1 // ON
#define EVENT_TIEC_RUOU_GUILD       1 // ON
#define HUY_HIEU_CUONG_HOA		    0 // 0: Tắt, 1: Bật
//MC bot
//----------------------------
//	 ZG-Defines			     -
//	 1:ON  (CPP AND HEADER!) -
//	 2:OFF (CPP AND HEADER!) -
//----------------------------

#ifndef POINT_CUSTOM 
#define POINT_CUSTOM 1 // ShopPointEx
#endif


#define PC_POINT			 2 //Agregar algun npc o bot con PCP
#define HAPPY_HOURS			 2 //[80%]->Falta Arreglar Los Vip!
#define MASTER_HOURS		 2
#define ACHERON_GUARDIAN	 2

#define BOT_BUFFER			 1

#define BOT_ATTACK   		 1

#define CUSTOM_PERSONAL_SHOP 1

#define ARCA_WAR			 2	// Mix nomas!!
#define GOLDEN_ARCHER		 2	// 

#define SKY_EVENT            1
#define LEO_THAP	         1
#define CHIEN_TRUONG_CO      1
#define ANTIHACKGS           1
//-- Need All Types
#define PG_CUSTOM			 1
#define ALLBOTSSTRUC		 1
//tue add bau cua
#define BAUCUA				 1 //ON
#define	NGAN_HANG_NGOC		 1 //ON
#define MAX_ITEM_SLOT		 30
//----
//tue add Moc nap
#define NEWXSHOP							1
#define B_MOCNAP							1
#define eMessageBox				255
#define SPK_ITEMTRADER						1
//-------------
// tue add Bom Mau
#define SPK_AUTOHP							1
// -----
// tue add Tu Chan
#define MOVE_ITEMBANK_DIR					0 //Trừ trực tiếp vào bank ( DH )
#define TU_LUYEN_NEW						1 
//tue add Hon Hoan
#define B_HON_HOAN							1
//tue add thue flag 
#define HT_THUE_FLAG						1
// ---
/////////////////////
#include <windows.h>
#include <winsock2.h>
#include <mswSock.h>
#include <commctrl.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <map>
#include <vector>
#include <queue>
#include <random>
#include <Rpc.h>
#include <algorithm>
#include <string>
#include <atltime.h>
#include <dbghelp.h>
#include <Psapi.h>
#include "..//..//..//Util//pugixml//pugixml.hpp"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"Psapi.lib")

#if(GAMESERVER_UPDATE>=701)
#if(NDEBUG==0)
#pragma comment(lib,"..\\..\\Util\\cryptopp\\Debug\\cryptlib.lib")
#else
#pragma comment(lib,"..\\..\\Util\\cryptopp\\Release\\cryptlib.lib")
#endif
#pragma comment(lib,"..\\..\\Util\\mapm\\mapm.lib")
#endif

typedef char chr;

typedef float flt;

typedef short shrt;

typedef unsigned __int64 QWORD;
