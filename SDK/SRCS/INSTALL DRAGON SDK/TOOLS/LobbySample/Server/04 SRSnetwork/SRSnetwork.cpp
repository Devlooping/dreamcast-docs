/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRSnetwork.cpp
 *
 *                  SRS_Net_Open 	 : �R�l�N�V��������
 *                  SRS_Net_Close    : �R�l�N�V�������
 *                  SRS_Net_RcvReq 	 : �N���C�A���g�v����M�҂�
 *                  SRS_Net_SendReq  : �v���������M
 *
 *  Content:    Lobby server �l�b�g���[�N���䃉�C�u����
 *
 ***************************************************************************/
/*================================
    �C���N���[�h
==================================*/
#include <windows.h>
#include <stdio.h>

#include "SRScom.h"
#include "SRSdebug.h"
#include "SRSnetwork.h"

/*================================
  �O���[�o����
==================================*/
SOCKET	AcceptClientSd;			//�ڑ�����p�\�P�b�g�f�B�X�N���v�^
SOCKET	RcvClientSd;			//��M�p�\�P�b�g�f�B�X�N���v�^
SOCKET	SendClientSd;			//��M�p�\�P�b�g�f�B�X�N���v�^
/*=============================================================================
     �֐�   �F�R�l�N�V��������
             �i�� Lobby�N���C�A���g�p�̃R�l�N�V��������������j
     ����   �FiPort      �|�[�g�ԍ�
     �o��   �F�Ȃ�
     �߂�l �F����  0
              �ُ� -1

==============================================================================*/
__declspec( dllexport )int __stdcall SRS_Net_Open( int iPort )
{
	SOCKADDR_IN addr;
	WORD 	wVersionRequested = MAKEWORD(1, 1);	// Winsock 1.1 ��v��
	WSADATA wsaData;						// Winsock �̏ڍ�
	int		iResult;						//
	char	cMsg[256];						//

	//=================================
	// �\�P�b�g������
	//=================================
	if( WSAStartup( wVersionRequested, &wsaData ) != 0 ) 
	{
		iResult = WSAGetLastError();
		sprintf( cMsg, "SRS_Net_RcvReq : socket ready error:%d",iResult );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	//=================================
	//�@�\�P�b�g����
	//=================================
	AcceptClientSd = socket( AF_INET, SOCK_STREAM, 0 );
	
	if( AcceptClientSd == INVALID_SOCKET )	// �\�P�b�g�������s
	{
		iResult = WSAGetLastError();
		sprintf( cMsg, "SRS_Ini_Main : socket error %d", iResult);
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons( iPort );			
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//===================================
	// �\�P�b�g�o�C���h 
	//===================================
	iResult = bind( AcceptClientSd, (LPSOCKADDR)&addr, sizeof(addr) );
	if(  iResult == SOCKET_ERROR )
	{
		iResult = WSAGetLastError();
		sprintf( cMsg, "SRS_Net_RcvReq : bind error:%d", iResult );
		SRS_Dbg_WriteLog( cMsg );
		closesocket( AcceptClientSd );
		return( -1 );
	}
	return( 0 );
}

/*=============================================================================
     �֐�   �F�R�l�N�V��������iLobby �N���C�A���g�Ƃ̃R�l�N�V�������������j
     ����   �F�Ȃ�
     �o��   �F�Ȃ�
     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
__declspec( dllexport )int __stdcall SRS_Net_Close( void )
{
	int		iRtn;
	char	cMsg[256];
	//====================================
	//  �\�P�b�g���
	//====================================
	closesocket( AcceptClientSd );		// �ڑ�����p�\�P�b�g���
	closesocket( RcvClientSd );			// ��M�p�\�P�b�g���
	closesocket( SendClientSd );		// ���M�p�\�P�b�g���

	iRtn = WSACleanup();
	if( iRtn == SOCKET_ERROR )
	{
		iRtn = WSAGetLastError();
		sprintf( cMsg, "SRS_Net_Close :Socket CleanUp error:%d\n", iRtn );
		return( -1 );
	}

	return( 0 );
}

/*=============================================================================
     �֐�   �F�N���C�A���g�v����M�҂�
             �iLobby �N���C�A���g����̗v����҂����킹��j
     ����   �FlpRcvData  ��M�f�[�^��|�C���^
			  szSize     ��M�f�[�^�撷	
     �o��   �F�Ȃ�
     �߂�l �F���� ��M�f�[�^��
              �ُ� -1

==============================================================================*/
__declspec( dllexport )int __stdcall SRS_Net_RcvReq( char *lpRcvData, size_t szSize )
{
	SOCKADDR_IN clientAddr;						// �\�P�b�g�A�h���X
	IN_ADDR clientIn;							//
	int 	nClientAddrLen;						// �\�P�b�g�A�h���X�̒���					
	WORD 	wVersionRequested = MAKEWORD(1, 1);	// Winsock 1.1 ��v��
	int 	nBytesRecv;							// ��M�o�C�g��
	int 	iRecvBytes;							// ���v��M�o�C�g��
	int 	nResult;							// 
	int		iRequestSize;						// �v���f�[�^�T�C�Y
	struct	SRS_IF_FIX *lpIfFix;				//
	char	cMsg[256];							//

	//===================================
	//�@�ڑ��𒮎悷�� 
	//===================================
	nResult = listen( AcceptClientSd , 5 );	//�L���[�ɂ͂T�܂ł��ς܂��
	if( nResult == SOCKET_ERROR )
	{
		nResult = WSAGetLastError();
		sprintf( cMsg, "SRS_Net_RcvReq : listen error %d",nResult);
		SRS_Dbg_WriteLog( cMsg );
		closesocket( AcceptClientSd );
		return( -1 );
	}

	nClientAddrLen = sizeof( clientAddr );

	//==========================================
	// �N���C�A���g��M�p�R�l�N�V�����m���҂�
	//==========================================
	SRS_Dbg_WriteLog( "SRS_Net_RcvReq : accept" );
	RcvClientSd = accept( AcceptClientSd, (LPSOCKADDR)&clientAddr, &nClientAddrLen );
	if( RcvClientSd == INVALID_SOCKET )
	{
		nResult = WSAGetLastError();
		sprintf( cMsg, "SRS_Net_RcvReq : accept(RcvClient)error:%d", nResult );
		SRS_Dbg_WriteLog( cMsg );
		closesocket( AcceptClientSd );
		return( -1 );
	}

	// �S�o�C�g�̂h�o�A�h���X���h�o�A�h���X�\���̂ɃR�s�[����
	memcpy( &clientIn, &clientAddr.sin_addr.s_addr, 4 );

	//==========================================
	// �v����M�i���ʈ�j
	//==========================================
	for( iRecvBytes = 0, nBytesRecv = 0 ; iRecvBytes < (SRS_IF_FIX_SZ - 4); )
	{
		nBytesRecv = recv( RcvClientSd, lpRcvData, szSize, 0 );
		if( nBytesRecv == SOCKET_ERROR )	// �ʐM�ُ�H
		{
			nResult = WSAGetLastError();
			sprintf( cMsg, "SRS_Net_RcvReq : recv error:%d", nResult );
			SRS_Dbg_WriteLog( cMsg );
			closesocket( RcvClientSd );
			closesocket( AcceptClientSd );
			return( -1 );
		}
		sprintf( cMsg, "SRS_Net_RcvReq : Recv %d bytes", nBytesRecv );
		SRS_Dbg_WriteLog( cMsg );
		iRecvBytes += nBytesRecv;
	}  
	//==========================================
	// �v����M�i�ŗL��j
	//==========================================
	lpIfFix = (struct SRS_IF_FIX *)lpRcvData;
	iRequestSize = SRS_IF_FIX_SZ - 4 + lpIfFix->IF_iInformationSize;	// �v���f�[�^���Z�o
	for(  ; iRecvBytes < iRequestSize ; )
	{
		nBytesRecv = recv( RcvClientSd, lpRcvData, szSize, 0 );
		if( nBytesRecv == SOCKET_ERROR )	// �ʐM�ُ�H
		{
			nResult = WSAGetLastError();
			sprintf( cMsg, "SRS_Net_RcvReq : recv error:%d", nResult );
			SRS_Dbg_WriteLog( cMsg );
			closesocket( RcvClientSd );
			closesocket( AcceptClientSd );
			return( -1 );
		}
		sprintf( cMsg, "SRS_Net_RcvReq : Recv %d bytes", nBytesRecv );
		SRS_Dbg_WriteLog( cMsg );
		iRecvBytes += nBytesRecv;
	}
	memcpy( (char *)&lpIfFix->IF_iIp, &clientAddr.sin_addr.s_addr, 4 );	// �N���C�A���gIP�ݒ�

	SRS_Dbg_WriteData( "SRS_Net_RcvReq : �v����M", lpRcvData, iRequestSize, "ab" );

	closesocket( RcvClientSd );
	return( iRecvBytes );
}

/*=============================================================================
     �֐�   �F��񑗐M�iLobby �N���C�A���g�֏��𑗐M����j
     ����   �FlpSendData  ���M�f�[�^��|�C���^
			  iSize		  ���M�f�[�^��	
     �o��   �F�Ȃ�
     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
__declspec( dllexport )int __stdcall SRS_Net_SendReq( int iClientIP, char *lpSendData, size_t szSize )
{
	SOCKET	ClientSd;							//
	int 	iResult;							//
	int 	iSendLen;							// ���M�f�[�^��
	int 	iSendData;							// ���M�f�[�^���i�J�E���g�p�j
	char	cDummyBuff[16];						// �v�����B�҂��p�_�~�[�o�b�t�@
	char	cMsg[256];

	// select�p�ϐ�   
	struct fd_set *lpReadFds = new struct fd_set;		//�����޽������W��
	struct timeval *lpSockTime = new struct timeval;	//�^�C�}�\����
	struct timeval *lpConnectTime = new struct timeval;	//select�p�^�C�}�\����
	struct fd_set *lpWriteFds = new struct fd_set;		//�����޽������̏W���i�������ݏ�ԎQ�Ɓj
	// socket�p
	WORD      wVersionRequested = MAKEWORD(1, 1);		// Winsock 1.1 ��v��
	WSADATA   wsaData;									// Winsock �̏ڍ�
	SOCKADDR_IN   ClientAddr;							//�N���C�A���g���̃A�h���X�\����
	SOCKADDR_IN   ServerAddr;							//�T�[�o���̃A�h���X�\����
	u_long ulCmdArg = 1;								//�u���b�L���O���[�h


	/*==================================*/
	/* �\�P�b�g������                   */
	/*==================================*/
	sprintf( cMsg, "SRS_Net_SendReq :���M�v�� %d bytes", szSize );
	SRS_Dbg_WriteLog( cMsg );
	SRS_Dbg_WriteData( "SRS_Net_SendReq :���M�v��", lpSendData, szSize, "ab" );

	iResult = WSAStartup( wVersionRequested, &wsaData );
	if ( iResult != 0 )
	{
		iResult = WSAGetLastError();
		sprintf( cMsg, "SRS_Net_SendReq : WSAStartup error:%d", iResult );
		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}

	ClientSd = socket( AF_INET, SOCK_STREAM, 0 );	//�N���C�A���g���M�p�\�P�b�g�쐬

	/*==================================*/
	/* �Z�b�V�����m�� 		 			*/
	/*==================================*/
	// �A�h���X�\���̂ɃN���C�A���g�A�h���X������������
	ClientAddr.sin_family = AF_INET;
	ClientAddr.sin_port = 0;							// �|�[�g���蓖�āi0:�C�ӂ̖��g�p�̃|�[�g�j
	ClientAddr.sin_addr.s_addr = htonl(INADDR_ANY);	

	// �\�P�b�g�ɃA�h���X���o�C���h����
	iResult = bind( ClientSd, (LPSOCKADDR)&ClientAddr, sizeof(ClientAddr));
	if( iResult == SOCKET_ERROR )
	{
		iResult = WSAGetLastError();
		sprintf( cMsg, "SRS_Net_SendReq : bind error:%d", iResult );
		SRS_Dbg_WriteLog( cMsg );

		iResult = closesocket(ClientSd);
		return( -1 );
	}
	// �A�h���X�\���̂ɃT�[�o�A�h���X������������
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons( SRS_PORT_CLIENT );		
	ServerAddr.sin_addr.s_addr = iClientIP;	

	//�@�R�l�N�V�����m���@
	SRS_Dbg_WriteLog( "SRS_Net_SendReq : connect" );
	iResult = connect( ClientSd, (LPSOCKADDR)&ServerAddr, sizeof(ServerAddr) );
	if( iResult == SOCKET_ERROR )
	{
		iResult = WSAGetLastError();			//�G���[�ڍׂ��l��
		if( iResult == WSAEWOULDBLOCK )			//�\�P�b�g�G���[���u���b�N�̏ꍇ
		{
			FD_ZERO(lpWriteFds);				//�\�P�b�g�f�X�N�v���^�̏W����������
			FD_SET(ClientSd, lpWriteFds);	//�Ď�����\�P�b�g��ݒ�

			// select�ɂ��Ď�
			lpConnectTime->tv_sec = 10;			//�b
			lpConnectTime->tv_usec = 0;			//�~���b

			sprintf( cMsg, "SRS_Net_SendReq : select wait=%d�b", lpConnectTime->tv_sec );
			SRS_Dbg_WriteLog( cMsg );
			iResult = select(0, NULL, lpWriteFds, NULL, lpConnectTime);

			//�@�^�C���A�E�g�܂��̓G���[�̏ꍇ
			if( (iResult == 0) || (iResult == SOCKET_ERROR) )
			{
				sprintf( cMsg, "SRS_Net_SendReq : select error %d", iResult );
				SRS_Dbg_WriteLog( cMsg );
				iResult = closesocket(ClientSd);
				return( -1 );
			}
		}
		else
		{
			// �u���b�N�ȊO�̃G���[
			sprintf( cMsg, "SRS_Net_SendReq : connect error %d", iResult );
			SRS_Dbg_WriteLog( cMsg );
			iResult = closesocket(ClientSd);
			return( -1 );
		}
	}
	/*==================================*/
	/* �v���i�����j���M		  			*/
	/*==================================*/
	for( iSendLen = 0 ; iSendLen < (int)szSize ; )
	{
		iSendData = send( ClientSd, (char*)lpSendData, szSize, 0 );
		if ( iSendData == SOCKET_ERROR )
		{
			iResult = WSAGetLastError();
			sprintf( cMsg,"SRS_Net_SendReq : Send error :%d", iResult );
			SRS_Dbg_WriteLog( cMsg );
			closesocket( ClientSd );
			return( -1 );
		}
		iSendLen += iSendData;
		sprintf( cMsg, "SRS_Net_SendReq : Send %d/%d bytes",iSendLen, szSize );
		SRS_Dbg_WriteLog( cMsg );
	}
	/*==================================*/
	/* �v�����B�`�F�b�N  				*/
	/*==================================*/
	// �N���C�A���g�����R�l�N�V�������������܂ő҂�
	iResult = recv( ClientSd, cDummyBuff, 16, 0 );
	closesocket( ClientSd );
	return( 0 );
}

