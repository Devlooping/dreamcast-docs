/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRSinitial.cpp
 *					SRS_Ini_Server			(�T�[�o������)
 *					SRS_Ini_GetPort			(�󂫃|�[�g�l��)
 *					SRS_Ini_UsePort			(�g�p���|�[�g�l��)
 *					SRS_Ini_ReleasePort		(�g�p�|�[�g���)
 *					SRS_Ini_TermProc		(Lobby�v���Z�X���)
 *					SRS_Ini_CreateLobby		(���r�[�v���Z�X����)
 *					SRS_Ini_CreateProcess	(�v���Z�X����)
 *
 *  Content:    Lobby server �����ݒ聕�ڑ��v���҂��v���Z�X
 *
 ***************************************************************************/
/*================================
    �C���N���[�h
==================================*/
#include <windows.h>
#include <stdio.h>

#include "SRScom.h"
#include "SRSdebug.h"
#include "SRSregistry.h"
#include "SRSmanage.h"
#include "SRSinitial.h"

/*================================
    �O���[�o����
==================================*/
SOCKET 	AcceptClientSd;					//�ڑ�����p�\�P�b�g�f�B�X�N���v�^
SOCKET 	ClientSd;						//�N���C�A���g�\�P�b�g�f�B�X�N���v�^
struct	SRS_PORT_LIST 	*lpPortList;	//	�|�[�g�ԍ����X�g�|�C���^

HKEY ghKey = NULL;

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
     �֐�   �F�����ݒ聕�ڑ��v���҂�
			�i�T�[�o�����ݒ�� Lobby�N���C�A���g����̐ڑ��v����҂j
     ����   �F�Ȃ�
     �o��   �F�Ȃ�
     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
int WINAPI WinMain( HINSTANCE hInstanc,HINSTANCE hPrevInstanc,
					LPSTR lpszCmdLin,int nCmdSho )
{
	SOCKADDR_IN addr;
	SOCKADDR_IN clientAddr;					//�\�P�b�g�A�h���X
	IN_ADDR clientIn;						//
	int 	nClientAddrLen;					//�\�P�b�g�A�h���X�̒���					
	WORD 	wVersionRequested = MAKEWORD(1, 1);	// Winsock 1.1 ��v��
	WSADATA wsaData;						// Winsock �̏ڍ�
	int 	nBytesRecv;
	int 	iRecvBytes;
	int 	nResult;
	int		iRtn;
	int		iResult;						//���ʗp�ϐ�
	int		iSendData;						//send�p ���M�o�C�g���i���ʁj
	int		iSendLen;						//send�p ���M�o�C�g��
	int		iSendSize;						//���M�f�[�^��

	int		iLoopOut;						// �v���҂��I�����
	int		iClientIp;						// �N���C�A���g�h�o�A�h���X
	int		iPort;							// �|�[�g�ԍ�
	struct 	SRS_IF_FIX	*lpIfFix;			//
	struct	SRS_IF_CONNECT	*lpIfConnect;	//
	struct	SRS_REG_INFO *lpRegInfo;		//
	unsigned char	cIp[8];
	char	cIfFix[256];					// �C���^�t�F�[�X��
	char	cRegInfo[SRS_REG_INFO_SZ];		//
	char	cSendMessage[SRS_MAX_LOGMSG];	//
	char	cMsg[256];						//

//=================================================================================
//
//		�ʐM���ݒ�
//
//=================================================================================
	CopyFile( OUT_FILE_LOG, OUT_FILE_LOGBACK, FALSE );
	CopyFile( OUT_FILE_DATA, OUT_FILE_DATABACK, FALSE );
	DeleteFile( OUT_FILE_LOG );			// ���O�t�@�C���폜
	DeleteFile( OUT_FILE_DATA );		// �f�[�^�t�@�C���폜
	
	//======================================
	//  �T�[�o��������
	//======================================
	iRtn = SRS_Ini_Server( );			// �T�[�o�����ݒ�
	if( iRtn != 0 )						// �����ݒ莸�s�H
	{
//		sprintf( cMsg, "SRS_Ini_Server error" );
//		SRS_Dbg_WriteLog( cMsg );
		iRtn = SRS_Reg_Log( 0 );
		return( -1 );
	}

	//=================================
	// �\�P�b�g������
	//=================================
	if( WSAStartup( wVersionRequested, &wsaData ) != 0 ) 
	{
//		nResult = WSAGetLastError();
//		sprintf( cMsg, "SRS_Ini_main : socket ready error:%d",errno);
//		SRS_Dbg_WriteLog( cMsg );
		iRtn = SRS_Reg_Log( 0 );
		return( -1 );
	}
	//=================================
	//�@�\�P�b�g����
	//=================================
	AcceptClientSd = socket( AF_INET, SOCK_STREAM, 0 );
	
	if( AcceptClientSd == INVALID_SOCKET )	// �\�P�b�g�������s
	{
//		errno = WSAGetLastError();
//		sprintf( cMsg, "SRS_Ini_Main : socket error %d",errno);
//		SRS_Dbg_WriteLog( cMsg );
		iRtn = SRS_Reg_Log( 0 );
		return( -1 );
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons( SRS_PORT_SERVER );			
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//===================================
	// �\�P�b�g�o�C���h 
	//===================================
	nResult = bind( AcceptClientSd, (LPSOCKADDR)&addr, sizeof(addr) );
	if(  nResult == SOCKET_ERROR )
	{
//		nResult = WSAGetLastError();
//		sprintf( cMsg, "SRS_Ini_main : bind error:%d", nResult );
//		SRS_Dbg_WriteLog( cMsg );
		closesocket( AcceptClientSd );
		iRtn = SRS_Reg_Log( 0 );
		return( -1 );
	}
	//===================================
	//�@�ڑ��𒮎悷�� 
	//===================================
//  NOTE :
//    �o�b�N���O�́AWinSock���ߏ�Ȏ����������Ă邱�Ƃ�h�~����ی�ԂƂ��ē��삷��B
//    �P�K�̏����Ԋu���ɂT�ȏ�̐ڑ�������ƃN���C�A���g�� WSAECONNREFUSED(10061�F�ڑ����ہj
//    ���ʒm�����B

	nResult = listen( AcceptClientSd , 5 );	//�o�b�N���O�Ƃ��ăL���[�ɂ͂T�܂ł��ς܂��
	if( nResult == SOCKET_ERROR )
	{
//		nResult = WSAGetLastError();
//		sprintf( cMsg, "SRS_Ini_main : listen error %d",nResult);
//		SRS_Dbg_WriteLog( cMsg );
		closesocket( AcceptClientSd );
		iRtn = SRS_Reg_Log( 0 );
		return( -1 );
	}

	//==========================================================================
	//
	//	�v���҂����킹
	//
	//==========================================================================
	for( iLoopOut = 0 ; iLoopOut == 0 ; )
	{
//		GetPrivateProfileString( "Message", "Message", "���b�Z�[�W���o�^����Ă��܂���", cSendMessage, sizeof(cSendMessage), ".\\message.txt");
//		SRS_Dbg_WriteLog( cSendMessage );

		//==========================================
		// �N���C�A���g��M�p�R�l�N�V�����m���҂�
		//==========================================
		nClientAddrLen = sizeof( clientAddr );
//		SRS_Dbg_WriteLog( "SRS_Ini_main : accept" );
		ClientSd = accept( AcceptClientSd, (LPSOCKADDR)&clientAddr, &nClientAddrLen );
		if( ClientSd == INVALID_SOCKET )
		{
//			nResult = WSAGetLastError();
//			sprintf( cMsg, "SRS_Ini_main : accept(RcvClient)error:%d", nResult );
//			SRS_Dbg_WriteLog( cMsg );
			closesocket( AcceptClientSd );
			iRtn = SRS_Reg_Log( 0 );
			return( -1 );
		}

		// �S�o�C�g�̂h�o�A�h���X���h�o�A�h���X�\���̂ɃR�s�[����
 		memcpy( &clientIn, &clientAddr.sin_addr.s_addr, 4 );
		memcpy( (char *)&iClientIp, &clientIn, 4 );

		//==========================================
		// �v����M
		//==========================================
//		SRS_Dbg_WriteLog( "SRS_Ini_main : recv" );
		iRecvBytes = 0;
		nBytesRecv = 0;
		do
		{
			nBytesRecv = recv( ClientSd, cIfFix, SRS_IF_FIX_SZ, 0 );
//			sprintf( cMsg, "SRS_Ini_main : Recv %d bytes", nBytesRecv );
//			SRS_Dbg_WriteLog( cMsg );

			iRecvBytes += nBytesRecv;
//			sprintf( cMsg, "SRS_Ini_main : ********* %d", iRecvBytes );
//			SRS_Dbg_WriteLog( cMsg );
		}while( (nBytesRecv != SOCKET_ERROR) &&  nBytesRecv != 0 && iRecvBytes != SRS_IF_FIX_SZ );
		lpIfFix = (struct SRS_IF_FIX *)cIfFix;
//		sprintf( cMsg, "SRS_Ini_main : %04x, %04x, %08x, %08x, %08x", lpIfFix->IF_wRequest, lpIfFix->IF_wResult, lpIfFix->IF_iInformationSize, lpIfFix->IF_iPortNo, lpIfFix->IF_iIp );
//		SRS_Dbg_WriteLog( cMsg );

		if( nBytesRecv == SOCKET_ERROR)
		{
//			nResult = WSAGetLastError();
//			sprintf( cMsg, "SRS_Ini_main : recv error:%d", nResult );
//			SRS_Dbg_WriteLog( cMsg );
			closesocket( ClientSd );
			closesocket( AcceptClientSd );
			iRtn = SRS_Reg_Log( 0 );
			return( -1 );
		}
//		SRS_Dbg_WriteData( "SRS_Ini_main : recv SRS_IF_FIX", cIfFix, SRS_IF_FIX_SZ, "ab" );
//		SRS_Dbg_WriteLog( "SRS_Ini_main : clear 2!" );

		//===================================================
		//  ��M�v������
		//===================================================
		lpIfFix = (struct SRS_IF_FIX *)cIfFix;
		lpIfFix->IF_iIp = iClientIp;		// �N���C�A���g�h�o�A�h���X�ݒ�
		memset( cIp, 0, 8 );
		memcpy( cIp, (char *)&iClientIp, 4 );
//		sprintf( cMsg, "SRS_Ini_main : clear 3! [%d]", lpIfFix->IF_wRequest );
//		SRS_Dbg_WriteLog( cMsg );
		switch( lpIfFix->IF_wRequest )
		{
			//==================================
			// �ڑ��v��  ��M
			//==================================
			case	IF_REQ_CONNECT      :

						sprintf( cMsg, "SRS_Ini_main�F�ڑ��v����M IP=%d.%d.%d.%d", cIp[0],cIp[1],cIp[2],cIp[3] );
						SRS_Dbg_WriteLog( cMsg );

						iPort = SRS_Ini_GetPort( iClientIp );	// �󂫃|�[�g����
						if( iPort == -1 )						// �������s�H
						{
							//�@�����쐬�i�ُ퉞���F�ő�l�I�[�o�j
							lpIfFix->IF_wResult = IF_CMP_MAXOVER;
							lpIfFix->IF_iInformationSize = 0;
							iSendSize = SRS_IF_FIX_SZ;
						}
						else
						{	
							//====================================
							//  �k���������v���Z�X����
							//====================================
							iRtn = SRS_Ini_CreateLobby( iPort );
							if( iRtn == 0 )						// ���������H
							{
								//�@�����쐬�i���퉞���j
								lpIfFix->IF_wResult = IF_CMP_SUCCESS;
								lpIfFix->IF_iInformationSize = SRS_IF_CONNECT_SZ;
								lpIfFix->IF_iPortNo = iPort;	// �|�[�g�ԍ��ݒ�
								lpRegInfo = (struct SRS_REG_INFO *)cRegInfo;
								iRtn = SRS_Reg_GetComInfo( lpRegInfo );
								lpIfConnect = (struct SRS_IF_CONNECT *)lpIfFix->IF_cInformation;
								lpIfConnect->IF_CN_wMaxUser = lpRegInfo->wMaxUser;
								lpIfConnect->IF_CN_wMaxLobby = lpRegInfo->wMaxLobby;
								lpIfConnect->IF_CN_wMaxGroup = lpRegInfo->wMaxGroup;
								lpIfConnect->IF_CN_wMaxGame = lpRegInfo->wMaxGame;
								lstrcpy( lpIfConnect->IF_CN_cMessage, cSendMessage);

								iSendSize = SRS_IF_FIX_SZ + SRS_IF_CONNECT_SZ;
								//
								// �g���[�X�o��
								//
//								memset( cMsg, 0, 256 );
//								sprintf( cMsg, "�ڑ��v�����󂯕t�����B�i�N���C�A���g�h�o=%d.%d.%d.%d ���t�|�[�g=%04d�j", cIp[0],cIp[1],cIp[2],cIp[3],iPort );
//    							iRtn = SRS_Reg_Log( cMsg );
//								SRS_Dbg_WriteLog( cMsg );
							}
							else
							{
								//�@�����쐬�i�ُ퉞���FLobby�v���Z�X�N���G���[�j
								lpIfFix->IF_wResult = IF_CMP_PROCERR;
								lpIfFix->IF_iInformationSize = 0;
								iSendSize = SRS_IF_FIX_SZ;
							}
						}
						//===================================
						//  �������M
						//===================================
//						sprintf( cMsg, "SRS_Ini_main : �������M" );	
//						SRS_Dbg_WriteLog( cMsg );
//						SRS_Dbg_WriteData( cMsg, (char *)lpIfFix, iSendSize , "ab" );
						iSendLen = 0;
						do
						{
							iSendData = send( ClientSd, (char*)cIfFix, iSendSize, 0 );
							iSendLen += iSendData;								
						}while( (iSendData != SOCKET_ERROR) && (iSendLen != iSendSize) );

						if ( iSendData == SOCKET_ERROR )
						{
//							iResult = WSAGetLastError();
//							sprintf( cMsg,"SRS_Ini_main : Send error :%d", iResult );
//							SRS_Dbg_WriteLog( cMsg );
							closesocket( AcceptClientSd );
							closesocket( ClientSd );
							iRtn = SRS_Reg_Log( 0 );
							return( -1 );
						}
						closesocket( ClientSd );
						break;

			//==================================
			// ����v��  ��M
			//==================================
			case	IF_REQ_DISCONNECT	:	
//						SRS_Dbg_WriteLog( "SRS_Ini_main �F �ؒf�v��  ��M" );
						iPort = lpIfFix->IF_iPortNo;
//						sprintf( cMsg, "����v�����󂯕t�����B�i�N���C�A���g�h�o=%d.%d.%d.%d ���t�|�[�g=%04d�j", cIp[0],cIp[1],cIp[2],cIp[3],iPort );
//						iRtn = SRS_Reg_Log( cMsg );
//						SRS_Dbg_WriteLog( cMsg );
						closesocket( ClientSd );
						iRtn = SRS_Ini_TermProc( iPort );		// Lobby�v���Z�X���
//						SRS_Dbg_WriteLog( "SRS_Ini_main : SRS_Ini_TermProc�I��" );
						iRtn = SRS_Ini_ReleasePort( iPort );	// �|�[�g���
//						SRS_Dbg_WriteLog( "SRS_Ini_main : SRS_Ini_ReleasePort�I��" );
						break;

			//==================================
			// �I���v��  ��M
			//==================================
			case	IF_REQ_TERMINATE	:	
//						SRS_Dbg_WriteLog( "SRS_Ini_main �F �I���v��  ��M" );
						for( ; ; )
						{	
							iPort = SRS_Ini_UsePort( );				// �g�p���|�[�g����
							if( iPort == -1 )						// �g�p���|�[�g�Ȃ��H
							{
								break;
							}
							closesocket( ClientSd );
							iRtn = SRS_Ini_TermProc( iPort );		// Lobby�v���Z�X���
							iRtn = SRS_Ini_ReleasePort( iPort );	// �|�[�g���
						}
						iLoopOut = 1;
						break;

			//==================================
			// �V�X�e���G���[
			//==================================
			default						:
//						sprintf( cMsg, "SRS_Ini_main�F����`�v��(%04x)��M",lpIfFix->IF_wRequest);
//						SRS_Dbg_WriteLog( cMsg );
						iLoopOut = 1;
						closesocket( AcceptClientSd );
						closesocket( ClientSd );
						break;
		}
		closesocket( ClientSd );
//		SRS_Dbg_WriteLog( "SRS_Ini_main : ���̗v���҂���" );
	}

	closesocket( AcceptClientSd );
	iRtn = SRS_Reg_Log( 0 );
//	SRS_Dbg_WriteLog( "SRS_Ini_main : �T�[�o��~" );

	// 1.31.2000 added...
	if ( lpPortList )
	{
		free( lpPortList );
		lpPortList = NULL;
	}
	return( 0 );
}

/*=============================================================================
     �֐�   �F�T�[�o�����ݒ�i�T�[�o������̏����ݒ�������Ȃ��j
     ����   �F�Ȃ�
     �o��   �F�Ȃ�
     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
int	SRS_Ini_Server( void )
{
	int		iRtn;								//
	int		iAllocSize;							// �l���̈�T�C�Y
	int		iPortCount;							// �����|�[�g��
	int		iPortNo;							// �|�[�g�ԍ�
	int		iItem;								//
	struct	SRS_REG_INFO *lpRegInfo;			// �T�[�o���ʏ���|�C���^
	char	cPort[4];							//
	char	cRegInfo[SRS_REG_INFO_SZ];			// ���ʊǗ�����
	char	cMsg[256];

	//=============================================
	//  ���O���b�Z�[�W��������
	//=============================================
	iRtn = SRS_Ini_CreateProc( SRS_CREATE_ASYNC, SRS_PROC_LOG );
	//=============================================
	//  �T�[�o��񏉊���
	//=============================================
	iRtn = SRS_Mng_Startup( );
	if( iRtn == -1 )								// ���������s�H
	{
//		SRS_Dbg_WriteLog( "SRS_Mng_Startup error" );
		return( -1 );
	}
	//=============================================
	//  �|�[�g���X�g�쐬
	//=============================================
	lpRegInfo = (struct SRS_REG_INFO *)cRegInfo;
	iRtn = SRS_Reg_GetComInfo( lpRegInfo );		// ���ʊǗ������荞��
	if( iRtn == -1 )							// ��荞�ݎ��s�H
	{
//		sprintf( cMsg, "SRS_Ini_Server : SRS_Reg_GetComInfo error" );
//		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}

	lpPortList = 0;
	iPortCount = lpRegInfo->wMaxUser;				// �ő僆�[�U���ݒ�	
	iAllocSize = SRS_PORT_LIST_SZ + (SRS_PORT_ITEM_SZ * iPortCount);    
	lpPortList = (struct SRS_PORT_LIST *)malloc( iAllocSize );
	if( lpPortList == 0 )							// �l�����s�H
	{
//		sprintf( cMsg, "SRS_PORT_LIST alloc error" );
//		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	memset( (char *)lpPortList, 0, iAllocSize );
	//
	// �|�[�g�ԍ��ݒ�
	//
	iRtn = SRS_Reg_GetInfo( SRS_REG_KEY_COM, SRS_REG_CLIENTPORT, cPort );	// �擪�|�[�g�ԍ��l��
	if( iRtn == 0 )
	{
		sscanf( cPort, "%d", &iPortNo );
	}
	else
	{
		iPortNo = 7100;								// �f�t�H���g�\��|�[�g�擪�ԍ��ݒ�
	}
	lpPortList->PL_iItemNo = iPortCount;			// �|�[�g���ݒ�
	for( iItem = 0 ; iItem < iPortCount ; iItem++ )
	{
		lpPortList->PL_Item[iItem].PI_iPort = iPortNo;
		iPortNo = iPortNo + 1;						// �|�[�g�ԍ��X�V
	}
//	sprintf( cMsg, "Port-List size=%04x", iAllocSize );
//	SRS_Dbg_WriteData( cMsg, (char *)lpPortList, iAllocSize, "ab" );
	return( 0 );
}

/*=============================================================================
     �֐�   �F�󂫃|�[�g���ʒm�i�g�p�\�ȃT�[�o�|�[�g�ԍ���ʒm����j
     ����   �FiClientIp  �N���C�A���g�h�o�A�h���X
     �o��   �F�Ȃ�
     �߂�l �F���� �i�|�[�g�ԍ��j
              �ُ�  -1

==============================================================================*/
int SRS_Ini_GetPort( int iClientIp )
{
	int		iPort;									//
	int		iItem;									//

	//======================================
	//  ���蓖�čϔ���
	//======================================
	if( lpPortList == 0 )							// �|�[�g���X�g�Ȃ��H
	{
//		SRS_Dbg_WriteLog( "Port list error" );
		return( -1 );								// �V�X�e���G���[
	}
	// �h�o�A�h���X�����Ɋ��蓖�����Ă���ꍇ�͓���|�[�g�����蓖�Ă�
	for( iItem = 0 ; iItem < lpPortList->PL_iItemNo ; iItem++ )
	{
		if( lpPortList->PL_Item[iItem].PI_iIp == iClientIp )	// ����h�o�H
		{
			iPort = lpPortList->PL_Item[iItem].PI_iPort;
			return( iPort );
		}
	}

	//======================================
	//  ���g�p�|�[�g����
	//======================================
	for( iItem = 0 ; iItem < lpPortList->PL_iItemNo ; iItem++ )
	{
		if( lpPortList->PL_Item[iItem].PI_iIp == 0 )	// ���g�p�|�[�g�H
		{
			iPort = lpPortList->PL_Item[iItem].PI_iPort;
			lpPortList->PL_Item[iItem].PI_iIp = iClientIp;
			return( iPort );
		}
	}
	// ���g�p�|�[�g�Ȃ�
	return( -1 );
}

/*=============================================================================
     �֐�   �F�g�p���|�[�g�ԍ��ʒm�i�g�p���|�[�g�ԍ���ʒm����j
     ����   �F�Ȃ�
     �o��   �F�Ȃ�
     �߂�l �F����    �i�|�[�g�ԍ��j
              �ُ�  -1�i�g�p���|�[�g�Ȃ��j

==============================================================================*/
int SRS_Ini_UsePort( void )
{
	int		iPort;									//
	int		iItem;									//

	//======================================
	//  ���蓖�čϔ���
	//======================================
	if( lpPortList == 0 )							// �|�[�g���X�g�Ȃ��H
	{
//		SRS_Dbg_WriteLog( "Port list error" );
		return( -1 );								// �V�X�e���G���[
	}
	// �h�o�A�h���X�����Ɋ��蓖�����Ă���|�[�g����������
	for( iItem = 0 ; iItem < lpPortList->PL_iItemNo ; iItem++ )
	{
		if( lpPortList->PL_Item[iItem].PI_iIp != 0 )	// �g�p���|�[�g�H
		{
			iPort = lpPortList->PL_Item[iItem].PI_iPort;
			return( iPort );
		}
	}
	// �g�p�|�[�g�Ȃ�
	return( -1 );
}

/*=============================================================================
     �֐�   �F�|�[�g�ԋp�i�g�p���|�[�g�ԍ���ԋp����j
     ����   �FiPort  �|�[�g�ԍ�
     �o��   �F�Ȃ�
     �߂�l �F����  0
              �ُ�  -1(�V�X�e���G���[)

==============================================================================*/
int	SRS_Ini_ReleasePort( int iPort )
{
	int		iItem;		
	char	cPort[8];				// �|�[�g�ԍ�
	char	cLobbyProc[256];		// �폜 Lobby�v���Z�X��

	//======================================
	//  �ԋp�|�[�g����
	//======================================
	for( iItem = 0 ; iItem < lpPortList->PL_iItemNo ; iItem++ )
	{
		if( lpPortList->PL_Item[iItem].PI_iPort == iPort )	
		{
			lpPortList->PL_Item[iItem].PI_iIp = 0;		// �h�o�A�h���X�N���A
			//======================================
			//  �Y��Lobby�v���Z�X�폜
			//======================================
			memset( cLobbyProc, 0, 256 );
			memset( cPort, 0, 8 );
			strcpy( cLobbyProc, SRS_PROC_LOBBY );	// �폜�v���Z�X������
			sprintf( cPort, "%04d", iPort );		//
			strcat( cLobbyProc, cPort );			//
			strcat( cLobbyProc, ".exe" );			//
			DeleteFile( cLobbyProc );				// �v���Z�X�폜
			return( 0 );
		}
	}
	return( -1 );									// �V�X�e���G���[
}

/*=============================================================================
     �֐�    �FLobby�v���Z�X����
     ����    �FiPort  �|�[�g�ԍ�
     �o��    �F�Ȃ�
     �߂�l  �F����  0
               �ُ�  -1 (�������s)
	 ���L�����F�N������Lobby�v���Z�X���͊�{���Ƀ|�[�g�ԍ���t���������O�ɂȂ�
                 (��FSRSlobby7000.exe)

==============================================================================*/
int	SRS_Ini_CreateLobby( int iPort )
{
	int		iRtn;
	BOOL	fRtn;
	DWORD	dwRtn;
	char	cPort[8];				// �|�[�g�ԍ�
	char	cNewLobbyProc[256];		// �N�� Lobby�v���Z�X��
	char	cLobbyProc[256];		// �� Lobby�v���Z�X��
	char	cMsg[256];

	//======================================
	//  �N��Lobby�v���Z�X������
	//======================================
	memset( cNewLobbyProc, 0, 256 );
	memset( cPort, 0, 8 );
	strcpy( cNewLobbyProc, SRS_PROC_LOBBY );	// �v���Z�X�Œ薼�ݒ�
	sprintf( cPort, "%04d", iPort );
	strcat( cNewLobbyProc, cPort );
	strcat( cNewLobbyProc, ".exe" );
	//=======================================
	// �v���Z�X���ύX
	//=======================================
	memset( cLobbyProc, 0, 256 );
	strcpy( cLobbyProc, SRS_PROC_LOBBY );		// ���v���Z�X���ݒ�
	strcat( cLobbyProc, ".exe" );

	fRtn = DeleteFile( cNewLobbyProc );
	if( fRtn == FALSE )
	{
		dwRtn = GetLastError( );
		if( dwRtn != ERROR_FILE_NOT_FOUND )
		{
//			sprintf( cMsg, "DeleteFile error %d %s", dwRtn, cNewLobbyProc );
//			SRS_Dbg_WriteLog( cMsg );
			iRtn = SRS_Ini_TermProc( iPort );		// Lobby�v���Z�X�����I��
			return( -1 );
		}
	}

	fRtn = CopyFile( cLobbyProc, cNewLobbyProc, TRUE );
	if( fRtn == FALSE )		// �ύX���s�H
    {
//		dwRtn = GetLastError( );
//		sprintf( cMsg, "CopyFile error %d %s %s", dwRtn, cLobbyProc, cNewLobbyProc );
//		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
	}
	//=======================================
	// �v���Z�X�N���i�������^�j
	//=======================================
	sprintf( cMsg, "SRS_Ini_CreateProc %s", cNewLobbyProc );
	iRtn = SRS_Ini_CreateProc( SRS_CREATE_ASYNC, cNewLobbyProc );
	return( iRtn );
}
/*====================================================================
     �֐�   �F�v���Z�X�����i�w��v���Z�X���N������j
	 ����   �FiMode				�v���Z�X�������[�h
								SRS_CREATE_SYNC  : ����  �i�������A�j
								SRS_CREATE_ASYNC : �񓯊��i���������A�j
			  lpstrCommandLine  �R�}���h���C��
�@   �o��   �F�Ȃ�
�@   �߂�l �F 0  (����)
              -1  (�ُ�)

====================================================================*/
int SRS_Ini_CreateProc( int iMode, char *lpCommandLine )
{
	BOOL    cprc;								// ���A�l
	STARTUPINFO StartInfo;   					// �N������
    PROCESS_INFORMATION ProcInfo;				// �v���Z�X����

	DWORD	dwExitCode;							// �I����Ԃ��󂯎��A�h���X
	//
	// CreateProcess�p�����^
	//
    LPCTSTR lpszImageName = NULL;				// ���W���[�����̃A�h���X
	LPCTSTR lpszCommandLine;					// �R�}���h���C���̃A�h���X
    LPSECURITY_ATTRIBUTES lpsaProcess = NULL;	// �v���Z�X�Z�L�����e�B�����̃A�h���X
    LPSECURITY_ATTRIBUTES lpsaThread = NULL;	// �X���b�h�Z�L�����e�B�����̃A�h���X
    BOOL fInheritHandles = TRUE;				// �V�v���Z�X�̃n���h���p��
    DWORD fdwCreate = 0;						// �쐬�t���O
    LPVOID lpvEnvironment = NULL;				// �V���u���b�N�A�h���X
    LPCTSTR lpszCurDir = NULL;					// ���f�B���N�g�����A�h���X
    LPSTARTUPINFO lpsiStartInfo = &StartInfo;	// STARTUPINFO�̃A�h���X
    LPPROCESS_INFORMATION lppiProcInfo =		// �v���Z�X����̃A�h���X
									 &ProcInfo;
	//
	// �R�}���h���C���ݒ�
	//
	lpszCommandLine = lpCommandLine;			// �R�}���h���C���A�h���X�ݒ�
	//
	//  �N�����ݒ�
	//
    ZeroMemory(&StartInfo, sizeof(StartInfo));	// �N�����揉����
    StartInfo.cb = sizeof(StartInfo);			// �N������T�C�Y�ݒ�
    StartInfo.lpReserved = NULL;				// 
    StartInfo.lpDesktop = NULL;					// �f�X�N�g�b�v���w��Ȃ�
    StartInfo.lpTitle = NULL;					// �^�C�g���\���Ȃ�
    StartInfo.dwFlags = STARTF_USESHOWWINDOW;	// wShowWindow�L��
    StartInfo.wShowWindow = SW_HIDE;			// �E�B���h�E��\��
    StartInfo.cbReserved2 = 0;					//
    StartInfo.lpReserved2 = NULL;				//
	//
	// �v���Z�X�����i�N���j
	//
    cprc = CreateProcess( lpszImageName,
                		  (char *)lpszCommandLine,
                          lpsaProcess,
                          lpsaThread,
                          fInheritHandles,
                          fdwCreate,
                          lpvEnvironment,
                          lpszCurDir,
                          lpsiStartInfo,
                          lppiProcInfo );
    if ( cprc == TRUE )						// ���������H
	{
		if( iMode == SRS_CREATE_SYNC )		// �������[�h�H
    	{
			//
			// �����҂�
			//
			dwExitCode = STILL_ACTIVE;
			while( dwExitCode == STILL_ACTIVE )
			{
   				WaitForSingleObject( ProcInfo.hProcess, INFINITE );
				GetExitCodeProcess( ProcInfo.hProcess, &dwExitCode );
				if( dwExitCode != 0 )
				{
			    	return( -1 );
				}
			}
		}
		return( 0 );
    }
    return( -1 );
}
/*=============================================================================
     �֐�   �FLobby�v���Z�X�����I��
			�iLobby�v���Z�X�̋����I���������Ȃ��j
     ����   �FiPort	�i�|�[�g�ԍ��j
     �o��   �F�Ȃ�
     �߂�l �F���� 0
              �ُ� -1

==============================================================================*/
int SRS_Ini_TermProc( int iPort )
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
	iResult = WSAStartup( wVersionRequested, &wsaData );
	if ( iResult != 0 )
	{
//		iResult = WSAGetLastError();
//		sprintf( cMsg, "SRS_Ini_TermProc : WSAStartup error:%d", iResult );
//		SRS_Dbg_WriteLog( cMsg );
		return( -1 );
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
//		iResult = WSAGetLastError();
//		sprintf( cMsg, "SRS_Ini_TermProc : bind error:%d", iResult );
//		SRS_Dbg_WriteLog( cMsg );

		iResult = closesocket(ServerSd);
		return( -1 );
	}
	// �A�h���X�\���̂ɃT�[�o�A�h���X������������
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(iPort);
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
//				sprintf( cMsg, "SRS_Ini_TermProc : IP error:Sock[%d.%d.%d.%d] Reg[%d.%d.%d.%d]", (ServerAddr.sin_addr.s_addr & 0xff), ((ServerAddr.sin_addr.s_addr >> 8) & 0xff), ((ServerAddr.sin_addr.s_addr >> 16) & 0xff), ((ServerAddr.sin_addr.s_addr >> 24) & 0xff), (ip & 0xff), ((ip >> 8) & 0xff), ((ip >> 16) & 0xff), ((ip >> 24) & 0xff) );
//				SRS_Dbg_WriteLog( cMsg );
				ServerAddr.sin_addr.s_addr = ip;
			}
		}
	}
		
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

//			SRS_Dbg_WriteLog( "SRS_Ini_TermProc : select" );
			iResult = select(0, NULL, lpWriteFds, NULL, lpConnectTime);

			//�@�^�C���A�E�g�܂��̓G���[�̏ꍇ
			if( (iResult == 0) || (iResult == SOCKET_ERROR) )
			{
//				sprintf( cMsg, "SRS_Ini_TermProc : select error %d", iResult );
//				SRS_Dbg_WriteLog( cMsg );
				iResult = closesocket(ServerSd);
				return( -1 );
			}
		}
		else
		{
			// �u���b�N�ȊO�̃G���[
//			sprintf( cMsg, "SRS_Ini_TermProc : connect error %d", iResult );
//			SRS_Dbg_WriteLog( cMsg );
			iResult = closesocket(ServerSd);
			return( -1 );
		}
	}

	/*==================================*/
	/* �v�����M				  			*/
	/*==================================*/
	//�@�v���쐬
	memset( cIfFix, 0, 256 );
	lpIfFix = (struct SRS_IF_FIX *)cIfFix;
	lpIfFix->IF_wRequest = IF_REQ_TERMINATE;	// �I���ʒm

	//  �v�����M
	iSendLen = 0;
	do
	{
		iSendData = send( ServerSd, (char*)cIfFix, SRS_IF_FIX_SZ, 0 );
		iSendLen += iSendData;								
	}while( (iSendData != SOCKET_ERROR) && (iSendLen != SRS_IF_FIX_SZ) );

	if ( iSendData == SOCKET_ERROR )
	{
//		iResult = WSAGetLastError();
//		sprintf( cMsg,"SRS_Ini_TermProc : Send error :%d", iResult );
//		SRS_Dbg_WriteLog( cMsg );
		closesocket( ServerSd );
		return( -1 );
	}

	/*==================================*/
	/* �v�����B�`�F�b�N  				*/
	/*==================================*/
	// �N���C�A���g�����R�l�N�V�������������܂ő҂�
	iResult = recv( ServerSd, cDummyBuff, 16, 0 );

//	SRS_Dbg_WriteLog( "SRS_Ini_TermProc OK" );
	closesocket( ServerSd );

	return( 0 );
}