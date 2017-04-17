/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRSmanage.h
 *  Content:
 ***************************************************************************/
#ifndef _SRS_MANAGE_H_
#define _SRS_MANAGE_H_

//=========================================
//   �֐���`
//=========================================
//
// ��񑀍�n�֐�
//
__declspec( dllexport )int __stdcall SRS_Mng_Startup( void );
__declspec( dllexport )int __stdcall SRS_Mng_UserEntry( struct SRS_IF_FIX *lpIfFix );
__declspec( dllexport )int __stdcall SRS_Mng_UserDelete( struct SRS_IF_FIX *lpIfFix );
__declspec( dllexport )int __stdcall SRS_Mng_LobbyEntry( struct SRS_IF_FIX *lpIfFix );
__declspec( dllexport )int __stdcall SRS_Mng_LobbyDefect( struct SRS_IF_FIX *lpIfFix );
__declspec( dllexport )int __stdcall SRS_Mng_GroupMake( struct SRS_IF_FIX *lpIfFix );
__declspec( dllexport )int __stdcall SRS_Mng_GroupJoin( struct SRS_IF_FIX *lpIfFix );
__declspec( dllexport )int __stdcall SRS_Mng_GroupDefect( struct SRS_IF_FIX *lpIfFix );
__declspec( dllexport )int __stdcall SRS_Mng_GameJoin( struct SRS_IF_FIX *lpIfFix );
__declspec( dllexport )int __stdcall SRS_Mng_GameDefect( struct SRS_IF_FIX *lpIfFix );
__declspec( dllexport )int __stdcall SRS_Mng_GameStart( struct SRS_IF_FIX *lpIfFix );
__declspec( dllexport )int __stdcall SRS_Mng_GameEnd( struct SRS_IF_FIX *lpIfFix );
__declspec( dllexport )int __stdcall SRS_Mng_SaveInfo( struct SRS_IF_FIX *lpIfFix );
//
// ���Q�ƌn�֐�
//
__declspec( dllexport )int __stdcall SRS_Mng_AllGet( char *lpUser, char *lpLobby, char *lpGroup, char *lpGame );
__declspec( dllexport )int __stdcall SRS_Mng_GetMngSize( int iKind );
__declspec( dllexport )WORD __stdcall SRS_Mng_GetUserByName( char *lpUserName );
__declspec( dllexport )WORD __stdcall SRS_Mng_GetUserByIP( int );
__declspec( dllexport )WORD __stdcall SRS_Mng_GetLobbyByName( char *lpLobbyName );
__declspec( dllexport )WORD __stdcall SRS_Mng_GetGroupByName( char *lpGroupName );
__declspec( dllexport )int __stdcall SRS_Mng_GetUserByID( WORD wUserID, char *lpUserInfo );
__declspec( dllexport )int __stdcall SRS_Mng_GetLobbyByID( WORD wLobbyID, char *lpLobbyInfo );
__declspec( dllexport )int __stdcall SRS_Mng_GetGroupByID( WORD wGroupID, char *lpGroupInfo );
__declspec( dllexport )int __stdcall SRS_Mng_GetEntryNo( int iKind );

__declspec( dllexport )int __stdcall SRS_Mng_GetInfo( int iMode );
__declspec( dllexport )int __stdcall SRS_Mng_PutInfo( int = 0 );
__declspec( dllexport )void __stdcall SRS_Mng_PutPort( int iPort, int wID );

/***************************************************************************
 *
 *  �k���������T�[�o��`�l
 *
 ***************************************************************************/
//
//  ���r�[��
//
#define SRS_LOBBYNAME	"Lobby "

//
//  �Ǘ����t�@�C���p�X
//
#define SRS_PATH_USER	"SRSinfouser"
#define SRS_PATH_LOBBY	"SRSinfolobby"
#define SRS_PATH_GROUP	"SRSinfogroup"
#define SRS_PATH_GAME	"SRSinfogame"

//
//  ����荞�ݎ��̈�l�����[�h
//
#define	SRS_INFO_NO		0		// �l���Ȃ�
#define SRS_INFO_GET	1		// �l������

//
//  �����
//
#define	SRS_MNG_USER	0		// ���[�U���
#define SRS_MNG_LOBBY	1		// ���r�[���
#define SRS_MNG_GROUP	2		// �O���[�v���
#define SRS_MNG_GAME	3		// �Q�[�����


#endif
