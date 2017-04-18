/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRSregistly.h
 *  Content:    Lobby server registry access include file
 ***************************************************************************/
#ifndef _SRS_REGISTRY_H_
#define _SRS_REGISTRY_H_

//=========================================
//   �֐���`
//=========================================
__declspec( dllexport )int __stdcall SRS_Reg_GetComInfo( struct SRS_REG_INFO *lpRegInfo );
__declspec( dllexport )int __stdcall SRS_Reg_GetLobbyInfo( int iCount, struct SRS_REG_INFO_LOBBY *lpRegInfoLobby );
__declspec( dllexport )int __stdcall SRS_Reg_GetGameInfo( int iCount, struct SRS_REG_INFO_GAME *lpRegInfoGame );
__declspec( dllexport )int __stdcall SRS_Reg_GetInfo( char *lpRegKey, char *lpReg, char *lpBuff );
__declspec( dllexport )int __stdcall SRS_Reg_SetInfo( char *lpRegKey, char *lpReg,char *lpBuff );
__declspec( dllexport )int __stdcall SRS_Reg_SetInfoBinary( char *lpRegKey, char *lpReg, char *lpBuff, DWORD dwSize );
__declspec( dllexport )int __stdcall SRS_Reg_GetInfoBinary( char *lpRegKey, char *lpReg, char *lpBuff, DWORD dwSize );
__declspec( dllexport )int __stdcall SRS_Reg_SetInfoDWORD( char *lpRegKey, char *lpReg, DWORD dwInfo );
__declspec( dllexport )DWORD __stdcall SRS_Reg_GetInfoDWORD( char *lpRegKey,char *lpReg );
__declspec( dllexport )int __stdcall SRS_Reg_Log( char *lpLog );

//=========================================
// ���W�X�g���L�[
//=========================================
//  �Œ�
#define	SRS_REG_KEY_COM			"SOFTWARE\\LOBBY SERVER"			// �T�[�o���ʏ��
#define	SRS_REG_KEY_LOBBY		"SOFTWARE\\LOBBY SERVER\\lobby"		// ���r�[�ŗL���
#define	SRS_REG_KEY_GAME		"SOFTWARE\\LOBBY SERVER\\game"		// �Q�[���ŗL���
#define	SRS_REG_KEY_DEBUG		"SOFTWARE\\LOBBY SERVER\\debug"		// �f�o�b�O���
#define	SRS_REG_KEY_LOGGING		"SOFTWARE\\LOBBY SERVER\\logging"	// ���M���O���
#define	SRS_REG_KEY_CHATLOG		"SOFTWARE\\LOBBY SERVER\\chatlog"	// �`���b�g���O���

//=========================================
// ���W�X�g������
//=========================================
//  SOFTWARE\\LOBBY SERVER
#define	SRS_REG_CLIENTPORT	"client port"		// �N���C���g�����Đ擪�|�[�g�ԍ�
#define	SRS_REG_SEND		"send mode"			// �X�V���z�M�v��
#define	SRS_REG_MAXUSER		"max user"			// �ő�o�^���[�U��
#define	SRS_REG_MAXLOBBY	"max lobby"			// �ő�o�^���r�[��
#define	SRS_REG_MAXGAME		"max game"			// �ő�o�^�Q�[����
#define	SRS_REG_MAXGROUP	"max group"			// �ő�o�^�O���[�v��
#define	SRS_REG_LOGHANDLE	"logging handle"	// ���M���O�E�B���h�E�n���h��
#define	SRS_REG_LOGMSG		"logging message"	// ���M���O���b�Z�[�W


//  SOFTWARE\\LOBBY SERVER\\lobby & SOFTWARE\\LOBBY SERVER\\game
#define	SRS_REG_MAXENTRY	"max entry"			// �ő�Q���l���i���r�[���Q�[���j
#define	SRS_REG_NAME  	    "name"				// ���O�i���r�[���Q�[���j

//  SOFTWARE\\LOBBY SERVER\\debug
#define	SRS_REG_MODE  	    "mode"				// �f�o�b�O���[�h

//  SOFTWARE\\LOBBY SERVER\\chatlog				
#define	SRS_REG_CHATLOG_MODE "mode"				// �`���b�g���O���[�h

//  �`���b�g���O���[�h��`�l
#define	SRS_DBG_CHATLOG_OFF		"0"				// �o�͂Ȃ�
#define	SRS_DBG_CHATLOG_ON		"1"				// �S�o��

//  �z�M�v�� ��`�l
#define	SRS_SEND_MODE_NO	"0"					// �z�M���Ȃ�
#define	SRS_SEND_MODE_NEED	"1"					// �z�M����

//  �f�o�b�O���[�h��`�l
#define	SRS_DBG_MODE_NO		"0"					// �o�͂Ȃ�
#define	SRS_DBG_MODE_MSG	"1"					// �S�o�́i�t�@�C���o�́A���b�Z�[�W�\���j
#define	SRS_DBG_MODE_FILE	"2"					// �g���[�X���_���v�o��
#define	SRS_DBG_MODE_TRC	"3"					// �g���[�X�o�͂̂�

//=========================================
// ���W�X�g������
//=========================================

// ���ʊǗ����
struct	SRS_REG_INFO
{
	WORD	wMaxUser;				// �ő�o�^���[�U��
	WORD	wMaxLobby;				// �ő�o�^���r�[��
	WORD	wMaxGame;				// �ő�o�^�Q�[����
	WORD	wMaxGroup;				// �ő�o�^�O���[�v��
};
#define	SRS_REG_INFO_SZ		(sizeof(struct SRS_REG_INFO))

// �ŗL���r�[���
struct	SRS_REG_INFO_LOBBY
{
	WORD	wMaxUser;					// �ő�Q�����[�U��
	WORD	wReserve;					// ���U�[�u
	char	cName[SRS_MAX_NAME_LOBBY];	// ���r�[��
};
#define	SRS_REG_INFO_LOBBY_SZ		(sizeof(struct SRS_REG_INFO_LOBBY))

// �ŗL�Q�[�����
struct	SRS_REG_INFO_GAME
{
	WORD	wMaxUser;					// �ő�Q�����[�U��
	WORD	wReserve;					// ���U�[�u
	char	cName[SRS_MAX_NAME_GAME];	// �Q�[����
};
#define	SRS_REG_INFO_GAME_SZ		(sizeof(struct SRS_REG_INFO_GAME))

#endif
