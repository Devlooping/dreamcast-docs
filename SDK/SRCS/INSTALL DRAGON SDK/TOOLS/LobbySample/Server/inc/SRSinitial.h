/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRSinitial.h
 *  Content:    Lobby server initial process include file
 ***************************************************************************/
#ifndef _SRS_INITIAL_H_
#define _SRS_INITIAL_H_

//=========================================
//   �֐���`
//=========================================
int  SRS_Ini_Server( void );
int  SRS_Ini_GetPort( int iClientIp );
int  SRS_Ini_UsePort( void );
int  SRS_Ini_ReleasePort( int iPort );
int  SRS_Ini_CreateLobby( int iPort );
int  SRS_Ini_CreateProc( int iMode, LPSTR lpstrCommandLine );
int	 SRS_Ini_TermProc( int iPort );

//=========================================
//   �v���Z�X�������[�h
//=========================================
#define	SRS_CREATE_SYNC	  	0				// ���������v���Z�X
#define	SRS_CREATE_ASYNC	1 		 		// �񓯊������v���Z�X

//=========================================
//   �|�[�g�ԍ����X�g
//=========================================
//  �v�f
struct SRS_PORT_ITEM
{
	int PI_iIp;				// �g�p�N���C�A���g�h�o�A�h���X
	int	PI_iPort;			// �|�[�g�ԍ�
};
#define SRS_PORT_ITEM_SZ	( sizeof(struct SRS_PORT_ITEM) )

//  ���X�g
struct SRS_PORT_LIST
{
	int 	PL_iItemNo;
	struct	SRS_PORT_ITEM  PL_Item[1];
};
#define SRS_PORT_LIST_SZ	( (sizeof(struct SRS_PORT_LIST) - SRS_PORT_ITEM_SZ) )

#endif
