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

//
//	�`���b�g���O�o�͐�
#define	FILE_CHATLOG		"..\\Log\\Chat%02d%02d%02d.log"

/*====================================================================
 *	���[�J���֐�
 ====================================================================*/
int SRS_Dbg_GetRegChatlog( char *lpBuff );

/*====================================================================
     �֐�   �F�`���b�g���O�o��

     ����   �FlpstrMsg    �`���b�g���b�Z�[�W
     �o��   �F�Ȃ�
     �߂�l �F�Ȃ�
====================================================================*/
void SRS_Chat_WriteLog(int ip, LPSTR lpstrMsg)
{
	int			iRtn;
    FILE        *fFp = (FILE *)NULL;
    SYSTEMTIME  lpst;                    /* �V�X�e�����ԍ\���̂̃A�h���X */
	char        cLogfile[256];
    char        cDate[32];
    char        cBuff[16];
	char        *ipaddr = (char *)&ip;

	memset( cBuff, 0, sizeof(cBuff) );
	iRtn = SRS_Dbg_GetRegChatlog( cBuff );			// �`���b�g���O���Q��
	if( strcmp(cBuff, SRS_DBG_CHATLOG_NONE) == 0 )	// �o�͂Ȃ��H
	{
		return;
	}

	//
	GetLocalTime(&lpst);
	
	//
	//	���O�t�@�C���̍쐬
	//	�P���P�ʂŃ��O�t�@�C�����쐬����
	sprintf(cLogfile, FILE_CHATLOG, lpst.wYear % 100, lpst.wMonth, lpst.wDay);
    if ((fFp = fopen(cLogfile, "a")) == NULL)
		return;
	
	//	���O�t�H�[�}�b�g
	//	hh:mm:ss.mmm 255.255.255.0 <���b�Z�[�W>
	//	  ���M����     ���M��IP
	//	���b�Z�[�W�̒��ɑ��M��ID���܂ށi���j
	sprintf(cDate, "%02d:%02d:%02d.%03d %d.%d.%d.%d ",
	               lpst.wHour, lpst.wMinute, lpst.wSecond, lpst.wMilliseconds,
	               ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);

	fputs(cDate, fFp);
	fputs(lpstrMsg, fFp);
	fputc('\n', fFp);
	
	fclose( fFp );

	return;
}


/*=============================================================================
     �֐�   �F�`���b�g���O���Q��
			�i���W�X�g�����̃`���b�g���O�����Q�Ƃ���j

     ����   �FlpBuff  �i���ʊi�[��|�C���^�j

     �o��   �FlpRegBuff�i�Q�ƌ��ʁj

     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
int SRS_Dbg_GetRegChatlog( char *lpBuff )
{
    LONG    lRtn;                       // 
    HKEY    hKey;                       // �₢���킹��L�[�̃n���h��
    DWORD   dwSize;                     // ���W�X�g���Q�Ɨp�p�����^

    //===========================================
    // �w��L�[�I�[�v��
    //===========================================
    lRtn = RegOpenKeyEx( HKEY_LOCAL_MACHINE, SRS_REG_KEY_CHATLOG, 0, KEY_READ, &hKey );
    if( lRtn != ERROR_SUCCESS )     // �I�[�v�����s�H
    {
        return( -1 );
    }
    dwSize = 256;
    //===========================================
    // �w�荀�ڎQ�� 
    //===========================================
    lRtn = RegQueryValueEx( hKey, SRS_REG_CHATLOG_MODE, NULL, NULL, (unsigned char *)lpBuff, &dwSize );
    if( lRtn != ERROR_SUCCESS )         // �l�����s���H
    {
        // �L�[�̃n���h�����
        RegCloseKey( hKey );
        return( -1 );
    }
    RegCloseKey( hKey );

    return( 0 );
}
