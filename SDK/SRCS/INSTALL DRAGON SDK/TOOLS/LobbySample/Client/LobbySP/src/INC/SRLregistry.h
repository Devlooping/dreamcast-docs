/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRLcom.h
 *  Content:    Lobby client registry access include file
 *              99/11/29 �C�� �b�d�p���W�X�g���l�`�w�@�S�j�܂ŏ������݉̑Ή�
 ***************************************************************************/
#ifndef _SRL_REGISTRY_H_
#define _SRL_REGISTRY_H_

//=========================================
//   �֐���`
//=========================================
__declspec( dllexport )int __stdcall SRL_Reg_GetComInfo( struct SRL_REG_INFO *lpRegInfo );
__declspec( dllexport )void __stdcall SRL_Reg_Delete( void );
__declspec( dllexport )int __stdcall SRL_Reg_SetInfo( LPCTSTR lpRegKey, LPCTSTR lpReg, LPCTSTR lpBuff );
__declspec( dllexport )int __stdcall SRL_Reg_GetInfo( LPCTSTR lpRegKey,LPCTSTR lpReg, LPCTSTR lpBuff );
__declspec( dllexport )int __stdcall SRL_Reg_SetInfoBinary( LPCTSTR lpRegKey, LPCTSTR lpReg, LPCTSTR lpBuff, DWORD dwSize );
__declspec( dllexport )int __stdcall SRL_Reg_GetInfoBinary( LPCTSTR lpRegKey,LPCTSTR lpReg, LPCTSTR lpBuff, DWORD dwSize );
__declspec( dllexport )int __stdcall SRL_Reg_SetInfoDWORD( LPCTSTR lpRegKey,LPCTSTR lpReg, DWORD dwInfo );
__declspec( dllexport )DWORD __stdcall SRL_Reg_GetInfoDWORD( LPCTSTR lpRegKey,LPCTSTR lpReg );

//=========================================
// ���W�X�g���L�[
//=========================================
//  �Œ�
#define	SRL_REG_KEY_COM			TEXT("SOFTWARE\\LOBBY CLIENT")			// ���ʏ��
#define	SRL_REG_KEY_DEBUG		TEXT("SOFTWARE\\LOBBY CLIENT\\debug")	// �f�o�b�O���
#define	SRL_REG_KEY_TRACE		TEXT("SOFTWARE\\LOBBY CLIENT\\trace")	// TRACE���
#define	SRL_REG_KEY_MNGUSER		TEXT("SOFTWARE\\LOBBY CLIENT\\manage\\user")  // ���[�U���
#define	SRL_REG_KEY_MNGLOBBY	TEXT("SOFTWARE\\LOBBY CLIENT\\manage\\lobby") // ���r�[���
#define	SRL_REG_KEY_MNGGROUP	TEXT("SOFTWARE\\LOBBY CLIENT\\manage\\group") // �O���[�v���
#define	SRL_REG_KEY_MNGGAME		TEXT("SOFTWARE\\LOBBY CLIENT\\manage\\game")  // �Q�[�����
#define	SRL_REG_KEY_EVENT		TEXT("SOFTWARE\\LOBBY CLIENT\\event")	// �C�x���g�L���[���
#define	SRL_REG_KEY_HANDLE		TEXT("SOFTWARE\\LOBBY CLIENT\\handle")	// �E�B���h�E�n���h��
#define	SRL_REG_KEY_USER		TEXT("SOFTWARE\\LOBBY CLIENT\\user")	// ���[�U�o�^���
#define SRL_REG_KEY_WAIT		TEXT("SOFTWARE\\LOBBY CLIENT\\wait")	// �����҂����

//=========================================
// ���W�X�g������
//=========================================
//  ���ʏ��iSOFTWARE\\LOBBY CLIENT�j
#define	SRL_REG_SERVERIP		TEXT("server address")		// �T�[�o�A�h���X
#define	SRL_REG_CLIENT			TEXT("client name")			// �N���C�A���g��
#define SRL_REG_PORT			TEXT("port")				// �g�p�T�[�o�|�[�g�ԍ�
#define	SRL_REG_USER			TEXT("user name")			// ���[�U��
#define	SRL_REG_MODE			TEXT("mode")				// �f�o�b�O���[�h
#define	SRL_REG_MAXUSER			TEXT("max user")			// �ő�o�^���[�U��
#define	SRL_REG_MAXLOBBY		TEXT("max lobby")			// �ő�o�^���r�[��
#define	SRL_REG_MAXGAME			TEXT("max game")			// �ő�o�^�Q�[����
#define	SRL_REG_MAXGROUP		TEXT("max group")			// �ő�o�^�O���[�v��
#define	SRL_REG_EVENTQUE		TEXT("event que")			// �C�x���g�L���[
#define	SRL_REG_ERRORQUE		TEXT("error que")			// �G���[�L���[
#define	SRL_REG_MESSAGEQUE		TEXT("message que")			// ���b�Z�[�W�L���[
#define	SRL_REG_MESSAGEMASK		TEXT("message mask")		// ���b�Z�[�W�L���[�}�X�N
#define	SRL_REG_ERRORMASK		TEXT("error mask")			// �G���[�L���[�}�X�N
#define	SRL_REG_EVENTMASK		TEXT("event mask")			// �C�o���g�L���[�}�X�N
#define	SRL_REG_EVENTHANDLE		TEXT("event handle")		// �C�x���g�Ǘ��i�v����M�p�j�n���h��
#define	SRL_REG_EVENTREQMASK	TEXT("event request mask")	// �C�x���g�ʒm�}�X�N
#define	SRL_REG_EVENTREQ		TEXT("event request")		// �C�x���g�ʒm���
#define	SRL_REG_EVENTANS		TEXT("event answer")		// �C�x���g�������
#define	SRL_REG_STATUS			TEXT("user status")		    // ���[�U���
//
//  �v�������҂����iSOFTWARE\\LOBBY CLIENT\\WAIT�j
//
#define	SRL_REG_USERENTRY		TEXT("UserEntry")			// ���[�U�o�^
#define	SRL_REG_USERDELETE		TEXT("UserDelete")			// ���[�U�폜
#define	SRL_REG_GROUPMAKE		TEXT("GroupMake")			// �O���[�v�쐬
#define	SRL_REG_GROUPJOIN		TEXT("GroupJoin")			// �O���[�v�Q��
#define	SRL_REG_GROUPDEFECT		TEXT("GroupDefect")			// �O���[�v�E��
#define SRL_REG_ALLGET			TEXT("AllGet")				// �S���Q��

//  SOFTWARE\\LOBBY CLIENT\\manage\\user
#define	SRL_REG_USERMASK		TEXT("user mask")			// ���[�U���}�X�N
#define	SRL_REG_USERINFO		TEXT("user info")			// ���[�U�Ǘ����
#define	SRL_REG_USERITEM		TEXT("user")			    // ���[�U���

//  SOFTWARE\\LOBBY CLIENT\\manage\\lobby
#define	SRL_REG_LOBBYMASK		TEXT("lobby mask")			// ���r�[���}�X�N
#define	SRL_REG_LOBBYINFO		TEXT("lobby info")			// ���r�[�Ǘ����
#define	SRL_REG_LOBBYITEM		TEXT("lobby")			    // ���r�[���

//  SOFTWARE\\LOBBY CLIENT\\manage\\group
#define	SRL_REG_GROUPMASK		TEXT("group mask")			// �O���[�v���}�X�N
#define	SRL_REG_GROUPINFO		TEXT("group info")			// �O���[�v�Ǘ����
#define	SRL_REG_GROUPITEM		TEXT("group")			    // �O���[�v���

//  SOFTWARE\\LOBBY CLIENT\\manage\\game
#define	SRL_REG_GAMEMASK		TEXT("game mask")			// �Q�[�����}�X�N
#define	SRL_REG_GAMEINFO		TEXT("game info")			// �Q�[���Ǘ����
#define	SRL_REG_GAMEITEM		TEXT("game")			    // �Q�[�����


//=========================================
// ���ڒl
//=========================================
// �}�X�N���
#define	SRL_REG_MASK_OFF	TEXT("0")					// �Q�Ɖ\
#define	SRL_REG_MASK_ON		TEXT("1")					// �Q�ƕs��

// �f�o�b�O���[�h
#define SRL_REG_MODE_NO		TEXT("0")					// �o�͂Ȃ�
#define SRL_REG_MODE_MSG	TEXT("1")					// ���b�Z�[�W�\������
#define SRL_REG_MODE_FILE	TEXT("2")					// �t�@�C���o�͂̂݁i�g���[�X���_���v�j
#define SRL_REG_MODE_TRC	TEXT("3")					// �g���[�X�o�͂̂�

// ���[�U���
#define	SRL_REG_USER_OFF	TEXT("0")					// ���o�^
#define	SRL_REG_USER_ON		TEXT("1")					// �o�^

// �����҂����
#define SRL_REG_WAIT_ON			TEXT("1")				// �����҂����
#define SRL_REG_WAIT_OFF		TEXT("0")				// �v���Ȃ�
#define SRL_REG_WAIT_OK			TEXT("O")				// �v�����튮��
#define SRL_REG_WAIT_NG			TEXT("N")				// �v���ُ튮��
//#define SRL_REG_WAIT_OK			TEXT("OK")				// �v�����튮��
//#define SRL_REG_WAIT_NG			TEXT("NG")				// �v���ُ튮��

//=========================================
// ���W�X�g������
//=========================================

// ���ʊǗ����
typedef struct	SRL_REG_INFO
{
	WORD	wMaxUser;				// �ő�o�^���[�U��
	WORD	wMaxLobby;				// �ő�o�^���r�[��
	WORD	wMaxGame;				// �ő�o�^�Q�[����
	WORD	wMaxGroup;				// �ő�o�^�O���[�v��
} SRL_REGINFO, * LPSRL_REGINFO;
#define	SRL_REG_INFO_SZ		(sizeof(struct SRL_REG_INFO))


#endif
