/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRSregistry.cpp
 *                	SRS_Reg_GetInfo	      : ���W�X�g�����ځi�L�����N�^�j�Q��
 *                	SRS_Reg_SetInfo	      : ���W�X�g�����ځi�L�����N�^�j�ݒ�
 *                	SRS_Reg_GetInfoBinary : ���W�X�g�����ځi�o�C�i���j�Q��
 *                	SRS_Reg_SetInfoBinary : ���W�X�g�����ځi�o�C�i���j�ݒ�
 *                	SRS_Reg_GetInfoDWORD  : ���W�X�g�����ځiDWORD�j�Q��
 *                	SRS_Reg_SetInfoDWORD  : ���W�X�g�����ځiDWORD�j�ݒ�
 *                  SRS_Reg_GetLobbyInfo  : ���W�X�g�����ځi���r�[���j�Q��
 *                  SRS_Reg_GetGameInfo   : ���W�X�g�����ځi�Q�[�����j�Q��
 *                  SRS_Reg_GetComInfo    : ���W�X�g�����ځi���ʏ��j�Q��
 *                  SRS_Reg_Log           : ���O���ʒm
 *
 *  Content:    Lobby server & Lobby Client common tools 
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

//=================================
// ���[�J���֐�
//=================================
WORD SRS_Reg_STRINGtoWORD( char *lpBuff );

/*=============================================================================
     �֐�   �F���W�X�g�����ݒ�
			�i�w�肳�ꂽ���W�X�g�����ɕ��������ݒ肷��j

     ����   �FlpRegKey�i�ݒ背�W�X�g���L�[�j
			  lpReg   �i�ݒ背�W�X�g�����ځj
			  lpBuff  �i�ݒ���i�[��|�C���^�j

     �o��   �F�Ȃ�

     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
__declspec( dllexport )int __stdcall SRS_Reg_SetInfo( char *lpRegKey,char *lpReg,char *lpBuff )
{
    LONG    lRtn=0;                     // LONG�^���A�l
	DWORD	dwRtn;
    HKEY    hKey;                       // �₢���킹��L�[�̃n���h��
    DWORD   dwDisposition;              // 
	char	cMsg[256];

    /*==========================================*/
    /*   �L�[�̃I�[�v���i���݂��Ȃ��ꍇ�͍쐬�j */
    /*==========================================*/
    lRtn = RegCreateKeyEx( HKEY_LOCAL_MACHINE, lpRegKey, 0, NULL, 
                           REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
                           &hKey, &dwDisposition );    
    if( lRtn != ERROR_SUCCESS )
    {
		dwRtn = GetLastError( );
		SRS_Dbg_WriteLog( cMsg );
		sprintf( cMsg, "SRS_Reg_SetInfo : RegCreateKeyEx Error %d %s\n",dwRtn, lpRegKey ); 
		return( -1 );
    }

    /*==================================*/
    /*   �l�ƃf�[�^�̐ݒ�               */
    /*==================================*/
    lRtn = RegSetValueEx( hKey, lpReg, 0, REG_SZ, (LPBYTE)lpBuff, (strlen(lpBuff))+1 );
    if( lRtn != ERROR_SUCCESS )
    {
		dwRtn = GetLastError( );
		sprintf( cMsg, "SRS_Reg_SetInfo : RegSetValueEx Error %d %s ",dwRtn, lpReg );
		SRS_Dbg_WriteLog( cMsg );
        RegCloseKey( hKey );
        return( -1 );
    }
    RegCloseKey( hKey );
    return( 0 );
}

/*=============================================================================
     �֐�   �F���W�X�g�����Q��
			�i�w�肳�ꂽ���W�X�g�����̏����Q�Ƃ���j

     ����   �FlpRegKey�i�Q�ƃ��W�X�g���L�[�j
			  lpReg   �i�Q�ƃ��W�X�g�����ځj
			  lpBuff  �i���ʊi�[��|�C���^�j

     �o��   �FlpRegBuff�i�Q�ƌ��ʁj

     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
__declspec( dllexport )int __stdcall SRS_Reg_GetInfo( char *lpRegKey,char *lpReg, char *lpBuff )
{
    LONG    lRtn;                       // 
    HKEY    hKey;                       // �₢���킹��L�[�̃n���h��
    DWORD   dwSize;                     // ���W�X�g���Q�Ɨp�p�����^
    char    cMsg[256]; 				    //

    //===========================================
    // �w��L�[�I�[�v��
    //===========================================
    lRtn = RegOpenKeyEx( HKEY_LOCAL_MACHINE, lpRegKey, 0, KEY_READ, &hKey );
    if( lRtn != ERROR_SUCCESS )     // �I�[�v�����s�H
    {
        sprintf( cMsg, "SRS_Reg_GetInfo : RegOpenKeyEx Error(%d) %s", lRtn, lpRegKey );
        SRS_Dbg_WriteLog( cMsg );
        return( -1 );
    }
    dwSize = 256;
    //===========================================
    // �ݒ�l�l�� 
    //===========================================
    lRtn = RegQueryValueEx( hKey, lpReg, NULL, NULL, (unsigned char *)lpBuff, &dwSize );
    if( lRtn != ERROR_SUCCESS )         // �l�����s���H
    {
        // �L�[�̃n���h�����
        RegCloseKey( hKey );
        if( lRtn == ERROR_MORE_DATA )   // �o�b�t�@���s�����H
        {
            // �o�b�t�@���s�����͕K�v�Ȓ�����dwSize�ɓ����Ă���
            sprintf( cMsg, "SRS_Reg_GetInfo : MORE_DATA %d %s %s",dwSize,lpRegKey, lpReg );
        }
        else
        {
            sprintf( cMsg,"SRS_Reg_GetInfo : RegQueryValueEx Error(%d) %s %s",lRtn,lpRegKey,lpReg);
        }
        SRS_Dbg_WriteLog( cMsg );
        return( -1 );
    }
    RegCloseKey( hKey );

    return( 0 );
}

/*=============================================================================
     �֐�   �F���W�X�g�����ʏ��Q��
			�i���W�X�g�����̋��ʏ����Q�Ƃ���j

     ����   �FlpRegInfo�i���ʏ��i�[��|�C���^�j

     �o��   �FlpRegInfo�i�Q�ƌ��ʁj

     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
__declspec( dllexport )int __stdcall SRS_Reg_GetComInfo( struct SRS_REG_INFO *lpRegInfo )
{
    LONG    lRtn;                       // 
	int		iRtn;						//
	int		iMax;						// �ő�l
    HKEY    hKey;                       // �₢���킹��L�[�̃n���h��
    DWORD   dwSize;                     // ���W�X�g���Q�Ɨp�p�����^
    char    *lpBuff;                    // ���W�X�g���Q�Ɨp�p�����^
    char    cBuff[8];	 			    //
    char    cMsg[256]; 				    //

    //===========================================
    // ���ʏ��I�[�v��
    //===========================================
    lRtn = RegOpenKeyEx( HKEY_LOCAL_MACHINE, SRS_REG_KEY_COM, 0, KEY_READ, &hKey );
    if( lRtn != ERROR_SUCCESS )     // �I�[�v�����s�H
    {
        sprintf( cMsg, "SRS_Reg_GetCominfo : RegOpenKeyEx Error(%d) %s", lRtn, SRS_REG_KEY_COM );
        SRS_Dbg_WriteLog( cMsg );
        return( -1 );
    }
	lpBuff = cBuff;

    //===========================================
    // �ő�o�^���[�U���Q�� 
    //===========================================

	memset( cBuff, 0, sizeof(cBuff) );
    dwSize = (DWORD)sizeof(cBuff);
    lRtn = RegQueryValueEx( hKey,SRS_REG_MAXUSER,NULL,NULL,(unsigned char *)lpBuff, &dwSize );
    if( lRtn != ERROR_SUCCESS )         // �l�����s���H
    {
        // �L�[�̃n���h�����
        RegCloseKey( hKey );
        if( lRtn == ERROR_MORE_DATA )   // �o�b�t�@���s�����H
        {
            // �o�b�t�@���s�����͕K�v�Ȓ�����dwSize�ɓ����Ă���
            sprintf( cMsg, "SRS_Reg_GetComInfo : MORE_DATA %d %s %s",
												dwSize,SRS_REG_KEY_COM, SRS_REG_MAXUSER );
        }
        else
        {
            sprintf( cMsg, "SRS_Reg_GetComInfo : RegQueryValueEx Error(%d) %s %s",
												 lRtn,SRS_REG_KEY_COM, SRS_REG_MAXUSER );
        }
        SRS_Dbg_WriteLog( cMsg );
        return( -1 );
    }
	iRtn = sscanf( lpBuff, "%d", &iMax );
	lpRegInfo->wMaxUser = (WORD)iMax;			// �ő�o�^���[�U���ݒ�

    //===========================================
    // �ő�o�^���r�[���Q�� 
    //===========================================
	memset( cBuff, 0, sizeof(cBuff) );
    dwSize = (DWORD)sizeof(cBuff);
    lRtn = RegQueryValueEx( hKey,SRS_REG_MAXLOBBY,NULL,NULL,(unsigned char *)lpBuff, &dwSize );
    if( lRtn != ERROR_SUCCESS )         // �l�����s���H
    {
        // �L�[�̃n���h�����
        RegCloseKey( hKey );
        if( lRtn == ERROR_MORE_DATA )   // �o�b�t�@���s�����H
        {
            // �o�b�t�@���s�����͕K�v�Ȓ�����dwSize�ɓ����Ă���
            sprintf( cMsg, "SRS_Reg_GetComInfo : MORE_DATA %d %s %s",
												dwSize,SRS_REG_KEY_COM, SRS_REG_MAXLOBBY );
        }
        else
        {
            sprintf( cMsg, "SRS_Reg_GetComInfo : RegQueryValueEx Error(%d) %s %s",
												 lRtn,SRS_REG_KEY_COM, SRS_REG_MAXLOBBY );
        }
        SRS_Dbg_WriteLog( cMsg );
        return( -1 );
    }
	iRtn = sscanf( lpBuff, "%d", &iMax );
	lpRegInfo->wMaxLobby = (WORD)iMax;			// �ő�o�^���r�[���ݒ�

    //===========================================
    // �ő�o�^�Q�[�����Q�� 
    //===========================================
	memset( cBuff, 0, sizeof(cBuff) );
    dwSize = sizeof(cBuff);
    lRtn = RegQueryValueEx( hKey,SRS_REG_MAXGAME,NULL,NULL,(unsigned char *)lpBuff, &dwSize );
    if( lRtn != ERROR_SUCCESS )         // �l�����s���H
    {
        // �L�[�̃n���h�����
        RegCloseKey( hKey );
        if( lRtn == ERROR_MORE_DATA )   // �o�b�t�@���s�����H
        {
            // �o�b�t�@���s�����͕K�v�Ȓ�����dwSize�ɓ����Ă���
            sprintf( cMsg, "SRS_Reg_GetComInfo : MORE_DATA %d %s %s",
												dwSize,SRS_REG_KEY_COM, SRS_REG_MAXGAME );
        }
        else
        {
            sprintf( cMsg, "SRS_Reg_GetComInfo : RegQueryValueEx Error(%d) %s %s",
												 lRtn,SRS_REG_KEY_COM, SRS_REG_MAXGAME );
        }
        SRS_Dbg_WriteLog( cMsg );
        return( -1 );
    }
	iRtn = sscanf( lpBuff, "%d", &iMax );
	lpRegInfo->wMaxGame = (WORD)iMax;			// �ő�o�^�Q�[�����ݒ�

    //===========================================
    // �ő�o�^�O���[�v���Q�� 
    //===========================================
	memset( cBuff, 0, sizeof(cBuff) );
    dwSize = sizeof(cBuff);
    lRtn = RegQueryValueEx( hKey,SRS_REG_MAXGROUP,NULL,NULL,(unsigned char *)lpBuff, &dwSize );
    if( lRtn != ERROR_SUCCESS )         // �l�����s���H
    {
        // �L�[�̃n���h�����
        RegCloseKey( hKey );
        if( lRtn == ERROR_MORE_DATA )   // �o�b�t�@���s�����H
        {
            // �o�b�t�@���s�����͕K�v�Ȓ�����dwSize�ɓ����Ă���
            sprintf( cMsg, "SRS_Reg_GetComInfo : MORE_DATA %d %s %s",
												dwSize,SRS_REG_KEY_COM, SRS_REG_MAXGROUP );
        }
        else
        {
            sprintf( cMsg, "SRS_Reg_GetComInfo : RegQueryValueEx Error(%d) %s %s",
												 lRtn,SRS_REG_KEY_COM, SRS_REG_MAXGROUP );
        }
        SRS_Dbg_WriteLog( cMsg );
        return( -1 );
    }
	iRtn = sscanf( lpBuff, "%d", &iMax );
	lpRegInfo->wMaxGroup = (WORD)iMax;			// �ő�o�^�O���[�v���ݒ�

    RegCloseKey( hKey );

    return( 0 );
}

/*=============================================================================
     �֐�    �F���W�X�g�����r�[�ŗL���Q��
			 �i���W�X�g�����̃��r�[�ŗL�����Q�Ƃ���j

     ����    �FlpRegInfoLobby�i���r�[�ŗL���i�[��|�C���^�j
               iCount        �i�ő働�r�[���j
     �o��    �FlpRegInfoLobby�i�Q�ƌ��ʁF���r�[�ŗL��� x �ő働�r�[���j

     �߂�l  �F���� 0
               �ُ� -1
 	 ���L�����F�i�[��T�C�Y�́A�h�P���r�[�ŗL���� x �ő働�r�[���h���ȏ��
               �ۏ؂��鎖

==============================================================================*/
__declspec( dllexport )int __stdcall SRS_Reg_GetLobbyInfo( int iCount, 
                                     struct SRS_REG_INFO_LOBBY *lpRegInfoLobby )
{
    LONG    lRtn;                       //
	int		iRtn;						//
	int		iLoop;						//
	WORD	wMax;						// �ő�l��
    HKEY    hKey;                       // �₢���킹��L�[�̃n���h��
    DWORD   dwSize;                     // ���W�X�g���Q�Ɨp�p�����^
    char    *lpBuff;                    // ���W�X�g���Q�Ɨp�p�����^
    char    cBuff[256]; 			    //
	char	cKeyLobby[256];				//
    char    cMsg[256]; 				    //
	struct	SRS_REG_INFO_LOBBY *lpLobbyInfo;
	
	//=========================================
	//  �SLobby�ŗL���l��
	//=========================================
	lpLobbyInfo = lpRegInfoLobby;
	for( iLoop = 0 ; iLoop < iCount;  )
	{
		memset( cKeyLobby, 0, sizeof(cKeyLobby));
		strcpy( cKeyLobby, SRS_REG_KEY_LOBBY );
		sprintf( cMsg, "%d",iLoop + 1 );
		strcat( cKeyLobby, cMsg );		// ���r�[�ŗL���L�[�쐬
	    //===========================================
    	// LOBBY�ŗL���I�[�v��
    	//===========================================
    	lRtn = RegOpenKeyEx( HKEY_LOCAL_MACHINE, cKeyLobby, 0, KEY_READ, &hKey );
    	if( lRtn != ERROR_SUCCESS )    				// �I�[�v�����s�H
   	 	{
    	    sprintf( cMsg, "SRS_Reg_GetLobbyInfo : RegOpenKeyEx Error(%d) %s", lRtn, cKeyLobby );
    	    SRS_Dbg_WriteLog( cMsg );
        	return( -1 );
		}
		lpBuff = cBuff;
   		dwSize = sizeof(cBuff);

    	//===========================================
    	// ���r�[���Q�� 
    	//===========================================
		memset( cBuff, 0, sizeof(cBuff));
    	lRtn = RegQueryValueEx( hKey,SRS_REG_NAME,NULL,NULL,(unsigned char *)lpBuff, &dwSize );
    	if( lRtn != ERROR_SUCCESS )         // �l�����s���H
   	 	{
    	    // �L�[�̃n���h�����
        	RegCloseKey( hKey );
        	if( lRtn == ERROR_MORE_DATA )   // �o�b�t�@���s�����H
        	{
            	// �o�b�t�@���s�����͕K�v�Ȓ�����dwSize�ɓ����Ă���
            	sprintf( cMsg, "SRS_Reg_GetLobbyInfo : MORE_DATA %d %s %s",
												dwSize, cKeyLobby, SRS_REG_NAME );
        	}
        	else
       	 	{
            	sprintf( cMsg, "SRS_Reg_GetLobbyInfo : RegQueryValueEx Error(%d) %s %s",
												 lRtn, cKeyLobby, SRS_REG_NAME );
	        }
    	    SRS_Dbg_WriteLog( cMsg );
        	return( -1 );
    	}
		memset( lpLobbyInfo->cName, 0, SRS_MAX_NAME_LOBBY );
		strcpy( lpLobbyInfo->cName, lpBuff );		// ���r�[���ݒ�

    	//===========================================
    	// �ő働�r�[���l���Q�� 
    	//===========================================
		memset( cBuff, 0, sizeof(cBuff) );
    	lRtn = RegQueryValueEx( hKey,SRS_REG_MAXENTRY,NULL,NULL,(unsigned char *)lpBuff, &dwSize );
    	if( lRtn != ERROR_SUCCESS )         // �l�����s���H
    	{
        	// �L�[�̃n���h�����
        	RegCloseKey( hKey );
        	if( lRtn == ERROR_MORE_DATA )   // �o�b�t�@���s�����H
        	{
           		// �o�b�t�@���s�����͕K�v�Ȓ�����dwSize�ɓ����Ă���
            	sprintf( cMsg, "SRS_Reg_GetLobbyInfo : MORE_DATA %d %s %s",
												dwSize, cKeyLobby, SRS_REG_MAXENTRY );
       		}
        	else
        	{
            	sprintf( cMsg, "SRS_Reg_GetLobbyInfo : RegQueryValueEx Error(%d) %s %s",
												 lRtn, cKeyLobby, SRS_REG_MAXENTRY );
        	}
        	SRS_Dbg_WriteLog( cMsg );
        	return( -1 );
    	}
		iRtn = sscanf( lpBuff, "%d", &wMax );
		lpLobbyInfo->wMaxUser = wMax;			// �ő働�r�[���l���ݒ�
	    RegCloseKey( hKey );
		lpLobbyInfo = lpLobbyInfo + 1;				// ����|�C���g�X�V
		iLoop++;
	}
    return( 0 );
}
/*=============================================================================
     �֐�    �F���W�X�g���Q�[���ŗL���Q��
			 �i���W�X�g�����̃Q�[���ŗL�����Q�Ƃ���j

     ����    �FlpRegInfoGame �i���r�[�ŗL���i�[��|�C���^�j
               iCount        �i�ő�Q�[�����j
     �o��    �FlpRegInfoGame �i�Q�ƌ��ʁF�Q�[���ŗL��� x �ő�Q�[�����j

     �߂�l  �F���� 0
               �ُ� -1
 	 ���L�����F�i�[��T�C�Y�́A�h�P�Q�[���ŗL���� x �ő�Q�[�����h���ȏ��
               �ۏ؂��鎖

==============================================================================*/
__declspec( dllexport )int __stdcall SRS_Reg_GetGameInfo( int iCount, 
                                     struct SRS_REG_INFO_GAME *lpRegInfoGame )
{
    LONG    lRtn;                       //
	int		iRtn;						//
	int		iLoop;						//
	WORD	wMax;						// �ő�l��
    HKEY    hKey;                       // �₢���킹��L�[�̃n���h��
    DWORD   dwSize;                     // ���W�X�g���Q�Ɨp�p�����^
    char    *lpBuff;                    // ���W�X�g���Q�Ɨp�p�����^
    char    cBuff[256]; 			    //
	char	cKeyGame[256];				//
    char    cMsg[256]; 				    //
	struct	SRS_REG_INFO_GAME *lpGameInfo;
	
	//=========================================
	//  �SGAME�ŗL���l��
	//=========================================
	lpGameInfo = lpRegInfoGame;
	for( iLoop = 0 ; iLoop < iCount; iLoop++ )
	{
		memset( cKeyGame, 0, sizeof(cKeyGame) );
		strcpy( cKeyGame, SRS_REG_KEY_GAME );
		sprintf( cMsg, "%d",iLoop + 1 );
		strcat( cKeyGame, cMsg );			// �Q�[���ŗL���L�[�쐬
	    //===========================================
    	// GAME�ŗL���I�[�v��
    	//===========================================
    	lRtn = RegOpenKeyEx( HKEY_LOCAL_MACHINE, cKeyGame, 0, KEY_READ, &hKey );
    	if( lRtn != ERROR_SUCCESS )     			// �I�[�v�����s�H
   	 	{
    	    sprintf( cMsg, "SRS_Com_GetGameInfo : RegOpenKeyEx Error(%d) %s", lRtn, cKeyGame );
    	    SRS_Dbg_WriteLog( cMsg );
        	return( -1 );
		}
		lpBuff = cBuff;
   		dwSize = 256;

    	//===========================================
    	// �Q�[�����Q�� 
    	//===========================================
		memset( cBuff, 0, sizeof(cBuff) );
    	lRtn = RegQueryValueEx( hKey,SRS_REG_NAME,NULL,NULL,(unsigned char *)lpBuff, &dwSize );
    	if( lRtn != ERROR_SUCCESS )         // �l�����s���H
   	 	{
    	    // �L�[�̃n���h�����
        	RegCloseKey( hKey );
        	if( lRtn == ERROR_MORE_DATA )   // �o�b�t�@���s�����H
        	{
            	// �o�b�t�@���s�����͕K�v�Ȓ�����dwSize�ɓ����Ă���
            	sprintf( cMsg, "SRS_Com_GetGameInfo : MORE_DATA %d %s %s",
												dwSize, cKeyGame, SRS_REG_NAME );
        	}
        	else
       	 	{
            	sprintf( cMsg, "SRS_Com_GetGameInfo : RegQueryValueEx Error(%d) %s %s",
												 lRtn, cKeyGame, SRS_REG_NAME );
	        }
    	    SRS_Dbg_WriteLog( cMsg );
        	return( -1 );
    	}
		memset( lpGameInfo->cName, 0, SRS_MAX_NAME_GAME );
		strcpy( lpGameInfo->cName, lpBuff );		// �Q�[�����ݒ�

    	//===========================================
    	// �ő�Q���l���Q�� 
    	//===========================================
		memset( cBuff, 0, sizeof(cBuff) );
    	lRtn = RegQueryValueEx( hKey,SRS_REG_MAXENTRY,NULL,NULL,(unsigned char *)lpBuff, &dwSize );
    	if( lRtn != ERROR_SUCCESS )         // �l�����s���H
    	{
        	// �L�[�̃n���h�����
        	RegCloseKey( hKey );
        	if( lRtn == ERROR_MORE_DATA )   // �o�b�t�@���s�����H
        	{
           		// �o�b�t�@���s�����͕K�v�Ȓ�����dwSize�ɓ����Ă���
            	sprintf( cMsg, "SRS_Com_GetGameInfo : MORE_DATA %d %s %s",
												dwSize, cKeyGame, SRS_REG_MAXENTRY );
       		}
        	else
        	{
            	sprintf( cMsg, "SRS_Com_GetGameInfo : RegQueryValueEx Error(%d) %s %s",
												 lRtn, cKeyGame, SRS_REG_MAXENTRY );
        	}
        	SRS_Dbg_WriteLog( cMsg );
        	return( -1 );
    	}
		iRtn = sscanf( lpBuff, "%d", &wMax );
		lpGameInfo->wMaxUser = wMax;			// �ő�Q���l���ݒ�
	    RegCloseKey( hKey );
		lpGameInfo = lpGameInfo + 1;			// ����|�C���g�X�V
	}
    return( 0 );
}
/*=============================================================================
     �֐�   �F���W�X�g�����ݒ�i�o�C�i���j
			�i�w�肳�ꂽ���W�X�g�����ɏ���ݒ肷��j

     ����   �FlpRegKey�i�ݒ背�W�X�g���L�[�j
			  lpReg   �i�ݒ背�W�X�g�����ځj
			  lpBuff  �i�ݒ���i�[��|�C���^�j
			  dwSize  �i�ݒ�L�����j

     �o��   �F�Ȃ�

     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
__declspec( dllexport )int __stdcall SRS_Reg_SetInfoBinary( char *lpRegKey, char *lpReg, char *lpBuff, DWORD dwSize )
{
    LONG    lRtn=0;                     // LONG�^���A�l
	DWORD	dwRtn;
    HKEY    hKey;                       // �₢���킹��L�[�̃n���h��
    DWORD   dwDisposition;              // 
	char	cMsg[256];

    /*==========================================*/
    /*   �L�[�̃I�[�v���i�쐬�j 				*/
    /*==========================================*/
    lRtn = RegCreateKeyEx( HKEY_LOCAL_MACHINE, lpRegKey, 0, NULL, 
                           REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
                           &hKey, &dwDisposition );    
    if( lRtn != ERROR_SUCCESS )
    {
		dwRtn = GetLastError( );
        sprintf( cMsg, "SRS_Reg_SetInfoBinary : RegQueryValueEx Error %d", dwRtn );
        SRS_Dbg_WriteLog( cMsg );
		
        return( -1 );
    }
    /*==================================*/
    /*   �l�ƃf�[�^�̐ݒ�               */
    /*==================================*/

    lRtn = RegSetValueEx( hKey, lpReg, 0, REG_BINARY, 
            (const unsigned char *)lpBuff, dwSize );
    if( lRtn != ERROR_SUCCESS )
    {
		dwRtn = GetLastError( );
        sprintf( cMsg, "SRS_Reg_SetInfoBinary : RegSetValueEx Error %d", dwRtn );
        SRS_Dbg_WriteLog( cMsg );
        RegCloseKey( hKey );
        return( -1 );
    }
    RegCloseKey( hKey );
    return( 0 );
}
/*=============================================================================
     �֐�   �F���W�X�g�����Q�Ɓi�o�C�i���j
			�i�w�肳�ꂽ���W�X�g�����̏����Q�Ƃ���j

     ����   �FlpRegKey�i�Q�ƃ��W�X�g���L�[�j
			  lpReg   �i�Q�ƃ��W�X�g�����ځj
			  lpBuff  �i���ʊi�[��|�C���^�j
			  dwSize  �i�i�[��T�C�Y�j

     �o��   �FlpRegBuff�i�Q�ƌ��ʁj

     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
 __declspec( dllexport )int __stdcall SRS_Reg_GetInfoBinary( char *lpRegKey,char *lpReg, char *lpBuff, DWORD dwSize )
{
    LONG    lRtn;                       // 
	DWORD	dwRtn;
    HKEY    hKey;                       // �₢���킹��L�[�̃n���h��
	char	cMsg[256];					//

    //===========================================
    // �w��L�[�I�[�v��
    //===========================================
    lRtn = RegOpenKeyEx( HKEY_LOCAL_MACHINE, lpRegKey, 0, KEY_READ, &hKey );
    if( lRtn != ERROR_SUCCESS )     // �I�[�v�����s�H
    {
		dwRtn = GetLastError();
		sprintf( cMsg, "SRS_Reg_GetInfoBinary : RegOpenKeyEx Error %d", dwRtn );
        SRS_Dbg_WriteLog( cMsg );
        return( -1 );
    }
    //===========================================
    // �ݒ�l�Q�� 
    //===========================================
    lRtn = RegQueryValueEx( hKey, lpReg, NULL, NULL, (unsigned char *)lpBuff, &dwSize );
    if( lRtn != ERROR_SUCCESS )         // �l�����s���H
    {
        // �L�[�̃n���h�����
        RegCloseKey( hKey );
        if( lRtn == ERROR_MORE_DATA )   // �o�b�t�@���s�����H
        {
            // �o�b�t�@���s�����͕K�v�Ȓ�����dwSize�ɓ����Ă���
            sprintf( cMsg, "SRS_Reg_GetInfoBinary : MORE_DATA %d", dwSize );
            SRS_Dbg_WriteLog( cMsg );
        }
        else
        {
			dwRtn = GetLastError();
            sprintf( cMsg,"SRS_Reg_GetInfoBinary : RegQueryValueEx Error %d",dwRtn );
			SRS_Dbg_WriteLog( cMsg );
        }
        return( -1 );
    }
    RegCloseKey( hKey );

    return( 0 );
}

/*=============================================================================
     �֐�   �F���W�X�g�����ݒ�i�c�v�n�q�c�j
			�i�w�肳�ꂽ���W�X�g�����ɂc�v�n�q�c����ݒ肷��j

     ����   �FlpRegKey�i�ݒ背�W�X�g���L�[�j
			  lpReg   �i�ݒ背�W�X�g�����ځj
			  dwInfo  �i�ݒ���j

     �o��   �F�Ȃ�

     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
__declspec( dllexport )int __stdcall SRS_Reg_SetInfoDWORD( char *lpRegKey,
                                                           char *lpReg,
                                                           DWORD dwInfo )
{
    LONG    lRtn=0;                     // LONG�^���A�l
	DWORD	dwRtn;
    HKEY    hKey;                       // �₢���킹��L�[�̃n���h��
    DWORD   dwDisposition;              // 
	char	cMsg[256];

    /*==========================================*/
    /*   �L�[�̃I�[�v���i�쐬�j 				*/
    /*==========================================*/
    lRtn = RegCreateKeyEx( HKEY_LOCAL_MACHINE, lpRegKey, 0, NULL, 
                           REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
                           &hKey, &dwDisposition );    
    if( lRtn != ERROR_SUCCESS )
    {
		dwRtn = GetLastError( );
		sprintf( cMsg, "SRS_Reg_SetInfoDWORD : RegCreateKeyEx Error %d",dwRtn );
		SRS_Dbg_WriteLog( cMsg );
		
        return( -1 );
    }
    /*==================================*/
    /*   �l�ƃf�[�^�̐ݒ�               */
    /*==================================*/

    lRtn = RegSetValueEx( hKey, lpReg, 0, REG_DWORD, (LPBYTE)&dwInfo, (DWORD)4 );
    if( lRtn != ERROR_SUCCESS )
    {
		dwRtn = GetLastError( );
		sprintf( cMsg,"SRS_Reg_SetInfoDWORD : RegSetValueEx Error %d",dwRtn ); 
		SRS_Dbg_WriteLog( cMsg );
        RegCloseKey( hKey );
        return( -1 );
    }
    RegCloseKey( hKey );
    return( 0 );
}
/*=============================================================================
     �֐�   �F���W�X�g�����Q�Ɓi�c�v�n�q�c�j
			�i�w�肳�ꂽ���W�X�g�����̂c�v�n�q�c�����Q�Ƃ���j

     ����   �FlpRegKey�i�Q�ƃ��W�X�g���L�[�j
			  lpReg   �i�Q�ƃ��W�X�g�����ځj

     �o��   �F�Ȃ�

     �߂�l �F���� �Q�ƒl
              �ُ� -1

==============================================================================*/
__declspec( dllexport )DWORD __stdcall SRS_Reg_GetInfoDWORD( char *lpRegKey, char *lpReg)
 {
    LONG    lRtn;                       // 
	DWORD	dwRtn;
	DWORD	dwInfo;
	DWORD	dwSize;
    HKEY    hKey;                       // �₢���킹��L�[�̃n���h��
	char	cMsg[256];

    //===========================================
    // �w��L�[�I�[�v��
    //===========================================
    lRtn = RegOpenKeyEx( HKEY_LOCAL_MACHINE, lpRegKey, 0, KEY_READ, &hKey );
    if( lRtn != ERROR_SUCCESS )     // �I�[�v�����s�H
    {
		dwRtn = GetLastError();
		sprintf( cMsg, "SRS_Reg_GetInfoDWORD : RegOpenKeyEx Error %d",dwRtn );
		SRS_Dbg_WriteLog( cMsg );
        return( -1 );
    }
    //===========================================
    // �ݒ�l�Q�� 
    //===========================================
	dwSize = 4;
    lRtn = RegQueryValueEx( hKey, lpReg, NULL, NULL, (LPBYTE)&dwInfo, &dwSize );
    if( lRtn != ERROR_SUCCESS )         // �l�����s���H
    {
        // �L�[�̃n���h�����
        RegCloseKey( hKey );
        if( lRtn == ERROR_MORE_DATA )   // �o�b�t�@���s�����H
        {
            // �o�b�t�@���s�����͕K�v�Ȓ�����dwSize�ɓ����Ă���
			sprintf( cMsg,"SRS_Reg_GetInfoDWORD : MORE_DATA %d",dwSize ); 
			SRS_Dbg_WriteLog( cMsg );
        }
        else
        {
			dwRtn = GetLastError();
			sprintf( cMsg,"SRS_Reg_GetInfoDWORD : RegQueryValueEx Error %d",dwRtn );
			SRS_Dbg_WriteLog( cMsg );
        }
        return( -1 );
    }
    RegCloseKey( hKey );

    return( dwInfo );
}

/*=============================================================================
     �֐�   �FSRS_Reg_Log�i���O����ʒm����j
     ����   �FlpLog�i���O���j *�m�t�k�k�|�C���^�̏ꍇ�͏I���ʒm�iWM_DESTROY)
     �o��   �F�Ȃ�
     �߂�l �F�O�i����j
              -1�i�ُ�j
=============================================================================*/
__declspec( dllexport )int __stdcall SRS_Reg_Log( char *lpLog )
{
	HWND	hEvent;
	int		iRtn;

	//========================================
	//  �C�x���g�ʒm
	//========================================
	//hEvent = FindWindow( "#32770", "SRSlog" );
	hEvent = FindWindow( "SRSLogV2", "SRSlog" );
	if( hEvent == NULL )
	{
		return( -1 );
	}
	if( lpLog == NULL )		// �I���ʒm�H
	{
		SendMessage( hEvent, WM_DESTROY, (WPARAM)(SRS_MSG_LOG), 0 );
		return( 0 );
	}
	iRtn = SRS_Reg_SetInfo( SRS_REG_KEY_LOGGING, SRS_REG_LOGMSG, lpLog );
	if( iRtn == -1 )
	{
		return( -1 );
	}
	SendMessage( hEvent, WM_COMMAND, (WPARAM)(SRS_MSG_LOG), 0 );
	return( 0 );
}

/*=============================================================================
     �֐�   �F10�i������WORD�l�ϊ�
			�i�w�肳�ꂽ�P�O�i��������v�n�q�c�^�ɕϊ�����j
     ����   �FlpBuff�i�ϊ�������|�C���^�j
     �o��   �F�Ȃ�
     �߂�l �F���� WORD�^�ϊ��l
              �ُ� -1

==============================================================================*/
WORD SRS_Reg_STRINGtoWORD( char *lpBuff )
{
	int		iStrLen;
	int		i;
	WORD	wValue;
	WORD	wTotalValue;
	char	cNo;
	char	cString[256];

	memset( cString, 0, 256 );
	iStrLen = strlen( lpBuff );			// ������
	memcpy( cString, lpBuff, iStrLen );
	wTotalValue = 0;
	for( i = 1 ; iStrLen > 0 ; i = i * 10 )
	{
		iStrLen --;
		cNo = cString[iStrLen];
		cNo = cNo - 0x30;
		wValue = (WORD)cNo * i;
		wTotalValue = wTotalValue + wValue;
	}
	return( wTotalValue );
}
