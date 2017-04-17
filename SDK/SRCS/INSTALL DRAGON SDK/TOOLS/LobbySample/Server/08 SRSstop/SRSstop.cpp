/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRSstop.cpp
 *
 *  Content:    Lobby server terminate command �T�[�o�I���R�}���h
 *
 ***************************************************************************/
/*================================
    �C���N���[�h
==================================*/
#include <windows.h>
#include <stdio.h>

#include "SRScom.h"
#include "SRSdebug.h"

//////////////////////
// Additional Info.
// fixed... 2000.1.21 ���������[�N�o�O - chiba
// fixed... 2000.1.24 �T�[�o�[IP�A�h���X�擾���@�ύX - chiba
//

HKEY ghKey;

LONG RegistryOpen()
{
	return RegOpenKeyEx( HKEY_LOCAL_MACHINE, "SOFTWARE\\LOBBY SERVER", 0, KEY_ALL_ACCESS, &ghKey );
}

void RegistryClose()
{
	if ( ghKey )
	{
		RegCloseKey( ghKey );
		ghKey = NULL;
	}
}

void RegistryGetValue( char *str, char *dst )
{
	char temp[MAX_PATH];
	DWORD dw;
	dw = MAX_PATH;
	RegQueryValueEx( ghKey, str, NULL, NULL, (LPBYTE) &temp, &dw );
	strcpy( dst, temp );
}

DWORD SRSLog_IPLoad()
{
	char str[MAX_PATH];
	DWORD bi1, bi2, bi3, bi4;
	bi1 = bi2 = bi3 =  bi4 = 0;
	if ( RegistryOpen() ) return 0;
	RegistryGetValue( "IP Address", str );
	RegistryClose();
	sscanf( str, "%d.%d.%d.%d", &bi1, &bi2, &bi3, &bi4 );
	return ((bi1 & 0xff) | ((bi2 & 0xff) << 8) | ((bi3 & 0xff) << 16) | ((bi4 & 0xff) << 24));
}

/*=============================================================================
     �֐�   �FLobby�T�[�o �v���Z�X�I��
			�iLobby�v���҂��v���Z�X�ւ̏I���ʒm�������Ȃ��j
     ����   �F�Ȃ�
     �o��   �F�Ȃ�
     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
int WINAPI WinMain( HINSTANCE hInstanc,HINSTANCE hPrevInstanc,
					LPSTR lpszCmdLin,int nCmdSho )
{
	SOCKET	ServerSd;			
	int 	iResult;							//
	int 	iSendLen;							// ���M�f�[�^��
	int 	iSendData;							// ���M�f�[�^���i�J�E���g�p�j
    PHOSTENT lphostent;     					// �z�X�g���̍\���̃|�C���^
	struct	SRS_IF_FIX *lpIfFix;				//
	char	cIfFix[256];						//
	char	cServerName[32];					//
	char	cDummyBuff[16];						//
	char	cMsg[256];

	// select�p�ϐ�   
//	struct fd_set *lpReadFds = new struct fd_set;		//�����޽������W��
//	struct timeval *lpSockTime = new struct timeval;	//�^�C�}�\����
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
	iResult = WSAStartup( wVersionRequested, &wsaData );
	if ( iResult != 0 )
	{
		iResult = WSAGetLastError();
		sprintf( cMsg, "SRSstop : WSAStartup error:%d", iResult );
		SRS_Dbg_WriteLog( cMsg );
		goto ERRORRET;
//		return( -1 );
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
		sprintf( cMsg, "SRSstop : bind error:%d", iResult );
		SRS_Dbg_WriteLog( cMsg );

		iResult = closesocket(ServerSd);
		goto ERRORRET;
//		return( -1 );
	}
	// �A�h���X�\���̂ɃT�[�o�A�h���X������������
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(SRS_PORT_SERVER);
    gethostname( cServerName, sizeof(cServerName) );
    lphostent = gethostbyname(cServerName);
    memcpy( (char *)&ServerAddr.sin_addr.s_addr, *lphostent->h_addr_list, 4);
	{
		//IP�A�h���X�����W�X�g������Q�b�g���Ă݂�B������烌�W�X�g���̂ق����g���B
		DWORD ip;
		ip = SRSLog_IPLoad();
		if ( ip != 0 )
		{
			if ( ip != ServerAddr.sin_addr.s_addr )
			{
				sprintf( cMsg, "SRSstop : IP error:Sock[%d.%d.%d.%d] Reg[%d.%d.%d.%d]", (ServerAddr.sin_addr.s_addr & 0xff), ((ServerAddr.sin_addr.s_addr >> 8) & 0xff), ((ServerAddr.sin_addr.s_addr >> 16) & 0xff), ((ServerAddr.sin_addr.s_addr >> 24) & 0xff), (ip & 0xff), ((ip >> 8) & 0xff), ((ip >> 16) & 0xff), ((ip >> 24) & 0xff) );
				SRS_Dbg_WriteLog( cMsg );
				ServerAddr.sin_addr.s_addr = ip;
			}
		}
	}
/*
	{
		char str[MAX_PATH];
		sprintf( str, "%d.%d.%d.%d", (ServerAddr.sin_addr.s_addr & 0xff), ((ServerAddr.sin_addr.s_addr >> 8) & 0xff), ((ServerAddr.sin_addr.s_addr >> 16) & 0xff), ((ServerAddr.sin_addr.s_addr >> 24) & 0xff) );
		MessageBox( NULL, cServerName, str, MB_OK );
	}
*/
	//�@�R�l�N�V�����m���@
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

			SRS_Dbg_WriteLog( "SRSstop : select" );
			iResult = select(0, NULL, lpWriteFds, NULL, lpConnectTime);

			//�@�^�C���A�E�g�܂��̓G���[�̏ꍇ
			if( (iResult == 0) || (iResult == SOCKET_ERROR) )
			{
				sprintf( cMsg, "SRSstop : select error %d", iResult );
				SRS_Dbg_WriteLog( cMsg );
				iResult = closesocket(ServerSd);
				goto ERRORRET;
//				return( -1 );
			}
		}
		else
		{
			// �u���b�N�ȊO�̃G���[
			sprintf( cMsg, "SRSstop : connect error %d", iResult );
			SRS_Dbg_WriteLog( cMsg );
			iResult = closesocket(ServerSd);
			goto ERRORRET;
//			return( -1 );
		}
	}

	/*==================================*/
	/* �v�����M				  			*/
	/*==================================*/
	//�@�v���쐬
	memset( cIfFix, 0, 256 );
	lpIfFix = (struct SRS_IF_FIX *)cIfFix;
	lpIfFix->IF_wRequest = IF_REQ_TERMINATE;

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
		sprintf( cMsg,"SRSstop : Send error :%d", iResult );
		SRS_Dbg_WriteLog( cMsg );
		closesocket( ServerSd );
		goto ERRORRET;
//		return( -1 );
	}

	/*==================================*/
	/* �v�����B�`�F�b�N  				*/
	/*==================================*/
	// �N���C�A���g�����R�l�N�V�������������܂ő҂�
	iResult = recv( ServerSd, cDummyBuff, 16, 0 );

	SRS_Dbg_WriteLog( "SRSstop OK" );
	closesocket( ServerSd );

	delete lpConnectTime;
	delete lpWriteFds;

	return( 0 );

ERRORRET:
	delete lpConnectTime;
	delete lpWriteFds;

	return ( -1 );
}
