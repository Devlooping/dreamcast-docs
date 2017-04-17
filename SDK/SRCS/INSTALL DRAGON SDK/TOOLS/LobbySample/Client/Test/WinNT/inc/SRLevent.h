/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRLevent.h
 *  Content:    Lobby client event manage include
 *                                           
 ***************************************************************************/

#ifndef _SRLevent_H
#define _SRLevent_H

//////////////////////////////////////////////
//           �}�C�N���錾

//*********************************************
//*	���r�[API�C�x���g�R�[�h�i����C�x���g�j
//*********************************************
enum SRLEVENT_E
{
	SRLEVENT_NOTHING = 1000,				// ���g�p
	SRLEVENT_INITIALIZED,					// ���r�[API����������

	SRLEVENT_CONNECTION_START,				// ���r�[�T�[�o�ւ̐ڑ��J�n
	SRLEVENT_CONNECTION_INITIALIZING,		// ���r�[�T�[�o�ւ̐ڑ���
	SRLEVENT_CONNECTION_DIALING,			// ���r�[�T�[�o�ւ̓d�b��
	SRLEVENT_CONNECTION_WAITINGSERVERREPLY,	// ���r�[�T�[�o����̐ڑ������҂�
	SRLEVENT_CONNECTION_ESTABLISHING,		// ���r�[�T�[�o�Ƃ̐ڑ��m����
	SRLEVENT_CONNECTION_CONNECTED,			// ���r�[�T�[�o�ւ̐ڑ�����
	SRLEVENT_CONNECTION_CLOSED,				// ���r�[�T�[�o�Ƃ̐ڑ���؂藣��

	SRLEVENT_USER_ADDED,					// �V�K���[�U�����O�C������
	SRLEVENT_USER_REMOVED,					// 10 �����[�U�����O�A�E�g����

	SRLEVENT_USER_GETLISTSTART,				// ���[�U���X�g�̎擾���J�n����
	SRLEVENT_USER_GETLISTDONE,				// ���[�U���X�g�̎擾���������� 

	SRLEVENT_USER_REFRESH_START,			// ���[�U���̍X�V���J�n����
	SRLEVENT_USER_REFRESH_END,				// ���[�U���̍X�V���������� 

	SRLEVENT_SEARCHUSER_START,				// ���[�U�������J�n����
	SRLEVENT_SEARCHUSER_END,				// ���[�U�������I������

	SRLEVENT_LOBBY_JOINED,					// ���r�[�ɎQ������
	SRLEVENT_LOBBY_LEAVE,					// ���r�[���痣�E����

	SRLEVENT_LOBBY_GETLISTSTART,			// ���r�[���X�g�̎擾���J�n����
	SRLEVENT_LOBBY_GETLISTDONE,				// ���r�[���X�g�̎擾���������� 

	SRLEVENT_LOBBY_REFRESH_START,			// ���r�[���̍X�V���J�n����
	SRLEVENT_LOBBY_REFRESH_END,				// ���r�[���̍X�V���������� 
	
	SRLEVENT_GAME_LAUNCH,					// �Q�[�����N�����鏀������
	SRLEVENT_GAME_STARTED,					// �Q�[�����J�n���ꂽ
	SRLEVENT_GAME_FINISHED,					// �Q�[�����I������
	SRLEVENT_GAME_JOINED,					// �Q�[���ɎQ������
	SRLEVENT_GAME_LEAVE,					// �Q�[������E�ނ���

	SRLEVENT_GAME_GETLISTSTART,				// �Q�[�����̍X�V���J�n����
	SRLEVENT_GAME_GETLISTDONE,				// �Q�[�����̍X�V����������
	
	SRLEVENT_GROUP_ADDED,					// 30 �`�[�����쐬����
	SRLEVENT_GROUP_JOINSTART,				// �`�[���ւ̎Q�����J�n����
	SRLEVENT_GROUP_JOINED,					// �`�[���ւ̎Q������������
	SRLEVENT_GROUP_LEAVE,					// �`�[�����痣�E����
	SRLEVENT_GROUP_DESTROYED,				// �`�[������������
	SRLEVENT_GROUP_CHANGEOWNER,				// �`�[����Î҂��ւ����


	SRLEVENT_GROUP_REFRESH_START,			// �`�[�����̍X�V���J�n����
	SRLEVENT_GROUP_REFRESH_END,				// �`�[�����̍X�V����������

	SRLEVENT_RENEWAL,						// ���[�U��񂪍X�V���ꂽ
	SRLEVENT_TERMINATE,						// �v���Z�X�I���C�x���g

	SRLEVENT_GROUP_GETLISTSTART,			// �`�[�����̎擾���J�n����
	SRLEVENT_GROUP_GETLISTDONE,				// �`�[�����̎擾����������

};

//****************************************************
//*	���r�[API�C�x���g�R�[�h�i���b�Z�[�W�C�x���g�j
//****************************************************
enum SRLMESSAGE_E
{
	SRLMESSAGE_NOTHING = 2000,		// ���g�p
	SRLMESSAGE_RECEIVED,			// ���b�Z�[�W����M����
	SRLMESSAGE_CHAT,				// �`���b�g���b�Z�[�W����M����
};

//****************************************************
//*	���r�[API�C�x���g�R�[�h	(�G���[�C�x���g�j
//****************************************************
enum SRLERROR_E
{
	SRLERROR_NOTHING = 3000,		// ���g�p
	SRLERROR_GENERIC,				// �ėp�I�G���[
	SRLERROR_COMERROR,				// �ʐM�G���[
	SRLERROR_CONNECTION_FAILED,		// �ڑ����s
	SRLERROR_RASDEVICE,				// RAS�ڑ����s
	SRLERROR_INVALIDPARAMS,			// �����p�����[�^
	SRLERROR_NO_ANSWER,				// �T�[�o���疳����
	SRLERROR_DISCONNECTED,			// �ڑ����؂藣���ꂽ
	SRLERROR_SENDFAILED,			// ���M�G���[
	SRLERROR_OVERFLOWED,			// �I�[�o�t���[
	SRLERROR_TIMEOUT,				// �^�C���A�E�g
	SRLERROR_SERVERNOTREPLY,		// �T�[�o��������
	SRLERROR_NOTENOUGHMEMORY,		// ���\�[�X���s��
	SRLERROR_INVALIDPASSWORD,		// �p�X���[�h������
	SRLERROR_INVALIDACCOUNT,		// �A�J�E���g������
	SRLERROR_NOTCONNECTED,			// �ڑ��ł��Ȃ�
	SRLERROR_ALREADYCONNECTED,		// ���łɐڑ����Ă���
	SRLERROR_GAME_LAUNCHFAILED,		// �Q�[���̋N�����s
	SRLERROR_CREATEGROUPFAILED,		// �`�[�����쐬�ł��Ȃ�
	SRLERROR_GROUP_FULL,			// �`�[�����������z����
	SRLERROR_USERALREADYEXIST,		// ���łɓ��ꖼ���[�U�����݂���
	SRLERROR_DUPLICATEUSERNAME,		// ���ꖼ���[�U�o�^�������Ȃ�
	SRLERROR_GROUP_ALREADYEXIST,	// ���ꖼ�`�[�������݂���
	SRLERROR_CHAT_EMPTYMESSAGE,		// ��`���b�g���b�Z�[�W�͑��M�ł��Ȃ�
	SRLERROR_LOBBY_FULL,			// ���r�[���̃��[�U�o�^����t�ł�
	SRLERROR_MAXMSGQUE,				// ���b�Z�[�W�L���[����t�ł�
	SRLERROR_MAXEVENTQUE,			// �C�x���g�L���[����t�ł�
	SRLERROR_MAXERRORQUE			// �G���[�L���[����t�ł�
};


#define SRL_QUE_EVENT 	TEXT("SRLevent.que")
#define SRL_QUE_ERROR 	TEXT("SRLerror.que")
#define SRL_QUE_MESSAGE TEXT("SRLmessage.que")

/********************************/
/*	�C�x���g�L���[�\����		*/
/********************************/
#define SRL_MAXEVENTCOUNT	    100
#define SRL_EVENTQUE_NOTUSED    0
#define SRL_EVENTQUE_ADD	    1
#define SRL_EVENTQUE_READ	    2
#define SRL_EVENTQUE_DELETE	    3

/********************************/
/*	�C�x���g�L���[���f��		*/
/********************************/
#define SRL_EVENTMODE_EVENT     0  //�C�x���g
#define SRL_EVENTMODE_ERROR     1  //�G���[
#define SRL_EVENTMODE_MESSAGE   2  //���b�Z�[�W


//////////////////////////////////////////////
//           �\���̐錾

/********************************/
/*	�C�x���g�L���[�\����		*/
/********************************/

// �C���^�t�F�[�X��
// �C�x���g����
typedef struct _SRL_EVENTMNG{
	DWORD	EM_dwEventID;
	char	EM_cMessage[SRS_MAX_MESSAGE];
} SRLEVENTMNG,*LPSRLEVENTMNG; 
#define SRLEVENTMNG_SZ  (sizeof(SRLEVENTMNG))

// �C�x���g���Ǘ���
typedef struct _SRL_EVENTQUE{
	int					iEventQueCount;
	LONG				lEventQueTop;
	LONG				lEventQueEnd;
}SRLEVENTQUE, *LPSRLEVENTQUE;
#define SRLEVENTQUE_SZ  (sizeof(SRLEVENTQUE))


//////////////////////////////////////////////
//           �֐��錾

//int SRL_Evt_Initial( HINSTANCE hInstance );
int SRL_Evt_PutInfo( LPSTR lpInfoName, LPSTR lpGetBuff, size_t szGetSize );
int SRL_Evt_GetInfo( LPSTR lpInfoName, LPSTR lpGetBuff, size_t szGetSize );

__declspec( dllexport )int __stdcall SRL_Evt_Initial( void);
__declspec( dllexport )int __stdcall SRL_Evt_GetEventCount( int nEventMode );
//BOOL SRL_Evt_GetEventCount( int nEventMode , int *pCount );
__declspec( dllexport )BOOL __stdcall SRL_Evt_GetEvent ( int nEventMode, LPSRLEVENTMNG lpEvent );
__declspec( dllexport )BOOL __stdcall SRL_Evt_SetEvent( int nEventMode, LPSRLEVENTMNG lpEvent );
//BOOL SRL_REQProc(SRLEVENT sRecvData, LPSRLSENDDATA pSendData);
#endif
