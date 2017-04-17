/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRSmanage.cpp
 *
 *					== ��񑀍�n�֐� ==
 *					SRS_Mng_Startup		: �������쐬
 *					SRS_Mng_UserEntry	: ���[�U�o�^
 *					SRS_Mng_UserDelete	: ���[�U�폜
 *					SRS_Mng_LobbyEntry	: ���r�[�Q��
 *					SRS_Mng_LobbyDefect	: ���r�[�E��
 *					SRS_Mng_GroupMake	: �O���[�v�쐬
 *					SRS_Mng_GroupJoin	: �O���[�v�Q��
 *					SRS_Mng_GroupDefect	: �O���[�v�E��
 *					SRS_Mng_GameJoin	: �Q�[���Q��
 *					SRS_Mng_GameDefect	: �Q�[���E��
 *					SRS_Mng_GameEnd		: �Q�[���I��
 *					SRS_Mng_GameStart	: �Q�[���J�n
 *					SRS_Mng_SaveInfo	: �ŗL���Ҕ�
 *
 *					== ���Q�ƌn�֐� ==
 *					SRS_Mng_AllGet		   : �S���l��
 *                  SRS_Mng_GetMngSize	   : �Ǘ�����T�C�Y�Q��
 *					SRS_Mng_GetUserByName  : ���[�U�h�c�Q��
 *					SRS_Mng_GetUserByID    : ���[�U���Q��
 *					SRS_Mng_GetLobbyByName : ���r�[�h�c�Q��
 *					SRS_Mng_GetLobbyByID   : ���r�[���Q��
 *					SRS_Mng_GetGroupByName : �O���[�v�h�c�Q��
 *					SRS_Mng_GetGroupByID   : �O���[�v���Q��
 *					SRS_Mng_GetEntry  	   : �o�^���Q��
 *
 *                  == ���ʊ֐� ==
 *                  SRS_Mng_GetInfo		   : �Ǘ����ǂݍ���
 *                  SRS_Mng_PutInfo		   : �Ǘ���񏑂�����
 *
 *  Content:    Lobby server �T�[�o���Ǘ����C�u����
 *
 ***************************************************************************/
/*================================
    �C���N���[�h
==================================*/
#include <windows.h>
#include <stdio.h>

#include "SRScom.h"
#include "SRSmanage.h"
#include "SRSdebug.h"
#include "SRSregistry.h"

#define ROOTZ	0	// customize flag

/*================================
    �O���[�o����
==================================*/
int		iUserSize = 0;
int		iLobbySize = 0;
int		iGroupSize = 0;
int		iGameSize = 0;
struct 	SRS_INFO_USER	*lpInfoUser = 0;	// ���[�U����
struct	SRS_INFO_LOBBY	*lpInfoLobby = 0;	// ���r�[����
struct	SRS_INFO_GROUP	*lpInfoGroup = 0;	// �O���[�v����
struct  SRS_INFO_GAME	*lpInfoGame = 0;	// �Q�[������

/*=============================================================================
     �֐�   �F�����ݒ�iLobby�T�[�o�Ǘ��e�[�u���Q���쐬����j
				���|�U���惊�X�g
				�Q�[������
				���r�[����
				�҂����킹����
     ����   �F�Ȃ�
     �o��   �F�Ȃ�
     �߂�l �F���� 0
              �ُ� -1�i�������s���j
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_Startup( void )
{
	int		iRtn;									//
	int		iAllocSize;								// �l���T�C�Y
	int		iPoint;									// ���r�[���e�[�u���|�C���g
	WORD	wMaxUser;								// �ő僆�[�U��
	WORD	wMaxLobby;								// �ő働�r�[��
	WORD	wMaxGame;								// �ő�Q�[����
	WORD	wMaxGroup;								// �ő�O���[�v��
	WORD	wCount;									// �Q���l��
	WORD	wID;									// �h�c�����p
	FILE	*fFp = (FILE *)NULL;					//
	struct	SRS_REG_INFO		*lpRegInfo;			// �T�[�o���ʏ���|�C���^
	struct	SRS_REG_INFO_LOBBY	*lpRegInfoLobby;	// ���r�[�ŗL����|�C���^
	struct	SRS_REG_INFO_GAME	*lpRegInfoGame;		// �Q�[���ŗL����|�C���^
	char	cRegInfo[SRS_REG_INFO_SZ];				// ���ʊǗ�����
	char	cMsg[256];								//


	//==============================================
	//  ������
	//==============================================
	iUserSize = 0;
	iLobbySize = 0;
	iGroupSize = 0;
	iGameSize = 0;
	lpInfoUser = 0;
	lpInfoLobby = 0;
	lpInfoGroup = 0;
	lpInfoGame = 0;
	DeleteFile( SRS_PATH_USER );
	DeleteFile( SRS_PATH_LOBBY );
	DeleteFile( SRS_PATH_GROUP );
	DeleteFile( SRS_PATH_GAME );

	//==============================================
	//  �Ϗ���荞��
	//==============================================
	//
	// ���ʊǗ����
	//
	lpRegInfo = (struct SRS_REG_INFO *)cRegInfo;
	iRtn = SRS_Reg_GetComInfo( lpRegInfo );		// ���ʊǗ������荞��
	if( iRtn == -1 )							// ��荞�ݎ��s�H
	{
		sprintf( cMsg, "SRS_Mng_Startup : SRS_Reg_GetComInfo error" );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	wMaxUser  =	lpRegInfo->wMaxUser;			// �ő僆�[�U��
	wMaxLobby = lpRegInfo->wMaxLobby;			// �ő働�r�[��
	wMaxGame  = lpRegInfo->wMaxGame;			// �ő�Q�[����
	wMaxGroup = lpRegInfo->wMaxGroup;			// �ő�O���[�v��
	//
	// ���r�[�ŗL���l��
	//
	iAllocSize = SRS_REG_INFO_LOBBY_SZ * wMaxLobby;
	lpRegInfoLobby = (struct SRS_REG_INFO_LOBBY *)malloc( iAllocSize );
	if( lpRegInfoLobby == 0 )
	{
		sprintf( cMsg, "??? malloc SRS_REG_INFO_LOBBY (%d) ???", iAllocSize );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	iRtn = SRS_Reg_GetLobbyInfo( (int)wMaxLobby, lpRegInfoLobby );	// ���r�[�ŗL���l��
	if( iRtn == -1 )
	{
		realloc( (char *)lpInfoUser, 0 );
		realloc( (char *)lpInfoLobby, 0 );
		sprintf( cMsg, "SRS_Mng_Startup : SRS_Reg_GetComInfo error" );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	//
	// �Q�[���ŗL���l��
	//
	iAllocSize = SRS_REG_INFO_GAME_SZ * wMaxGame;
	lpRegInfoGame = (struct SRS_REG_INFO_GAME *)malloc( iAllocSize );
	if( lpRegInfoGame == 0 )
	{
		sprintf( cMsg, "??? malloc SRS_REG_INFO_GAME (%d) ???", iAllocSize );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	iRtn = SRS_Reg_GetGameInfo( (int)wMaxGame, lpRegInfoGame );		// �Q�[���ŗL���l��
	if( iRtn == -1 )
	{
		sprintf( cMsg, "SRS_Mng_Startup : SRS_Reg_GetGameInfo error" );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	
	//==============================================
	//  ���[�U����쐬
	//==============================================
	iUserSize = SRS_USER_ITEM_SZ * (wMaxUser+1);
	iUserSize = iUserSize + SRS_INFO_USER_COM_SZ;
	lpInfoUser = (struct SRS_INFO_USER *)malloc( iUserSize );
	if( lpInfoUser == 0 )
	{
		sprintf( cMsg, "??? malloc SRS_INFO_USER (%d) ???", iUserSize );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	memset( (char *)lpInfoUser, 0, iUserSize );
	lpInfoUser->US_wMaxUser = wMaxUser;			// �ő僆�[�U���ݒ�
	//
	// ���[�U�h�c�ݒ�
	//
	for( wID = 0 ; wID < wMaxUser + 1 ; wID++ )
	{
		lpInfoUser->US_info_user[wID].USI_wUserID = wID;	// ���[�U�h�c�ݒ�
	}

	//==============================================
	//  ���r�[����쐬
	//==============================================
	iLobbySize = SRS_LOBBY_ITEM_SZ * wMaxLobby;
	iLobbySize = iLobbySize + SRS_INFO_LOBBY_COM_SZ;
	lpInfoLobby = (struct SRS_INFO_LOBBY *)malloc( iLobbySize );
	if( lpInfoLobby == 0 )
	{
		sprintf( cMsg, "??? malloc SRS_INFO_LOBBY (%d) ???", iLobbySize );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	memset( (char *)lpInfoLobby, 0, iLobbySize );
	lpInfoLobby->LB_wMaxLobby = wMaxLobby;					// �ő働�r�[���ݒ�
	//
	//  ���r�[���ݒ�
	//
	for( wID=0, iPoint=1 ; wID < wMaxLobby ; wID++ )
	{
		lpInfoLobby->LB_info_lobby[wID].LBI_wLobbyID = wID; 		// ���r�[�h�c�ݒ�
		wCount = lpRegInfoLobby->wMaxUser;							//
		lpInfoLobby->LB_info_lobby[wID].LBI_wMaxLobbyUser = wCount;	// �ő�Q���l���ݒ�
		memcpy( lpInfoLobby->LB_info_lobby[wID].LBI_cLobbyName, 	// ���r�[���ݒ�
										lpRegInfoLobby->cName, SRS_MAX_NAME_LOBBY );
		lpRegInfoLobby = lpRegInfoLobby + 1;
	}

	//==============================================
	//  �O���[�v����쐬
	//==============================================
	iGroupSize = SRS_GROUP_ITEM_SZ * (wMaxGroup+1);
	iGroupSize = iGroupSize + SRS_INFO_GROUP_COM_SZ;
	lpInfoGroup = (struct SRS_INFO_GROUP *)malloc( iGroupSize );
	if( lpInfoGroup == 0 )
	{
		sprintf( cMsg, "??? malloc SRS_INFO_GROUP (%d) ???", iGroupSize );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	memset( (char *)lpInfoGroup, 0, iGroupSize );
	lpInfoGroup->GP_wMaxGroup = wMaxGroup;			// �ő�O���[�v���ݒ�
	//
	// �O���[�v�h�c�ݒ�
	//
	for( wID = 0 ; wID < wMaxGroup + 1; wID++ )
	{
		lpInfoGroup->GP_info_group[wID].GPI_wGroupID = wID;	//�O���[�v�h�c�ݒ�
		lpInfoGroup->GP_info_group[wID].GPI_wMaxUser = 		//�O���[�v���ő僆�[�U���ݒ�
											SRS_MAX_GROUP_USER;
	}

	//==============================================  
	//  �Q�[������쐬
	//==============================================
	iGameSize = SRS_GAME_ITEM_SZ * (wMaxGame+1);
	iGameSize = iGameSize + SRS_INFO_GAME_COM_SZ;
	lpInfoGame = (struct SRS_INFO_GAME *)malloc( iGameSize );
	if( lpInfoGame == 0 )
	{
		sprintf( cMsg, "??? malloc SRS_INFO_GAME (%d) ???", iGameSize );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	memset( (char *)lpInfoGame, 0, iGameSize );
	lpInfoGame->GM_wMaxGame = wMaxGame;			// �ő�Q�[�����ݒ�
	lpInfoGame->GM_wEntryGame = 0;				// �o�^�Q�[�����ݒ�

	//
	// �Q�[���h�c�ݒ� 
	//
	for( wID = 1 ; wID < wMaxGame + 1 ; wID++ )
	{
		lpInfoGame->GM_info_game[wID].GMI_wGameID = wID;			// �Q�[���h�c�ݒ�
		lpInfoGame->GM_wEntryGame = lpInfoGame->GM_wEntryGame + 1;	// �o�^�Q�[�����X�V
		wCount = lpRegInfoGame->wMaxUser;							//
		lpInfoGame->GM_info_game[wID].GMI_wMaxGameUser = wCount;	// �ő�Q���l���ݒ�
		memcpy( lpInfoGame->GM_info_game[wID].GMI_cGameName,		// �Q�[�����ݒ�
										lpRegInfoGame->cName, SRS_MAX_NAME_GAME );
		lpRegInfoGame = lpRegInfoGame + 1;
	}

	//=============================================
	//  ���Ҕ�
	//=============================================
	iRtn = SRS_Mng_PutInfo( );

	sprintf( cMsg, "userinfo=%d, lobbyinfo=%d, groupinfo=%d, gameinfo=%d",
             iUserSize, iLobbySize, iGroupSize, iGameSize);
	SRS_Dbg_WriteLog( cMsg );
	return( 0 );
}

/*=============================================================================
     �֐�   �F���[�U�o�^�iLobby�T�[�o�Ǘ��e�[�u���ɐV�K���[�U��o�^����j
     ����   �FlpIfFix   �C���^�t�F�[�X�u���b�N
     �o��   �F�Ȃ�
     �߂�l �F���� 0
              �ُ� -1�i�o�^���[�U���I�[�o�j
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_UserEntry( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;									//
	int		iID;									// ���r�[�o�^�pID
	WORD	wID;									// ���[�U�h�c
	struct  SRS_IF_USER_ENTRY	*lpUserEntry;		// ���[�U�o�^�ŗL��|�C���^
	struct  SRS_IF_USER_DELETE	*lpUserDelete;		// ���[�U�폜�ŗL��|�C���^
	struct	SRS_IF_FIX			*lpIfFixLocal;		// �����p�C���^�t�F�[�X��|�C���^
	char	cIfFixLocal[256];						// �����p�C���^�t�F�[�X��
	char	cMsg[256];

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		lpIfFix->IF_wResult = IF_CMP_SEQERR;
		SRS_Dbg_WriteLog( "SRS_Mng_UserEntry : SRS_Mng_GetInfo error" );
		return( -1 );
	}
	//==============================================  
	//  �o�^�ʒu����
	//==============================================
	if( lpInfoUser->US_wEntryUser >= lpInfoUser->US_wMaxUser )	// �o�^�I�[�o�[�H
	{
		lpIfFix->IF_wResult = IF_CMP_MAXOVER;
		SRS_Dbg_WriteLog( "SRS_Mng_UserEntry : MAX OVER" );
		return( -1 );
	}
	lpUserEntry = (struct SRS_IF_USER_ENTRY *)lpIfFix->IF_cInformation;

	//==============================================
	//	�Q�d�o�^�`�F�b�N
	//==============================================
	for( wID = 1 ; wID < lpInfoUser->US_wMaxUser ; wID++ )
	{
		if( lpInfoUser->US_info_user[wID].USI_dwCondetion != SRS_SEQ_NOENTRY )	// �o�^�ρH
		{
			//
			// ����N���C�A���g���`�F�b�N
			//
			if( (memcmp( lpInfoUser->US_info_user[wID].USI_cClientName, 	// ����N���C�A���g���H
						 lpUserEntry->IF_UE_cClientName, SRS_MAX_NAME_CLIENT )) == 0 )
			{
				//
				// ���[�U�폜
				// �i�Ȃ�炩�̌����Ŏg�p����Ă��Ȃ����[�U��񂪎c���Ă���Ɣ��f����j
				//
				// ���[�U�폜�p�C���^�t�F�[�X��쐬
				lpIfFixLocal = (struct SRS_IF_FIX *)cIfFixLocal;
				memset( (char *)lpIfFixLocal, 0, sizeof(cIfFixLocal) );
				lpIfFixLocal->IF_wRequest = IF_REQ_USER_DELETE;
				lpIfFixLocal->IF_iInformationSize = 4;
				lpUserDelete = (struct SRS_IF_USER_DELETE *)&lpIfFixLocal->IF_cInformation[0];
				lpUserDelete->IF_UD_wUserID = lpInfoUser->US_info_user[wID].USI_wUserID;

				iRtn = SRS_Mng_UserDelete( (struct SRS_IF_FIX *)lpIfFixLocal );

				lpIfFix->IF_wResult = IF_CMP_MULTI;		// �Q�d�o�^�G���[
				sprintf( cMsg, "SRS_Mng_UserEntry : client multi entry %s", lpUserEntry->IF_UE_cClientName);
				SRS_Dbg_WriteLog( cMsg );
				return( -1 );
			}
			//
			// ���ꃆ�[�U���`�F�b�N
			//
			if( (memcmp( lpInfoUser->US_info_user[wID].USI_cUserName, 	// ���ꃆ�[�U���H
						 lpUserEntry->IF_UE_cUserName, SRS_MAX_NAME_USER )) == 0 )
			{
				lpIfFix->IF_wResult = IF_CMP_MULTI;		// �Q�d�o�^�G���[
				sprintf( cMsg, "SRS_Mng_UserEntry : user multi entry %s", lpUserEntry->IF_UE_cUserName);
				SRS_Dbg_WriteLog( cMsg );
				return( -1 );
			}
		}
	}
	//==============================================  
	//  �o�^�ʒu����
	//==============================================
	for( wID = 1 ; wID < lpInfoUser->US_wMaxUser + 1 ; wID++ )
	{
		if( lpInfoUser->US_info_user[wID].USI_dwCondetion == SRS_SEQ_NOENTRY )	// ���o�^��H
		{
			// ���ݒ�
			lpInfoUser->US_wEntryUser = lpInfoUser->US_wEntryUser + 1;		// �o�^���X�V
			lpInfoUser->US_info_user[wID].USI_dwCondetion = SRS_SEQ_LOBBYENTRY;	// ��ԁ����r�[�Q��
   			lpInfoUser->US_info_user[wID].USI_iIp = lpIfFix->IF_iIp;		// �h�o�A�h���X�ݒ�
			memcpy( lpInfoUser->US_info_user[wID].USI_cClientName,			// �N���C�A���g���ݒ�
			 		lpUserEntry->IF_UE_cClientName, SRS_MAX_NAME_CLIENT );
			memcpy( lpInfoUser->US_info_user[wID].USI_cUserName,			// ���[�U���ݒ�
			 		lpUserEntry->IF_UE_cUserName, SRS_MAX_NAME_USER );

			//==============================================  
			//  ���C�����r�[�o�^
			//==============================================
			if( lpInfoLobby->LB_info_lobby[0].LBI_wLobbyUser + 1 >= 
				lpInfoLobby->LB_info_lobby[0].LBI_wMaxLobbyUser )
			{
				lpIfFix->IF_wResult = IF_CMP_MAXOVER;
				SRS_Dbg_WriteLog( "SRS_Mng_UserEntry : MAX OVER in lobby" );
				return( -1 );
			}
			lpInfoLobby->LB_info_lobby[0].LBI_wLobbyUser = 					// ���r�[�����[�U���X�V
							lpInfoLobby->LB_info_lobby[0].LBI_wLobbyUser + 1;
			for( iID = 0 ; iID < lpInfoUser->US_wMaxUser ; iID++ )
			{
				if( lpInfoLobby->LB_info_lobby[0].LBI_wUserID[iID] == 0 )	// �󂫁H
				{
					lpInfoLobby->LB_info_lobby[0].LBI_wUserID[iID] = wID;	// ���[�UID�ݒ�
					//=============================================
					//  ���Ҕ�
					//=============================================
					iRtn = SRS_Mng_PutInfo( );
					lpIfFix->IF_wResult = IF_CMP_SUCCESS;					// �o�^����
					return( 0 );
				}
			}
			lpIfFix->IF_wResult = IF_CMP_ERROR;			// �o�^���s�i�V�X�e���G���[�j
			SRS_Dbg_WriteLog( "SRS_Mng_UserEntry : client multi entry" );
			return( -1 );
		}
	}
	// �o�^�ʒu�Ȃ��̏ꍇ
	lpIfFix->IF_wResult = IF_CMP_ERROR;		// �V�X�e���G���[
	SRS_Dbg_WriteLog( "SRS_Mng_UserEntry : �o�^�ʒu�Ȃ�" );
	return( -1 );
}

/*=============================================================================
     �֐�    �F���[�U�폜�iLobby�T�[�o�Ǘ��e�[�u�����烆�[�U���폜����j
     ����    �FlpIfFix   �C���^�t�F�[�X�u���b�N
     �o��    �F�Ȃ�
     �߂�l  �F���� 0
               �ُ� -1
	 ���L�����F�폜���郆�[�U���O���[�v�Q�����̏ꍇ�̓O���[�v�E�ނ������Ȃ�
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_UserDelete( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;									//
	WORD	wID;									// �h�c
	WORD	wUserID;								// ���[�U�h�c
	WORD	wLobbyID;								// ���r�[�h�c
	struct  SRS_IF_USER_DELETE	*lpUserDelete;		// ���[�U�폜�ŗL��|�C���^
	struct	SRS_IF_GROUP_DEFECT *lpGroupDefect;		// �O���[�v�폜�ŗL��|�C���^
	struct	SRS_IF_FIX *lpIfFixLocal;				// �����p�C���^�t�F�[�X��|�C���^
	char	cIfFixLocal[256];						// �����p�C���^�t�F�[�X��
	char	cMsg[256];

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		lpIfFix->IF_wResult = IF_CMP_SEQERR;
		SRS_Dbg_WriteLog( "SRS_Mng_UserDelete : SRS_Mng_GetInfo error" );
		return( -1 );
	}

	//=============================================
	// �O���[�v��񂩂烆�[�U�폜
	//=============================================
	lpUserDelete = (struct SRS_IF_USER_DELETE *)lpIfFix->IF_cInformation;
	wUserID = lpUserDelete->IF_UD_wUserID;					
	if( wUserID > lpInfoUser->US_wMaxUser )			// �s���h�c�H
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;
		sprintf( cMsg, "SRS_Mng_UserDelete : wrong ID %d", wUserID );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	if( lpInfoUser->US_info_user[wUserID].USI_wGroupID != 0 )		// �O���[�v�Q�����H
	{
		// �O���[�v�E�ޗp�C���^�t�F�[�X��쐬
		memset( cIfFixLocal, 0, 256 );
		lpIfFixLocal = (struct SRS_IF_FIX *)cIfFixLocal;
		lpIfFixLocal->IF_wRequest = IF_REQ_GROUP_DEFECT;
		lpIfFixLocal->IF_iInformationSize = sizeof(struct SRS_IF_GROUP_DEFECT);
		lpGroupDefect = (struct SRS_IF_GROUP_DEFECT *)lpIfFixLocal->IF_cInformation;
    	lpGroupDefect->IF_GD_wUserID = wUserID;
    	lpGroupDefect->IF_GD_wGroupID = lpInfoUser->US_info_user[wUserID].USI_wGroupID;

		iRtn = SRS_Mng_GroupDefect( lpIfFixLocal );				// �O���[�v�E��
		//==============================================  
		//  ���W�J
		//==============================================
		iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
		if( iRtn == -1 )
		{
			lpIfFix->IF_wResult = IF_CMP_SEQERR;
			SRS_Dbg_WriteLog( "SRS_Mng_UserDelete : SRS_Mng_GetInfo error after GroupDefect" );
			return( -1 );
		}
	}

	//==============================================
	// ���r�[��񂩂烆�[�U�폜
	//==============================================
	wLobbyID = lpInfoUser->US_info_user[wUserID].USI_wLobbyID;
	for( wID = 0 ; wID < lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wMaxLobbyUser ; wID++ )
	{
		if( wUserID == lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wUserID[wID] )
		{
			lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wUserID[wID] = 0;	// ���[�U�h�c����
			lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wLobbyUser =       // ���r�[�����[�U���X�V
				lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wLobbyUser -1;
		}
	}
	//==============================================
	// ���[�U��񏉊���
	//==============================================
	lpInfoUser->US_info_user[wUserID].USI_wLobbyID = 0;					// ���r�[�h�c
	lpInfoUser->US_info_user[wUserID].USI_wGroupID = 0;					// �O���[�v�h�c
	lpInfoUser->US_info_user[wUserID].USI_iIp = 0;						// �h�o�A�h���X
	memset( lpInfoUser->US_info_user[wUserID].USI_cClientName, 0,		// �N���C�A���g��
														SRS_MAX_NAME_CLIENT );
	memset( lpInfoUser->US_info_user[wUserID].USI_cUserName, 0, 		// ���[�U��
														SRS_MAX_NAME_USER );	
	memset( lpInfoUser->US_info_user[wUserID].USI_cUserArea, 0,			// ���[�U�ŗL��
													 	SRS_MAX_USERAREA );	
	lpInfoUser->US_info_user[wUserID].USI_dwCondetion = SRS_SEQ_NOENTRY; // ��ԁ����o�^
	lpInfoUser->US_wEntryUser = lpInfoUser->US_wEntryUser - 1;			 // �o�^���X�V
	lpIfFix->IF_wResult = IF_CMP_SUCCESS;								 // �폜����

	//=============================================
	//  ���Ҕ�
	//=============================================
	iRtn = SRS_Mng_PutInfo( );
	return( 0 );
}

/*=============================================================================
     �֐�    �F���r�[���[�U�Q���i���r�[���Ƀ��[�U��o�^����j
     ����    �FlpIfFix   �C���^�t�F�[�X�u���b�N
     �o��    �F�Ȃ�
     �߂�l  �F���� 0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_LobbyEntry( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;									//
	WORD	wUserID;								// ���[�U�h�c
	WORD	wLobbyID;								// ���r�[�h�c
	WORD	wOldLobbyID;							// �����r�[�h�c
	WORD	wID;									// ��Ɨp�h�c
	struct	SRS_IF_LOBBY_ENTRY  *lpLobbyEntry;		// ���r�[�Q���ŗL��|�C���^
	struct  SRS_USER_ITEM *lpUserItem;				//
	char	cUserItem[SRS_USER_ITEM_SZ];
	char	cMsg[256];

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		lpIfFix->IF_wResult = IF_CMP_SEQERR;
		SRS_Dbg_WriteLog( "SRS_Mng_LobbyEntry : SRS_Mng_GetInfo error" );
		return( -1 );
	}

	lpLobbyEntry = (struct SRS_IF_LOBBY_ENTRY *)lpIfFix->IF_cInformation;
	wUserID = lpLobbyEntry->IF_LE_wUserID;			//
	wLobbyID = lpLobbyEntry->IF_LE_wLobbyID;		//

	//
	// ���[�U���Q��
	//
	memset( cUserItem, 0, SRS_USER_ITEM_SZ );
	iRtn = SRS_Mng_GetUserByID( wUserID, cUserItem );
	if( iRtn == -1 )
	{
		lpIfFix->IF_wResult = IF_CMP_SEQERR;				// �V�[�P���X�G���[
		sprintf( cMsg, "SRS_Mng_LobbyEntry : SRS_Mng_GetUserByID error ID=%d",wUserID );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	lpUserItem = (struct SRS_USER_ITEM *)cUserItem;
	wOldLobbyID = lpUserItem->USI_wLobbyID;					// �����r�[�h�c�Ҕ�
	//==============================================
	//  �Q���l���`�F�b�N
	//==============================================
	if( (lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wLobbyUser + 1) >=
						lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wMaxLobbyUser )
	{
		lpIfFix->IF_wResult = IF_CMP_MAXOVER;				// �ő吔�I�[�o
		SRS_Dbg_WriteLog( "SRS_Mng_LobbyEntry : MAX OVER" );
		return( -1 );
	}
	
	//==============================================
	//  �Q�����[�U�h�c�ݒ�
	//==============================================
	for( wID = 0 ; wID < lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wMaxLobbyUser ; wID++ )
	{
		if( lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wUserID[wID] == 0 ) // ���[�U�h�c���o�^�H
		{
			lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wUserID[wID] = wUserID;
			lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wLobbyUser =		// �Q�����[�U���X�V
                           lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wLobbyUser + 1;
			lpInfoLobby->LB_info_lobby[wLobbyID].LBI_iLockword = 0;		// �r������

			lpInfoUser->US_info_user[wUserID].USI_wLobbyID = wLobbyID;	// �Q�����r�[�h�c�ݒ�

			//==============================================
			//	�����r�[�E��
			//==============================================
			lpInfoLobby->LB_info_lobby[wOldLobbyID].LBI_wLobbyUser = 
								lpInfoLobby->LB_info_lobby[wOldLobbyID].LBI_wLobbyUser - 1;
			for( wID=0 ; wID < lpInfoLobby->LB_info_lobby[wOldLobbyID].LBI_wMaxLobbyUser ; wID++ )
			{
				if( lpInfoLobby->LB_info_lobby[wOldLobbyID].LBI_wUserID[wID] == wUserID )
				{
					lpInfoLobby->LB_info_lobby[wOldLobbyID].LBI_wUserID[wID] = 0;
				}
			}
			lpIfFix->IF_wResult = IF_CMP_SUCCESS;					// �Q������

			//=============================================
			//  ���Ҕ�
			//=============================================
			iRtn = SRS_Mng_PutInfo( );
			return( 0 );
		}
	}
	// �Q���ʒu�Ȃ��̏ꍇ
	
	lpIfFix->IF_wResult = IF_CMP_ERROR;						// �V�X�e���G���[
	SRS_Dbg_WriteLog( "SRS_Mng_LobbyEntry : �Q���ʒu�Ȃ�(system error)" );

	return( -1 );
}

/*=============================================================================
     �֐�    �F���r�[���[�U�E�ށi���r�[��񂩂烆�[�U���폜����j
     ����    �FlpIfFix   �C���^�t�F�[�X�u���b�N
     �o��    �F�Ȃ�
     �߂�l  �F���� 0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_LobbyDefect( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;									//
	WORD	wUserID;								// ���[�U�h�c
	WORD	wLobbyID;								// ���r�[�h�c
	WORD	wID;									// ��Ɨp�h�c
	struct	SRS_IF_LOBBY_DEFECT  *lpLobbyDefect;	// ���r�[�E�ތŗL��|�C���^
//	char	cMsg[256];

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		lpIfFix->IF_wResult = IF_CMP_SEQERR;
		SRS_Dbg_WriteLog( "SRS_Mng_LobbyDelete : SRS_Mng_GetInfo error" );
		return( -1 );
	}

	lpLobbyDefect = (struct SRS_IF_LOBBY_DEFECT *)lpIfFix->IF_cInformation;
	wUserID = lpLobbyDefect->IF_LD_wUserID;			//
	wLobbyID = lpLobbyDefect->IF_LD_wLobbyID;		//

	//=============================================
	//  ���r�[�E��
	//=============================================
	// �Y�����[�U�h�c����
	for( wID = 0 ; wID < lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wMaxLobbyUser ; wID++ )
	{
		if( lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wUserID[wID] == wUserID ) // �h�c��v�H
		{
			lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wUserID[wID] = 0;		// ���[�U�h�c�N���A
			lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wLobbyUser =			// ���r�[�����[�U���X�V
						lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wLobbyUser - 1;
			lpInfoUser->US_info_user[wUserID].USI_wLobbyID = 0;				// �Q�����r�[�h�c�N���A
			break;
		}
	}
	if( wID >= lpInfoLobby->LB_info_lobby[wLobbyID].LBI_wMaxLobbyUser )
	{
		lpIfFix->IF_wResult = IF_CMP_MAXOVER;								// �ő�I�[�o
		SRS_Dbg_WriteLog( "SRS_Mng_UserDelete : MAX OVER(system error)" );
		return( -1 );
	}

	//=============================================
	//  ���Ҕ�
	//=============================================
	iRtn = SRS_Mng_PutInfo( );	// ���A�`�F�b�N�Ȃ�
	return( 0 );
}

/*=============================================================================
     �֐�    �F�O���[�v�쐬�i���r�[���ɃO���[�v���쐬����j
     ����    �FlpIfFix   �C���^�t�F�[�X�u���b�N
     �o��    �F�Ȃ�
     �߂�l  �F���� 0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_GroupMake( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;									//
	WORD	wID;									//
	WORD	wUserID;								// ���[�U�h�c
	struct  SRS_IF_GROUP_MAKE	*lpGroupMake;		// �O���[�v�쐬�ŗL��|�C���^
	char	cMsg[256];

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		lpIfFix->IF_wResult = IF_CMP_SEQERR;
		SRS_Dbg_WriteLog( "SRS_Mng_GroupMake : SRS_Mng_GetInfo error" );
		return( -1 );
	}

	//==============================================
	//  �o�^���`�F�b�N
	//==============================================
	if( lpInfoGroup->GP_wEntryGroup >= lpInfoGroup->GP_wMaxGroup )	// �o�^�I�[�o�[�H
	{
		lpIfFix->IF_wResult = IF_CMP_MAXOVER;
		SRS_Dbg_WriteLog( "SRS_Mng_GroupMake : MAX OVER" );
		return( -1 );
	}
	lpGroupMake = (struct SRS_IF_GROUP_MAKE *)lpIfFix->IF_cInformation;
	wUserID = lpGroupMake->IF_GM_wUserID;

	//==============================================  
	//  �����`�F�b�N
	//==============================================
	for( wID = 1 ; wID < lpInfoGroup->GP_wMaxGroup + 1 ; wID++ )
	{
		if( (strcmp(lpGroupMake->IF_GM_cGroupName,		// ����O���[�v���H
					lpInfoGroup->GP_info_group[wID].GPI_cGroupName)) == 0 )
		{
			lpIfFix->IF_wResult = IF_CMP_MULTI;
			sprintf( cMsg, "SRS_Mng_GroupMake : multi group %s",lpGroupMake->IF_GM_cGroupName );
			SRS_Dbg_WriteLog( cMsg );
			return( -1 );
		}
	}
	//==============================================  
	//  �o�^�ʒu����
	//==============================================
	for( wID = 1 ; wID < lpInfoGroup->GP_wMaxGroup + 1 ; wID++ )
	{
		if( lpInfoGroup->GP_info_group[wID].GPI_cGroupName[0] == 0 )		// ���o�^�H
		{
			// �V�O���[�v�o�^
			lpInfoGroup->GP_info_group[wID].GPI_iLockword = (int)wUserID;	// �r���J�n
			lpInfoGroup->GP_info_group[wID].GPI_wUserID[0] = wUserID;		// ���[�UID�ݒ�i��Áj
			lpInfoGroup->GP_info_group[wID].GPI_wEntryUser = 1;				// �Q�����[�U���ݒ�
			memcpy( lpInfoGroup->GP_info_group[wID].GPI_cGroupName,		// �O���[�v���ݒ�
					 lpGroupMake->IF_GM_cGroupName, SRS_MAX_NAME_GROUP );
			lpInfoGroup->GP_info_group[wID].GPI_iLockword = 0;				// �r������
			lpInfoGroup->GP_wEntryGroup =  									// �o�^�O���[�v���X�V
									lpInfoGroup->GP_wEntryGroup + 1;
			lpInfoUser->US_info_user[wUserID].USI_wGroupID = wID;			// �O���[�v�h�c�ݒ�
			lpInfoUser->US_info_user[wUserID].USI_dwCondetion = 			// ��� �� �O���[�v�Q��
														SRS_SEQ_GROUPENTRY;
			lpIfFix->IF_wResult = IF_CMP_SUCCESS;							// �O���[�v�쐬����

			//=============================================
			//  ���Ҕ�
			//=============================================
			iRtn = SRS_Mng_PutInfo( );
			return( 0 );
		}
	}
	// �o�^�ʒu�Ȃ��̏ꍇ
	lpInfoGroup->GP_info_group[wID].GPI_iLockword = 0;		// �r������
	lpIfFix->IF_wResult = IF_CMP_ERROR;					// �V�X�e���G���[
	SRS_Dbg_WriteLog( "SRS_Mng_GroupMake : �o�^�ʒu�Ȃ�");
	return( -1 );
}
/*=============================================================================
     �֐�    �F�O���[�v�Q���i���łɂ���O���[�v�ɎQ������j
     ����    �FlpIfFix   �C���^�t�F�[�X�u���b�N
     �o��    �F�Ȃ�
     �߂�l  �F���� 0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_GroupJoin( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;									//
	int		iSearch;								//
	WORD	wUserID;								// ���[�U�h�c
	WORD	wGroupID;								// �O���[�v�h�c
	struct  SRS_IF_GROUP_JOIN	*lpGroupJoin;		// �O���[�v�Q���ŗL��|�C���^
	char	cMsg[256];

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		lpIfFix->IF_wResult = IF_CMP_SEQERR;
		return( -1 );
	}

	//==============================================  
	//  �h�c�`�F�b�N
	//==============================================
	lpGroupJoin = (struct SRS_IF_GROUP_JOIN *)lpIfFix->IF_cInformation;
	wUserID = lpGroupJoin->IF_GJ_wUserID;
	wGroupID = lpGroupJoin->IF_GJ_wGroupID;

	if( wUserID > lpInfoUser->US_wMaxUser )			// �s�����[�U�h�c�H
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;			// �V�X�e���G���[
		sprintf( cMsg, "SRS_Mng_GroupJoin : wrong User ID %d",wUserID );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	if( wGroupID > lpInfoGroup->GP_wMaxGroup )		// �s���O���[�v�h�c�H
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;			// �V�X�e���G���[
		sprintf( cMsg, "SRS_Mng_GroupJoin : wrong Group ID %d",wGroupID );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}

	//==============================================  
	//  �Q���l���`�F�b�N
	//==============================================
	if( lpInfoGroup->GP_info_group[wGroupID].GPI_wEntryUser >= 	// ����I�[�o�H
		lpInfoGroup->GP_info_group[wGroupID].GPI_wMaxUser )
	{
		lpIfFix->IF_wResult = IF_CMP_MAXOVER;					// �ő吔�I�[�o
		sprintf( cMsg, "SRS_Mng_GroupJoin : MAX OVER" );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	
	//==============================================  
	//  �O���[�v�Q��
	//==============================================
	lpInfoGroup->GP_info_group[wGroupID].GPI_iLockword = wUserID;	// �r���J�n

	// �Q���ʒu����
	for( iSearch = 1 ; iSearch < lpInfoGroup->GP_info_group[wGroupID].GPI_wMaxUser ; iSearch++ )
	{
		if( lpInfoGroup->GP_info_group[wGroupID].GPI_wUserID[iSearch] == 0 ) // �󂫁H
		{
			lpInfoGroup->GP_info_group[wGroupID].GPI_wUserID[iSearch] = wUserID; //���[�U�h�c�ݒ�
			lpInfoGroup->GP_info_group[wGroupID].GPI_wEntryUser =			// �Q�����[�U���X�V
					lpInfoGroup->GP_info_group[wGroupID].GPI_wEntryUser + 1;
			lpInfoGroup->GP_info_group[wGroupID].GPI_iLockword = 0;			// �r������

			lpInfoUser->US_info_user[wUserID].USI_wGroupID = wGroupID;		// �O���[�v�h�c�ݒ�
			lpInfoUser->US_info_user[wUserID].USI_dwCondetion = 			// ��� �� �O���[�v�Q��
														SRS_SEQ_GROUPENTRY;
			lpIfFix->IF_wResult = IF_CMP_SUCCESS;

			//=============================================
			//  ���Ҕ�
			//=============================================	
			iRtn = SRS_Mng_PutInfo( );
			return( 0 );
		}
	}
	// �Q���ʒu���Ȃ��ꍇ
	lpInfoGroup->GP_info_group[wGroupID].GPI_iLockword = 0;	// �r������
	lpIfFix->IF_wResult = IF_CMP_ERROR;					// �V�X�e���G���[
	sprintf( cMsg, "SRS_Mng_GroupJoin : �Q���ʒu�Ȃ�" );
	SRS_Dbg_WriteLog( cMsg );

	return( -1 );
}
/*=============================================================================
     �֐�    �F�O���[�v�E�ށi�O���[�v����E�ނ���j
     ����    �FlpIfFix   �C���^�t�F�[�X�u���b�N
     �o��    �F�Ȃ�
     �߂�l  �F���� 0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_GroupDefect( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;									//
	int		iSearch;								//
	WORD	wUserID;								// ���[�U�h�c
	WORD	wGroupID;								// �O���[�v�h�c
	struct  SRS_IF_GROUP_DEFECT	*lpGroupDefect;		// �O���[�v�E�ތŗL��|�C���^
	char	cMsg[256];

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		lpIfFix->IF_wResult = IF_CMP_SEQERR;
		SRS_Dbg_WriteLog( "SRS_Mng_GroupDefect : SRS_Mng_GetInfo error" );
		return( -1 );
	}

	//==============================================  
	//  �h�c�`�F�b�N
	//==============================================
	lpGroupDefect = (struct SRS_IF_GROUP_DEFECT *)lpIfFix->IF_cInformation;
	wUserID = lpGroupDefect->IF_GD_wUserID;
	wGroupID = lpGroupDefect->IF_GD_wGroupID;

	if( wUserID >= lpInfoUser->US_wMaxUser )		// �s�����[�U�h�c�H
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;			// �V�X�e���G���[
		sprintf( cMsg, "SRS_Mng_GroupDefect : wrong User ID %d",wUserID );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	if( wGroupID > lpInfoGroup->GP_wMaxGroup )		// �s���O���[�v�h�c�H
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;			// �V�X�e���G���[
		sprintf( cMsg, "SRS_Mng_GroupDefect : wrong Group ID %d",wGroupID );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}

	//==============================================  
	//  �Q�����[�U�폜
	//==============================================
	lpInfoGroup->GP_info_group[wGroupID].GPI_iLockword = wUserID;	// �r���J�n
	// �폜���[�U����
	for( iSearch = 0 ; iSearch < lpInfoGroup->GP_info_group[wGroupID].GPI_wMaxUser ; iSearch++ )
	{
		if( lpInfoGroup->GP_info_group[wGroupID].GPI_wUserID[iSearch] == wUserID )	// �h�c��v�H
		{
			lpInfoGroup->GP_info_group[wGroupID].GPI_wUserID[iSearch] = 0;			// �h�c�N���A
			lpInfoGroup->GP_info_group[wGroupID].GPI_wEntryUser =					// �Q���l���X�V
						lpInfoGroup->GP_info_group[wGroupID].GPI_wEntryUser - 1;
			if( lpInfoGroup->GP_info_group[wGroupID].GPI_wEntryUser == 0 )		// �Q�����[�U�Ȃ��H
			{
				//
				//	�O���[�v�폜
				//
				lpInfoGroup->GP_wEntryGroup = 								// �o�^�O���[�v���X�V
												lpInfoGroup->GP_wEntryGroup - 1;
				memset( lpInfoGroup->GP_info_group[wGroupID].GPI_cGroupName,	// �O���[�v���N���A
															 0, SRS_MAX_NAME_GROUP );
				lpInfoGroup->GP_info_group[wGroupID].GPI_wGameID = 0;		// �Q�[���h�c�N���A
			}
			lpInfoGroup->GP_info_group[wGroupID].GPI_iLockword = 0;			// �r������

			lpInfoUser->US_info_user[wUserID].USI_wGroupID = 0;				// �O���[�v�h�c�N���A
			lpInfoUser->US_info_user[wUserID].USI_dwCondetion = SRS_SEQ_GAMEENTRY; // ��� �� �Q�[���I��
														
			//=============================================
			//  ���Ҕ�
			//=============================================
			iRtn = SRS_Mng_PutInfo( );
			lpIfFix->IF_wResult = IF_CMP_SUCCESS;				// �O���[�v�E�ސ���
			return( 0 );
		}
	}
	// ���ꃆ�[�U�h�c���Ȃ��ꍇ
	lpInfoGroup->GP_info_group[wGroupID].GPI_iLockword = 0;		// �r������
	lpIfFix->IF_wResult = IF_CMP_ERROR;							// �V�X�e���G���[
	sprintf( cMsg, "SRS_Mng_GroupDefect : �폜���[�U�h�c�Ȃ� group=%d, user=%d",wGroupID,wUserID );
	SRS_Dbg_WriteLog( cMsg );
	return( -1 );
}

/*=============================================================================
     �֐�    �F�S���Q�Ɓi�S�Ǘ�����ʒm����j
     ����    �FlpUser   ���[�U���i�[��|�C���^�i���s�v�̏ꍇ��NULL�|�C���^�j
			   lpLobby	���r�[���i�[��|�C���^�i���s�v�̏ꍇ��NULL�|�C���^�j
			   lpGroup	�O���[�v���i�[��|�C���^�i���s�v�̏ꍇ��NULL�|�C���^�j
			   lpGame   �Q�[�����i�[��|�C���^�i���s�v�̏ꍇ��NULL�|�C���^�j
     �o��    �F�Ȃ�
     �߂�l  �F���� 0
               �ُ� -1

=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_AllGet( char *lpUser, char *lpLobby,
													 char *lpGroup, char *lpGame )
{
	int		iRtn;							//

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		return( -1 );
	}
	//==============================================
	//  ���l��
	//==============================================
	if( lpUser != NULL )		// ���[�U���l���H
	{
		memcpy( lpUser, lpInfoUser, iUserSize );
	}
	if( lpLobby != NULL )		// ���r�[���l���H
	{
		memcpy( lpLobby, lpInfoLobby, iLobbySize );
	}

#if ROOTZ
	if( lpGroup != NULL )		// �O���[�v���l���H
	{
		memcpy( lpGroup, lpInfoGroup, iGroupSize );
	}
	if( lpGame != NULL )		// �Q�[�����l���H
	{
		memcpy( lpGame, lpInfoGame, iGameSize );
	}
#endif

	return( 0 );
}

/*=============================================================================
     �֐�    �F�Ǘ�����T�C�Y�Q�Ɓi�w�肳�ꂽ�Ǘ�����T�C�Y��ʒm����j
     ����    �FiKind   �Ǘ�������
                       SRS_MNG_USER	�i���[�U���j
					   SRS_MNG_LOBBY�i���r�[���j
					   SRS_MNG_GROUP�i�O���[�v���j
					   SRS_MNG_GAME	�i�Q�[�����j
     �o��    �F�Ȃ�
     �߂�l  �F���� ����T�C�Y
               �ُ� -1

=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_GetMngSize( int iKind )
{
	int		iRtn;							//
	int		iSize;							//

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		return( -1 );
	}
	//==============================================
	//  ���T�C�Y�l��
	//==============================================
	switch( iKind )	
	{
		case SRS_MNG_USER  : iSize = iUserSize;
							 break;
		case SRS_MNG_LOBBY : iSize = iLobbySize;
							 break;
		case SRS_MNG_GROUP : iSize = iGroupSize;
							 break;
		case SRS_MNG_GAME  : iSize = iGameSize;
							 break;
		default            : iSize = -1;
							 break;
	}
	return( iSize );
}

/*=============================================================================
     �֐�    �F�Q�[���Q��
     ����    �FlpIfFix   �C���^�t�F�[�X�u���b�N
     �o��    �F�Ȃ�
     �߂�l  �F���� 0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_GameJoin( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;									//
	WORD	wID;									//
	struct	SRS_IF_GAME_JOIN *lpGameJoin;			//
	
	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;
		return( -1 );
	}
	//==============================================
	//  ���X�V
	//==============================================
	lpGameJoin = (struct SRS_IF_GAME_JOIN *)lpIfFix->IF_cInformation;
	wID = lpGameJoin->IF_GJ_wUserID;
	if( wID >= lpInfoUser->US_wMaxUser )			// �s���h�c�H
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;			// �V�X�e���G���[
		return( -1 );
	}
	lpInfoUser->US_info_user[wID].USI_wGameID = lpGameJoin->IF_GJ_wGameID;	// �Q�[��ID�ݒ�
	lpInfoUser->US_info_user[wID].USI_dwCondetion = SRS_SEQ_GAMEENTRY;		// ��� �� �Q�[���I��
	lpIfFix->IF_wResult = IF_CMP_SUCCESS;	

	//=============================================
	//  ���Ҕ�
	//=============================================
	iRtn = SRS_Mng_PutInfo( );
	return( 0 );
}

/*=============================================================================
     �֐�    �F�Q�[���E��
     ����    �FlpIfFix   �C���^�t�F�[�X�u���b�N
     �o��    �F�Ȃ�
     �߂�l  �F���� 0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_GameDefect( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;									//
	WORD	wID;									//
	struct	SRS_IF_GAME_DEFECT *lpGameDefect;		//
	
	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;
		return( -1 );
	}
	//==============================================
	//  ���X�V
	//==============================================
	lpGameDefect = (struct SRS_IF_GAME_DEFECT *)lpIfFix->IF_cInformation;
	wID = lpGameDefect->IF_GD_wUserID;
	if( wID >= lpInfoUser->US_wMaxUser )			// �s���h�c�H
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;			// �V�X�e���G���[
		return( -1 );
	}
	lpInfoUser->US_info_user[wID].USI_wGameID = 0;	// �Q�[��ID�N���A
	lpInfoUser->US_info_user[wID].USI_dwCondetion = SRS_SEQ_LOBBYENTRY;	// ��� �� ���r�[�Q��
	lpIfFix->IF_wResult = IF_CMP_SUCCESS;	

	//=============================================
	//  ���Ҕ�
	//=============================================
	iRtn = SRS_Mng_PutInfo( );
	return( 0 );
}

/*=============================================================================
     �֐�    �F�Q�[���J�n
     ����    �FlpIfFix   �C���^�t�F�[�X�u���b�N
     �o��    �F�Ȃ�
     �߂�l  �F���� 0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_GameStart( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;									//
	WORD	wID;									//
	struct	SRS_IF_GAME_START *lpGameStart;			//
	
	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;
		return( -1 );
	}
	//==============================================
	//  ���X�V
	//==============================================
	lpGameStart = (struct SRS_IF_GAME_START *)lpIfFix->IF_cInformation;
	wID = lpGameStart->IF_GS_wUserID;
	if( wID >= lpInfoUser->US_wMaxUser )			// �s���h�c�H
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;			// �V�X�e���G���[
		return( -1 );
	}
	lpInfoUser->US_info_user[wID].USI_dwCondetion = SRS_SEQ_PLAY;	// ��� �� �Q�[����
	lpIfFix->IF_wResult = IF_CMP_SUCCESS;	

	//=============================================
	//  ���Ҕ�
	//=============================================
	iRtn = SRS_Mng_PutInfo( );
	return( 0 );
}

/*=============================================================================
     �֐�    �F�Q�[���I��
     ����    �FlpIfFix   �C���^�t�F�[�X�u���b�N
     �o��    �F�Ȃ�
     �߂�l  �F���� 0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_GameEnd( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;									//
	WORD	wID;									//
	struct	SRS_IF_GAME_END *lpGameEnd;				//
	
	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;
		return( -1 );
	}

	//==============================================
	//  ���X�V
	//==============================================
	lpGameEnd = (struct SRS_IF_GAME_END *)lpIfFix->IF_cInformation;
	wID = lpGameEnd->IF_GE_wUserID;
	if( wID >= lpInfoUser->US_wMaxUser )			// �s���h�c�H
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;			// �V�X�e���G���[
		return( -1 );
	}
	lpInfoUser->US_info_user[wID].USI_dwCondetion = SRS_SEQ_GROUPENTRY;	// ��� �� �O���[�v�Q��
	lpIfFix->IF_wResult = IF_CMP_SUCCESS;	

	//=============================================
	//  ���Ҕ�
	//=============================================
	iRtn = SRS_Mng_PutInfo( );
	return( 0 );
}

/*=============================================================================
     �֐�    �F�ŗL���Ҕ�
     ����    �FlpIfFix   �C���^�t�F�[�X�u���b�N
     �o��    �F�Ȃ�
     �߂�l  �F���� 0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_SaveInfo( struct SRS_IF_FIX *lpIfFix )
{
	int		iRtn;									//
	WORD	wID;									//
	struct	SRS_IF_SAVE_INFO *lpSaveInfo;			//
	
	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;
		return( -1 );
	}

	//==============================================
	//  ���X�V
	//==============================================
	lpSaveInfo = (struct SRS_IF_SAVE_INFO *)lpIfFix->IF_cInformation;
	wID = lpSaveInfo->IF_SI_wUserID;
	if( wID >= lpInfoUser->US_wMaxUser )			// �s���h�c�H
	{
		lpIfFix->IF_wResult = IF_CMP_ERROR;			// �V�X�e���G���[
		return( -1 );
	}
	memcpy( lpInfoUser->US_info_user[wID].USI_cUserArea,lpSaveInfo->IF_SI_cInfo,SRS_MAX_USERAREA );

	//=============================================
	//  ���Ҕ�
	//=============================================
	iRtn = SRS_Mng_PutInfo( );
	return( 0 );
}

/*=============================================================================
     �֐�    �F���[�U�h�c�l���i���[�U���ɑΉ����郆�[�U�h�c���l������j
     ����    �FlpUserName  ���[�U����|�C���^�i�Q�O�o�C�g�ȏ��ۏ؂��邱�Ɓj
     �o��    �F�Ȃ�
     �߂�l  �F���� ���[�U�h�c
               �ُ� SRS_WORD_ERROR (65535)
	 ���L�����F���[�U���̍Ō�͂m�t�k�k���ݒ肳��Ă��邱��
=============================================================================*/
__declspec( dllexport )WORD __stdcall SRS_Mng_GetUserByName( char *lpUserName )
{
	int		iRtn;								//
	WORD	wID;								//
//	char	cMsg[256];

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		return( SRS_WORD_ERROR );
	}

	//==============================================
	// �Y�����[�U����
	//==============================================
	for( wID = 0 ; wID < lpInfoUser->US_wMaxUser ; wID++ )
	{
		if( (memcmp( lpInfoUser->US_info_user[wID].USI_cUserName,	// ���[�U����v�H
					  lpUserName, SRS_MAX_NAME_USER )) == 0 )
		{
			return( wID );
		}
	}
	return( SRS_WORD_ERROR );
}

// ��t���ǉ�
//
// IP���烆�[�UID���Ƃ��ė���
__declspec( dllexport )WORD __stdcall SRS_Mng_GetUserByIP( int ip )
{
	int		iRtn;								//
	WORD	wID;								//

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		return( SRS_WORD_ERROR );
	}

	//==============================================
	// �Y�����[�U����
	//==============================================
	for( wID = 0 ; wID < lpInfoUser->US_wMaxUser ; wID++ )
	{
/*
		{
			char str[64], str2[32];
			int i;
			i = lpInfoUser->US_info_user[wID].USI_iIp;
			sprintf( str2, "%d", wID );
			sprintf( str, "%d.%d.%d.%d\n%d.%d.%d.%d", (ip >> 24 & 0xff), (ip >> 16 & 0xff), (ip >> 8 & 0xff), (ip & 0xff), (i >> 24 & 0xff), (i >> 16 & 0xff), (i >> 8 & 0xff), (i & 0xff) );
			MessageBox( NULL, str, str2, MB_OK );
		}
*/
		if ( lpInfoUser->US_info_user[wID].USI_iIp == ip )	// ���[�U����v�H
		{
			return( wID );
		}
	}
	return( SRS_WORD_ERROR );
}

/*=============================================================================
     �֐�    �F���r�[�h�c�l���i���r�[���ɑΉ����郍�r�[�h�c���l������j
     ����    �FlpLobbyName  ���r�[����|�C���^�i�P�U�o�C�g�ȏ��ۏ؂��邱�Ɓj
     �o��    �F�Ȃ�
     �߂�l  �F���� ���r�[�h�c
               �ُ� SRS_WORD_ERROR(65535)
	 ���L�����F���r�[���̍Ō�͂m�t�k�k���ݒ肳��Ă��邱��
=============================================================================*/
__declspec( dllexport )WORD __stdcall SRS_Mng_GetLobbyByName( char *lpLobbyName )
{
	int		iRtn;								//
	WORD	wID;								//

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		return( SRS_WORD_ERROR );
	}

	//==============================================
	//  �Y�����r�[����
	//==============================================
	for( wID = 0 ; lpInfoLobby->LB_wMaxLobby ; wID++ )
	{
		if( (memcmp( lpInfoLobby->LB_info_lobby[wID].LBI_cLobbyName,  // ���r�[����v�H
										 lpLobbyName, SRS_MAX_NAME_LOBBY )) == 0 )
		{
			return( wID );
		}
	}
	return( SRS_WORD_ERROR );
}

/*=============================================================================
     �֐�    �F�O���[�v�h�c�l���i�O���[�v���ɑΉ�����O���[�v�h�c���l������j
     ����    �FlpLobbyName  �O���[�v����|�C���^�i�Q�O�o�C�g�ȏ��ۏ؂��邱�Ɓj
     �o��    �F�Ȃ�
     �߂�l  �F���� �O���[�v�h�c
               �ُ� SRS_WORD_ERROR(65535)
	 ���L�����F�O���[�v���̍Ō�͂m�t�k�k���ݒ肳��Ă��邱��
=============================================================================*/
__declspec( dllexport )WORD __stdcall SRS_Mng_GetGroupByName( char *lpGroupName )
{
	int		iRtn;								//
	WORD	wID;								//

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		return( SRS_WORD_ERROR );
	}

	//==============================================
	//  �Y���O���[�v����
	//==============================================
	for( wID = 0 ; lpInfoGroup->GP_wMaxGroup ; wID++ )
	{
		if( (memcmp( lpInfoGroup->GP_info_group[wID].GPI_cGroupName, 	// �O���[�v����v�H
										 lpGroupName, SRS_MAX_NAME_GROUP )) == 0 )
		{
			return( wID );
		}
	}
	return( SRS_WORD_ERROR );
}

/*=============================================================================
     �֐�    �F���[�U���l���i���[�U�h�c�ɑΉ����郆�[�U�����l������j
     ����    �FwUserName  ���[�UID
			   lpUserInfo ���[�U���i�[��A�h���X
                         �i�̈�L������ SRS_USER_ITEM �\���� �ȏ��ۏ؂��邱�Ɓj
     �o��    �F���[�U��� �i�w�肳�ꂽ���[�U����ɏ�񂪃R�s�[�����j
     �߂�l  �F����  0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_GetUserByID( WORD wUserID, char *lpUserInfo )
{
	int		iRtn;
	int		iItemSize;

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		return( -1 );
	}

	//==============================================
	//  �Y�����[�U���l��
	//==============================================
	iItemSize  = SRS_USER_ITEM_SZ;
	memset( lpUserInfo, 0, iItemSize );
	if( wUserID >= lpInfoUser->US_wMaxUser )	// �s���h�c�H
	{
		return( -1 );
	}
	memcpy( lpUserInfo,
            (char *)lpInfoUser->US_info_user[wUserID].USI_cClientName,	SRS_USER_ITEM_SZ );
	return( 0 );
}

/*=============================================================================
     �֐�    �F���r�[���l���i���r�[�h�c�ɑΉ����郍�r�[�����l������j
     ����    �FwLobbyName  ���r�[ID
			   lpLobbyInfo ���r�[���i�[��A�h���X
                         �i�̈�L������ SRS_LOBBY_ITEM_SZ �ȏ��ۏ؂��邱�Ɓj
     �o��    �F���r�[��� �i�w�肳�ꂽ���r�[����ɏ�񂪃R�s�[�����j
     �߂�l  �F����  0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_GetLobbyByID( WORD wLobbyID, char *lpLobbyInfo )
{
	int		iRtn;	

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		return( -1 );
	}

	//==============================================
	//  �Y�����r�[���l��
	//==============================================
	memset( lpLobbyInfo, 0, SRS_LOBBY_ITEM_SZ );
	if( wLobbyID >= lpInfoLobby->LB_wMaxLobby )	// �s���h�c�H
	{
		return( -1 );
	}
	memset( lpLobbyInfo, 0, SRS_LOBBY_ITEM_SZ );
	memcpy( lpLobbyInfo,
            (char *)&(lpInfoLobby->LB_info_lobby[wLobbyID].LBI_iLockword),	SRS_LOBBY_ITEM_SZ );

	return( 0 );
}

/*=============================================================================
     �֐�    �F�O���[�v���l���i�O���[�v�h�c�ɑΉ�����O���[�v�����l������j
     ����    �FwLobbyName  �O���[�vID
			   lpLobbyInfo �O���[�v���i�[��A�h���X
                         �i�̈�L������ SRS_GROUP_ITEM_SZ �ȏ��ۏ؂��邱�Ɓj
     �o��    �F�O���[�v��� �i�w�肳�ꂽ�O���[�v����ɏ�񂪃R�s�[�����j
     �߂�l  �F����  0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_GetGroupByID( WORD wGroupID, char *lpGroupInfo )
{
	int		iRtn;


	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		return( -1 );
	}

	//==============================================
	//  �Y���O���[�v���l��
	//==============================================
	memset( lpGroupInfo, 0, SRS_GROUP_ITEM_SZ );
	if( wGroupID >= lpInfoGroup->GP_wMaxGroup )	// �s���h�c�H
	{
		return( -1 );
	}
	memset( lpGroupInfo, 0, SRS_GROUP_ITEM_SZ );
	memcpy( lpGroupInfo,
            (char *)&(lpInfoGroup->GP_info_group[wGroupID].GPI_iLockword),	SRS_GROUP_ITEM_SZ );

	return( 0 );
}

/*=============================================================================
     �֐�    �F�o�^���Q�Ɓi�Y�����̓o�^�����l������j
     ����    �FiKind  ����� 
                      SRS_MNG_USER  �o�^���[�U��
                      SRS_MNG_LOBBY �o�^���r�[��
                      SRS_MNG_GROUP �o�^�O���[�v��
                      SRS_MNG_GAME  �o�^�Q�[����
     �o��    �F�Ȃ�
     �߂�l  �F����  �o�^��
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_GetEntryNo( int iKind )
{
	int		iRtn;

	//==============================================  
	//  ���W�J
	//==============================================
	iRtn = SRS_Mng_GetInfo( SRS_INFO_NO );
	if( iRtn == -1 )
	{
		return( -1 );
	}
	//==============================================  
	//  �o�^���l��
	//==============================================
	switch( iKind )
	{
		case  SRS_MNG_USER	: iRtn = (int)lpInfoUser->US_wEntryUser;
							  break;	
		case  SRS_MNG_LOBBY	: iRtn = (int)lpInfoLobby->LB_wMaxLobby;
						      break;
		case  SRS_MNG_GROUP	: iRtn = (int)lpInfoGroup->GP_wEntryGroup;
							  break;
		case  SRS_MNG_GAME	: iRtn = (int)lpInfoGame->GM_wEntryGame;
							  break;	
		default				: iRtn = 0;
							  break;	
	}
	return( iRtn );
}

/*=============================================================================
     �֐�    �F�Ǘ����ǂݍ��݁i�e�Ǘ�������������ɓW�J����j
     ����    �FiMode   �̈�l�����[�h  SRS_INFO_GET :�l������
                                       SRS_INFO_NO  :�l�����Ȃ�
     �o��    �F�Ǘ���񂪃O���[�o����ɃR�s�[�����
     �߂�l  �F����  0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_GetInfo( int iMode )
{
	int		iRtn;
	BOOL	fRtn;
	DWORD	dwRtn;
	DWORD	dwReadByte;
	WORD	wMaxUser;
	WORD	wMaxLobby;
	WORD	wMaxGroup;
	WORD	wMaxGame;
	HANDLE	hHandle;
	struct	SRS_REG_INFO	*lpRegInfo;	
	char	cRegInfo[SRS_REG_INFO_SZ];
	char	cMsg[256];

	//==============================================
	//  �Ϗ���荞��
	//==============================================
	// ���ʊǗ����
	lpRegInfo = (struct SRS_REG_INFO *)cRegInfo;
	iRtn = SRS_Reg_GetComInfo( lpRegInfo );		// ���ʊǗ������荞��
	if( iRtn == -1 )							// ��荞�ݎ��s�H
	{
		sprintf( cMsg, "SRS_Mng_GetInfo : SRS_Reg_GetComInfo error" );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	wMaxUser  =	lpRegInfo->wMaxUser;			// �ő僆�[�U��
	wMaxLobby = lpRegInfo->wMaxLobby;			// �ő働�r�[��
	wMaxGame  = lpRegInfo->wMaxGame;			// �ő�Q�[����
	wMaxGroup = lpRegInfo->wMaxGroup;			// �ő�O���[�v��
	//
	// ����ݒ�
	//
	iUserSize  = SRS_USER_ITEM_SZ  * (wMaxUser+1);
	iUserSize  = iUserSize + SRS_INFO_USER_COM_SZ;

	iLobbySize = SRS_LOBBY_ITEM_SZ * wMaxLobby;
	iLobbySize = iLobbySize + SRS_INFO_LOBBY_COM_SZ;

	iGroupSize = SRS_GROUP_ITEM_SZ * (wMaxGroup+1);
	iGroupSize = iGroupSize + SRS_INFO_GROUP_COM_SZ;

	iGameSize  = SRS_GAME_ITEM_SZ  * (wMaxGame+1);
	iGameSize  = iGameSize + SRS_INFO_GAME_COM_SZ;

	//==============================================
	//  ����l��
	//==============================================
	if( iMode == SRS_INFO_GET )			// �̈�l���H
	{

		// << ���[�U����l�� >>
		lpInfoUser = (struct SRS_INFO_USER *)malloc( iUserSize );
		if( lpInfoUser == 0 )
		{
			sprintf( cMsg, "??? malloc SRS_INFO_USER (%d) ???", iUserSize );
			SRS_Dbg_WriteLog( cMsg );
			return( -1 );
		}
		memset( (char *)lpInfoUser, 0, iUserSize );

		// << ���r�[����l�� >>
		lpInfoLobby = (struct SRS_INFO_LOBBY *)malloc( iLobbySize );
		if( lpInfoLobby == 0 )
		{
			sprintf( cMsg, "??? malloc SRS_INFO_LOBBY (%d) ???", iLobbySize );
			SRS_Dbg_WriteLog( cMsg );
			return( -1 );
		}
		memset( (char *)lpInfoLobby, 0, iLobbySize );

		// << �O���[�v����l�� >>
		lpInfoGroup = (struct SRS_INFO_GROUP *)malloc( iGroupSize );
		if( lpInfoGroup == 0 )
		{
			sprintf( cMsg, "??? malloc SRS_INFO_GROUP (%d) ???", iGroupSize );
			SRS_Dbg_WriteLog( cMsg );
			return( -1 );
		}
		memset( (char *)lpInfoGroup, 0, iGroupSize );

		// << �Q�[������l�� >>
		lpInfoGame = (struct SRS_INFO_GAME *)malloc( iGameSize );
		if( lpInfoGame == 0 )
		{
			sprintf( cMsg, "??? malloc SRS_INFO_GAME (%d) ???", iGameSize );
			SRS_Dbg_WriteLog( cMsg );
			return( -1 );
		}
		memset( (char *)lpInfoGame, 0, iGameSize );

	}
	//==============================================
	//  ���[�U���ǂݍ���
	//==============================================
	for( ; ; )
	{
		hHandle = CreateFile( SRS_PATH_USER, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES)NULL, 
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,(HANDLE)NULL );
		if( hHandle != INVALID_HANDLE_VALUE )
		{
			break;
		}
		dwRtn = GetLastError();
		sprintf( cMsg, "SRS_Mng_GetInfo : reateFile error %d %s", dwRtn, SRS_PATH_USER );
		SRS_Dbg_WriteLog( cMsg );
		if( dwRtn != ERROR_SHARING_VIOLATION )	// �t�@�C���r�����ȊO�H
		{
			return( -1 );
		}
		Sleep( 50 );
	}
	fRtn = 0;
	dwReadByte = (DWORD)iUserSize;
	fRtn = ReadFile( hHandle, (char *)lpInfoUser, iUserSize, &dwReadByte, NULL );
	if( fRtn == false )
	{
		dwRtn = GetLastError();
		sprintf( cMsg, "SRS_Mng_GetInfo : ReadFile error %d %s", dwRtn, SRS_PATH_USER );
		SRS_Dbg_WriteLog( cMsg );
		CloseHandle( hHandle );
		return( -1 );
	}
	fRtn = CloseHandle( hHandle );

	//==============================================
	//  ���r�[���ǂݍ���
	//==============================================
	for( ; ; )
	{
		hHandle = CreateFile( SRS_PATH_LOBBY, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES)NULL, 
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,(HANDLE)NULL );
		if( hHandle != INVALID_HANDLE_VALUE )
		{
			break;
		}
		dwRtn = GetLastError();
		sprintf( cMsg, "CreateFile error %d %s", dwRtn, SRS_PATH_LOBBY );
		SRS_Dbg_WriteLog( cMsg );
		if( dwRtn != ERROR_SHARING_VIOLATION )	// �t�@�C���r�����ȊO�H
		{
			return( -1 );
		}
		Sleep( 50 );
	}
	fRtn = 0;
	dwReadByte = (DWORD)iLobbySize;
	fRtn = ReadFile( hHandle, (char *)lpInfoLobby, iLobbySize, &dwReadByte, NULL );
	if( fRtn == false )
	{
		dwRtn = GetLastError();
		sprintf( cMsg, "ReadFile error %d %s", dwRtn, SRS_PATH_LOBBY );
		SRS_Dbg_WriteLog( cMsg );
		CloseHandle( hHandle );
		return( -1 );
	}
	fRtn = CloseHandle( hHandle );

#if ROOTZ
	//==============================================
	//  �O���[�v���ǂݍ���
	//==============================================
	for( ; ; )
	{
		hHandle = CreateFile( SRS_PATH_GROUP, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES)NULL, 
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,(HANDLE)NULL );
		if( hHandle != INVALID_HANDLE_VALUE )
		{
			break;
		}
		dwRtn = GetLastError();
		sprintf( cMsg, "CreateFile error %d %s", dwRtn, SRS_PATH_GROUP );
		SRS_Dbg_WriteLog( cMsg );
		if( dwRtn != ERROR_SHARING_VIOLATION )	// �t�@�C���r�����ȊO�H
		{
			return( -1 );
		}
		Sleep( 500 );
	}
	dwReadByte = (DWORD)iGroupSize;
	fRtn = ReadFile( hHandle, (char *)lpInfoGroup, iGroupSize, &dwReadByte, NULL );
	if( fRtn == false )
	{
		dwRtn = GetLastError();
		sprintf( cMsg, "ReadFile error %d %s", dwRtn, SRS_PATH_GROUP );
		SRS_Dbg_WriteLog( cMsg );
		CloseHandle( hHandle );
		return( -1 );
	}
	fRtn = CloseHandle( hHandle );

	//==============================================
	//  �Q�[�����ǂݍ���
	//==============================================
	for( ; ; )
	{
		hHandle = CreateFile( SRS_PATH_GAME, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES)NULL, 
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,(HANDLE)NULL );
		if( hHandle != INVALID_HANDLE_VALUE )
		{
			break;
		}
		dwRtn = GetLastError();
		sprintf( cMsg, "CreateFile error %d %s", dwRtn, SRS_PATH_GAME );
		SRS_Dbg_WriteLog( cMsg );
		if( dwRtn != ERROR_SHARING_VIOLATION )	// �t�@�C���r�����ȊO�H
		{
			return( -1 );
		}
		Sleep( 500 );
	}
	fRtn = 0;
	dwReadByte = (DWORD)iGameSize;
	fRtn = ReadFile( hHandle, (char *)lpInfoGame, iGameSize, &dwReadByte, NULL );
	if( fRtn == false )
	{
		dwRtn = GetLastError();
		sprintf( cMsg, "ReadFile error %d %s", dwRtn, SRS_PATH_GAME );
		SRS_Dbg_WriteLog( cMsg );
		CloseHandle( hHandle );
		return( -1 );
	}
	fRtn = CloseHandle( hHandle );
#endif

	return( 0 );
}

__declspec( dllexport )void __stdcall SRS_Mng_PutPort( int iPort, int wID )
{
	lpInfoUser->US_info_user[wID].USI_iPort = iPort;
}

/*=============================================================================
     �֐�    �F�Ǘ���񏑂����݁i�e�Ǘ������t�@�C���ɑҔ�����j
     ����    �F�Ȃ�
     �o��    �F�Ǘ���񂪃t�@�C���ɃR�s�[�����
     �߂�l  �F����  0
               �ُ� -1
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Mng_PutInfo( int wID /* = 0 */ )
{
    size_t  szSize = 0;						//
    size_t  szBlock = 1;					//
	FILE	*fFp = (FILE *)NULL;			//
	DWORD	dwRtn;							//
	char	cMsg[256];

	SYSTEMTIME systime;
	GetLocalTime( &systime );
	lpInfoUser->US_info_user[wID].USI_time.wYear = systime.wYear;
	lpInfoUser->US_info_user[wID].USI_time.wMonth = systime.wMonth;
	lpInfoUser->US_info_user[wID].USI_time.wDayOfWeek = systime.wDayOfWeek;
	lpInfoUser->US_info_user[wID].USI_time.wDay = systime.wDay;
	lpInfoUser->US_info_user[wID].USI_time.wHour = systime.wHour;
	lpInfoUser->US_info_user[wID].USI_time.wMinute = systime.wMinute;
	lpInfoUser->US_info_user[wID].USI_time.wSecond = systime.wSecond;
	lpInfoUser->US_info_user[wID].USI_time.wMilliseconds = systime.wMilliseconds;

	//==========================================
	//  �t�@�C���o��
	//==========================================
	// ���[�U���
	for( ; ; )
	{
		fFp = fopen( SRS_PATH_USER, "wb");
		if( fFp != (FILE *)NULL )
		{
			fwrite( (char *)lpInfoUser, iUserSize, szBlock, fFp );
			fclose( fFp );
			break;
		}
		dwRtn = GetLastError();
		sprintf( cMsg, "??? fopen error user %d ???",dwRtn );
		SRS_Dbg_WriteLog( cMsg );
		if( dwRtn != ERROR_SHARING_VIOLATION )	// �t�@�C���r�����ȊO�H
		{
			return( -1 );
		}
		Sleep( 50 );
	}

	fwrite( (char *)lpInfoUser, (size_t)iUserSize, szBlock, fFp );
	fclose( fFp );


	// ���r�[���
	for( ; ; )
	{
		fFp = fopen( SRS_PATH_LOBBY, "wb");
		if( fFp != (FILE *)NULL )
		{
			fwrite( (char *)lpInfoLobby, iLobbySize, szBlock, fFp );
			fclose( fFp );
			break;
		}
		dwRtn = GetLastError();
		sprintf( cMsg, "??? fopen error lobby  %d ???", dwRtn );
		SRS_Dbg_WriteLog( cMsg );
		if( dwRtn != ERROR_SHARING_VIOLATION )	// �t�@�C���r�����ȊO�H
		{
			return( -1 );
		}
		Sleep( 50 );
	}
	fwrite( (char *)lpInfoLobby, (size_t)iLobbySize, szBlock, fFp );
	fclose( fFp );

#if ROOTZ
	// �O���[�v���
	for( ; ; )
	{
		fFp = fopen( SRS_PATH_GROUP, "wb");
		if( fFp != (FILE *)NULL )
		{
			fwrite( (char *)lpInfoGroup, iGroupSize, szBlock, fFp );
			fclose( fFp );
			break;
		}
		dwRtn = GetLastError();
		sprintf( cMsg, "??? fopen error group %d ???", dwRtn );
		SRS_Dbg_WriteLog( cMsg );
		if( dwRtn != ERROR_SHARING_VIOLATION )	// �t�@�C���r�����ȊO�H
		{
			return( -1 );
		}
		Sleep( 500 );
	}
	fwrite( (char *)lpInfoGroup, (size_t)iGroupSize, szBlock, fFp );
	fclose( fFp );


	// �Q�[�����
	for( ; ; )
	{
		fFp = fopen( SRS_PATH_GAME, "wb");
		if( fFp != (FILE *)NULL )
		{
			fwrite( (char *)lpInfoGame, iGameSize, szBlock, fFp );
			fclose( fFp );
			break;
		}
		dwRtn = GetLastError();
		sprintf( cMsg, "??? fopen error game %d ???", dwRtn );
		SRS_Dbg_WriteLog( cMsg );
		if( dwRtn != ERROR_SHARING_VIOLATION )	// �t�@�C���r�����ȊO�H
		{
			return( -1 );
		}
		Sleep( 500 );
	}
	fwrite( (char *)lpInfoGame, (size_t)iGameSize, szBlock, fFp );
	fclose( fFp );
#endif

	return( 0 );
}
