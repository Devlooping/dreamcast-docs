/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRSlobby.cpp
 *					SRS_Lby_GetPort			(�󂫃|�[�g�l��)
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

BOOL InitInstance( HINSTANCE hInst, int nCmdShow )
{
	HWND hWnd;

	hWnd = CreateWindow( "SRSlobby", "SRSlobby", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInst, NULL );
	if ( ! hWnd ) return FALSE;
	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );
	return TRUE;
}

BOOL InitApp( HINSTANCE hInst, WNDPROC fnWndProc, LPCTSTR szClassNm )
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = fnWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "SRSlobby";
	wc.hIconSm = wc.hIcon;
	return ( RegisterClassEx( &wc ) );
}

//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_CREATE:
		break;
	case WM_CLOSE:
		DestroyWindow( hWnd );
		break;
	case WM_DESTROY:
		break;
	default:
		return( DefWindowProc( hWnd, msg, wParam, lParam ) );
	}
	return 0;
}

/*=============================================================================
     �֐�   �FLobby�v���Z�X������ �� �N���C�A���g�v���҂��v���Z�X
			�iLobby�v���Z�X�����ݒ�� Lobby�N���C�A���g�v���̏����������Ȃ��j
     ����   �F�Ȃ�
     �o��   �F�Ȃ�
     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
int WINAPI WinMain( HINSTANCE hInstanc,HINSTANCE hPrevInstanc,
					LPSTR lpszCmdLin,int nCmdSho )
{
	int		iRtn;								//
	int		iNameLen;							// 
	int		iPort;								// �v���҂����킹�|�[�g�ԍ�
	int		iNo;								// 
	int		iRcvSize;							// ��M�f�[�^��
	size_t	szSize;								//
	struct	SRS_IF_FIX *lpIfFix;				// �C���^�t�F�[�X��|�C���^
	LPTSTR	lpProcName;							// �v���Z�X���|�C���^
	char	*lpRcvBuff;							// ��M�o�b�t�@��|�C���^
	char	cProcName[256];						// �v���Z�X����
	char	cMsg[256];
	HWND hEvent;
	hEvent = FindWindow( "SRSLogV2", "SRSlog" );

//	if ( ! InitApp( hInstanc, WndProc, "SRSlobby" ) ) return FALSE;
//	if ( ! InitInstance( hInstanc, nCmdSho ) ) return FALSE;

	//========================================
	// �|�[�g�ԍ��l��
	//========================================
	memset( cProcName, 0, 256 );				//
	lpProcName = GetCommandLine();				// �R�}���h���C���l��
	sprintf( cProcName, "%s",lpProcName );		//
	iNameLen = strlen( SRS_PROC_LOBBY );		// ��{�����l��

	//
	// �ԍ��ϊ��i�S���L�����N�^���o�C�i�����l�j
	//
	iNo = (int)(cProcName[iNameLen] & 0x0f);	// �S����
	iPort = iNo * 1000;							// 
	iNo = (int)(cProcName[iNameLen+1] & 0x0f);	// �R����
	iPort = iPort + (iNo * 100);				// 
	iNo = (int)(cProcName[iNameLen+2] & 0x0f);	// �Q����
	iPort = iPort + (iNo * 10);					// 
	iNo = (int)(cProcName[iNameLen+3] & 0x0f);	// �P����
	iPort = iPort + iNo;						// 

	//======================================== 
	// Lobby�T�[�o���ݒ�
	//========================================
	//
	//  �Ǘ������ݒ�
	//
	iRtn = SRS_Mng_GetInfo( SRS_INFO_GET );		// �Ǘ�����荞��
	if( iRtn != 0 )								// ��荞�ݎ��s�H
	{
		sprintf( cMsg, "SRS_Lby_Main : SRS_Mng_GetInfo error" );
		SRS_Dbg_WriteLog( cMsg );				
		return( -1 );
	}
	//
	//  �l�b�g���[�N���ݒ�
	//
	lpRcvBuff = (char *)malloc( SRS_MAX_RCV );	// ��M�o�b�t�@�l��
	if( lpRcvBuff == NULL )
	{
		sprintf( cMsg, "SRS_Lby_main : malloc error" );
		SRS_Dbg_WriteLog( cMsg );				
		SRS_Lby_Term( iPort );					// ���r�[�������
		return( -1 );
	}
	memset( lpRcvBuff, 0, SRS_MAX_RCV );

	iRtn = SRS_Net_Open( iPort );				// �R�l�N�V��������
	if( iRtn == -1 )							// �ʐM�ُ�H
	{
		SRS_Lby_Term( iPort );					// ���r�[�������
		return( -1 );
	}
	//========================================
	// �v���҂�
	//========================================
	for( ; ; )
	{
		szSize = SRS_MAX_RCV;
		iRcvSize = SRS_Net_RcvReq( (char *)lpRcvBuff, szSize  );	// �N���C�A���g�v���҂�
		if( iRcvSize == -1 )											// �ʐM�ُ�H
		{	
			iRtn = SRS_Net_Close( );									// �R�l�N�V�������
			sprintf( cMsg, "SRS_Lby_Main : SRS_Net_Receive error" );
			SRS_Dbg_WriteLog( cMsg );				
			SRS_Lby_Term( iPort );										// ���r�[�������
			return( -1 );				
		}
		//=======================================
		//  ��M�v������
		//=======================================

		lpIfFix = (struct SRS_IF_FIX *)lpRcvBuff;

		// ���N�G�X�g��SRSLog�ɑ���
		SendMessage( hEvent, WM_COMMAND, (WPARAM)WM_USER+2, (LPARAM)lpIfFix->IF_wRequest );

		switch( lpIfFix->IF_wRequest )
		{
			case	IF_REQ_ALL			: 	// �S���Q��
						iRtn = SRS_Lby_ReqAll( lpIfFix );
						break;

			case	IF_REQ_USER_ENTRY	:	// ���[�U�o�^
						iRtn = SRS_Lby_ReqUserEntry( lpIfFix );
						break;

			case	IF_REQ_USER_DELETE	:	// ���[�U�폜
						iRtn = SRS_Lby_ReqUserDelete( lpIfFix );
						break;

			case	IF_REQ_LOBBY_ENTRY	:	// ���r�[���[�U�Q��
						iRtn = SRS_Lby_ReqLobbyEntry( lpIfFix );
						break;

			case	IF_REQ_LOBBY_DEFECT	:	// ���r�[���[�U�E��
						iRtn = SRS_Lby_ReqLobbyDefect( lpIfFix );
						break;

			case	IF_REQ_GROUP_MAKE	:	// �O���[�v�쐬
						iRtn = SRS_Lby_ReqGroupMake( lpIfFix );
						break;

			case	IF_REQ_GROUP_JOIN	:	// �O���[�v�Q��
						iRtn = SRS_Lby_ReqGroupJoin( lpIfFix );
						break;

			case	IF_REQ_GROUP_DEFECT	:	// �O���[�v�E��
						iRtn = SRS_Lby_ReqGroupDefect( lpIfFix );
						break;

			case	IF_REQ_GAME_JOIN	:	// �Q�[���Q��
						iRtn = SRS_Lby_ReqGameJoin( lpIfFix );
						break;

			case	IF_REQ_GAME_DEFECT	:	// �Q�[���E��
						iRtn = SRS_Lby_ReqGameDefect( lpIfFix );
						break;

			case	IF_REQ_GAME_START	:	// �Q�[���J�n
						iRtn = SRS_Lby_ReqGameStart( lpIfFix );
						break;

			case	IF_REQ_GAME_END		:	// �Q�[���I��
						iRtn = SRS_Lby_ReqGameEnd( lpIfFix );
						break;

			case	IF_REQ_SENDMSG		:	// ���b�Z�[�W���M
						iRtn = SRS_Lby_ReqSendMsg( lpIfFix );
						break;

			case	IF_REQ_SAVE_INFO	:	// �ŗL���Ҕ�
						iRtn = SRS_Lby_ReqSaveInfo( lpIfFix );
						break;

			case	IF_REQ_TERMINATE	:	// �I���ʒm
				{

//					char str[260];
//						WORD wID = SRS_Mng_GetUserByIP( lpIfFix->IF_iIp );
//						*((LPWORD) (lpIfFix->IF_cInformation)) = wID;
//						sprintf( str, "%d - %08x", wID, lpIfFix->IF_iIp );
//						MessageBox( NULL, str, "", MB_OK );
//						*((LPWORD) (lpIfFix->IF_cInformation)) = 1;
						*((LPWORD) (lpIfFix->IF_cInformation)) = (WORD) SendMessage( hEvent, WM_COMMAND, (WPARAM)WM_USER+4, 0 );

//						lpIfFix->IF_wRequest = IF_REQ_USER_DELETE;
						sprintf( cMsg, "SRSlobby : �I���O���� IP=%08x", lpIfFix->IF_iIp );
						SRS_Dbg_WriteLog( cMsg );

						SRS_Lby_ReqUserDeleteEX( lpIfFix );
						iRtn = SRS_Net_Close( );
						SRS_Dbg_WriteLog( "SRSlobby : �I���ʒm" );
						SendMessage( hEvent, WM_COMMAND, (WPARAM)WM_USER+3, 0 );
						ExitProcess( 0 );
				}
			default						:   // �V�X�e���G���[
						iRtn = SRS_Lby_ReqSeqError( lpIfFix );	 // �v���V�[�P���X�G���[
						SRS_Dbg_WriteLog( "SRSlobby : �V�X�e���G���[ " );
						break;
		}

		// ��t���ǉ�
		{
			WORD wID;

			wID = SRS_Mng_GetUserByIP( lpIfFix->IF_iIp );
			if ( wID != SRS_WORD_ERROR )
			{
				SRS_Mng_PutPort( iPort, wID );
				SRS_Mng_PutInfo( wID );
			}

			// SRSLog�̕\�����A�b�v�f�[�g����
			if ( lpIfFix->IF_wRequest == IF_REQ_USER_ENTRY || lpIfFix->IF_wRequest == IF_REQ_USER_DELETE )
				SendMessage( hEvent, WM_COMMAND, (WPARAM)WM_USER+3, 0 );
		}

		//
		//  �ʐM�ُ픻��
		//
		if( iRtn != 0 )				// �ʐM�ُ픭���H
		{
			SRS_Lby_Term( iPort );	// ���r�[�������
			break;
		}
	}
	//========================================
	// �΃N���C�A���g�ʐM�����
	//========================================
	iRtn = SRS_Net_Close( );
	return( 0 );
}

/*=============================================================================
     �֐�    �F����v����s
			  �iLobby�N���C�A���g�ɑ���ɉ���v���𔭍s����j
     ����    �FiPort  (�N���C�A���g�ʒm�|�[�g�ԍ�)
     �o��    �F�Ȃ�
     �߂�l  �F�Ȃ�

==============================================================================*/
void SRS_Lby_Term( int iPort )
{
	struct	SRS_IF_FIX	*lpIfFix;		//
	char 	cIfFix[256];				//
	char	cMsg[256];

	// socket�p
	SOCKET	ServerSd;								//
	int 	iResult;								//
	int 	iSendLen;								// ���M�f�[�^��
	int 	iSendData;								// ���M�f�[�^���i�J�E���g�p�j
	WORD    wVersionRequested = MAKEWORD(1, 1);		// Winsock 1.1 ��v��
	WSADATA wsaData;								// Winsock �̏ڍ�
	SOCKADDR_IN   ClientAddr;						//�N���C�A���g���̃A�h���X�\����
	SOCKADDR_IN   ServerAddr;						//�T�[�o���̃A�h���X�\����
	u_long ulCmdArg = 1;							//�u���b�L���O���[�h
    PHOSTENT lphostent; 							//�z�X�g���̍\���̃|�C���^
	char	cServerName[32];						//
	char	cDummyBuff[16];							//
	DWORD	dwSize;									//

	if( 0 == 0 )
		return;
	//========================================
	//  �R�l�N�V�����m��
	//========================================
	// select�p�ϐ�   
	struct fd_set *lpReadFds = new struct fd_set;		//�����޽������W��
	struct timeval *lpSockTime = new struct timeval;	//�^�C�}�\����
	struct timeval *lpConnectTime = new struct timeval;	//select�p�^�C�}�\����
	struct fd_set *lpWriteFds = new struct fd_set;		//�����޽������̏W���i�������ݏ�ԎQ�Ɓj

	/*==================================*/
	/* �\�P�b�g������                   */
	/*==================================*/
	iResult = WSAStartup( wVersionRequested, &wsaData );
	if ( iResult != 0 )
	{
		iResult = WSAGetLastError();
		sprintf( cMsg, "SRS_Lby_Term : WSAStartup error:%d", iResult );
		SRS_Dbg_WriteLog( cMsg );
		return;
	}

	ServerSd = socket( AF_INET, SOCK_STREAM, 0 );	//�T�[�o���M�p�\�P�b�g�쐬

	/*==================================*/
	/* �Z�b�V�����m�� 		 			*/
	/*==================================*/
	// �A�h���X�\���̂ɃN���C�A���g�A�h���X������������
	ClientAddr.sin_family = AF_INET;
	ClientAddr.sin_port = 0;							// �|�[�g���蓖�āi0:�C�ӂ̖��g�p�̃|�[�g�j
	ClientAddr.sin_addr.s_addr = htonl(INADDR_ANY);	

	// �\�P�b�g�ɃA�h���X���o�C���h����
	iResult = bind( ServerSd, (LPSOCKADDR)&ClientAddr, sizeof(ClientAddr));
	if( iResult == SOCKET_ERROR )
	{
		iResult = WSAGetLastError();
		sprintf( cMsg, "SRS_Lby_Term : bind error:%d", iResult );
		SRS_Dbg_WriteLog( cMsg );

		iResult = closesocket(ServerSd);
		return;
	}
	// �A�h���X�\���̂ɃT�[�o�A�h���X������������
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(SRS_PORT_SERVER);
	memset( cServerName, 0, sizeof(cServerName) );
	dwSize = (DWORD)sizeof(cServerName);
    GetComputerName( cServerName, &dwSize );	// �T�[�o���l��
    lphostent = gethostbyname(cServerName);		// IP�A�h���X�l��
    memcpy( (char *)&ServerAddr.sin_addr.s_addr, *lphostent->h_addr_list, 4);

	//�@�R�l�N�V�����m���@
	SRS_Dbg_WriteLog( "SRS_Lby_Term : connect" );
	iResult = connect( ServerSd, (LPSOCKADDR)&ServerAddr, sizeof(ServerAddr) );
	if( iResult == SOCKET_ERROR )
	{
		iResult = WSAGetLastError();			//�G���[�ڍׂ��l��
		if( iResult == WSAEWOULDBLOCK )			//�\�P�b�g�G���[���u���b�N�̏ꍇ
		{
			FD_ZERO(lpWriteFds);				//�\�P�b�g�f�X�N�v���^�̏W����������
			FD_SET(ServerSd, lpWriteFds);	//�Ď�����\�P�b�g��ݒ�

			// select�ɂ��Ď�
			lpConnectTime->tv_sec = 10;			//�b
			lpConnectTime->tv_usec = 0;			//�~���b

			SRS_Dbg_WriteLog( "SRS_Lby_Term : select" );
			iResult = select(0, NULL, lpWriteFds, NULL, lpConnectTime);

			//�@�^�C���A�E�g�܂��̓G���[�̏ꍇ
			if( (iResult == 0) || (iResult == SOCKET_ERROR) )
			{
				sprintf( cMsg, "SRS_Lby_Term : select error %d", iResult );
				SRS_Dbg_WriteLog( cMsg );
				iResult = closesocket(ServerSd);
				return;
			}
		}
		else
		{
			// �u���b�N�ȊO�̃G���[
			sprintf( cMsg, "SRS_Lby_Term : connect error %d", iResult );
			SRS_Dbg_WriteLog( cMsg );
			iResult = closesocket(ServerSd);
			return;
		}
	}

	/*==================================*/
	/* �v�����M				  			*/
	/*==================================*/
	//�@�v���쐬
	memset( cIfFix, 0, 256 );
	lpIfFix = (struct SRS_IF_FIX *)cIfFix;
	lpIfFix->IF_wRequest = IF_REQ_DISCONNECT;
	lpIfFix->IF_iPortNo = iPort;

	//  �v�����M
	iSendLen = 0;
	do
	{
		iSendData = send( ServerSd, (char*)cIfFix, SRS_IF_FIX_SZ, 0 );
		iSendLen += iSendData;								
	}while( (iSendData != SOCKET_ERROR) && (iSendLen != SRS_IF_FIX_SZ) );

	if ( iSendData == SOCKET_ERROR )
	{
		iResult = WSAGetLastError();
		sprintf( cMsg,"SRS_Lby_Term : Send error :%d", iResult );
		SRS_Dbg_WriteLog( cMsg );
	}
	/*==================================*/
	/* �v�����B�`�F�b�N  				*/
	/*==================================*/
	// �N���C�A���g�����R�l�N�V�������������܂ő҂�
	iResult = recv( ServerSd, cDummyBuff, 16, 0 );

	closesocket( ServerSd );
	return;
}