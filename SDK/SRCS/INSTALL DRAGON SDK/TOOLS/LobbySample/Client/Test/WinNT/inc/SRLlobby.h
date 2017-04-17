/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRLlobby.h
 *  Description: ���r�[API�p��`�t�@�C��
 *  Content:    Lobby Api include file
 *  Created:    1999/06/07
 *  Update:	  
 ***************************************************************************/
#ifndef _SRL_LOBBY_H_
#define _SRL_LOBBY_H_

#define LINKAGE extern "C"

#ifdef __cplusplus
extern "C" {
#endif



#ifndef __cplusplus
typedef enum { false, true } bool;
#endif

/*************************************/
/* Win32 �݊��p��`                   */
/*************************************/

typedef void			*LPVOID, *PVOID;
typedef const char		*LPCCHAR, *PCCHAR;

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif


typedef char			*LPCHAR, *PCHAR;
typedef char			STR;

#define SRL_CLIENTNAME	"GOBOU"
/************************************/
/*   �v���Z�X��			  */
/************************************/

/*************************************/
/* �ϐ���`                          */
/*************************************/
#define		SRL_OK		TRUE
#define		SRL_NG		FALSE

/*************************************/
/* API�pLobby���̈�\���̃|�C���^�|  */
/*************************************/
typedef	struct SRS_INFO_USER	SRLUSERLIST,*LPSRLUSERLIST;	
typedef	struct SRS_USER_ITEM	SRLUSER, *LPSRLUSER;
typedef	struct SRS_INFO_GAME	SRLGAMELIST,*LPSRLGAMELIST;
typedef	struct SRS_GAME_ITEM	SRLGAME,*LPSRLGAME;
typedef	struct SRS_INFO_LOBBY	SRLLOBBYLIST,*LPSRLLOBBYLIST;
typedef	struct SRS_LOBBY_ITEM	SRLLOBBY,*LPSRLLOBBY;
typedef	struct SRS_INFO_GROUP	SRLGROUPLIST,*LPSRLGROUPLIST;
typedef	struct SRS_GROUP_ITEM	SRLGROUP,*LPSRLGROUP;


/*************************************/
// Termination Flag for SRL_Terminate()
#define SRL_TERMINATE_COMPLETE			1
#define SRL_TERMINATE_KEEPCONNECTION	2


/****************************/
/*  �|�[�g�ԍ�              */
/****************************/
#define SRL_CLIENT_PORTNO		8000

/****************************/
/*  �z�X�gIP�A�h���X		*/
/****************************/

/*************************************/
// SRL_Proceed() Flag
#define SRL_PROCEED_NORMAL	1


/****************************/
/*  ���b�Z�[�W�^�C�v        */
/****************************/
// SRL Message Type
#define SRLMESSAGE_ANY			0
#define SRLMESSAGE_SERVER		1
#define SRLMESSAGE_INFOMESSAGE	2
#define SRLMESSAGE_CHAT			3
#define SRLMESSAGE_PRIVATE		4


/************************************************/
/*   API�p�\���̒�`	                           */
/************************************************/


/********************************/
/*   �ʐM�f�o�C�X���		     */
/********************************/
#define	SRL_MAXENTRYNAME	20
#define	SRL_MAXPHONENUMBER	128
#define	SRL_MAXIPADDRESS	15
#define	SRL_MAXDEVICENAME	32
#define	SRL_MAXUSERNAME	20


typedef struct _SRL_DEVICE
{
	DWORD	DV_dwDeviceType;						// �ڑ��^�C�v
	char	DV_cEntryName[SRL_MAXENTRYNAME+1];		// �G���g���[��
	char	DV_cPhoneNumber[SRL_MAXPHONENUMBER+1];	// �d�b�ԍ�
	char	DV_cIPAddress[SRL_MAXIPADDRESS+1];		// IP�A�h���X
	char	DV_cDeviceName[SRL_MAXDEVICENAME+1];	// �f�o�C�X��
	DWORD	DV_dwPortNo;							// �|�[�g�ԍ�
} SRLDEVICE, *LPSRLDEVICE; 
/********************************/
/*   ���O�C���A�J�E���g���	     */
/********************************/
typedef struct _SRL_ACCOUNT
{
	char	AC_cHostName[20];					// ���r�[�T�[�o��
	char	AC_cClientName[SRS_MAX_NAME_CLIENT];	// �N���C�A���g���i���O�C���h�c�j
	char	AC_cUserName[SRS_MAX_NAME_USER];					// ���[�U��
	char	AC_cPassword[SRS_MAX_PASSWORD];					// �p�X���[�h
	int	AC_iIPAdr;						// �h�o�A�h���X
} SRLACCOUNT, *LPSRLACCOUNT;
/*******************************/
/*   �ڑ����		           */
/*******************************/
typedef struct _SRL_CONNECTION
{
	DWORD		CN_dwType;
	bool		CN_bConnected;
	SRLACCOUNT	CN_Account;
	SRLDEVICE	CN_SrlDevice;
}  SRLCONNECTION, *LPSRLCONNECTION;
/********************************/
/*   ���[�U�o�^���				*/
/********************************/
typedef struct	_SRL_USER_ENTRY
{
	char		UE_cClientName[SRS_MAX_NAME_CLIENT];	// �N���C�A���g��
	char		UE_cUserName[SRS_MAX_NAME_USER];	// ���[�U��
} SRLUSERENTRY, *LPSRLUSERENTRY;

#define SRS_USER_ENTRY_SZ	(sizeof(struct _SRL_USER_ENTRY))

/********************************/
/*   �O���[�v�o�^���		    */
/********************************/
typedef struct	_SRL_GROUP_MAKE
{
	WORD	GM_wUserID;				// ��Ã��[�UID
	WORD	wReserve;				// ���U�[�u
	char	GM_cGroupName[SRS_MAX_NAME_GROUP];			// �O���[�v��
}SRLGROUPMAKE,*LPSRLGROUPMAKE;
 
#define SRS_GROUP_MAKE_SZ	(sizeof(_SRL_GROUP_MAKE))

/********************************/
/*  �C�x���g���b�Z�[�W��`      */
/********************************/
#define SRL_EVTMSG_INITIALIZED				("���r�[API�̏��������������܂���")
#define SRL_EVTMSG_CONNECTION_START			("���r�[�T�[�o�֐ڑ��J�n���܂���")
#define SRL_EVTMSG_CONNECTION_INITIALIZING	("���r�[�T�[�o�֐ڑ����ł�")
#define SRL_EVTMSG_CONNECTION_DIALING		("���r�[�T�[�o�֓d�b���ł�")
#define SRL_EVTMSG_CONNECTION_WAITINGSERVERREPLY ("���r�[�T�[�o����̐ڑ������҂��ł�")
#define SRL_EVTMSG_CONNECTION_ESTABLISHING	("���r�[�T�[�o�Ƃ̐ڑ��m�����ł�")
#define	SRL_EVTMSG_CONNECTION_CONNECTED		("���r�[�T�[�o�Ɛڑ��������܂���")
#define	SRL_EVTMSG_CONNECTION_CLOSED		("���r�[�T�[�o�Ƃ̐ڑ���؂藣���܂���")
#define SRL_EVTMSG_MESSAGE_RECEIVED			("���b�Z�[�W����M����")
#define	SRL_EVTMSG_MESSAGE_CHAT				("�`���b�g���b�Z�[�W����M���܂���")
#define SRL_EVTMSG_USER_ADDED				("�V�K���[�U�����O�C�����܂���")
#define SRL_EVTMSG_USER_REMOVED				("�����[�U�����O�A�E�g���܂���")
#define SRL_EVTMSG_USER_GETLISTSTART		("���[�U���X�g�̎擾���J�n���܂���")
#define SRL_EVTMSG_USER_GETLISTDONE			("���[�U���X�g�̎擾���������܂���")
#define SRL_EVTMSG_USER_REFRESH_START		("���[�U���̍X�V���J�n���܂���")
#define	SRL_EVTMSG_USER_REFRESH_END			("���[�U���̍X�V���������܂���")
#define SRL_EVTMSG_SEARCHUSER_START			("���[�U�������J�n���܂���")
#define SRL_EVTMSG_SEARCHUSER_END			("���[�U�������I�����܂���")
#define SRL_EVTMSG_LOBBY_JOINED				("���r�[�ɎQ�����܂���")
#define SRL_EVTMSG_LOBBY_LEAVE				("���r�[���痣�E���܂���")
#define SRL_EVTMSG_LOBBY_GETLISTSTART		("���r�[���X�g�̎擾���J�n���܂���")
#define SRL_EVTMSG_LOBBY_GETLISTDONE		("���r�[���X�g�̎擾���������܂���")

#define SRL_EVTMSG_LOBBY_REFRESH_START		("���r�[���̍X�V���J�n���܂���")
#define SRL_EVTMSG_LOBBY_REFRESH_END		("���r�[���̍X�V���������܂���")
	
#define SRL_EVTMSG_GAME_LAUNCH				("�Q�[�����N�����鏀�����������܂���")
#define SRL_EVTMSG_GAME_STARTED				("�Q�[�����J�n����܂���")
#define SRL_EVTMSG_GAME_FINISHED			("�Q�[�����I�����܂���")

#define SRL_EVTMSG_GAME_GETLISTSTART		("�Q�[�����̍X�V���J�n���܂���")
#define SRL_EVTMSG_GAME_GETLISTDONE			("�Q�[�����̍X�V���������܂���")
#define SRL_EVTMSG_GROUP_JOINSTART			("�`�[���ւ̎Q�����J�n���܂���")
#define SRL_EVTMSG_GROUP_JOINED				("�`�[���ւ̎Q�����������܂���")
#define SRL_EVTMSG_GROUP_LEAVE				("�`�[�����痣�E���܂���")
#define SRLVENT_GROUP_DESTROYED				("�`�[�����������܂���")
#define SRL_EVTMSG_GROUP_CHANGEOWNER		("�`�[����Î҂��ւ��܂���")


#define SRL_EVTMSG_GROUP_REFRESH_START		("�`�[�����̍X�V���J�n���܂���")
#define SRL_EVTMSG_GROUP_REFRESH_END		("�`�[�����̍X�V���������܂���")
/*
#define SRL_EVTMSG_INITIALIZED				TEXT("���r�[API�̏��������������܂���")
#define SRL_EVTMSG_CONNECTION_START			TEXT("���r�[�T�[�o�֐ڑ��J�n���܂���")
#define SRL_EVTMSG_CONNECTION_INITIALIZING	TEXT("���r�[�T�[�o�֐ڑ����ł�")
#define SRL_EVTMSG_CONNECTION_WAITINGSERVERREPLY TEXT("���r�[�T�[�o����̐ڑ������҂��ł�")
#define SRL_EVTMSG_CONNECTION_ESTABLISHING	TEXT("���r�[�T�[�o�Ƃ̐ڑ��m�����ł�")
#define	SRL_EVTMSG_CONNECTION_CONNECTED		TEXT("���r�[�T�[�o�Ɛڑ��������܂���")
#define	SRL_EVTMSG_CONNECTION_CLOSED		TEXT("���r�[�T�[�o�Ƃ̐ڑ���؂藣���܂���")
#define SRL_EVTMSG_MESSAGE_RECEIVED			TEXT("���b�Z�[�W����M����")
#define	SRL_EVTMSG_MESSAGE_CHAT				TEXT("�`���b�g���b�Z�[�W����M���܂���")
#define SRL_EVTMSG_USER_ADDED				TEXT("�V�K���[�U�����O�C�����܂���")
#define SRL_EVTMSG_USER_REMOVED				TEXT("�����[�U�����O�A�E�g���܂���")
#define SRL_EVTMSG_USER_GETLISTSTART		TEXT("���[�U���X�g�̎擾���J�n���܂���")
#define SRL_EVTMSG_USER_GETLISTDONE			TEXT("���[�U���X�g�̎擾���������܂���")
#define SRL_EVTMSG_USER_REFRESH_START		TEXT("���[�U���̍X�V���J�n���܂���")
#define	SRL_EVTMSG_USER_REFRESH_END			TEXT("���[�U���̍X�V���������܂���")
#define SRL_EVTMSG_SEARCHUSER_START			TEXT("���[�U�������J�n���܂���")
#define SRL_EVTMSG_SEARCHUSER_END			TEXT("���[�U�������I�����܂���")
#define SRL_EVTMSG_LOBBY_JOINED				TEXT("���r�[�ɎQ�����܂���")
#define SRL_EVTMSG_LOBBY_LEAVE				TEXT("���r�[���痣�E���܂���")
#define SRL_EVTMSG_LOBBY_GETLISTSTART		TEXT("���r�[���X�g�̎擾���J�n���܂���")
#define SRL_EVTMSG_LOBBY_GETLISTDONE		TEXT("���r�[���X�g�̎擾���������܂���")

#define SRL_EVTMSG_LOBBY_REFRESH_START		TEXT("���r�[���̍X�V���J�n���܂���")
#define SRL_EVTMSG_LOBBY_REFRESH_END		TEXT("���r�[���̍X�V���������܂���")
	
#define SRL_EVTMSG_GAME_LAUNCH				TEXT("�Q�[�����N�����鏀�����������܂���")
#define SRL_EVTMSG_GAME_STARTED				TEXT("�Q�[�����J�n����܂���")
#define SRL_EVTMSG_GAME_FINISHED			TEXT("�Q�[�����I�����܂���")

#define SRL_EVTMSG_GAME_GETLISTSTART		TEXT("�Q�[�����̍X�V���J�n���܂���")
#define SRL_EVTMSG_GAME_GETLISTDONE			TEXT("�Q�[�����̍X�V���������܂���")
#define SRL_EVTMSG_GROUP_JOINSTART			TEXT("�`�[���ւ̎Q�����J�n���܂���")
#define SRL_EVTMSG_GROUP_JOINED				TEXT("�`�[���ւ̎Q�����������܂���")
#define SRL_EVTMSG_GROUP_LEAVE				TEXT("�`�[�����痣�E���܂���")
#define SRLVENT_GROUP_DESTROYED				TEXT("�`�[�����������܂���")
#define SRL_EVTMSG_GROUP_CHANGEOWNER		TEXT("�`�[����Î҂��ւ��܂���")


#define SRL_EVTMSG_GROUP_REFRESH_START		TEXT("�`�[�����̍X�V���J�n���܂���")
#define SRL_EVTMSG_GROUP_REFRESH_END		TEXT("�`�[�����̍X�V���������܂���")
*/
/******************************/
/*  �G���[���b�Z�[�W��`      */
/******************************/
#define	SRL_ERRMSG_GENERIC					("�ėp�I�G���[���������܂���")
#define	SRL_ERRMSG_COMERROR					("�ʐM�G���[���������܂���")
#define	SRL_ERRMSG_CONNECTION_FAILED		("�ڑ������s���܂���")
#define	SRL_ERRMSG_RASDEVICE				("RAS�ڑ����s���܂���")
#define	SRL_ERRMSG_INVALIDPARAMS			("�����p�����[�^")
#define	SRL_ERRMSG_NO_ANSWER				("�T�[�o���疳�����ł�")
#define	SRL_ERRMSG_DISCONNECTED				("�ڑ����؂藣����܂���")
#define	SRL_ERRMSG_SENDFAILED				("���M�G���[���������܂���")
#define	SRL_ERRMSG_OVERFLOWED				("�I�[�o�t���[���������܂���")
#define	SRL_ERRMSG_TIMEOUT					("�^�C���A�E�g��������")
#define	SRL_ERRMSG_SERVERNOTREPLY			("�T�[�o�������܂���")
#define	SRL_ERRMSG_NOTENOUGHMEMORY			("���\�[�X���s�����Ă��܂�")
#define	SRL_ERRMSG_INVALIDPASSWORD			("�p�X���[�h�������ł�")
#define	SRL_ERRMSG_INVALIDACCOUNT			("�A�J�E���g�������ł�")
#define	SRL_ERRMSG_NOTCONNECTED				("�ڑ��ł��܂���")
#define	SRL_ERRMSG_ALREADYCONNECTED			("���łɐڑ����Ă��܂�")
#define	SRL_ERRMSG_GAME_LAUNCHFAILED		("�Q�[���̋N�������s���܂���")
#define	SRL_ERRMSG_CREATEGROUPFAILED		("�`�[�����쐬�ł��܂���")
#define	SRL_ERRMSG_GROUP_FULL				("�`�[�����������z���܂���")
#define	SRL_ERRMSG_USERALREADYEXIST			("���łɓ��ꖼ���[�U�����݂��܂�")
#define	SRL_ERRMSG_DUPLICATEUSERNAME		("���ꖼ���[�U�o�^�������܂���")
#define	SRL_ERRMSG_GROUP_ALREADYEXIST		("���ꖼ�`�[�������݂��܂�")
#define	SRL_ERRMSG_CHAT_EMPTYMESSAGE		("��`���b�g���b�Z�[�W�͑��M�ł��܂���")
#define	SRL_ERRMSG_LOBBY_FULL				("���r�[���̃��[�U�o�^����t�ł�")
#define	SRL_ERRMSG_MAXMSGQUE				("���b�Z�[�W�L���[����t�ł�")
#define	SRL_ERRMSG_MAXEVENTQUE				("�C�x���g�L���[����t�ł�")
#define	SRL_ERRMSG_MAXERRORQUE				("�G���[�L���[����t�ł�")
/*
#define	SRL_ERRMSG_GENERIC					TEXT("�ėp�I�G���[���������܂���")
#define	SRL_ERRMSG_COMERROR					TEXT("�ʐM�G���[���������܂���")
#define	SRL_ERRMSG_CONNECTION_FAILED			TEXT("�ڑ������s���܂���")
#define	SRL_ERRMSG_RASDEVICE					TEXT("RAS�ڑ����s���܂���")
#define	SRL_ERRMSG_INVALIDPARAMS				TEXT("�����p�����[�^")
#define	SRL_ERRMSG_NO_ANSWER					TEXT("�T�[�o���疳�����ł�")
#define	SRL_ERRMSG_DISCONNECTED				TEXT("�ڑ����؂藣����܂���")
#define	SRL_ERRMSG_SENDFAILED					TEXT("���M�G���[���������܂���")
#define	SRL_ERRMSG_OVERFLOWED					 TEXT("�I�[�o�t���[���������܂���")
#define	SRL_ERRMSG_TIMEOUT					TEXT("�^�C���A�E�g��������")
#define	SRL_ERRMSG_SERVERNOTREPLY				TEXT("�T�[�o�������܂���")
#define	SRL_ERRMSG_NOTENOUGHMEMORY			TEXT("���\�[�X���s�����Ă��܂�")
#define	SRL_ERRMSG_INVALIDPASSWORD			TEXT("�p�X���[�h�������ł�")
#define	SRL_ERRMSG_INVALIDACCOUNT				TEXT("�A�J�E���g�������ł�")
#define	SRL_ERRMSG_NOTCONNECTED				TEXT("�ڑ��ł��܂���")
#define	SRL_ERRMSG_ALREADYCONNECTED			TEXT("���łɐڑ����Ă��܂�")
#define	SRL_ERRMSG_GAME_LAUNCHFAILED			TEXT("�Q�[���̋N�������s���܂���")
#define	SRL_ERRMSG_CREATEGROUPFAILED			TEXT("�`�[�����쐬�ł��܂���")
#define	SRL_ERRMSG_GROUP_FULL					TEXT("�`�[�����������z���܂���")
#define	SRL_ERRMSG_USERALREADYEXIST			TEXT("���łɓ��ꖼ���[�U�����݂��܂�")
#define	SRL_ERRMSG_DUPLICATEUSERNAME			TEXT("���ꖼ���[�U�o�^�������܂���")
#define	SRL_ERRMSG_GROUP_ALREADYEXIST			TEXT("���ꖼ�`�[�������݂��܂�")
#define	SRL_ERRMSG_CHAT_EMPTYMESSAGE			TEXT("��`���b�g���b�Z�[�W�͑��M�ł��܂���")
#define	SRL_ERRMSG_LOBBY_FULL					TEXT("���r�[���̃��[�U�o�^����t�ł�")
#define	SRL_ERRMSG_MAXMSGQUE					TEXT("���b�Z�[�W�L���[����t�ł�")
#define	SRL_ERRMSG_MAXEVENTQUE				TEXT("�C�x���g�L���[����t�ł�")
#define	SRL_ERRMSG_MAXERRORQUE				TEXT("�G���[�L���[����t�ł�")
*/
/********************************************************/
/*														*/
/* API�֐���`											*/
/*														*/
/*******************************************************/
// Lobby API������
__declspec(dllexport)  bool __stdcall SRL_Initialize( void );
__declspec(dllexport)  bool __stdcall SRL_InitializeDevice ( LPSRLDEVICE lpDevice );

// �T�[�o�Ƃ̐ڑ�����
__declspec(dllexport)  bool __stdcall SRL_InitializeEntry ( char *cClientName, char *cUserName, char *cPassword);
__declspec(dllexport)  bool __stdcall SRL_ConnectToServer( int iServerIndex );
__declspec(dllexport)  bool __stdcall SRL_Disconnect( void );
__declspec(dllexport)  bool __stdcall SRL_Reconnect( void );
__declspec(dllexport)  bool __stdcall SRL_Terminate( DWORD dwFlag );

// �e����̎擾

// ���[�U�֘A����
__declspec(dllexport)  WORD __stdcall SRL_GetUserByName( char *lpUserName );
__declspec(dllexport)  bool __stdcall SRL_GetUserByID( WORD wUserID, LPSRLUSER lpUserInfo );
__declspec(dllexport)  bool __stdcall SRL_CreateUser( char *cClinetName, char *cUserName );
__declspec(dllexport)  bool __stdcall SRL_DestroyUser( WORD wUserID );	
__declspec(dllexport)  bool __stdcall SRL_GetUserList ( LPSRLUSERLIST	lpUserList );
__declspec(dllexport)  bool __stdcall SRL_GetUserInfo( WORD wUserID, LPSRLUSER lpUserInfo );
__declspec(dllexport)  bool __stdcall SRL_SendUserSaveData( LPCHAR lpcSaveData );


// ���r�[�֘A����
__declspec(dllexport)  WORD __stdcall SRL_GetLobbyByName( char *lpLobbyName );
__declspec(dllexport)  bool __stdcall SRL_GetLobbyList ( LPSRLLOBBYLIST	lpLobbyList );
__declspec(dllexport)  bool __stdcall SRL_GetLobbyInfo ( WORD	wLobbyID, LPSRLLOBBY lpLobbyInfo );
__declspec(dllexport)  bool __stdcall SRL_GetLobbyByID( WORD wLobbyID,LPSRLLOBBY lpLobbyInfo );
__declspec(dllexport)  bool __stdcall SRL_RefreshLobbyList( void );
__declspec(dllexport)  bool __stdcall SRL_JoinLobby( WORD wUserID, WORD wLobbyID );
__declspec(dllexport)  bool __stdcall SRL_LeaveLobby( WORD wUserID, WORD wLobbyID );

// �Q�[���֘A����
__declspec(dllexport)  bool __stdcall SRL_GetGameList( LPSRLGAMELIST lpGameList );
__declspec(dllexport)  bool __stdcall SRL_GetGameInfo (WORD wGameID, LPSRLGAME lpGameInfo);
__declspec(dllexport)  bool __stdcall SRL_LaunchRequest( WORD wUserID );
__declspec(dllexport)  bool __stdcall SRL_StartedGame( WORD wUserID );
__declspec(dllexport)  bool __stdcall SRL_FinishedGame( WORD wUserID );

// �`�[���֘A����
__declspec(dllexport)  WORD __stdcall SRL_GetGroupByName( char *lpGroupName );
__declspec(dllexport)  bool __stdcall SRL_GetGroupByID( WORD wGroupID,LPSRLGROUP lpGroupInfo );
__declspec(dllexport)  bool __stdcall SRL_CreateGroup( WORD wUserID, LPSTR lpGroupName );
__declspec(dllexport)  bool __stdcall SRL_GetGroupList ( LPSRLGROUPLIST lpGroupList );
__declspec(dllexport)  bool __stdcall SRL_GetGroupInfo ( WORD wGroupID, LPSRLGROUP	lpGroupInfo);
__declspec(dllexport)  bool __stdcall SRL_JoinGroup ( WORD wUserID , LPSTR szPassword ,WORD wGroupID);
__declspec(dllexport)  bool __stdcall SRL_LeaveGroup ( WORD wUserID, WORD wGroupID );

// ���b�Z�[�W���M
__declspec(dllexport)  bool __stdcall SRL_GetMsg ( LPSRLEVENTMNG lpMessageInfo );
__declspec(dllexport)  bool __stdcall SRL_SendChat (WORD wUserID,  LPSTR lpMessage );
__declspec(dllexport)  int  __stdcall SRL_GetMsgCount( void );


// �C�x���g���̎擾( if none,  NULL )
__declspec(dllexport)  bool __stdcall SRL_GetEvent ( LPSRLEVENTMNG lpEventInfo );
__declspec(dllexport)  int  __stdcall SRL_GetEventCount( void );
__declspec(dllexport)  char * __stdcall SRL_GetEventString( int iEventId );


// �G���[
__declspec(dllexport)  bool __stdcall SRL_GetError ( LPSRLEVENTMNG lpErrorInfo );
__declspec(dllexport)  int  __stdcall SRL_GetErrorCount( void );
__declspec(dllexport)  char * __stdcall SRL_GetErrorString( int iErrorId );




#ifdef __cplusplus
}
#endif


#endif // __SRLLOBBY_INCLUDED__
// End of File.
