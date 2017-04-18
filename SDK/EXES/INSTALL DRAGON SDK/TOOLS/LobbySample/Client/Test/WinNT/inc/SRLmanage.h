/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRLmanage.h
 *  Content:    Lobby client management user information include file
 ***************************************************************************/
#ifndef _SRL_MANAGE_H_
#define _SRL_MANAGE_H_

//=========================================
//   �֐���`
//=========================================
__declspec( dllexport )int __stdcall SRL_Mng_Startup( void );
__declspec( dllexport )int __stdcall SRL_Mng_Renewal( char *lpRenewal );
__declspec( dllexport )int __stdcall SRL_Mng_AllGet( char *lpUser, char *lpLobby, char *lpGroup, char *lpGame );
__declspec( dllexport )WORD __stdcall SRL_Mng_GetUserByName( char *lpUserName );
__declspec( dllexport )WORD __stdcall SRL_Mng_GetLobbyByName( char *lpLobbyName );
__declspec( dllexport )WORD __stdcall SRL_Mng_GetGroupByName( char *lpGroupName );
__declspec( dllexport )int __stdcall SRL_Mng_GetUserByID( WORD wUserID, char *lpUserInfo );
__declspec( dllexport )int __stdcall SRL_Mng_GetLobbyByID( WORD wLobbyID, char *lpLobbyInfo );
__declspec( dllexport )int __stdcall SRL_Mng_GetGroupByID( WORD wGroupID, char *lpGroupInfo );

__declspec( dllexport )int __stdcall SRL_Mng_GetEntryNo( int iKind );
__declspec( dllexport )int __stdcall SRL_Mng_GetInfo( void );
__declspec( dllexport )int __stdcall SRL_Mng_PutInfo( void );

/***************************************************************************
 *
 *  �k���������N���C�A���g��`�l
 *
 ***************************************************************************/
//
//  �Ǘ����t�@�C���p�X
//
#define SRL_PATH_USER	"SRLinfouser"
#define SRL_PATH_LOBBY	"SRLinfolobby"
#define SRL_PATH_GROUP	"SRLinfogroup"
#define SRL_PATH_GAME	"SRLinfogame"

//
//  �����
//
#define	SRL_MNG_USER	0		// ���[�U���
#define SRL_MNG_LOBBY	1		// ���r�[���
#define SRL_MNG_GROUP	2		// �O���[�v���
#define SRL_MNG_GAME	3		// �Q�[�����
#endif
