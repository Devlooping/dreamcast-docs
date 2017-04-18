/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRSdebug.h
 *  Content:    Lobby server & Lobby client debug tools include
 *                                           
 ***************************************************************************/
#ifndef _SRS_DEBUG_H_
#define _SRS_DEBUG_H_

/*======================
 �֐��錾
======================*/
__declspec( dllexport )void __stdcall SRS_Dbg_WriteLog( LPSTR lpstrMsg );
__declspec( dllexport )void __stdcall SRS_Dbg_WriteData( char *lpTitle, char *lpData, size_t sSize, char *lpMode );

// �����֐�
int	SRS_Dbg_GetRegDebug( char *lpBuff );
int SRS_Dbg_Log( char *lpLog );

//=================================
//  ��`
//=================================

// �o�̓t�@�C����
#define	OUT_FILE_LOG		"..\\Log\\SRV_TEXT.LOG"
#define	OUT_FILE_DATA		"..\\Log\\SRV_DATA.LOG"
#define	OUT_FILE_LOGBACK	"..\\Log\\SRV_TEXT.BAK"
#define	OUT_FILE_DATABACK	"..\\Log\\SRV_DATA.BAK"

#endif
