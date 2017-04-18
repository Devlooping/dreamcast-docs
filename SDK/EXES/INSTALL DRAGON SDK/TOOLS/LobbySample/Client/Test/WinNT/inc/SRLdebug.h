/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRLdebug.h
 *  Content:    Lobby client debug tools include
 *                                           
 ***************************************************************************/
#ifndef _SRL_DEBUG_H_
#define _SRL_DEBUG_H_

//=================================
//  ����OS
//=================================
//#define  WINCE				// Windows CE �̏ꍇ�ɗL���ɂ���

//=================================
//  �ǉ���`�iWindowsCE�p�j
//=================================
#ifdef WINCE 

#include "winsock.h"
#include "winuser.h"

#undef  SW_HIDE
#define SW_HIDE 0

#undef  STARTF_USESHOWWINDOW
#define STARTF_USESHOWWINDOW 1

#undef  WM_COMMAND
#define WM_COMMAND WM_USER+10

#endif

// �f�o�b�O���[�h
#define SRL_DBG_TRACE_ERROR	 0					// �o�͂Ȃ�
#define SRL_DBG_TRACE_1  	 1					// ���b�Z�[�W�\������
#define SRL_DBG_TRACE_2  	 2					// ���b�Z�[�W�\������
#define SRL_DBG_TRACE_3  	 3					// ���b�Z�[�W�\������



/*======================
 �֐��錾
======================*/
__declspec( dllexport )void __stdcall SRL_Dbg_WriteData( char *lpTitle, char *lpData, size_t sSize, char *lpMode );
__declspec( dllexport )void __stdcall SRL_Dbg_WriteLog( char *lpstrMsg );

__declspec( dllexport )void __stdcall SRL_Dbg_DebugPrintf(int iTraceLevel, IN PTCHAR pszFormat,...);
__declspec( dllexport )int __stdcall SRL_Dbg_GetRegDebug( LPCTSTR lpBuff );

// �����֐�
int  SRL_Dbg_RegGetInfoBinary( LPCTSTR lpRegKey, LPCTSTR lpReg, LPCTSTR lpBuff, DWORD dwSize );

//=================================
//  ��`
//=================================
// �o�̓t�@�C����
#define	OUT_FILE_LOG	"CLIENT_log.log"
#define	OUT_FILE_DATA	"CLIENT_data.log"

#endif
