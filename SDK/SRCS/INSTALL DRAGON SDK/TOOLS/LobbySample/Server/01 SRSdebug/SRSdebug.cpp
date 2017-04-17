/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRScom.cpp
 *                	SRS_ComWriteData	: �g���[�X�o��
 *					SRS_ComWriteLog		: �_���v�o��
 *  Content:    Lobby server & Lobby Client debug tools
 *
 ***************************************************************************/
//================================
// �C���N���[�h      
//================================
#include <windows.h>
#include <stdio.h>

#include "SRScom.h"
#include "SRSdebug.h"
#include "SRSregistry.h"

/*====================================================================
     �֐�   �F�g���[�X�o�́i�C�Ӄg���[�X���t�@�C���ɏo�͂���j

     ����   �FlpstrMsg    �o�̓��b�Z�[�W
     �o��   �F�Ȃ�
     �߂�l �F�Ȃ�
====================================================================*/
__declspec( dllexport )void __stdcall SRS_Dbg_WriteLog( LPSTR lpstrMsg )
{
    size_t      szSize = 0;
    size_t      szBlock = 1;
    int         iRet = 0;
	int			iLevel = 0;
	int			iRtn;
    FILE        *fFp = (FILE *)NULL;
    SYSTEMTIME  lpst;                    /* �V�X�e�����ԍ\���̂̃A�h���X */
	DWORD		dwDefault = 0;
	char		cFilePath[256];
    char        cDate[32];
    char        cBuff[16];

	memset( cBuff, 0, sizeof(cBuff) );
	iRtn = SRS_Dbg_GetRegDebug( cBuff );			// �f�o�b�O���Q��
	if( strcmp(cBuff, SRS_DBG_MODE_NO) == 0 )		// �o�͂Ȃ��H
	{
		return;
	}
	if( (strcmp(cBuff, SRS_DBG_MODE_FILE) != 0) &&	// ����`���[�h�H
		(strcmp(cBuff, SRS_DBG_MODE_TRC) != 0)  &&
		(strcmp(cBuff, SRS_DBG_MODE_MSG) != 0) )
	{
		return;
	}
	if( strcmp(cBuff, SRS_DBG_MODE_MSG) == 0 )	// ���b�Z�[�W�\������H
	{
		MessageBox( NULL,lpstrMsg,"debug message", MB_OK );
	}					

	strcpy( cFilePath, OUT_FILE_LOG ); 
    fFp = fopen( cFilePath, "a");
	if( fFp == (FILE *)NULL )
		return;
	//
	//  �����ݒ�
	//
	GetLocalTime(&lpst);
	memset( cBuff, 0, sizeof(cBuff) );
	memset( cDate, 0, sizeof(cDate) );
	itoa( lpst.wYear, cBuff, 10 );
	strcpy( cDate, cBuff );
	strcat( cDate, "." );

	memset( cBuff, 0, sizeof(cBuff) );
	itoa( lpst.wMonth, cBuff, 10 );
	if( lpst.wMonth < 10 )
		strcat( cDate, "0" );
	strcat( cDate, cBuff );
	strcat( cDate, "." );

	memset( cBuff, 0, sizeof(cBuff) );
	itoa( lpst.wDay, cBuff, 10 );
	if( lpst.wDay < 10 )
		strcat( cDate, "0" );
	strcat( cDate, cBuff );
	strcat( cDate, " " );

	memset( cBuff, 0, sizeof(cBuff) );
	itoa( lpst.wHour, cBuff, 10 );
	if( lpst.wHour < 10 )
		strcat( cDate, "0" );
	strcat( cDate, cBuff );
	strcat( cDate, ":" );

	memset( cBuff, 0, sizeof(cBuff) );
	itoa( lpst.wMinute, cBuff, 10 );
	if( lpst.wMinute < 10 )
		strcat( cDate, "0" );
	strcat( cDate, cBuff );
	strcat( cDate, ":" );

	memset( cBuff, 0, sizeof(cBuff) );
	itoa( lpst.wSecond, cBuff, 10 );
	if( lpst.wSecond < 10 )
		strcat( cDate, "0" );
	strcat( cDate, cBuff );
	strcat( cDate, " " );
	memset( cBuff, 0, sizeof(cBuff) );
	itoa( lpst.wMilliseconds, cBuff, 10 );
	if( lpst.wMilliseconds < 10 )
		strcat( cDate, "00" );
	else if( lpst.wMilliseconds < 100 )
		strcat( cDate, "0" );
	strcat( cDate, cBuff );
	strcat( cDate, "  " );     
	szSize = strlen( cDate );
	fwrite( cDate, szSize, szBlock, fFp );
	szSize = strlen( (char *)lpstrMsg );
	fwrite( (char *)lpstrMsg, szSize, szBlock, fFp );
	memset( cBuff, 0, sizeof(cBuff) );
	strcpy( cBuff, "\n" );
	fwrite( (char *)cBuff, 1, szBlock, fFp );
	
	fclose( fFp );

	return;
}

/*====================================================================
     �֐�   �F�_���v�o�́i�C�ӗ̈���t�@�C���Ƀo�C�i���o�͂���j

     ����   �FlpstrTitle    �^�C�g��
              lpstrData		�o�͑Ώۈ�ւ̃|�C���^
			  sSize			�L����
			  lpMode		�o�͌`���ifopen mode �p�����^�ւ̎w��l�j
                               "a" :�ǉ�
                               "w" :�㏑��
                               "b" : BINARY
							*�ʏ�́A"ab"�܂���"wb"���w�肷��
    �o��   �F�Ȃ�
     �߂�l �F�Ȃ�
====================================================================*/
__declspec( dllexport )void __stdcall SRS_Dbg_WriteData( char *lpstrTitle,
											    		 char *lpstrData,
     													 size_t sSize,
														 char *lpMode )
{
    size_t      szBlock = 1;
    int         iRet = 0;
    SYSTEMTIME  lpst;                    /* �V�X�e�����ԍ\���̂̃A�h���X */
	FILE        *fFp = (FILE *)NULL;
	int			iLen;
	int			iLevel = 0;
	int			iRtn;
	DWORD		dwDefault = 0;
	char		cFilePath[256];
    char        cDate[32];
	char		cTitle[128];
	char		cBuff[16];

	memset( cBuff, 0, sizeof(cBuff) );
	iRtn = SRS_Dbg_GetRegDebug( cBuff );			// �f�o�b�O���Q��
	if( (strcmp(cBuff, SRS_DBG_MODE_NO) == 0) ||	// �o�͂Ȃ��H
		(strcmp(cBuff, SRS_DBG_MODE_TRC) == 0) ) 
	{
		return;
	}
	if( (strcmp(cBuff, SRS_DBG_MODE_FILE) != 0) &&	// ����`���[�h�H
		(strcmp(cBuff, SRS_DBG_MODE_MSG) != 0) )
	{
		return;
	}
	//
	//  �����ݒ�
	//
	GetLocalTime(&lpst);
	memset( cBuff, 0, sizeof(cBuff) );
	memset( cDate, 0, sizeof(cDate) );
	itoa( lpst.wYear, cBuff, 10 );
	strcpy( cDate, cBuff );
	strcat( cDate, "." );

	memset( cBuff, 0, sizeof(cBuff) );
	itoa( lpst.wMonth, cBuff, 10 );
	if( lpst.wMonth < 10 )
		strcat( cDate, "0" );
	strcat( cDate, cBuff );
	strcat( cDate, "." );

	memset( cBuff, 0, sizeof(cBuff) );
	itoa( lpst.wDay, cBuff, 10 );
	if( lpst.wDay < 10 )
		strcat( cDate, "0" );
	strcat( cDate, cBuff );
	strcat( cDate, " " );

	memset( cBuff, 0, sizeof(cBuff) );
	itoa( lpst.wHour, cBuff, 10 );
	if( lpst.wHour < 10 )
		strcat( cDate, "0" );
	strcat( cDate, cBuff );
	strcat( cDate, ":" );

	memset( cBuff, 0, sizeof(cBuff) );
	itoa( lpst.wMinute, cBuff, 10 );
	if( lpst.wMinute < 10 )
		strcat( cDate, "0" );
	strcat( cDate, cBuff );
	strcat( cDate, ":" );

	memset( cBuff, 0, sizeof(cBuff) );
	itoa( lpst.wSecond, cBuff, 10 );
	if( lpst.wSecond < 10 )
		strcat( cDate, "0" );
	strcat( cDate, cBuff );
	strcat( cDate, " " );
	memset( cBuff, 0, sizeof(cBuff) );
	itoa( lpst.wMilliseconds, cBuff, 10 );
	if( lpst.wMilliseconds < 10 )
		strcat( cDate, "00" );
	else if( lpst.wMilliseconds < 100 )
		strcat( cDate, "0" );
	strcat( cDate, cBuff );
	strcat( cDate, "  " );     

	//
	//  �̈�o��
	//
	memset( cFilePath, 0, sizeof(cFilePath) );
	strcpy( cFilePath, OUT_FILE_DATA ); 
	fFp = fopen( cFilePath, lpMode );
	if( fFp == (FILE *)NULL )
		return;
	memset( cTitle, 0, sizeof(cTitle) );
	sprintf( cTitle, "### %s %s ###", lpstrTitle, cDate );
	iLen = strlen( cTitle );
	fwrite( cTitle, iLen, szBlock, fFp );
	fwrite( lpstrData, sSize, szBlock, fFp );
	fclose( fFp );
	return;
}

/*=============================================================================
     �֐�   �F�f�o�b�O���Q��
			�i���W�X�g�����̃f�o�b�O�����Q�Ƃ���j

     ����   �FlpBuff  �i���ʊi�[��|�C���^�j

     �o��   �FlpRegBuff�i�Q�ƌ��ʁj

     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
int SRS_Dbg_GetRegDebug( char *lpBuff )
{
    LONG    lRtn;                       // 
    HKEY    hKey;                       // �₢���킹��L�[�̃n���h��
    DWORD   dwSize;                     // ���W�X�g���Q�Ɨp�p�����^

    //===========================================
    // �w��L�[�I�[�v��
    //===========================================
    lRtn = RegOpenKeyEx( HKEY_LOCAL_MACHINE, SRS_REG_KEY_DEBUG, 0, KEY_READ, &hKey );
    if( lRtn != ERROR_SUCCESS )     // �I�[�v�����s�H
    {
        return( -1 );
    }
    dwSize = 256;
    //===========================================
    // �w�荀�ڎQ�� 
    //===========================================
    lRtn = RegQueryValueEx( hKey, SRS_REG_MODE, NULL, NULL, (unsigned char *)lpBuff, &dwSize );
    if( lRtn != ERROR_SUCCESS )         // �l�����s���H
    {
        // �L�[�̃n���h�����
        RegCloseKey( hKey );
        return( -1 );
    }
    RegCloseKey( hKey );

    return( 0 );
}
