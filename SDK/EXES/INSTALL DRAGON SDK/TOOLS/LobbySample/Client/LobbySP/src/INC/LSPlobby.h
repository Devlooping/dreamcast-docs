/*==========================================================================;
 *
 *	Copyright (C) 1996-1997 Microsoft Corporation.  All Rights Reserved.
 *
 *	File:       LSPlobby.h
 *	Content:	LOBBY-SP Aplcation Interface defined
 *	History:
 *	1999/09/02	Created it
 *=========================================================================*/
#ifndef __LSPLOBBY_INCLUDED__
#define __LSPLOBBY_INCLUDED__

#include <windows.h>
#include "SRScom.h"
//==========================================================================
//
//	LobbySP Interface Area Defined (for GetPlayerData & GetGroupData)
//
//==========================================================================
//  �v���C����񃊃X�g
typedef struct LSP_PLAYERDATA
{
	DWORD	dwPlayerCount;			// �v���C�����
	DWORD	dwPlayerID;				// ���v���C���h�c
	SRS_USERITEM	useritem[1];	// �v���C������
} LSP_PLAYERDATA, * LPLSP_PLAYERDATA;

// �O���[�v��񃊃X�g
typedef struct LSP_GROUPDATA
{
	DWORD	dwGroupCount;			// �O���[�v���
	DWORD	dwGroupID;				// ���O���[�v�h�c
	SRS_GROUPITEM	groupitem[1];	// �O���[�v����
} LSP_GROUPDATA, * LPLSP_GROUPDATA;

#define		LSP_MASK_GROUP		0xF000	// GROUPID MASK
#define		LSP_SLIPMASK_GROUP	0x0FFF	// GROUPID SLIP MASK

#define		LSP_REG_SERVERIP	TEXT("server address")		// �T�[�o�A�h���X
#define		LSP_REG_CLIENT		TEXT("client name")			// �N���C�A���g��
#define		LSP_REG_KEY_COM		TEXT("SOFTWARE\\LOBBY CLIENT")	

#endif // __LSPLOBBY_INCLUDED__

