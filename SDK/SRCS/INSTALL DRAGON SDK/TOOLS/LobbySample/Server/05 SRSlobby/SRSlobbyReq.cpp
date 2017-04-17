/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRSlobbyReq.cpp
 *               	SRS_Lby_ReqAll 			(�S���Q�Ɨv��)
 *					SRS_Lby_ReqUserEntry	(���[�U�o�^�v��)
 *					SRS_Lby_ReqUserDelete	(���[�U�폜�v��)
 *	 				SRS_Lby_ReqLobbyEntry	(���r�[�o�^�v��)
 *	 				SRS_Lby_ReqLobbyDefect	(���r�[�E�ޗv��)
 *	 				SRS_Lby_ReqGroupMake	(�O���[�v�쐬�v��)
 *	 				SRS_Lby_ReqGroupJoin	(�O���[�v�Q���v��)
 *	 				SRS_Lby_ReqGroupDefect	(�O���[�v�E�ޗv��)
 *	 				SRS_Lby_ReqGameJoin	    (�Q�[���Q���v��)
 *	 				SRS_Lby_ReqGameDefect	(�Q�[���E�ޗv��)
 *	 				SRS_Lby_ReqGameStart	(�Q�[���J�n�v��)
 *	 				SRS_Lby_ReqGameEnd		(�Q�[���I���v��)
 *	 				SRS_Lby_ReqSendMsg		(���b�Z�[�W���M�v��)
 *	 				SRS_Lby_ReqSaveInfo		(�ŗL���Ҕ�v��)
 *	 				SRS_Lby_ReqSeqError		(�v���V�[�P���X�G���[)
 *	 				SRS_Lby_ReqResultSend	(�v���������M)
 *
 *  Content:    Lobby Server  Lobby�N���C�A���g�v�������v���Z�X
 *
 ***************************************************************************/
/*================================
    �C���N���[�h
==================================*/
#include <windows.h>
#include <stdio.h>

#include "SRScom.h"
#include "SRSdebug.h"
#include "SRSmanage.h"
#include "SRSlobby.h"
#include "SRSnetwork.h"
#include "SRSregistry.h"

//
//	�`���b�g���O�o�͐�
#define	FILE_CHATLOG		"..\\Log\\Chat%02d%02d%02d.log"

/*================================
    �O���[�o����
==================================*/
/*====================================================================
 *	���[�J���֐�
 ====================================================================*/
int SRS_Dbg_GetRegChatlog( char *lpBuff );

WORD	wUserID = 0;					// ���[�U�h�c
/*=============================================================================
     �֐�    �F�S���Q�Ɨv��
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqAll( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;							//
	int		iClientIP;						//
	int		iUserSize;						//
	int		iLobbySize;						//
	int		iGroupSize;						//
	int		iGameSize;						//
	int		iPoint;							//
	size_t	szSendSize;						//
	struct	SRS_IF_FIX *lpSendFix;			//
	struct	SRS_IF_RENEWAL *lpIfRenewal;	// �X�V����|�C���^
	struct	SRS_REG_INFO *lpRegInfo;		// ���ʏ���|�C���^
	char	*lpSendData;					//
	char	*lpUserInfo;					//
	char	*lpLobbyInfo;					//
	char	*lpGroupInfo;					//
	char	*lpGameInfo;					//
	char	cRegInfo[SRS_REG_INFO_SZ];		// ���ʏ���
	char	cMsg[256];


	//=======================================
	//  ����T�C�Y�Z�o
	//=======================================
	sprintf( cMsg, "SRS_Lby_ReqAll : �S���Q�� 0x%04x",lpIfFix->IF_wRequest );
	SRS_Dbg_WriteLog( cMsg );

	iClientIP = lpIfFix->IF_iIp;
	lpRegInfo = (struct SRS_REG_INFO *)cRegInfo;
	iRtn = SRS_Reg_GetComInfo( lpRegInfo );		// ���ʊǗ������荞��
	if( iRtn == -1 )							// ��荞�ݎ��s�H
	{
		lpIfFix->IF_wRequest = IF_REQ_ALL;		// �v���ݒ�
		lpIfFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpIfFix->IF_iInformationSize = 0;
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpIfFix, (size_t)SRS_IF_FIX_SZ );
		sprintf( cMsg, "SRS_Lby_ReqAll : SRS_Reg_GetComInfo error" );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	iUserSize = SRS_Mng_GetMngSize( SRS_MNG_USER );		// ���[�U����T�C�Y
	iLobbySize = SRS_Mng_GetMngSize( SRS_MNG_LOBBY );	// ���r�[����T�C�Y
	iGroupSize = SRS_Mng_GetMngSize( SRS_MNG_GROUP );	// �O���[�v����T�C�Y
	iGameSize = SRS_Mng_GetMngSize( SRS_MNG_GAME );		// �Q�[������T�C�Y
	szSendSize = (size_t)(SRS_IF_FIX_SZ + SRS_IF_RENEWAL_SZ + 	// �S���T�C�Y
							iUserSize + iLobbySize + iGroupSize + iGameSize );

	//=======================================
	//  ���l��
	//=======================================
	lpSendData = (char *)malloc( szSendSize );
	if( lpSendData == NULL )	// �l�����s�H
	{
		lpIfFix->IF_wRequest = IF_REQ_ALL;		// �v���ݒ�
		lpIfFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpIfFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpIfFix, (size_t)SRS_IF_FIX_SZ );
		sprintf( cMsg, "SRS_Lby_ReqAll : Cannot malloc information data area" );
		SRS_Dbg_WriteLog( cMsg );
		return( 0 );
	}
	memset( lpSendData, 0, szSendSize );
	//=======================================
	//  �����쐬
	//=======================================
	//
	// �ŗL��ݒ�
	lpSendFix = (struct SRS_IF_FIX *)lpSendData;
    lpSendFix->IF_wRequest = IF_REQ_ALL;							// �v���ݒ�
	lpSendFix->IF_wResult = IF_CMP_SUCCESS;							// ���ʐ���
	lpSendFix->IF_iInformationSize = szSendSize - SRS_IF_FIX_SZ;	//
	lpSendFix->IF_iPortNo = lpIfFix->IF_iPortNo;					// �|�[�g�ԍ��R�s�[
	lpSendFix->IF_iIp = lpIfFix->IF_iIp;							// IP�A�h���X�R�s�[

	//
	// �X�V����ݒ�
	lpIfRenewal = (struct SRS_IF_RENEWAL *)lpSendFix->IF_cInformation;
	lpIfRenewal->IF_RE_wUserInfoNo  = lpRegInfo->wMaxUser + 1;	// �ő僆�[�U���ݒ�
	lpIfRenewal->IF_RE_wLobbyInfoNo = lpRegInfo->wMaxLobby;		// �ő働�r�[���ݒ�
	lpIfRenewal->IF_RE_wGroupInfoNo = lpRegInfo->wMaxGroup + 1;	// �ő�O���[�v���ݒ�
	lpIfRenewal->IF_RE_wGameInfoNo  = lpRegInfo->wMaxGame + 1;	// �ő�Q�[�����ݒ�

	// ���[�U���
	lpUserInfo = (char *)malloc( iUserSize );
	if( lpUserInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		sprintf( cMsg, "SRS_Lby_ReqAll : Cannot malloc user information data" );
		SRS_Dbg_WriteLog( cMsg );
		return( 0 );
	}
	
	// ���r�[����
	lpLobbyInfo = (char *)malloc( iLobbySize );
	if( lpLobbyInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		sprintf( cMsg, "SRS_Lby_ReqAll : Cannot malloc lobby information data" );
		SRS_Dbg_WriteLog( cMsg );
		return( 0 );
	}

	// �O���[�v����
	lpGroupInfo = (char *)malloc( iGroupSize );
	if( lpGroupInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		sprintf( cMsg, "SRS_Lby_ReqAll : Cannot malloc gouup information data" );
		SRS_Dbg_WriteLog( cMsg );
		return( 0 );
	}

	// �Q�[������
	lpGameInfo = (char *)malloc( iGameSize );
	if( lpGameInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		realloc( lpGroupInfo, 0 );
		sprintf( cMsg, "SRS_Lby_ReqAll : Cannot malloc game information data" );
		SRS_Dbg_WriteLog( cMsg );
		return( 0 );
	}

	//
	// �S���l��
	sprintf( cMsg, "SRS_Lby_ReqAll : Call SRS_Mng_AllGet()" );
	SRS_Dbg_WriteLog( cMsg );

	iRtn = SRS_Mng_AllGet( lpUserInfo, lpLobbyInfo, lpGroupInfo, lpGameInfo );
	if( iRtn == -1 )							// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_SEQERR;	// �v���V�[�P���X�G���[
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		realloc( lpGroupInfo, 0 );
		realloc( lpGameInfo, 0 );
		sprintf( cMsg, "SRS_Lby_ReqAll : SRS_Mng_AllGet() FALSE" );
		SRS_Dbg_WriteLog( cMsg );
		return( 0 );
	}
	

	// �Ǘ����ݒ�
	iPoint = 0;
	memcpy( (char *)&lpIfRenewal->IF_RE_cInfo[iPoint], lpUserInfo, iUserSize );
	iPoint = iUserSize;
	memcpy( (char *)&lpIfRenewal->IF_RE_cInfo[iPoint], lpLobbyInfo, iLobbySize );
	iPoint = iUserSize + iLobbySize;
	memcpy( (char *)&lpIfRenewal->IF_RE_cInfo[iPoint], lpGroupInfo, iGroupSize );
	iPoint = iUserSize + iLobbySize + iGroupSize;
	memcpy( (char *)&lpIfRenewal->IF_RE_cInfo[iPoint], lpGameInfo, iGameSize );

	//=======================================
	//  �������M
	//=======================================
	SRS_Dbg_WriteData( "SRS_Lby_ReqAll : �S���Q�� ����", lpSendData, szSendSize, "ab" );
	iRtn = SRS_Net_SendReq( iClientIP, lpSendData, (size_t)szSendSize );

	realloc( lpUserInfo, 0 );
	realloc( lpLobbyInfo, 0 );
	realloc( lpGroupInfo, 0 );
	realloc( lpGameInfo, 0 );
	realloc( lpSendData, 0 );

	return( iRtn );
}

/*=============================================================================
     �֐�    �F���[�U�o�^�v��
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqUserEntry( struct SRS_IF_FIX *lpIfFix )
{
	WORD	wLobbyID;
	int		iRtn;							//
	int		iClientIP;						//
	int		iUserSize;						//
	int		iLobbySize;						//
	int		iGroupSize;						//
	int		iGameSize;						//
	int		iPoint;							//
	size_t	szSendSize;						//
	struct	SRS_IF_FIX 		*lpSendFix;		//
	struct	SRS_IF_USER_ENTRY *lpUserEntry;	//
	struct	SRS_IF_RENEWAL 	*lpSendRenewal;	// �X�V����|�C���^
	struct	SRS_REG_INFO	*lpRegInfo;		// ���ʏ���|�C���^
	struct	SRS_INFO_USER	*lpSendUser;	//
	struct	SRS_INFO_LOBBY	*lpSendLobby;	//
	char	*lpSendData;					//
	char	*lpUserInfo;					//
	char	*lpLobbyInfo;					//
	char	*lpGroupInfo;					//
	char	*lpGameInfo;					//
	char	cRegInfo[SRS_REG_INFO_SZ];		// ���ʏ���
	char	cIfFix[SRS_IF_FIX_SZ + SRS_IF_USER_ENTRY_SZ];	// ���[�U�o�^����
	char	cSendMode[4];
	char	cMsg[256];

	sprintf( cMsg, "SRS_Lby_ReqUserEntry : ���[�U�o�^ 0x%04x",lpIfFix->IF_wRequest);
	SRS_Dbg_WriteLog( cMsg );

	memset( cIfFix, 0, SRS_IF_FIX_SZ + SRS_IF_USER_ENTRY_SZ );
	memcpy( cIfFix, (char *)lpIfFix, SRS_IF_FIX_SZ + SRS_IF_USER_ENTRY_SZ );
	lpSendFix = (struct SRS_IF_FIX *)cIfFix;
	iClientIP = lpIfFix->IF_iIp;

	iRtn = SRS_Mng_UserEntry( lpSendFix );	// ���[�U�o�^
	if( iRtn == -1 )						// ���[�U�o�^���s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_USER_ENTRY;
		lpSendFix->IF_iInformationSize = 0;
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( iRtn );
	}

	//=======================================
	//  ����T�C�Y�Z�o
	//=======================================
	lpRegInfo = (struct SRS_REG_INFO *)cRegInfo;
	iRtn = SRS_Reg_GetComInfo( lpRegInfo );		// ���ʊǗ������荞��
	if( iRtn == -1 )							// ��荞�ݎ��s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_USER_ENTRY;	// ���[�U�o�^
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		sprintf( cMsg, "SRS_Lby_ReqUserEntry : SRS_Reg_GetComInfo error" );
		SRS_Dbg_WriteLog( cMsg );
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( -1 );
	}
	iUserSize  = SRS_Mng_GetMngSize( SRS_MNG_USER );			// ���[�U����T�C�Y
	iLobbySize = SRS_Mng_GetMngSize( SRS_MNG_LOBBY );	    	// ���r�[����T�C�Y
	iGroupSize = SRS_Mng_GetMngSize( SRS_MNG_GROUP );   		// �O���[�v����T�C�Y
	iGameSize  = SRS_Mng_GetMngSize( SRS_MNG_GAME );		    // �Q�[������T�C�Y
	szSendSize = (size_t)(SRS_IF_FIX_SZ + SRS_IF_RENEWAL_SZ + 	// �S���T�C�Y
							iUserSize + iLobbySize + iGroupSize + iGameSize );
	//=======================================
	//  ���l��
	//=======================================
	lpSendData = (char *)malloc( szSendSize );
	if( lpSendData == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_USER_ENTRY;		// �v���ݒ�
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( 0 );
	}
	memset( lpSendData, 0, szSendSize );
	//=======================================
	//  �����쐬
	//=======================================
	//
	// �ŗL��ݒ�
	lpSendFix = (struct SRS_IF_FIX *)lpSendData;
    lpSendFix->IF_wRequest = IF_REQ_USER_ENTRY;						// �v���ݒ�
	lpSendFix->IF_wResult = IF_CMP_SUCCESS;							// ���ʐ���
	lpSendFix->IF_iInformationSize = szSendSize - SRS_IF_FIX_SZ;	//
	lpSendFix->IF_iPortNo = lpIfFix->IF_iPortNo;					// �|�[�g�ԍ��R�s�[
	lpSendFix->IF_iIp = lpIfFix->IF_iIp;							// IP�A�h���X�R�s�[

	//
	// �X�V����ݒ�
	lpSendRenewal = (struct SRS_IF_RENEWAL *)lpSendFix->IF_cInformation;
	lpSendRenewal->IF_RE_wUserInfoNo  = lpRegInfo->wMaxUser + 1;	// ���[�U���ݒ�
	lpSendRenewal->IF_RE_wLobbyInfoNo = lpRegInfo->wMaxLobby;		// ���r�[���ݒ�
	lpSendRenewal->IF_RE_wGroupInfoNo = lpRegInfo->wMaxGroup + 1;	// �O���[�v���ݒ�
	lpSendRenewal->IF_RE_wGameInfoNo  = lpRegInfo->wMaxGame + 1;	// �Q�[�����ݒ�

	// ���[�U���
	lpUserInfo = (char *)malloc( iUserSize );
	if( lpUserInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		return( 0 );
	}
	
	// ���r�[����
	lpLobbyInfo = (char *)malloc( iLobbySize );
	if( lpLobbyInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		return( 0 );
	}

	// �O���[�v����
	lpGroupInfo = (char *)malloc( iGroupSize );
	if( lpGroupInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		return( 0 );
	}

	// �Q�[������
	lpGameInfo = (char *)malloc( iGameSize );
	if( lpGameInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		realloc( lpGroupInfo, 0 );
		return( 0 );
	}

	//
	// �S���l��
	iRtn = SRS_Mng_AllGet( lpUserInfo, lpLobbyInfo, lpGroupInfo, lpGameInfo );
	if( iRtn == -1 )							// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_SEQERR;	// �v���V�[�P���X�G���[
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		realloc( lpGroupInfo, 0 );
		realloc( lpGameInfo, 0 );
		return( 0 );
	}

	// �Ǘ����ݒ�
	iPoint = 0;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpUserInfo, iUserSize );
	iPoint = iUserSize;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpLobbyInfo, iLobbySize );
	iPoint = iUserSize + iLobbySize;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpGroupInfo, iGroupSize );
	iPoint = iUserSize + iLobbySize + iGroupSize;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpGameInfo, iGameSize );

	//=======================================
	//  �������M
	//=======================================
	sprintf( cMsg, "SRS_Lby_UserEntry : �������M %d bytes", szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	SRS_Dbg_WriteData( cMsg, lpSendData, szSendSize, "ab" );

	iRtn = SRS_Net_SendReq( iClientIP, lpSendData, (size_t)szSendSize );

	//=======================================
	//  �X�V���z�M
	//=======================================
	realloc( lpSendData, 0 );
	realloc( lpGroupInfo, 0 );
	realloc( lpGameInfo, 0 );
	strcpy( cSendMode, SRS_SEND_MODE_NO );
	iRtn = SRS_Reg_GetInfo( SRS_REG_KEY_COM, SRS_REG_SEND, cSendMode );
	if( (strncmp(cSendMode, SRS_SEND_MODE_NO, sizeof(SRS_SEND_MODE_NO))) == 0 )	// �z�M�Ȃ��H
	{
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		realloc( lpSendData, 0 );
		return( 0 );
	}
	//
    // �z�M���쐬
	//
	SRS_Dbg_WriteLog( "SRS_Lby_ReqUserEntry : �X�V���z�M�J�n" );
	szSendSize = SRS_IF_FIX_SZ + SRS_IF_RENEWAL_SZ + SRS_INFO_USER_SZ + SRS_INFO_LOBBY_SZ;
	lpSendData = (char *)malloc( szSendSize );
	if( lpSendData == NULL )
	{
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		realloc( lpSendData, 0 );
		SRS_Dbg_WriteLog( "SRS_Lby_ReqUserEntry : malloc error" );
		return( -1 );
	}
	memset( lpSendData, 0, szSendSize );

	lpSendFix = (struct SRS_IF_FIX *)lpSendData;
	lpSendFix->IF_wRequest = IF_REQ_RENEWAL;
	lpSendFix->IF_iInformationSize = szSendSize - SRS_IF_FIX_SZ;
	lpSendRenewal = (struct SRS_IF_RENEWAL *)lpSendFix->IF_cInformation;
	lpSendRenewal->IF_RE_wUserInfoNo  = 1;		// �X�V���[�U���ݒ�
	lpSendRenewal->IF_RE_wLobbyInfoNo = 1;		// �X�V���r�[���ݒ�
	//
	// �Ǘ����ݒ�
	//
	lpSendUser 	= (struct SRS_INFO_USER *)lpUserInfo;
	lpSendLobby = (struct SRS_INFO_LOBBY *)lpLobbyInfo;
	lpUserEntry = (struct SRS_IF_USER_ENTRY *)lpIfFix->IF_cInformation;

	wUserID = SRS_Mng_GetUserByName( lpUserEntry->IF_UE_cUserName );
	if( wUserID == SRS_WORD_ERROR )
	{
		sprintf( cMsg, "SRS_Lby_ReqUserEntry : SRS_Mng_GetUserByName error");
		SRS_Dbg_WriteLog( cMsg );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		realloc( lpSendData, 0 );
		return( -1 );
	}

	wLobbyID = SRS_ID_MAINLOBBY;
	iPoint = 0;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpUserInfo, SRS_INFO_USER_COM_SZ );
	iPoint = SRS_INFO_USER_COM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint],
             lpSendUser->US_info_user[wUserID].USI_cClientName, SRS_USER_ITEM_SZ );
	iPoint = iPoint + SRS_USER_ITEM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpLobbyInfo, SRS_INFO_LOBBY_COM_SZ );
	iPoint = iPoint + SRS_INFO_LOBBY_COM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint],
            (char *)&lpSendLobby->LB_info_lobby[wLobbyID].LBI_iLockword, SRS_LOBBY_ITEM_SZ );

	// ���z�M
	sprintf( cMsg, "SRS_Lby_UserEntry : �X�V���z�M %d bytes",szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	iRtn = SRS_Lby_SendInfo( wUserID, wLobbyID, lpSendData, (size_t)szSendSize );

	realloc( lpUserInfo, 0 );
	realloc( lpLobbyInfo, 0 );
	realloc( lpSendData, 0 );
	return( 0 );
}

/*=============================================================================
     �֐�    �F���[�U�폜�v��
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqUserDelete( struct SRS_IF_FIX *lpIfFix )
{
	WORD	wLobbyID;
	int		iRtn;							//
	int		iClientIP;						//
	int		iUserSize;						//
	int		iLobbySize;						//
	int		iPoint;							//
	size_t	szSendSize;						//
	struct	SRS_IF_FIX 		*lpSendFix;		//
	struct	SRS_IF_USER_DELETE *lpUserDelete;
	struct	SRS_IF_RENEWAL 	*lpSendRenewal;	// �X�V����|�C���^
	struct	SRS_REG_INFO	*lpRegInfo;		// ���ʏ���|�C���^
	struct	SRS_INFO_USER	*lpSendUser;	//
	struct	SRS_INFO_LOBBY	*lpSendLobby;	//
	struct	SRS_USER_ITEM	*lpUserItem;	//
	char	*lpSendData;					//
	char	*lpUserInfo;					//
	char	*lpLobbyInfo;					//
	char	cRegInfo[SRS_REG_INFO_SZ];		// ���ʏ���
	char	cIfFix[SRS_IF_FIX_SZ + SRS_IF_USER_DELETE_SZ];	// ���[�U�폜����
	char	cSendMode[4];
	char	cUserItem[SRS_USER_ITEM_SZ];
	char	cMsg[256];


	sprintf( cMsg, "SRS_Lby_ReqUserDelete : ���[�U�폜 0x%04x",lpIfFix->IF_wRequest);
	SRS_Dbg_WriteLog( cMsg );

	memset( cIfFix, 0, SRS_IF_FIX_SZ + SRS_IF_USER_DELETE_SZ );
	memcpy( cIfFix, (char *)lpIfFix, SRS_IF_FIX_SZ + SRS_IF_USER_DELETE_SZ );
	lpSendFix = (struct SRS_IF_FIX *)cIfFix;
	iClientIP = lpIfFix->IF_iIp;

	//=======================================
	//  ���[�U�폜
	//=======================================
	lpUserDelete = (struct SRS_IF_USER_DELETE *)lpIfFix->IF_cInformation;
	wUserID  = lpUserDelete->IF_UD_wUserID;			// ���[�U�h�c�ݒ�
	iRtn = SRS_Mng_GetUserByID( wUserID, cUserItem );
	if( iRtn == -1 )
	{
		lpSendFix->IF_wRequest = IF_REQ_USER_DELETE;
		lpSendFix->IF_wResult = IF_CMP_SEQERR;		// �v���V�[�P���X�G���[
		lpSendFix->IF_iInformationSize = 0;
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( iRtn );
	}
	lpUserItem = (struct SRS_USER_ITEM *)cUserItem;
	wLobbyID = lpUserItem->USI_wLobbyID;			// ���r�[�h�c�ݒ�

	iRtn = SRS_Mng_UserDelete( lpSendFix );	// ���[�U�폜
	if( iRtn == -1 )						// ���[�U�폜���s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_USER_DELETE;
		lpSendFix->IF_iInformationSize = 0;
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( iRtn );
	}

	//=======================================
	//  ����T�C�Y�Z�o
	//=======================================
	lpRegInfo = (struct SRS_REG_INFO *)cRegInfo;
	iRtn = SRS_Reg_GetComInfo( lpRegInfo );		// ���ʊǗ������荞��
	if( iRtn == -1 )							// ��荞�ݎ��s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_USER_DELETE;	// ���[�U�폜
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;		// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		sprintf( cMsg, "SRS_Lby_ReqUserDelete : SRS_Reg_GetComInfo error" );
		SRS_Dbg_WriteLog( cMsg );
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( -1 );
	}
	szSendSize = (size_t)(SRS_IF_FIX_SZ + SRS_IF_RENEWAL_SZ + 	// ���T�C�Y
						  SRS_INFO_USER_SZ + SRS_INFO_LOBBY_SZ );

	//=======================================
	//  ���l��
	//=======================================
	lpSendData = (char *)malloc( szSendSize );
	if( lpSendData == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_USER_DELETE;		// �v���ݒ�
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;			// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( 0 );
	}
	memset( lpSendData, 0, szSendSize );
	//=======================================
	//  �����쐬
	//=======================================
	//
	// �ŗL��ݒ�
	lpSendFix = (struct SRS_IF_FIX *)lpSendData;
    lpSendFix->IF_wRequest = IF_REQ_USER_DELETE;					// �v���ݒ�
	lpSendFix->IF_wResult = IF_CMP_SUCCESS;							// ���ʐ���
	lpSendFix->IF_iInformationSize = szSendSize - SRS_IF_FIX_SZ;	//
	lpSendFix->IF_iPortNo = lpIfFix->IF_iPortNo;					// �|�[�g�ԍ��R�s�[
	lpSendFix->IF_iIp = lpIfFix->IF_iIp;							// IP�A�h���X�R�s�[

	//
	// �X�V����ݒ�
	lpSendRenewal = (struct SRS_IF_RENEWAL *)lpSendFix->IF_cInformation;
	lpSendRenewal->IF_RE_wUserInfoNo  = 1;		// ���[�U���ݒ�
	lpSendRenewal->IF_RE_wLobbyInfoNo = 1;		// ���r�[���ݒ�

	iUserSize  = SRS_Mng_GetMngSize( SRS_MNG_USER );			// ���[�U����T�C�Y
	iLobbySize = SRS_Mng_GetMngSize( SRS_MNG_LOBBY );	    	// ���r�[����T�C�Y
	// ���[�U���
	lpUserInfo = (char *)malloc( iUserSize );
	if( lpUserInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		return( 0 );
	}
	
	// ���r�[����
	lpLobbyInfo = (char *)malloc( iLobbySize );
	if( lpLobbyInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		return( 0 );
	}

	//
	// �S���l��
	iRtn = SRS_Mng_AllGet( lpUserInfo, lpLobbyInfo, 0, 0 );
	if( iRtn == -1 )							// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_SEQERR;	// �v���V�[�P���X�G���[
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		return( 0 );
	}

	// �Ǘ����ݒ�
	lpSendUser 	= (struct SRS_INFO_USER *)lpUserInfo;
	lpSendLobby = (struct SRS_INFO_LOBBY *)lpLobbyInfo;
	iPoint = 0;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpSendUser, SRS_INFO_USER_COM_SZ );
	iPoint = SRS_INFO_USER_COM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint],
             lpSendUser->US_info_user[wUserID].USI_cClientName, SRS_USER_ITEM_SZ );
	iPoint = iPoint + SRS_USER_ITEM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpLobbyInfo, SRS_INFO_LOBBY_COM_SZ );
	iPoint = iPoint + SRS_INFO_LOBBY_COM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint],
            (char *)&lpSendLobby->LB_info_lobby[wLobbyID].LBI_iLockword, SRS_LOBBY_ITEM_SZ );

	//=======================================
	//  �������M
	//=======================================
	sprintf( cMsg, "SRS_Lby_UserDelete : �������M %d bytes", szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	SRS_Dbg_WriteData( cMsg, lpSendData, szSendSize, "ab" );

	iRtn = SRS_Net_SendReq( iClientIP, lpSendData, (size_t)szSendSize );
	realloc( lpUserInfo, 0 );
	realloc( lpLobbyInfo, 0 );

	//=======================================
	//  �X�V���z�M
	//=======================================
    lpSendFix->IF_wRequest = IF_REQ_RENEWAL;					// �v���ݒ�
	strcpy( cSendMode, SRS_SEND_MODE_NO );
	iRtn = SRS_Reg_GetInfo( SRS_REG_KEY_COM, SRS_REG_SEND, cSendMode );
	if( (strncmp(cSendMode, SRS_SEND_MODE_NO, sizeof(SRS_SEND_MODE_NO))) == 0 )	// �z�M�Ȃ��H
	{
		realloc( lpSendData, 0 );
		return( 0 );
	}
	//
    // �z�M���쐬
	//
	sprintf( cMsg, "SRS_Lby_UserDelete : �X�V���z�M %d bytes",szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	iRtn = SRS_Lby_SendInfo( wUserID, wLobbyID, lpSendData, (size_t)szSendSize );

	realloc( lpSendData, 0 );
	return( 0 );
}

/*=============================================================================
     �֐�    �F���r�[�Q���v��
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqLobbyEntry( struct SRS_IF_FIX *lpIfFix )
{
	WORD	wLobbyID;							// ���r�[�h�c
	int		iRtn;								// 
	int		iClientIP;							// ���M�N���C���g�h�o
	int		iUserSize;							// ���[�U����T�C�Y
	int		iLobbySize;							// ���r�[����T�C�Y
	int		iPoint;								//
	size_t	szSendSize;							// ���M�T�C�Y
	struct	SRS_IF_FIX 		*lpSendFix;			// �C���^�t�F�[�X��|�C���^
	struct	SRS_IF_LOBBY_ENTRY *lpLobbyEntry;	// ���r�[�Q������
	struct	SRS_REG_INFO	*lpRegInfo;			// ���ʏ���|�C���^
	struct	SRS_IF_RENEWAL 	*lpSendRenewal;		// ���M�p�X�V����|�C���^
	struct	SRS_INFO_USER	*lpSendUser;		// ���M�p���[�U�Ǘ�����|�C���^
	struct	SRS_INFO_LOBBY	*lpSendLobby;		// ���M�p���r�[�Ǘ�����|�C���^
	char	*lpSendData;						// ���M�f�[�^��|�C���^
	char	*lpUserInfo;						// ���[�U�Ǘ�����|�C���^
	char	*lpLobbyInfo;						// ���r�[�Ǘ�����|�C���^
	char	cRegInfo[SRS_REG_INFO_SZ];			// ���ʏ���
	char	cIfFix[SRS_IF_FIX_SZ + SRS_IF_LOBBY_ENTRY_SZ];	// ���r�[�Q������
	char	cSendMode[4];
	char	cMsg[256];


	sprintf( cMsg, "SRS_Lby_ReqLobbyEntry : ���r�[�Q�� 0x%04x",lpIfFix->IF_wRequest);
	SRS_Dbg_WriteLog( cMsg );

	memset( cIfFix, 0, SRS_IF_FIX_SZ + SRS_IF_LOBBY_ENTRY_SZ );
	memcpy( cIfFix, (char *)lpIfFix, SRS_IF_FIX_SZ + SRS_IF_LOBBY_ENTRY_SZ );
	lpSendFix = (struct SRS_IF_FIX *)cIfFix;
	iClientIP = lpIfFix->IF_iIp;

	//=======================================
	//  ���r�[�Q��
	//=======================================
	lpLobbyEntry = (struct SRS_IF_LOBBY_ENTRY *)lpIfFix->IF_cInformation;
	wUserID  = lpLobbyEntry->IF_LE_wUserID;			// ���[�U�h�c�ݒ�
	wLobbyID = lpLobbyEntry->IF_LE_wLobbyID;		// ���r�[�h�c�ݒ�i�Q����j

	iRtn = SRS_Mng_LobbyEntry( lpSendFix );			// ���r�[�Q��
	if( iRtn == -1 )								// ���r�[�Q�����s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_LOBBY_ENTRY;
		lpSendFix->IF_iInformationSize = 0;
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( iRtn );
	}

	//=======================================
	//  ����T�C�Y�Z�o
	//=======================================
	lpRegInfo = (struct SRS_REG_INFO *)cRegInfo;
	iRtn = SRS_Reg_GetComInfo( lpRegInfo );		// ���ʊǗ������荞��
	if( iRtn == -1 )							// ��荞�ݎ��s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_LOBBY_ENTRY;	// 
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;		// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		sprintf( cMsg, "SRS_Lby_ReqLobbyEntry : SRS_Reg_GetComInfo error" );
		SRS_Dbg_WriteLog( cMsg );
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( -1 );
	}
	iUserSize  = SRS_Mng_GetMngSize( SRS_MNG_USER );			// ���[�U����T�C�Y
	iLobbySize = SRS_Mng_GetMngSize( SRS_MNG_LOBBY );			// ���r�[����T�C�Y

	// �ʒm��񁁂P���[�U���{�S���r�[���
	szSendSize = (size_t)(SRS_IF_FIX_SZ + SRS_IF_RENEWAL_SZ + 	// ���T�C�Y
						  SRS_INFO_USER_SZ + iLobbySize );

	//=======================================
	//  ���l��
	//=======================================
	lpSendData = (char *)malloc( szSendSize );
	if( lpSendData == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_LOBBY_ENTRY;		// �v���ݒ�
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;			// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( 0 );
	}
	memset( lpSendData, 0, szSendSize );
	//=======================================
	//  �����쐬
	//=======================================
	//
	// �ŗL��ݒ�
	lpSendFix = (struct SRS_IF_FIX *)lpSendData;
    lpSendFix->IF_wRequest = IF_REQ_LOBBY_ENTRY;					// �v���ݒ�
	lpSendFix->IF_wResult = IF_CMP_SUCCESS;							// ���ʐ���
	lpSendFix->IF_iInformationSize = szSendSize - SRS_IF_FIX_SZ;	//
	lpSendFix->IF_iPortNo = lpIfFix->IF_iPortNo;					// �|�[�g�ԍ��R�s�[
	lpSendFix->IF_iIp = lpIfFix->IF_iIp;							// IP�A�h���X�R�s�[

	//
	// �X�V����ݒ�
	lpSendRenewal = (struct SRS_IF_RENEWAL *)lpSendFix->IF_cInformation;
	lpSendRenewal->IF_RE_wUserInfoNo  = 1;						// ���[�U���ݒ�
	lpSendRenewal->IF_RE_wLobbyInfoNo = lpRegInfo->wMaxLobby;	// �ő働�r�[���ݒ�

	// ���[�U����l��
	lpUserInfo = (char *)malloc( iUserSize );
	if( lpUserInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		return( 0 );
	}
	
	// ���r�[����l��
	lpLobbyInfo = (char *)malloc( iLobbySize );
	if( lpLobbyInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		return( 0 );
	}

	//
	// �S���l��
	iRtn = SRS_Mng_AllGet( lpUserInfo, lpLobbyInfo, 0, 0 );
	if( iRtn == -1 )							// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_SEQERR;	// �v���V�[�P���X�G���[
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		return( 0 );
	}

	// �Ǘ����ݒ�
	lpSendUser 	= (struct SRS_INFO_USER *)lpUserInfo;
	lpSendLobby = (struct SRS_INFO_LOBBY *)lpLobbyInfo;
	iPoint = 0;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpSendUser, SRS_INFO_USER_COM_SZ );
	iPoint = SRS_INFO_USER_COM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint],
             lpSendUser->US_info_user[wUserID].USI_cClientName, SRS_USER_ITEM_SZ );
	iPoint = iPoint + SRS_USER_ITEM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpLobbyInfo, iLobbySize );

	//=======================================
	//  �������M
	//=======================================
	sprintf( cMsg, "SRS_Lby_LobbyEntry : �������M %d bytes", szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	SRS_Dbg_WriteData( cMsg, lpSendData, szSendSize, "ab" );

	iRtn = SRS_Net_SendReq( iClientIP, lpSendData, (size_t)szSendSize );

	realloc( lpUserInfo, 0 );
	realloc( lpLobbyInfo, 0 );
	//=======================================
	//  �X�V���z�M
	//=======================================
	strcpy( cSendMode, SRS_SEND_MODE_NO );
	iRtn = SRS_Reg_GetInfo( SRS_REG_KEY_COM, SRS_REG_SEND, cSendMode );
	if( (strncmp(cSendMode, SRS_SEND_MODE_NO, sizeof(SRS_SEND_MODE_NO))) == 0 )	// �z�M�Ȃ��H
	{
		realloc( lpSendData, 0 );
		return( 0 );
	}
	//
    // �z�M���쐬
	//
    lpSendFix->IF_wRequest = IF_REQ_RENEWAL;					// �v���ݒ�
	sprintf( cMsg, "SRS_Lby_LobbyEntry : �X�V���z�M %d bytes",szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	iRtn = SRS_Lby_SendInfo( wUserID, wLobbyID, lpSendData, (size_t)szSendSize );

	realloc( lpSendData, 0 );
	return( 0 );
}

/*=============================================================================
     �֐�    �F���r�[�E�ޗv��
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqLobbyDefect( struct SRS_IF_FIX *lpIfFix )
{
	WORD	wLobbyID;							// ���r�[�h�c
	int		iRtn;								// 
	int		iClientIP;							// ���M�N���C���g�h�o
	int		iUserSize;							// ���[�U����T�C�Y
	int		iLobbySize;							// ���r�[����T�C�Y
	int		iPoint;								//
	size_t	szSendSize;							// ���M�T�C�Y
	struct	SRS_IF_FIX 		*lpSendFix;			// �C���^�t�F�[�X��|�C���^
	struct	SRS_IF_LOBBY_DEFECT *lpLobbyDefect;	// ���r�[�E�ޏ���
	struct	SRS_REG_INFO	*lpRegInfo;			// ���ʏ���|�C���^
	struct	SRS_IF_RENEWAL 	*lpSendRenewal;		// ���M�p�X�V����|�C���^
	struct	SRS_INFO_USER	*lpSendUser;		// ���M�p���[�U�Ǘ�����|�C���^
	struct	SRS_INFO_LOBBY	*lpSendLobby;		// ���M�p���r�[�Ǘ�����|�C���^
	char	*lpSendData;						// ���M�f�[�^��|�C���^
	char	*lpUserInfo;						// ���[�U�Ǘ�����|�C���^
	char	*lpLobbyInfo;						// ���r�[�Ǘ�����|�C���^
	char	cRegInfo[SRS_REG_INFO_SZ];			// ���ʏ���
	char	cIfFix[SRS_IF_FIX_SZ + SRS_IF_LOBBY_DEFECT_SZ];	// ���r�[�E�ޏ���
	char	cSendMode[4];
	char	cMsg[256];


	sprintf( cMsg, "SRS_Lby_ReqLobbyDefect : ���r�[�E�� 0x%04x",lpIfFix->IF_wRequest);
	SRS_Dbg_WriteLog( cMsg );

	memset( cIfFix, 0, SRS_IF_FIX_SZ + SRS_IF_LOBBY_DEFECT_SZ );
	memcpy( cIfFix, (char *)lpIfFix, SRS_IF_FIX_SZ + SRS_IF_LOBBY_DEFECT_SZ );
	lpSendFix = (struct SRS_IF_FIX *)cIfFix;
	iClientIP = lpIfFix->IF_iIp;

	//=======================================
	//  ���r�[�E��
	//=======================================
	lpLobbyDefect = (struct SRS_IF_LOBBY_DEFECT *)lpIfFix->IF_cInformation;
	wUserID  = lpLobbyDefect->IF_LD_wUserID;		// ���[�U�h�c�ݒ�
	wLobbyID = lpLobbyDefect->IF_LD_wLobbyID;		// ���r�[�h�c�ݒ�
	iRtn = SRS_Mng_LobbyDefect( lpSendFix );		// ���r�[�E��
	if( iRtn == -1 )								// ���r�[�E�ގ��s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_LOBBY_DEFECT;
		lpSendFix->IF_iInformationSize = 0;
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( iRtn );
	}

	//=======================================
	//  ����T�C�Y�Z�o
	//=======================================
	lpRegInfo = (struct SRS_REG_INFO *)cRegInfo;
	iRtn = SRS_Reg_GetComInfo( lpRegInfo );		// ���ʊǗ������荞��
	if( iRtn == -1 )							// ��荞�ݎ��s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_LOBBY_DEFECT;	// ���r�[�E��
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;		// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		sprintf( cMsg, "SRS_Lby_ReqLobbyDefect : SRS_Reg_GetComInfo error" );
		SRS_Dbg_WriteLog( cMsg );
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( -1 );
	}
	iUserSize  = SRS_Mng_GetMngSize( SRS_MNG_USER );			// ���[�U����T�C�Y
	iLobbySize = SRS_Mng_GetMngSize( SRS_MNG_LOBBY );			// ���r�[����T�C�Y

	// �ʒm��񁁂P���[�U���{�S���r�[���
	szSendSize = (size_t)(SRS_IF_FIX_SZ + SRS_IF_RENEWAL_SZ + 	// ���T�C�Y
						  SRS_INFO_USER_SZ + iLobbySize );

	//=======================================
	//  ���l��
	//=======================================
	lpSendData = (char *)malloc( szSendSize );
	if( lpSendData == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_LOBBY_DEFECT;		// �v���ݒ�
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;			// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( 0 );
	}
	memset( lpSendData, 0, szSendSize );
	//=======================================
	//  �����쐬
	//=======================================
	//
	// �ŗL��ݒ�
	lpSendFix = (struct SRS_IF_FIX *)lpSendData;
    lpSendFix->IF_wRequest = IF_REQ_LOBBY_DEFECT;					// �v���ݒ�
	lpSendFix->IF_wResult = IF_CMP_SUCCESS;							// ���ʐ���
	lpSendFix->IF_iInformationSize = szSendSize - SRS_IF_FIX_SZ;	//
	lpSendFix->IF_iPortNo = lpIfFix->IF_iPortNo;					// �|�[�g�ԍ��R�s�[
	lpSendFix->IF_iIp = lpIfFix->IF_iIp;							// IP�A�h���X�R�s�[

	//
	// �X�V����ݒ�
	lpSendRenewal = (struct SRS_IF_RENEWAL *)lpSendFix->IF_cInformation;
	lpSendRenewal->IF_RE_wUserInfoNo  = 1;							// ���[�U���ݒ�
	lpSendRenewal->IF_RE_wLobbyInfoNo = lpRegInfo->wMaxLobby;	// �ő働�r�[���ݒ�

	// ���[�U����l��
	lpUserInfo = (char *)malloc( iUserSize );
	if( lpUserInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		return( 0 );
	}
	
	// ���r�[����l��
	lpLobbyInfo = (char *)malloc( iLobbySize );
	if( lpLobbyInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		return( 0 );
	}

	//
	// �S���l��
	iRtn = SRS_Mng_AllGet( lpUserInfo, lpLobbyInfo, 0, 0 );
	if( iRtn == -1 )							// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_SEQERR;	// �v���V�[�P���X�G���[
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		return( 0 );
	}

	// �Ǘ����ݒ�
	lpSendUser 	= (struct SRS_INFO_USER *)lpUserInfo;
	lpSendLobby = (struct SRS_INFO_LOBBY *)lpLobbyInfo;
	iPoint = 0;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpSendUser, SRS_INFO_USER_COM_SZ );
	iPoint = SRS_INFO_USER_COM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint],
             lpSendUser->US_info_user[wUserID].USI_cClientName, SRS_USER_ITEM_SZ );
	iPoint = iPoint + SRS_USER_ITEM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpLobbyInfo, iLobbySize );

	//=======================================
	//  �������M
	//=======================================
	sprintf( cMsg, "SRS_Lby_LobbyDefect : �������M %d bytes", szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	SRS_Dbg_WriteData( cMsg, lpSendData, szSendSize, "ab" );

	iRtn = SRS_Net_SendReq( iClientIP, lpSendData, (size_t)szSendSize );

	realloc( lpUserInfo, 0 );
	realloc( lpLobbyInfo, 0 );
	//=======================================
	//  �X�V���z�M
	//=======================================
    lpSendFix->IF_wRequest = IF_REQ_RENEWAL;					// �v���ݒ�
	strcpy( cSendMode, SRS_SEND_MODE_NO );
	iRtn = SRS_Reg_GetInfo( SRS_REG_KEY_COM, SRS_REG_SEND, cSendMode );
	if( (strncmp(cSendMode, SRS_SEND_MODE_NO, sizeof(SRS_SEND_MODE_NO))) == 0 )	// �z�M�Ȃ��H
	{
		realloc( lpSendData, 0 );
		return( 0 );
	}
	//
    // �z�M���쐬
	//
	if( wLobbyID == SRS_ID_MAINLOBBY )		// ���C�����r�[�E�ށH
	{
		return( 0 );
	}
	sprintf( cMsg, "SRS_Lby_LobbyDefect : �X�V���z�M %d bytes",szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	iRtn = SRS_Lby_SendInfo( wUserID, wLobbyID, lpSendData, (size_t)szSendSize );
	iRtn = SRS_Lby_SendInfo( wUserID, (WORD)SRS_ID_MAINLOBBY, lpSendData, (size_t)szSendSize );
	realloc( lpSendData, 0 );
	return( 0 );
}

/*=============================================================================
     �֐�    �F�O���[�v�쐬�v��
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqGroupMake( struct SRS_IF_FIX *lpIfFix )
{
	WORD	wLobbyID;							// ���r�[�h�c
	int		iRtn;								// 
	int		iClientIP;							// ���M�N���C���g�h�o
	int		iUserSize;							// ���[�U����T�C�Y
	int		iLobbySize;							// ���r�[����T�C�Y
	int		iGroupSize;							// �O���[�v����T�C�Y
	int		iPoint;								//
	size_t	szSendSize;							// ���M�T�C�Y
	struct	SRS_IF_FIX 		*lpSendFix;			// �C���^�t�F�[�X��|�C���^
	struct	SRS_IF_GROUP_MAKE *lpGroupMake;		// �O���[�v�쐬����
	struct	SRS_REG_INFO	*lpRegInfo;			// ���ʏ���|�C���^
	struct	SRS_IF_RENEWAL 	*lpSendRenewal;		// ���M�p�X�V����|�C���^
	struct	SRS_INFO_USER	*lpSendUser;		// ���M�p���[�U�Ǘ�����|�C���^
	struct	SRS_INFO_LOBBY	*lpSendLobby;		// ���M�p���r�[�Ǘ�����|�C���^
	struct  SRS_INFO_GROUP  *lpSendGroup;		// ���M�p�O���[�v�Ǘ�����|�C���^
	struct	SRS_USER_ITEM   *lpUserItem;		// ���[�U����|�C���^
	char	*lpSendData;						// ���M�f�[�^��|�C���^
	char	*lpUserInfo;						// ���[�U�Ǘ�����|�C���^
	char	*lpLobbyInfo;						// ���r�[�Ǘ�����|�C���^
	char	*lpGroupInfo;						// �O���[�v�Ǘ�����|�C���^
	char	cRegInfo[SRS_REG_INFO_SZ];			// ���ʏ���
	char	cUserItem[SRS_USER_ITEM_SZ];		// ���[�U����
	char	cIfFix[SRS_IF_FIX_SZ + SRS_IF_GROUP_MAKE_SZ];	// �O���[�v�쐬����
	char	cSendMode[4];
	char	cMsg[256];


	sprintf( cMsg, "SRS_Lby_ReqGroupMake : �O���[�v�쐬 0x%04x",lpIfFix->IF_wRequest);
	SRS_Dbg_WriteLog( cMsg );

	memset( cIfFix, 0, SRS_IF_FIX_SZ + SRS_IF_GROUP_MAKE_SZ );
	memcpy( cIfFix, (char *)lpIfFix, SRS_IF_FIX_SZ + SRS_IF_GROUP_MAKE_SZ );
	lpSendFix = (struct SRS_IF_FIX *)cIfFix;
	iClientIP = lpIfFix->IF_iIp;

	//=======================================
	//  �O���[�v�쐬
	//=======================================
	lpGroupMake = (struct SRS_IF_GROUP_MAKE *)lpIfFix->IF_cInformation;
	wUserID  = lpGroupMake->IF_GM_wUserID;			// ���[�U�h�c�ݒ�
	iRtn = SRS_Mng_GetUserByID( wUserID, cUserItem );
	if( iRtn == -1 )
	{
		SRS_Dbg_WriteLog( "SRS_Lby_GroupMake : SRS_Mng_GetUserByID error" );
		lpSendFix->IF_wRequest = IF_REQ_GROUP_MAKE;
		lpSendFix->IF_wResult = IF_CMP_SEQERR;		// �v���V�[�P���X�G���[
		lpSendFix->IF_iInformationSize = 0;
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( iRtn );
	}
	lpUserItem = (struct SRS_USER_ITEM *)cUserItem;
	wLobbyID = lpUserItem->USI_wLobbyID;			// ���r�[�h�c�ݒ�

	iRtn = SRS_Mng_GroupMake( lpSendFix );			// �O���[�v�쐬
	if( iRtn == -1 )								// �O���[�v�쐬���s�H
	{
		sprintf( cMsg, "SRS_Lby_GroupMake : SRS_Mng_GroupMake error %04x",lpSendFix->IF_wResult );
		SRS_Dbg_WriteLog( cMsg );
		lpSendFix->IF_wRequest = IF_REQ_GROUP_MAKE;
		lpSendFix->IF_iInformationSize = 0;
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( iRtn );
	}

	//=======================================
	//  ����T�C�Y�Z�o
	//=======================================
	lpRegInfo = (struct SRS_REG_INFO *)cRegInfo;
	iRtn = SRS_Reg_GetComInfo( lpRegInfo );		// ���ʊǗ������荞��
	if( iRtn == -1 )							// ��荞�ݎ��s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_GROUP_MAKE;
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;		// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		sprintf( cMsg, "SRS_Lby_ReqGroupMake : SRS_Reg_GetComInfo error" );
		SRS_Dbg_WriteLog( cMsg );
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( -1 );
	}
	iUserSize  = SRS_Mng_GetMngSize( SRS_MNG_USER );			// ���[�U����T�C�Y
	iLobbySize = SRS_Mng_GetMngSize( SRS_MNG_LOBBY );			// ���r�[����T�C�Y
	iGroupSize = SRS_Mng_GetMngSize( SRS_MNG_GROUP );			// �O���[�v����T�C�Y

	// �ʒm��񁁂P���[�U���{�S���r�[���{�S�O���[�v���
	szSendSize = (size_t)(SRS_IF_FIX_SZ + SRS_IF_RENEWAL_SZ + 	// ���T�C�Y
						  SRS_INFO_USER_SZ + iLobbySize + iGroupSize );

	//=======================================
	//  ���M����l���i�����^�z�M�p�j
	//=======================================
	lpSendData = (char *)malloc( szSendSize );
	if( lpSendData == NULL )	// �l�����s�H
	{
		sprintf( cMsg,"SRS_Lby_GroupMake : malloc(lpSendData) error %d",szSendSize );
		SRS_Dbg_WriteLog( cMsg );
		lpSendFix->IF_wRequest = IF_REQ_GROUP_MAKE;		// �v���ݒ�
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;		// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( 0 );
	}
	memset( lpSendData, 0, szSendSize );
	//=======================================
	//  �����쐬
	//=======================================
	//
	// �ŗL��ݒ�
	lpSendFix = (struct SRS_IF_FIX *)lpSendData;
    lpSendFix->IF_wRequest = IF_REQ_GROUP_MAKE;						// �v���ݒ�
	lpSendFix->IF_wResult = IF_CMP_SUCCESS;							// ���ʐ���
	lpSendFix->IF_iInformationSize = szSendSize - SRS_IF_FIX_SZ;	//
	lpSendFix->IF_iPortNo = lpIfFix->IF_iPortNo;					// �|�[�g�ԍ��R�s�[
	lpSendFix->IF_iIp = lpIfFix->IF_iIp;							// IP�A�h���X�R�s�[

	//
	// �X�V����ݒ�
	lpSendRenewal = (struct SRS_IF_RENEWAL *)lpSendFix->IF_cInformation;
	lpSendRenewal->IF_RE_wUserInfoNo  = 1;							// ���[�U���ݒ�
	lpSendRenewal->IF_RE_wLobbyInfoNo = lpRegInfo->wMaxLobby;		// �ő働�r�[���ݒ�
	lpSendRenewal->IF_RE_wGroupInfoNo = lpRegInfo->wMaxGroup + 1;	// �ő�O���[�v���ݒ�

	// ���[�U����l��
	lpUserInfo = (char *)malloc( iUserSize );
	if( lpUserInfo == NULL )	// �l�����s�H
	{
		sprintf( cMsg,"SRS_Lby_GroupMake : malloc(lpUserInfo) error %d",iUserSize );
		SRS_Dbg_WriteLog( cMsg );
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		return( 0 );
	}
	
	// ���r�[����l��
	lpLobbyInfo = (char *)malloc( iLobbySize );
	if( lpLobbyInfo == NULL )	// �l�����s�H
	{
		sprintf( cMsg,"SRS_Lby_GroupMake : malloc(lpLobbyInfo) error %d",iLobbySize );
		SRS_Dbg_WriteLog( cMsg );
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		return( 0 );
	}

	// �O���[�v����l��
	lpGroupInfo = (char *)malloc( iGroupSize );
	if( lpGroupInfo == NULL )	// �l�����s�H
	{
		sprintf( cMsg,"SRS_Lby_GroupMake : malloc(lpGroupInfo) error %d",iGroupSize );
		SRS_Dbg_WriteLog( cMsg );

		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		return( 0 );
	}

	//
	// �S���l��
	iRtn = SRS_Mng_AllGet( lpUserInfo, lpLobbyInfo, lpGroupInfo, 0 );
	if( iRtn == -1 )							// �l�����s�H
	{
		SRS_Dbg_WriteLog( "SRS_Lby_GroupMake : SRS_Mng_AllGet error" );

		lpSendFix->IF_wResult = IF_CMP_SEQERR;	// �v���V�[�P���X�G���[
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		realloc( lpGroupInfo, 0 );
		return( 0 );
	}

	// �Ǘ����ݒ�
	lpSendUser 	= (struct SRS_INFO_USER *)lpUserInfo;
	lpSendLobby = (struct SRS_INFO_LOBBY *)lpLobbyInfo;
	lpSendGroup = (struct SRS_INFO_GROUP *)lpGroupInfo;

	iPoint = 0;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpSendUser, SRS_INFO_USER_COM_SZ );
	iPoint = SRS_INFO_USER_COM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint],
             lpSendUser->US_info_user[wUserID].USI_cClientName, SRS_USER_ITEM_SZ );
	iPoint = iPoint + SRS_USER_ITEM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpLobbyInfo, iLobbySize );
	
	sprintf( cMsg, "GroupMake: %d lobbyinfo", iPoint);
	SRS_Dbg_WriteLog( cMsg );
	iPoint = iPoint + iLobbySize;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpGroupInfo, iGroupSize );
	sprintf( cMsg, "GroupMake: %d Groupinfo", iPoint);
	SRS_Dbg_WriteLog( cMsg );

	//=======================================
	//  �������M
	//=======================================
	sprintf( cMsg, "SRS_Lby_GroupMake : �������M %d bytes", szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	SRS_Dbg_WriteData( cMsg, lpSendData, szSendSize, "ab" );

	iRtn = SRS_Net_SendReq( iClientIP, lpSendData, (size_t)szSendSize );

	realloc( lpUserInfo, 0 );
	realloc( lpLobbyInfo, 0 );
	realloc( lpGroupInfo, 0 );
	
	//=======================================
	//  �X�V���z�M
	//=======================================
    lpSendFix->IF_wRequest = IF_REQ_RENEWAL;					// �v���ݒ�
	strcpy( cSendMode, SRS_SEND_MODE_NO );
	iRtn = SRS_Reg_GetInfo( SRS_REG_KEY_COM, SRS_REG_SEND, cSendMode );
	if( (strncmp(cSendMode, SRS_SEND_MODE_NO, sizeof(SRS_SEND_MODE_NO))) == 0 )	// �z�M�Ȃ��H
	{
		realloc( lpSendData, 0 );
		return( 0 );
	}
	//
    // �z�M���쐬
	//
	sprintf( cMsg, "SRS_Lby_GroupMake : �X�V���z�M %d bytes",szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	iRtn = SRS_Lby_SendInfo( wUserID, wLobbyID, lpSendData, (size_t)szSendSize );

	realloc( lpSendData, 0 );
	return( 0 );
}

/*=============================================================================
     �֐�    �F�O���[�v�Q���v��
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqGroupJoin( struct SRS_IF_FIX *lpIfFix )
{
	WORD	wLobbyID;							// ���r�[�h�c
	WORD	wGroupID;							// �O���[�v�h�c
	int		iRtn;								// 
	int		iClientIP;							// ���M�N���C���g�h�o
	int		iUserSize;							// ���[�U����T�C�Y
	int		iGroupSize;							// �O���[�v����T�C�Y
	int		iPoint;								//
	size_t	szSendSize;							// ���M�T�C�Y
	struct	SRS_IF_FIX 		*lpSendFix;			// �C���^�t�F�[�X��|�C���^
	struct	SRS_IF_GROUP_JOIN *lpGroupJoin;		// �O���[�v�Q������
	struct	SRS_REG_INFO	*lpRegInfo;			// ���ʏ���|�C���^
	struct	SRS_IF_RENEWAL 	*lpSendRenewal;		// ���M�p�X�V����|�C���^
	struct	SRS_INFO_USER	*lpSendUser;		// ���M�p���[�U�Ǘ�����|�C���^
	struct  SRS_INFO_GROUP  *lpSendGroup;		// ���M�p�O���[�v�Ǘ�����|�C���^
	struct	SRS_USER_ITEM   *lpUserItem;		// ���[�U����|�C���^
	char	*lpSendData;						// ���M�f�[�^��|�C���^
	char	*lpUserInfo;						// ���[�U�Ǘ�����|�C���^
	char	*lpGroupInfo;						// �O���[�v�Ǘ�����|�C���^
	char	cRegInfo[SRS_REG_INFO_SZ];			// ���ʏ���
	char	cUserItem[SRS_USER_ITEM_SZ];		// ���[�U����
	char	cIfFix[SRS_IF_FIX_SZ + SRS_IF_GROUP_JOIN_SZ];	// �O���[�v�Q������
	char	cSendMode[4];
	char	cMsg[256];


	sprintf( cMsg, "SRS_Lby_ReqGroupJoin : �O���[�v�Q�� 0x%04x",lpIfFix->IF_wRequest);
	SRS_Dbg_WriteLog( cMsg );

	memset( cIfFix, 0, SRS_IF_FIX_SZ + SRS_IF_GROUP_JOIN_SZ );
	memcpy( cIfFix, (char *)lpIfFix, SRS_IF_FIX_SZ + SRS_IF_GROUP_JOIN_SZ );
	lpSendFix = (struct SRS_IF_FIX *)cIfFix;
	iClientIP = lpIfFix->IF_iIp;

	//=======================================
	//  �O���[�v�Q��
	//=======================================
	lpGroupJoin = (struct SRS_IF_GROUP_JOIN *)lpIfFix->IF_cInformation;
	wUserID  = lpGroupJoin->IF_GJ_wUserID;			// ���[�U�h�c�ݒ�
	wGroupID  = lpGroupJoin->IF_GJ_wGroupID;		// �O���[�v�h�c�ݒ�
	iRtn = SRS_Mng_GetUserByID( wUserID, cUserItem );
	if( iRtn == -1 )
	{
		lpSendFix->IF_wRequest = IF_REQ_GROUP_JOIN;
		lpSendFix->IF_wResult = IF_CMP_SEQERR;		// �v���V�[�P���X�G���[
		lpSendFix->IF_iInformationSize = 0;
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( iRtn );
	}
	lpUserItem = (struct SRS_USER_ITEM *)cUserItem;
	wLobbyID = lpUserItem->USI_wLobbyID;			// ���r�[�h�c�ݒ�

	iRtn = SRS_Mng_GroupJoin( lpSendFix );			// �O���[�v�Q��
	if( iRtn == -1 )								// �O���[�v�Q�����s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_GROUP_JOIN;
		lpSendFix->IF_iInformationSize = 0;
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( iRtn );
	}

	//=======================================
	//  ����T�C�Y�Z�o
	//=======================================
	lpRegInfo = (struct SRS_REG_INFO *)cRegInfo;
	iRtn = SRS_Reg_GetComInfo( lpRegInfo );		// ���ʊǗ������荞��
	if( iRtn == -1 )							// ��荞�ݎ��s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_GROUP_JOIN;
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;		// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		sprintf( cMsg, "SRS_Lby_ReqGroupJoin : SRS_Reg_GetComInfo error" );
		SRS_Dbg_WriteLog( cMsg );
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( -1 );
	}
	iUserSize  = SRS_Mng_GetMngSize( SRS_MNG_USER );			// ���[�U����T�C�Y
	iGroupSize = SRS_Mng_GetMngSize( SRS_MNG_GROUP );			// �O���[�v����T�C�Y

	// �ʒm��񁁂P���[�U���{�Q���O���[�v���
	szSendSize = (size_t)(SRS_IF_FIX_SZ + SRS_IF_RENEWAL_SZ + 	// ���T�C�Y
						  SRS_INFO_USER_SZ + SRS_INFO_GROUP_SZ );

	//=======================================
	//  ���M����l���i�����^�z�M�p�j
	//=======================================
	lpSendData = (char *)malloc( szSendSize );
	if( lpSendData == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_GROUP_JOIN;		// �v���ݒ�
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;		// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( 0 );
	}
	memset( lpSendData, 0, szSendSize );
	//=======================================
	//  �����쐬
	//=======================================
	//
	// �ŗL��ݒ�
	lpSendFix = (struct SRS_IF_FIX *)lpSendData;
    lpSendFix->IF_wRequest = IF_REQ_GROUP_JOIN;						// �v���ݒ�
	lpSendFix->IF_wResult = IF_CMP_SUCCESS;							// ���ʐ���
	lpSendFix->IF_iInformationSize = szSendSize - SRS_IF_FIX_SZ;	//
	lpSendFix->IF_iPortNo = lpIfFix->IF_iPortNo;					// �|�[�g�ԍ��R�s�[
	lpSendFix->IF_iIp = lpIfFix->IF_iIp;							// IP�A�h���X�R�s�[

	//
	// �X�V����ݒ�
	lpSendRenewal = (struct SRS_IF_RENEWAL *)lpSendFix->IF_cInformation;
	lpSendRenewal->IF_RE_wUserInfoNo  = 1;			// ���[�U���ݒ�
	lpSendRenewal->IF_RE_wGroupInfoNo = 1;			// �O���[�v���ݒ�

	// ���[�U����l��
	lpUserInfo = (char *)malloc( iUserSize );
	if( lpUserInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		return( 0 );
	}
	
	// �O���[�v����l��
	lpGroupInfo = (char *)malloc( iGroupSize );
	if( lpGroupInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		return( 0 );
	}

	//
	// �S���l��
	iRtn = SRS_Mng_AllGet( lpUserInfo, 0, lpGroupInfo, 0 );
	if( iRtn == -1 )							// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_SEQERR;	// �v���V�[�P���X�G���[
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpGroupInfo, 0 );
		return( 0 );
	}

	// �Ǘ����ݒ�
	lpSendUser 	= (struct SRS_INFO_USER *)lpUserInfo;
	lpSendGroup = (struct SRS_INFO_GROUP *)lpGroupInfo;

	iPoint = 0;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpSendUser, SRS_INFO_USER_COM_SZ );
	iPoint = SRS_INFO_USER_COM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint],
             lpSendUser->US_info_user[wUserID].USI_cClientName, SRS_USER_ITEM_SZ );
	iPoint = iPoint + SRS_USER_ITEM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpSendGroup, SRS_INFO_GROUP_COM_SZ );
	iPoint = iPoint + SRS_INFO_GROUP_COM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint],
            (char *)&lpSendGroup->GP_info_group[wGroupID].GPI_iLockword, SRS_GROUP_ITEM_SZ );

	//=======================================
	//  �������M
	//=======================================
	sprintf( cMsg, "SRS_Lby_GroupJoin : �������M %d bytes", szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	SRS_Dbg_WriteData( cMsg, lpSendData, szSendSize, "ab" );

	iRtn = SRS_Net_SendReq( iClientIP, lpSendData, (size_t)szSendSize );

	realloc( lpUserInfo, 0 );
	realloc( lpGroupInfo, 0 );
	
	//=======================================
	//  �X�V���z�M
	//=======================================
    lpSendFix->IF_wRequest = IF_REQ_RENEWAL;					// �v���ݒ�
	strcpy( cSendMode, SRS_SEND_MODE_NO );
	iRtn = SRS_Reg_GetInfo( SRS_REG_KEY_COM, SRS_REG_SEND, cSendMode );
	if( (strncmp(cSendMode, SRS_SEND_MODE_NO, sizeof(SRS_SEND_MODE_NO))) == 0 )	// �z�M�Ȃ��H
	{
		realloc( lpSendData, 0 );
		return( 0 );
	}
	//
    // �z�M���쐬
	//
	sprintf( cMsg, "SRS_Lby_GroupJoin : �X�V���z�M %d bytes",szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	iRtn = SRS_Lby_SendInfo( wUserID, wLobbyID, lpSendData, (size_t)szSendSize );

	realloc( lpSendData, 0 );
	return( 0 );
}

/*=============================================================================
     �֐�    �F�O���[�v�E�ޗv��
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqGroupDefect( struct SRS_IF_FIX *lpIfFix )
{
	WORD	wLobbyID;							// ���r�[�h�c
	WORD	wGroupID;							// �O���[�v�h�c
	int		iRtn;								// 
	int		iClientIP;							// ���M�N���C���g�h�o
	int		iUserSize;							// ���[�U����T�C�Y
	int		iLobbySize;							// ���r�[����T�C�Y
	int		iGroupSize;							// �O���[�v����T�C�Y
	int		iPoint;								//
	size_t	szSendSize;							// ���M�T�C�Y
	struct	SRS_IF_FIX 		*lpSendFix;			// �C���^�t�F�[�X��|�C���^
	struct	SRS_IF_GROUP_DEFECT *lpGroupDefect;	// �O���[�v�E�ޏ���
	struct	SRS_REG_INFO	*lpRegInfo;			// ���ʏ���|�C���^
	struct	SRS_IF_RENEWAL 	*lpSendRenewal;		// ���M�p�X�V����|�C���^
	struct	SRS_INFO_USER	*lpSendUser;		// ���M�p���[�U�Ǘ�����|�C���^
	struct	SRS_INFO_LOBBY	*lpSendLobby;		// ���M�p���r�[�Ǘ�����|�C���^
	struct  SRS_INFO_GROUP  *lpSendGroup;		// ���M�p�O���[�v�Ǘ�����|�C���^
	struct	SRS_USER_ITEM   *lpUserItem;		// ���[�U����|�C���^
	char	*lpSendData;						// ���M�f�[�^��|�C���^
	char	*lpUserInfo;						// ���[�U�Ǘ�����|�C���^
	char	*lpLobbyInfo;						// ���r�[�Ǘ�����|�C���^
	char	*lpGroupInfo;						// �O���[�v�Ǘ�����|�C���^
	char	cRegInfo[SRS_REG_INFO_SZ];			// ���ʏ���
	char	cUserItem[SRS_USER_ITEM_SZ];		// ���[�U����
	char	cIfFix[SRS_IF_FIX_SZ + SRS_IF_GROUP_DEFECT_SZ];	// �O���[�v�E�ޏ���
	char	cSendMode[4];
	char	cMsg[256];


	sprintf( cMsg, "SRS_Lby_ReqGroupDefect : �O���[�v�E�� 0x%04x",lpIfFix->IF_wRequest);
	SRS_Dbg_WriteLog( cMsg );

	memset( cIfFix, 0, SRS_IF_FIX_SZ + SRS_IF_GROUP_DEFECT_SZ );
	memcpy( cIfFix, (char *)lpIfFix, SRS_IF_FIX_SZ + SRS_IF_GROUP_DEFECT_SZ );
	lpSendFix = (struct SRS_IF_FIX *)cIfFix;
	iClientIP = lpIfFix->IF_iIp;

	//=======================================
	//  �O���[�v�E��
	//=======================================
	lpGroupDefect = (struct SRS_IF_GROUP_DEFECT *)lpIfFix->IF_cInformation;
	wUserID  = lpGroupDefect->IF_GD_wUserID;			// ���[�U�h�c�ݒ�
	wGroupID  = lpGroupDefect->IF_GD_wGroupID;			// �O���[�v�h�c�ݒ�
	iRtn = SRS_Mng_GetUserByID( wUserID, cUserItem );
	if( iRtn == -1 )
	{
		lpSendFix->IF_wRequest = IF_REQ_GROUP_DEFECT;
		lpSendFix->IF_wResult = IF_CMP_SEQERR;			// �v���V�[�P���X�G���[
		lpSendFix->IF_iInformationSize = 0;
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( iRtn );
	}
	lpUserItem = (struct SRS_USER_ITEM *)cUserItem;
	wLobbyID = lpUserItem->USI_wLobbyID;				// ���r�[�h�c�ݒ�

	iRtn = SRS_Mng_GroupDefect( lpSendFix );			// �O���[�v�E��
	if( iRtn == -1 )									// �O���[�v�E�ގ��s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_GROUP_DEFECT;
		lpSendFix->IF_iInformationSize = 0;
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( iRtn );
	}

	//=======================================
	//  ����T�C�Y�Z�o
	//=======================================
	lpRegInfo = (struct SRS_REG_INFO *)cRegInfo;
	iRtn = SRS_Reg_GetComInfo( lpRegInfo );		// ���ʊǗ������荞��
	if( iRtn == -1 )							// ��荞�ݎ��s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_GROUP_DEFECT;	// 
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;		// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		sprintf( cMsg, "SRS_Lby_ReqGroupDefect : SRS_Reg_GetComInfo error" );
		SRS_Dbg_WriteLog( cMsg );
		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( -1 );
	}
	iUserSize  = SRS_Mng_GetMngSize( SRS_MNG_USER );			// ���[�U����T�C�Y
	iLobbySize = SRS_Mng_GetMngSize( SRS_MNG_LOBBY );			// ���r�[����T�C�Y
	iGroupSize = SRS_Mng_GetMngSize( SRS_MNG_GROUP );			// �O���[�v����T�C�Y

	// �ʒm��񁁂P���[�U���{�P���r�[���{�P�O���[�v���
	szSendSize = (size_t)(SRS_IF_FIX_SZ + SRS_IF_RENEWAL_SZ + 	// ���T�C�Y
						  SRS_INFO_USER_SZ + SRS_INFO_LOBBY_SZ + SRS_INFO_GROUP_SZ );

	//=======================================
	//  ���M����l���i�����^�z�M�p�j
	//=======================================
	lpSendData = (char *)malloc( szSendSize );
	if( lpSendData == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_GROUP_DEFECT;	// �v���ݒ�
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;		// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( 0 );
	}
	memset( lpSendData, 0, szSendSize );
	//=======================================
	//  �����쐬
	//=======================================
	//
	// �ŗL��ݒ�
	lpSendFix = (struct SRS_IF_FIX *)lpSendData;
    lpSendFix->IF_wRequest = IF_REQ_GROUP_DEFECT;					// �v���ݒ�
	lpSendFix->IF_wResult = IF_CMP_SUCCESS;							// ���ʐ���
	lpSendFix->IF_iInformationSize = szSendSize - SRS_IF_FIX_SZ;	//
	lpSendFix->IF_iPortNo = lpIfFix->IF_iPortNo;					// �|�[�g�ԍ��R�s�[
	lpSendFix->IF_iIp = lpIfFix->IF_iIp;							// IP�A�h���X�R�s�[

	//
	// �X�V����ݒ�
	lpSendRenewal = (struct SRS_IF_RENEWAL *)lpSendFix->IF_cInformation;
	lpSendRenewal->IF_RE_wUserInfoNo  = 1;							// ���[�U���ݒ�
	lpSendRenewal->IF_RE_wLobbyInfoNo = lpRegInfo->wMaxLobby + 1;	// ���r�[���ݒ�
	lpSendRenewal->IF_RE_wGroupInfoNo = 1;							// �O���[�v���ݒ�

	// ���[�U����l��
	lpUserInfo = (char *)malloc( iUserSize );
	if( lpUserInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		return( 0 );
	}
	
	// ���r�[����l��
	lpLobbyInfo = (char *)malloc( iLobbySize );
	if( lpLobbyInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		return( 0 );
	}

	// �O���[�v����l��
	lpGroupInfo = (char *)malloc( iGroupSize );
	if( lpGroupInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		return( 0 );
	}

	//
	// �S���l��
	iRtn = SRS_Mng_AllGet( lpUserInfo, lpLobbyInfo, lpGroupInfo, 0 );
	if( iRtn == -1 )							// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_SEQERR;	// �v���V�[�P���X�G���[
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		realloc( lpGroupInfo, 0 );
		return( 0 );
	}

	// �Ǘ����ݒ�
	lpSendUser 	= (struct SRS_INFO_USER *)lpUserInfo;
	lpSendLobby = (struct SRS_INFO_LOBBY *)lpLobbyInfo;
	lpSendGroup = (struct SRS_INFO_GROUP *)lpGroupInfo;

	// ���[�U���ݒ�i�P���[�U�j
	iPoint = 0;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpSendUser, SRS_INFO_USER_COM_SZ );
	iPoint = SRS_INFO_USER_COM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint],
             lpSendUser->US_info_user[wUserID].USI_cClientName, SRS_USER_ITEM_SZ );
	iPoint = iPoint + SRS_USER_ITEM_SZ;

	// ���r�[���ݒ�i�P���r�[�j
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpLobbyInfo, SRS_INFO_LOBBY_COM_SZ );
	iPoint = iPoint + SRS_INFO_LOBBY_COM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint],
            (char *)&lpSendLobby->LB_info_lobby[wLobbyID].LBI_iLockword, SRS_LOBBY_ITEM_SZ );
	iPoint = iPoint + SRS_LOBBY_ITEM_SZ;

	// �O���[�v���ݒ�i�P�O���[�v�j
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpGroupInfo, SRS_INFO_GROUP_COM_SZ );
	iPoint = iPoint + SRS_INFO_GROUP_COM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint],
            (char *)&lpSendGroup->GP_info_group[wGroupID].GPI_iLockword, SRS_GROUP_ITEM_SZ );

	//=======================================
	//  �������M
	//=======================================
	sprintf( cMsg, "SRS_Lby_GroupDefect : �������M %d bytes", szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	SRS_Dbg_WriteData( cMsg, lpSendData, szSendSize, "ab" );

	iRtn = SRS_Net_SendReq( iClientIP, lpSendData, (size_t)szSendSize );

	realloc( lpUserInfo, 0 );
	realloc( lpLobbyInfo, 0 );
	realloc( lpGroupInfo, 0 );
	
	//=======================================
	//  �X�V���z�M
	//=======================================
    lpSendFix->IF_wRequest = IF_REQ_RENEWAL;					// �v���ݒ�
	strcpy( cSendMode, SRS_SEND_MODE_NO );
	iRtn = SRS_Reg_GetInfo( SRS_REG_KEY_COM, SRS_REG_SEND, cSendMode );
	if( (strncmp(cSendMode, SRS_SEND_MODE_NO, sizeof(SRS_SEND_MODE_NO))) == 0 )	// �z�M�Ȃ��H
	{
		realloc( lpSendData, 0 );
		return( 0 );
	}
	//
    // �z�M���쐬
	//
	sprintf( cMsg, "SRS_Lby_GroupDefect : �X�V���z�M %d bytes",szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	iRtn = SRS_Lby_SendInfo( wUserID, wLobbyID, lpSendData, (size_t)szSendSize );

	realloc( lpSendData, 0 );
	return( 0 );
}

/*=============================================================================
     �֐�    �F�Q�[���Q���v��
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqGameJoin( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;							//
	int		iRtnMng;						//
	int		iClientIP;						//
	struct	SRS_IF_FIX *lpSendIfFix;		//
	char	cIfFix[256];					//
	char	cMsg[256];						//

	sprintf( cMsg, "SRS_Lby_ReqGameJoin : �Q�[���Q�� 0x%04x",lpIfFix->IF_wRequest );
	SRS_Dbg_WriteLog( cMsg );
	memset( cIfFix, 0, 256 );
	memcpy( cIfFix, (char *)lpIfFix, SRS_MAX_RCV );
	iRtnMng = SRS_Mng_GameJoin( (struct SRS_IF_FIX *)cIfFix );

	// �������M
	iClientIP = lpIfFix->IF_iIp;
	lpSendIfFix = (struct SRS_IF_FIX *)cIfFix;
	lpSendIfFix->IF_iInformationSize = 0;
	iRtn = SRS_Net_SendReq( iClientIP, cIfFix, (size_t)SRS_IF_FIX_SZ );
	return( iRtn );
}

/*=============================================================================
     �֐�    �F�Q�[���E�ޗv��
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqGameDefect( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;							//
	int		iRtnMng;						//
	int		iClientIP;						//
	struct	SRS_IF_FIX *lpSendIfFix;		//
	char	cIfFix[256];					//
	char	cMsg[256];						//

	sprintf( cMsg, "SRS_Lby_ReqGameDefect : �Q�[���E�� 0x%04x",lpIfFix->IF_wRequest );
	SRS_Dbg_WriteLog( cMsg );
	memset( cIfFix, 0, 256 );
	memcpy( cIfFix, (char *)lpIfFix, SRS_MAX_RCV );
	iRtnMng = SRS_Mng_GameDefect( (struct SRS_IF_FIX *)cIfFix );

	// �������M
	iClientIP = lpIfFix->IF_iIp;
	lpSendIfFix = (struct SRS_IF_FIX *)cIfFix;
	lpSendIfFix->IF_iInformationSize = 0;
	iRtn = SRS_Net_SendReq( iClientIP, cIfFix, (size_t)SRS_IF_FIX_SZ );
	return( iRtn );
}

/*=============================================================================
     �֐�    �F�Q�[���J�n�v��
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqGameStart( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;							//
	int		iRtnMng;						//
	int		iClientIP;						//
	struct	SRS_IF_FIX *lpSendIfFix;		//
	char	cIfFix[256];					//
	char	cMsg[256];						//

	sprintf( cMsg, "SRS_Lby_ReqGameStart : �Q�[���J�n 0x%04x",lpIfFix->IF_wRequest );
	SRS_Dbg_WriteLog( cMsg );
	memset( cIfFix, 0, 256 );
	memcpy( cIfFix, (char *)lpIfFix, SRS_MAX_RCV );
	iRtnMng = SRS_Mng_GameStart( (struct SRS_IF_FIX *)cIfFix );

	// �������M
	iClientIP = lpIfFix->IF_iIp;
	lpSendIfFix = (struct SRS_IF_FIX *)cIfFix;
	lpSendIfFix->IF_iInformationSize = 0;
	iRtn = SRS_Net_SendReq( iClientIP, cIfFix, (size_t)SRS_IF_FIX_SZ );
	return( iRtn );
}

/*=============================================================================
     �֐�    �F�Q�[���I���v��
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqGameEnd( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;							//
	int		iRtnMng;						//
	int		iClientIP;						//
	struct	SRS_IF_FIX *lpSendIfFix;		//
	char	cIfFix[256];					//
	char	cMsg[256];						//

	sprintf( cMsg, "SRS_Lby_ReqGameEnd : �Q�[���I�� 0x%04x",lpIfFix->IF_wRequest );
	SRS_Dbg_WriteLog( cMsg );
	memset( cIfFix, 0, 256 );
	memcpy( cIfFix, (char *)lpIfFix, SRS_MAX_RCV );
	iRtnMng = SRS_Mng_GameEnd( (struct SRS_IF_FIX *)cIfFix );

	// �������M
	iClientIP = lpIfFix->IF_iIp;
	lpSendIfFix = (struct SRS_IF_FIX *)cIfFix;
	lpSendIfFix->IF_iInformationSize = 0;
	iRtn = SRS_Net_SendReq( iClientIP, cIfFix, (size_t)SRS_IF_FIX_SZ );
	return( iRtn );
}

/*=============================================================================
     �֐�    �F�ŗL���Ҕ�v��
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqSaveInfo( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;							//
	char	cMsg[256];

	sprintf( cMsg, "SRS_Lby_ReqSaveInfo : �ŗL���Ҕ� 0x%04x",lpIfFix->IF_wRequest );
	SRS_Dbg_WriteLog( cMsg );
	iRtn = SRS_Mng_SaveInfo( lpIfFix );

	return( iRtn );
}

/*=============================================================================
     �֐�    �F���b�Z�[�W���M�v��
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqSendMsg( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;							//
	int		iDstIP;						// �N���C�A���g�h�o�A�h���X
	int		iSrcIP;
	size_t	szSize;							//
	WORD	wUserID;						// ���M�����[�U�h�c
	struct	SRS_IF_FIX *lpLocalFix;			// ���[�J���C���^�t�F�[�X��|�C���^
	struct	SRS_IF_MESSAGE *lpIfMessage;	// ���b�Z�|�W��|�C���^
	struct	SRS_USER_ITEM *lpUserItem;		// ���[�U����|�C���^
	char	cIfFix[256];					// ���[�J���C���^�t�F�[�X��
	char	cUserItem[SRS_LOBBY_ITEM_SZ];	// ���[�U����
	char	cMsg[256];						


	iSrcIP = (int)lpIfFix->IF_iIp;
	//================================
	// ���r�[�����[�U���l��
	//================================
	sprintf( cMsg, "SRS_Lby_ReqSendMsg : ���b�Z�[�W���M 0x%04x",lpIfFix->IF_wRequest );
	SRS_Dbg_WriteLog( cMsg );

	lpIfMessage = (struct SRS_IF_MESSAGE *)lpIfFix->IF_cInformation;
	wUserID = lpIfMessage->IF_MS_wUserID;
	iRtn = SRS_Mng_GetUserByID( wUserID, cUserItem );		// ���[�U���l��
	if( iRtn == -1 )										// �l�����s�H
	{
		sprintf( cMsg, "SRS_Lby_ReqSendMsg : SRS_Mng_GetUserByID error ID=%04x",wUserID );
		SRS_Dbg_WriteLog( cMsg );
		return( 0 );
	}
	lpUserItem = (struct SRS_USER_ITEM *)cUserItem;
	iDstIP = lpUserItem->USI_iIp;							// ���M��IP�A�h���X

	//===============================
	// �v���쐬
	//===============================
	szSize = (size_t)(SRS_IF_FIX_SZ + SRS_IF_MESSAGE_SZ);
	memset( cIfFix, 0, 256 );
	memcpy( cIfFix, (char *)lpIfFix, szSize );	// ���b�Z�[�W�R�s�[
	lpLocalFix = (struct SRS_IF_FIX *)cIfFix;

	iRtn = SRS_Net_SendReq( iDstIP, (char *)cIfFix, szSize );

	//  �`���b�g���O�o��
	SRS_Lby_ChatLog( iSrcIP, iDstIP, lpIfMessage->IF_MS_cMessage );

	return( 0 );
}

/*=============================================================================
     �֐�    �F�v���V�[�P���X�G���[
     ����    �FlpIfFix  (�C���^�t�F�[�X��|�C���^)
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ʐM�ُ�)

==============================================================================*/
int	 SRS_Lby_ReqSeqError( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;							//
	int		iClientIP;						//
	struct	SRS_IF_FIX	*lpSendIfFix;		//
	char	cIfFix[256];
	char	cMsg[256];						

	//=========================================
	//  �����쐬�����M�i�v���V�[�P���X�G���[�j
	//=========================================
	sprintf( cMsg, "SRS_Lby_ReqSeqError : ??? 0x%04x",lpIfFix->IF_wRequest );
	SRS_Dbg_WriteLog( cMsg );
	iClientIP = lpIfFix->IF_iIp;
	memcpy( cIfFix, (char *)lpIfFix, SRS_MAX_RCV );
	lpSendIfFix = (struct SRS_IF_FIX *)cIfFix;
	lpSendIfFix->IF_wResult = IF_CMP_SEQERR;
	lpSendIfFix->IF_iInformationSize = 0;
	iRtn = SRS_Net_SendReq( iClientIP, cIfFix, (size_t)SRS_IF_FIX_SZ );
	return( iRtn );
}

/*=============================================================================
     �֐�    �F�X�V���z�M�i���r�[�����[�U�ɍX�V����z�M����j
     ����    �FwUserID   �i�X�V�����[�U�h�c)
			   wLobbyID	 �i�z�M���r�[�h�c�j
               lpSendData�i���M���j
			   szSendSize�i���M��񒷁j
     �o��    �F�Ȃ�
     �߂�l  �F 0 (����)
               -1 (�ُ�)

==============================================================================*/
int	 SRS_Lby_SendInfo( WORD wUserID, WORD wLobbyID, char *lpSendData, size_t szSendSize )
{
	int		iRtn;								//
	int		iClientIP;							//
	WORD	wId;								//
	WORD	wSendUserID;						// ���M�惆�[�UID
	struct	SRS_USER_ITEM  *lpUserItem;			//
	struct	SRS_LOBBY_ITEM *lpLobbyItem;		//
	char	cLobbyItem[SRS_LOBBY_ITEM_SZ];		// ���r�[���i�[��
	char	cUserItem[SRS_USER_ITEM_SZ];		// ���[�U���i�[��
	char	cMsg[256];

	sprintf( cMsg, "SRS_Lby_SendInfo start" );
	SRS_Dbg_WriteLog( cMsg );
	SRS_Dbg_WriteData( cMsg, lpSendData, szSendSize, "ab" );

	iRtn = SRS_Mng_GetLobbyByID( wLobbyID, cLobbyItem );	// ���r�[���l��
	if( iRtn == -1 )
	{
		sprintf( cMsg, "SRS_Lby_SendInfo : SRS_Mng_GetLobbyByID error ID=%d",wLobbyID );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}

	//=========================================
	//  �X�V��񑗐M
	//=========================================
	lpLobbyItem = (struct SRS_LOBBY_ITEM *)cLobbyItem;
	lpUserItem = (struct SRS_USER_ITEM *)cUserItem;
	for( wId = 0 ; wId < lpLobbyItem->LBI_wMaxLobbyUser ; wId++ )
	{
		if( (lpLobbyItem->LBI_wUserID[wId] != 0) &&  		// ���[�U�h�c�L�� ��
            (lpLobbyItem->LBI_wUserID[wId] != wUserID) ) 	// �X�V�����[�U�ȊO�H
		{
			wSendUserID = lpLobbyItem->LBI_wUserID[wId];			// ���[�UID�ݒ�
			iRtn = SRS_Mng_GetUserByID( wSendUserID, cUserItem );	// ���[�U���l��
			if( iRtn == 0 )
			{
				iClientIP = lpUserItem->USI_iIp;				// ���M��N���C�A���gIP
				sprintf( cMsg, "SRS_Lby_SendInfo : Send UserID %d",lpLobbyItem->LBI_wUserID[wId] );
	    		SRS_Dbg_WriteLog( cMsg );
				iRtn = SRS_Net_SendReq( iClientIP, lpSendData, szSendSize );	// ��񑗐M
			}
		}
	}
	return( iRtn );
}


/*====================================================================
     �֐�   �F�`���b�g���O�o��

     ����   �FlpstrMsg    �`���b�g���b�Z�[�W
     �o��   �F�Ȃ�
     �߂�l �F�Ȃ�
====================================================================*/
void SRS_Lby_ChatLog(int SrcIP, int DstIP, LPSTR lpstrMsg)
{
	int			iRtn;
    FILE        *fFp = (FILE *)NULL;
    SYSTEMTIME  lpst;                    /* �V�X�e�����ԍ\���̂̃A�h���X */
	char        cLogfile[256];
    char        cDate[64];
    char        cBuff[16];
	unsigned char *lpSaddr = (unsigned char *)&SrcIP;
	unsigned char *lpDaddr = (unsigned char *)&DstIP;



	memset( cBuff, 0, sizeof(cBuff) );
	iRtn = SRS_Dbg_GetRegChatlog( cBuff );			// �`���b�g���O���Q��
	if( strcmp(cBuff, SRS_DBG_CHATLOG_OFF) == 0 )	// �o�͂Ȃ��H
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
	//	hh:mm:ss.mmm 255.255.255.0 to 255.255.255.0 <���b�Z�[�W>
	//	  ���M����     ���M��IP         ���M��IP
#if 0
	sprintf(cDate, "%02d:%02d:%02d.%03d %d.%d.%d.%d to %d.%d.%d.%d",
	               lpst.wHour, lpst.wMinute, lpst.wSecond, lpst.wMilliseconds,
	               lpSaddr[0], lpSaddr[1], lpSaddr[2], lpSaddr[3],
	               lpDaddr[0], lpDaddr[1], lpDaddr[2], lpDaddr[3] );
#endif
	wsprintf(cDate, "%02d:%02d:%02d.%03d from %d.%d.%d.%d to %d.%d.%d.%d : %s\n" ,
	               lpst.wHour, lpst.wMinute, lpst.wSecond, lpst.wMilliseconds,
	               lpSaddr[0], lpSaddr[1], lpSaddr[2], lpSaddr[3],
	               lpDaddr[0], lpDaddr[1], lpDaddr[2], lpDaddr[3], lpstrMsg );
	fputs(cDate, fFp);

//	wsprintf(cDate, "%02d:%02d:%02d.%03d %x to %x : %s\n" ,
//	               lpst.wHour, lpst.wMinute, lpst.wSecond, lpst.wMilliseconds, SrcIP, DstIP, lpstrMsg );
//	fputs(cDate, fFp);
//	fputs(lpstrMsg, fFp);
//	fputc('\n', fFp);
	
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






int	 SRS_Lby_ReqUserDeleteEX( struct SRS_IF_FIX *lpIfFix )
{
	WORD	wLobbyID;
	int		iRtn;							//
	int		iClientIP;						//
	int		iUserSize;						//
	int		iLobbySize;						//
	int		iPoint;							//
	size_t	szSendSize;						//
	struct	SRS_IF_FIX 		*lpSendFix;		//
	struct	SRS_IF_USER_DELETE *lpUserDelete;
	struct	SRS_IF_RENEWAL 	*lpSendRenewal;	// �X�V����|�C���^
	struct	SRS_REG_INFO	*lpRegInfo;		// ���ʏ���|�C���^
	struct	SRS_INFO_USER	*lpSendUser;	//
	struct	SRS_INFO_LOBBY	*lpSendLobby;	//
	struct	SRS_USER_ITEM	*lpUserItem;	//
	char	*lpSendData;					//
	char	*lpUserInfo;					//
	char	*lpLobbyInfo;					//
	char	cRegInfo[SRS_REG_INFO_SZ];		// ���ʏ���
	char	cIfFix[SRS_IF_FIX_SZ + SRS_IF_USER_DELETE_SZ];	// ���[�U�폜����
	char	cSendMode[4];
	char	cUserItem[SRS_USER_ITEM_SZ];
	char	cMsg[256];


	sprintf( cMsg, "SRS_Lby_ReqUserDelete : ���[�U�폜 0x%04x",lpIfFix->IF_wRequest);
	SRS_Dbg_WriteLog( cMsg );

	memset( cIfFix, 0, SRS_IF_FIX_SZ + SRS_IF_USER_DELETE_SZ );
	memcpy( cIfFix, (char *)lpIfFix, SRS_IF_FIX_SZ + SRS_IF_USER_DELETE_SZ );
	lpSendFix = (struct SRS_IF_FIX *)cIfFix;
	iClientIP = lpIfFix->IF_iIp;

	SRS_Dbg_WriteLog( "Step 1" );
	//=======================================
	//  ���[�U�폜
	//=======================================
	lpUserDelete = (struct SRS_IF_USER_DELETE *)lpIfFix->IF_cInformation;
	wUserID  = lpUserDelete->IF_UD_wUserID;			// ���[�U�h�c�ݒ�
	iRtn = SRS_Mng_GetUserByID( wUserID, cUserItem );
	if( iRtn == -1 )
	{
		lpSendFix->IF_wRequest = IF_REQ_USER_DELETE;
		lpSendFix->IF_wResult = IF_CMP_SEQERR;		// �v���V�[�P���X�G���[
		lpSendFix->IF_iInformationSize = 0;
//		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( iRtn );
	}
	lpUserItem = (struct SRS_USER_ITEM *)cUserItem;
	wLobbyID = lpUserItem->USI_wLobbyID;			// ���r�[�h�c�ݒ�

	iRtn = SRS_Mng_UserDelete( lpSendFix );	// ���[�U�폜
	if( iRtn == -1 )						// ���[�U�폜���s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_USER_DELETE;
		lpSendFix->IF_iInformationSize = 0;
//		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( iRtn );
	}

	SRS_Dbg_WriteLog( "Step 2" );
	//=======================================
	//  ����T�C�Y�Z�o
	//=======================================
	lpRegInfo = (struct SRS_REG_INFO *)cRegInfo;
	iRtn = SRS_Reg_GetComInfo( lpRegInfo );		// ���ʊǗ������荞��
	if( iRtn == -1 )							// ��荞�ݎ��s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_USER_DELETE;	// ���[�U�폜
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;		// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		sprintf( cMsg, "SRS_Lby_ReqUserDelete : SRS_Reg_GetComInfo error" );
		SRS_Dbg_WriteLog( cMsg );
//		iRtn = SRS_Net_SendReq( iClientIP, (char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( -1 );
	}
	szSendSize = (size_t)(SRS_IF_FIX_SZ + SRS_IF_RENEWAL_SZ + 	// ���T�C�Y
						  SRS_INFO_USER_SZ + SRS_INFO_LOBBY_SZ );

	SRS_Dbg_WriteLog( "Step 3" );
	//=======================================
	//  ���l��
	//=======================================
	lpSendData = (char *)malloc( szSendSize );
	if( lpSendData == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wRequest = IF_REQ_USER_DELETE;		// �v���ݒ�
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;			// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
//		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		return( 0 );
	}
	memset( lpSendData, 0, szSendSize );

	SRS_Dbg_WriteLog( "Step 4" );
	//=======================================
	//  �����쐬
	//=======================================
	//
	// �ŗL��ݒ�
	lpSendFix = (struct SRS_IF_FIX *)lpSendData;
    lpSendFix->IF_wRequest = IF_REQ_USER_DELETE;					// �v���ݒ�
	lpSendFix->IF_wResult = IF_CMP_SUCCESS;							// ���ʐ���
	lpSendFix->IF_iInformationSize = szSendSize - SRS_IF_FIX_SZ;	//
	lpSendFix->IF_iPortNo = lpIfFix->IF_iPortNo;					// �|�[�g�ԍ��R�s�[
	lpSendFix->IF_iIp = lpIfFix->IF_iIp;							// IP�A�h���X�R�s�[

	//
	// �X�V����ݒ�
	lpSendRenewal = (struct SRS_IF_RENEWAL *)lpSendFix->IF_cInformation;
	lpSendRenewal->IF_RE_wUserInfoNo  = 1;		// ���[�U���ݒ�
	lpSendRenewal->IF_RE_wLobbyInfoNo = 1;		// ���r�[���ݒ�

	iUserSize  = SRS_Mng_GetMngSize( SRS_MNG_USER );			// ���[�U����T�C�Y
	iLobbySize = SRS_Mng_GetMngSize( SRS_MNG_LOBBY );	    	// ���r�[����T�C�Y
	// ���[�U���
	lpUserInfo = (char *)malloc( iUserSize );
	if( lpUserInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
//		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		return( 0 );
	}
	
	// ���r�[����
	lpLobbyInfo = (char *)malloc( iLobbySize );
	if( lpLobbyInfo == NULL )	// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_AREAOVER;	// �̈�s��
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
//		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		return( 0 );
	}

	//
	// �S���l��
	iRtn = SRS_Mng_AllGet( lpUserInfo, lpLobbyInfo, 0, 0 );
	if( iRtn == -1 )							// �l�����s�H
	{
		lpSendFix->IF_wResult = IF_CMP_SEQERR;	// �v���V�[�P���X�G���[
		lpSendFix->IF_iInformationSize = 0;
		//
		// �������M�i�G���[�j
//		iRtn = SRS_Net_SendReq( iClientIP,(char *)lpSendFix, (size_t)SRS_IF_FIX_SZ );
		realloc( lpSendData, 0 );
		realloc( lpUserInfo, 0 );
		realloc( lpLobbyInfo, 0 );
		return( 0 );
	}

	// �Ǘ����ݒ�
	lpSendUser 	= (struct SRS_INFO_USER *)lpUserInfo;
	lpSendLobby = (struct SRS_INFO_LOBBY *)lpLobbyInfo;
	iPoint = 0;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpSendUser, SRS_INFO_USER_COM_SZ );
	iPoint = SRS_INFO_USER_COM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint],
             lpSendUser->US_info_user[wUserID].USI_cClientName, SRS_USER_ITEM_SZ );
	iPoint = iPoint + SRS_USER_ITEM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint], lpLobbyInfo, SRS_INFO_LOBBY_COM_SZ );
	iPoint = iPoint + SRS_INFO_LOBBY_COM_SZ;
	memcpy( (char *)&lpSendRenewal->IF_RE_cInfo[iPoint],
            (char *)&lpSendLobby->LB_info_lobby[wLobbyID].LBI_iLockword, SRS_LOBBY_ITEM_SZ );

	//=======================================
	//  �������M
	//=======================================
	sprintf( cMsg, "SRS_Lby_UserDeleteEX : �������M %d bytes", szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	SRS_Dbg_WriteData( cMsg, lpSendData, szSendSize, "ab" );

//	iRtn = SRS_Net_SendReq( iClientIP, lpSendData, (size_t)szSendSize );
	realloc( lpUserInfo, 0 );
	realloc( lpLobbyInfo, 0 );

	//=======================================
	//  �X�V���z�M
	//=======================================
    lpSendFix->IF_wRequest = IF_REQ_RENEWAL;					// �v���ݒ�
	strcpy( cSendMode, SRS_SEND_MODE_NO );
	iRtn = SRS_Reg_GetInfo( SRS_REG_KEY_COM, SRS_REG_SEND, cSendMode );
	if( (strncmp(cSendMode, SRS_SEND_MODE_NO, sizeof(SRS_SEND_MODE_NO))) == 0 )	// �z�M�Ȃ��H
	{
		realloc( lpSendData, 0 );
		return( 0 );
	}
	//
    // �z�M���쐬
	//
	sprintf( cMsg, "SRS_Lby_UserDeleteEX : �X�V���z�M %d bytes",szSendSize );
	SRS_Dbg_WriteLog( cMsg );
	iRtn = SRS_Lby_SendInfo( wUserID, wLobbyID, lpSendData, (size_t)szSendSize );

	realloc( lpSendData, 0 );
	return( 0 );
}
