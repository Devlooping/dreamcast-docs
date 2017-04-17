/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRLcom.h
 *  Content:    Lobby client registry access include file
 ***************************************************************************/
#ifndef _SRL_REGISTRY_H_
#define _SRL_REGISTRY_H_

//=========================================
//   �֐���`
//=========================================
__declspec( dllexport )int __stdcall SRL_Reg_GetComInfo( struct SRL_REG_INFO *lpRegInfo );
__declspec( dllexport )void __stdcall SRL_Reg_Delete( void );
__declspec( dllexport )int __stdcall SRL_Reg_SetInfo( LPSTR lpRegKey, LPSTR lpReg, LPSTR lpBuff );
__declspec( dllexport )int __stdcall SRL_Reg_GetInfo( LPSTR lpRegKey,LPSTR lpReg, LPSTR lpBuff );
__declspec( dllexport )int __stdcall SRL_Reg_SetInfoBinary( LPSTR lpRegKey, LPSTR lpReg, LPSTR lpBuff, DWORD dwSize );
__declspec( dllexport )int __stdcall SRL_Reg_GetInfoBinary( LPSTR lpRegKey,LPSTR lpReg, LPSTR lpBuff, DWORD dwSize );
__declspec( dllexport )int __stdcall SRL_Reg_SetInfoDWORD( LPSTR lpRegKey,LPSTR lpReg, DWORD dwInfo );
__declspec( dllexport )DWORD __stdcall SRL_Reg_GetInfoDWORD( LPSTR lpRegKey,LPSTR lpReg );

//=========================================
// ���W�X�g���L�[
//=========================================
//  �Œ�
#define	SRL_REG_KEY_COM			"SOFTWARE\\LOBBY CLIENT"			// ���ʏ��
#define	SRL_REG_KEY_DEBUG		"SOFTWARE\\LOBBY CLIENT\\debug"	// �f�o�b�O���
#define	SRL_REG_KEY_TRACE		"SOFTWARE\\LOBBY CLIENT\\trace"	// TRACE���
#define	SRL_REG_KEY_MANAGE		"SOFTWARE\\LOBBY CLIENT\\manage"	// �Ǘ����
#define	SRL_REG_KEY_EVENT		"SOFTWARE\\LOBBY CLIENT\\event"	// �C�x���g�L���[���
#define	SRL_REG_KEY_HANDLE		"SOFTWARE\\LOBBY CLIENT\\handle"	// �E�B���h�E�n���h��
#define	SRL_REG_KEY_USER		"SOFTWARE\\LOBBY CLIENT\\user"	// ���[�U�o�^���

//=========================================
// ���W�X�g������
//=========================================
//  SOFTWARE\\LOBBY CLIENT
#define	SRL_REG_SERVERIP		"server address"		// �T�[�o�A�h���X
#define	SRL_REG_CLIENT			"client name"			// �N���C�A���g��
#define	SRL_REG_USER			"user name"			// ���[�U��
#define	SRL_REG_MODE			"mode"				// �f�o�b�O���[�h
#define	SRL_REG_MAXUSER			"max user"			// �ő�o�^���[�U��
#define	SRL_REG_MAXLOBBY		"max lobby"			// �ő�o�^���r�[��
#define	SRL_REG_MAXGAME			"max game"			// �ő�o�^�Q�[����
#define	SRL_REG_MAXGROUP		"max group"			// �ő�o�^�O���[�v��
#define	SRL_REG_USERINFO		"user info"			// ���[�U���
#define	SRL_REG_LOBBYINFO		"lobby info"			// ���r�[���
#define	SRL_REG_GROUPINFO		"group info"			// �O���[�v���
#define	SRL_REG_GAMEINFO		"game info"			// �Q�[�����
#define	SRL_REG_USERMASK		"user mask"			// ���[�U���}�X�N
#define	SRL_REG_LOBBYMASK		"lobby mask"			// ���r�[���}�X�N
#define	SRL_REG_GROUPMASK		"group mask"			// �O���[�v���}�X�N
#define	SRL_REG_GAMEMASK		"game mask"			// �Q�[�����}�X�N
#define	SRL_REG_EVENTQUE		"event que"			// �C�x���g�L���[
#define	SRL_REG_ERRORQUE		"error que"			// �G���[�L���[
#define	SRL_REG_MESSAGEQUE		"message que"			// ���b�Z�[�W�L���[
#define	SRL_REG_MESSAGEMASK		"message mask"		// ���b�Z�[�W�L���[�}�X�N
#define	SRL_REG_ERRORMASK		"error mask"			// �G���[�L���[�}�X�N
#define	SRL_REG_EVENTMASK		"event mask"			// �C�o���g�L���[�}�X�N
#define	SRL_REG_EVENTHANDLE		"event handle"		// �C�x���g�Ǘ��i�v����M�p�j�n���h��
#define	SRL_REG_EVENTREQMASK	"event request mask"	// �C�x���g�ʒm�}�X�N
#define	SRL_REG_EVENTREQ		"event request"		// �C�x���g�ʒm���
#define	SRL_REG_EVENTANS		"event answer"		// �C�x���g�������
#define	SRL_REG_STATUS			"user status"		    // ���[�U���

//=========================================
// ���ڒl
//=========================================
// �}�X�N���
#define	SRL_REG_MASK_OFF	"0"					// �Q�Ɖ\
#define	SRL_REG_MASK_ON		"1"					// �Q�ƕs��

// �f�o�b�O���[�h
#define SRL_REG_MODE_NO		"0"					// �o�͂Ȃ�
#define SRL_REG_MODE_MSG	"1"					// ���b�Z�[�W�\������
#define SRL_REG_MODE_FILE	"2"					// �t�@�C���o�͂̂݁i�g���[�X���_���v�j
#define SRL_REG_MODE_TRC	"3"					// �g���[�X�o�͂̂�

// ���[�U���
#define	SRL_REG_USER_OFF	"0"					// ���o�^
#define	SRL_REG_USER_ON		"1"					// �o�^

//=========================================
// ���W�X�g������
//=========================================

// ���ʊǗ����
struct	SRL_REG_INFO
{
	WORD	wMaxUser;				// �ő�o�^���[�U��
	WORD	wMaxLobby;				// �ő�o�^���r�[��
	WORD	wMaxGame;				// �ő�o�^�Q�[����
	WORD	wMaxGroup;				// �ő�o�^�O���[�v��
};
#define	SRL_REG_INFO_SZ		(sizeof(struct SRL_REG_INFO))


#endif
