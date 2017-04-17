/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRScom.h
 *  Content:    Lobby server & Lobby client common tools 
 ***************************************************************************/
#ifndef _SRS_COM_H_
#define _SRS_COM_H_

/***************************************************************************
 *
 *  ���ʒ�`
 *
 ***************************************************************************/
//=========================================
// �|�[�g�ԍ�
//=========================================
#define	SRS_PORT_SERVER		7000			// �T�[�o�ڑ��v���p�|�[�g�ԍ�
#define	SRS_PORT_CLIENT		6000			// �N���C�A���g�ڑ��v���p�|�[�g�ԍ�

//=========================================
//   �v���Z�X��
//=========================================
#define	SRS_PROC_INITIAL	"SRSinitial"		// �T�[�o�v���҂��v���Z�X
#define	SRS_PROC_LOG		"SRSlog.exe"		// ���O�\���v���Z�X
#define	SRS_PROC_LOBBY		"SRSlobby"		// �T�[�oLobby�v���Z�X
#define SRL_PROC_RCV		"SRLreceive.exe"	// �N���C�A���g��M�v���Z�X
#define SRL_PROC_SEND		"SRLsend.exe"	    // �N���C�A���g���M�v���Z�X

//=========================================
//   ���b�Z�[�W�ԍ�
//=========================================
#define SRS_MSG_LOG		WM_USER + 1					// �ڑ����O�ʒm�p

//=========================================
//   ���b�Z�[�W�ԍ�
//=========================================
#define SRS_WORD_ERROR		65535					// WORD�^�G���[���A�l

/***************************************************************************
 *
 *  �k���������T�[�o�Ǘ�����
 *
 ***************************************************************************/
//  ���[�U���
#define	SRS_SEQ_NOENTRY		(DWORD)0x000000	// ���o�^
#define	SRS_SEQ_LOBBYENTRY	(DWORD)0x010000	// ���r�[�Q���A�Q�[�����I��
#define	SRS_SEQ_GAMEENTRY	(DWORD)0x011000	// �Q�[���I���A�O���[�v���Q��
#define	SRS_SEQ_GROUPENTRY	(DWORD)0x011100	// �O���[�v�Q��
#define	SRS_SEQ_PLAY		(DWORD)0x011110	// �Q�[���Q����

//  �ő�l 
#define SRS_MAX_LOBBY_USER		64			// ���r�[���ő�Q�����[�U��
#define SRS_MAX_LOBBY_GROUP		32			// ���r�[���ő�쐬�O���[�v��
#define SRS_MAX_GROUP_USER    	64			// �O���[�v���ő�Q�����[�U��

#define SRS_MAX_NAME_CLIENT		16			// �ő�N���C�A���g����  
#define SRS_MAX_NAME_USER		40			// �ő僆�[�U����  
#define SRS_MAX_NAME_GAME		40			// �ő�Q�[������  
#define SRS_MAX_NAME_GROUP		40			// �ő�O���[�v����
#define SRS_MAX_NAME_LOBBY		40			// �ő働�r�[����  

#define SRS_MAX_PASSWORD		40			// �p�X���[�h��    
#define SRS_MAX_USERAREA		40			// ���[�U�ŗL��    
#define SRS_MAX_MESSAGE			128			// �ő僁�b�Z�[�W������
#define SRS_MAX_LOGMSG			256			// �ő働�M���O���b�Z�[�W������

//  �Ǘ�ID
#define	SRS_ID_MAINLOBBY	(WORD)0			// ���C�����r�[�h�c

//  �Q�[���Q������
#define SRS_GAME_PLAY		(WORD)0			// �Q�[���Q��
#define SRS_GAME_WATCH		(WORD)1			// �Q�[���ϐ�

//====================================
//  ���[�U����
//====================================
//
// ���[�U����
//
struct	SRS_USER_ITEM
{
	char	USI_cClientName[SRS_MAX_NAME_CLIENT];	// �N���C�A���g��
	char	USI_cUserName[SRS_MAX_NAME_USER];		// ���[�U��
	char	USI_cPassword[SRS_MAX_PASSWORD];		// �p�X���[�h
	int		USI_iIp;								// �h�o�A�h���X
	int		USI_iPort;								// �g�p�|�[�g�ԍ�
	WORD	USI_wUserID;							// ���[�U�h�c
	WORD	USI_wGameID;							// �Q���Q�[���h�c
	WORD	USI_wLobbyID;							// �Q�����r�[�h�c
	WORD	USI_wGroupID;							// �Q���O���[�v�h�c
	DWORD	USI_dwCondetion;						// ���
	char	USI_cUserArea[SRS_MAX_USERAREA];		// ���[�U�ŗL��
	SYSTEMTIME	USI_time;							// ���[�U���ԏ��	2000.1 ��t���t�������܂���
};				
#define SRS_USER_ITEM_SZ	(sizeof(struct SRS_USER_ITEM))

//
// �Ǘ�����
//
struct  SRS_INFO_USER
{
	WORD	US_wMaxUser;						// �ő僆�[�U��
	WORD	US_wEntryUser;						// �o�^���[�U��
	struct  SRS_USER_ITEM US_info_user[1];		// ���[�U����Q
};
#define SRS_INFO_USER_SZ		(sizeof(struct SRS_INFO_USER))
#define SRS_INFO_USER_COM_SZ	(SRS_INFO_USER_SZ - SRS_USER_ITEM_SZ)	// ���ʈ�T�C�Y
//====================================
//  �Q�[������
//====================================
//
// �Q�[������
//
struct	SRS_GAME_ITEM
{
	int		GMI_iLockword;						// ���b�N���[�h
	char	GMI_cGameName[SRS_MAX_NAME_GAME];	// �Q�[����
	WORD	GMI_wGameID;						// �Q�[���h�c
	WORD	GMI_wMaxGameUser;					// �ő�Q���l��
	DWORD	GMI_Reserve;						// ���U�[�u
};
#define SRS_GAME_ITEM_SZ	(sizeof(struct SRS_GAME_ITEM))

//
// �Ǘ�����
//
struct	SRS_INFO_GAME
{
	WORD	GM_wMaxGame;						// �ő�Q�[����
	WORD	GM_wEntryGame;						// �o�^�Q�[����
	struct  SRS_GAME_ITEM  GM_info_game[1];		// �Q�[������Q
};
#define SRS_INFO_GAME_SZ		(sizeof(struct SRS_INFO_GAME))
#define SRS_INFO_GAME_COM_SZ	(SRS_INFO_GAME_SZ - SRS_GAME_ITEM_SZ)	// ���ʈ�T�C�Y

//====================================
//  ���r�[����
//====================================
//
// ���r�[����
//
struct	SRS_LOBBY_ITEM
{
	int		LBI_iLockword;						// ���b�N���[�h
	char	LBI_cLobbyName[SRS_MAX_NAME_LOBBY];	// ���r�[��
	WORD	LBI_wLobbyID;						// ���r�[�h�c
	WORD	LBI_Reserve;						// ���U�[�u
	WORD	LBI_wMaxLobbyUser;					// �ő�Q���l��
	WORD	LBI_wLobbyUser;						// ���r�[�����[�U��
	WORD	LBI_wUserID[SRS_MAX_LOBBY_USER];	// �Q�����[�U�h�c
	WORD	LBI_wGroupID[SRS_MAX_LOBBY_GROUP];	// �쐬�O���[�v�h�c
};
#define SRS_LOBBY_ITEM_SZ	(sizeof(SRS_LOBBY_ITEM))

//
// �Ǘ�����
//
struct	SRS_INFO_LOBBY
{
	WORD	LB_wMaxLobby;						// ���r�[��
	WORD	LB_Reserved;						// ���U�[�u
	struct  SRS_LOBBY_ITEM LB_info_lobby[1];	// ���r�[����Q
};
#define SRS_INFO_LOBBY_SZ		(sizeof(struct SRS_INFO_LOBBY))
#define SRS_INFO_LOBBY_COM_SZ	(SRS_INFO_LOBBY_SZ - SRS_LOBBY_ITEM_SZ)	// ���ʈ�T�C�Y

//====================================
//	�O���[�v����
//====================================
//
// �O���[�v���v�f
//
struct	SRS_GROUP_ITEM
{
	int		GPI_iLockword;						// ���b�N���[�h
	char	GPI_cGroupName[SRS_MAX_NAME_GROUP];	// �O���[�v��
	WORD	GPI_wGroupID;						// �O���[�v�h�c
	WORD	GPI_wGameID;						// �Q�[���h�c
	WORD	GPI_wEntryUser;						// �Q�����[�U��
	WORD   	GPI_wMaxUser;						// �ő�Q���l��
	WORD	GPI_wUserID[SRS_MAX_GROUP_USER];	// �Q�����[�U�h�c
}; 
#define SRS_GROUP_ITEM_SZ	(sizeof(SRS_GROUP_ITEM))

//
// �Ǘ�����
//
struct	SRS_INFO_GROUP
{
	WORD	GP_wMaxGroup;						// �ő�O���[�v��
	WORD	GP_wEntryGroup;						// �o�^�O���[�v��
	struct  SRS_GROUP_ITEM	GP_info_group[1];	// �O���[�v����Q
};
#define SRS_INFO_GROUP_SZ		(sizeof(struct SRS_INFO_GROUP))
#define SRS_INFO_GROUP_COM_SZ	(SRS_INFO_GROUP_SZ - SRS_GROUP_ITEM_SZ)	// ���ʈ�T�C�Y

/***************************************************************************
 *
 *  �k���������T�[�o�|�N���C�A���g�ԃC���^�t�F�[�X��
 *
 ***************************************************************************/
//==============================
//  �v���R�[�h
//==============================
//
//  �N���C�A���g �� �T�[�o �v��
//
#define IF_REQ_CONNECT		0x0001		// �ڑ��v��
#define IF_REQ_DISCONNECT	0x0002		// ����v��
#define IF_REQ_LOGMSG		0x0003		// ���M���O���b�Z�[�W�ʒm

#define	IF_REQ_ALL			0x0101		// �S���Q��
#define IF_REQ_USER_ENTRY	0x0111		// ���[�U�o�^
#define IF_REQ_USER_DELETE	0x0112		// ���[�U�폜
#define IF_REQ_LOBBY_ENTRY	0x0121		// ���r�[���[�U�Q��
#define IF_REQ_LOBBY_DEFECT	0x0122		// ���r�[���[�U�E��
#define IF_REQ_GROUP_MAKE	0x0131		// �O���[�v�쐬
#define IF_REQ_GROUP_JOIN	0x0132		// �O���[�v�Q��
#define IF_REQ_GROUP_DEFECT	0x0133		// �O���[�v�E��
#define IF_REQ_GAME_JOIN	0x0141		// �Q�[���Q��
#define IF_REQ_GAME_DEFECT	0x0142		// �Q�[���E��
#define IF_REQ_GAME_START	0x0143		// �Q�[���J�n
#define IF_REQ_GAME_END		0x0144		// �Q�[���I��
#define IF_REQ_SENDMSG		0x0151		// ���b�Z�[�W���M
#define IF_REQ_SAVE_INFO	0x0161		// �ŗL���Ҕ�

//
// �T�[�o �� �N���C�A���g �v��
//
#define IF_REQ_RENEWAL		0x0201		// ���X�V
#define IF_REQ_RCVMSG		0x0202		// ���b�Z�[�W��M

//
// �R�}���h �� �T�[�o �v��
//
#define IF_REQ_TERMINATE	0xFFFF		// �I���v��

//================================
//  ���ʃR�[�h
//================================
#define IF_CMP_SUCCESS		0x0000		// ����
#define IF_CMP_MAXOVER		0xFF01		// �ő�l�I�[�o
#define IF_CMP_MULTI		0xFF02		// �Q�d�o�^
#define IF_CMP_AREAOVER		0xFF03		// �����s��
#define IF_CMP_SEQERR		0xFF04		// �v���V�[�P���X�G���[
#define IF_CMP_PROCERR		0xFF05		// Lobby�v���Z�X�N���G���[
#define IF_CMP_ERROR		0xFFFF		// �V�X�e���i�v���O�����j�G���[

//====================================
//  �C���^�t�F�[�X�u���b�N
//====================================
//
//  �Œ��
//
struct  SRS_IF_FIX
{
	WORD	IF_wRequest;			// �v���R�[�h
	WORD	IF_wResult;				// ���ʃR�[�h
	int		IF_iInformationSize;	// ���撷
	int		IF_iPortNo;				// �T�[�o�ʐM�p�|�[�g�ԍ�
	int		IF_iIp;					// �N���C�A���gIP�A�h���X
	char	IF_cInformation[4];		// ����
};
#define	SRS_IF_FIX_SZ	(sizeof(struct SRS_IF_FIX) - 4)

//====================================
//  ����i�v���j
//====================================
//=========================
//  �ڑ��v�� 
//=========================
struct	SRS_IF_CONNECT
{
	WORD	IF_CN_wMaxUser;			// �ő僆�[�U��
	WORD	IF_CN_wMaxLobby;		// �ő働�r�[��
	WORD	IF_CN_wMaxGroup;		// �ő�O���[�v��
	WORD	IF_CN_wMaxGame;			// �ő�Q�[����
	char	IF_CN_cMessage[SRS_MAX_MESSAGE];
};
#define	SRS_IF_CONNECT_SZ 		(sizeof(struct SRS_IF_CONNECT))

//========================
//  ���M���O���b�Z�[�W
//========================
struct	SRS_IF_LOGMSG
{
	char	IF_LM_cMessage[SRS_MAX_LOGMSG];		// �\�����b�Z�[�W
};
#define	SRS_IF_LOGMSG_SZ 		(sizeof(struct SRS_IF_LOGMSG))

//========================
//  ���[�U�o�^ 
//========================
struct	SRS_IF_USER_ENTRY
{
	char	IF_UE_cClientName[SRS_MAX_NAME_CLIENT];	// �N���C�A���g��
	char	IF_UE_cUserName[SRS_MAX_NAME_USER];		// ���[�U��
};
#define SRS_IF_USER_ENTRY_SZ	(sizeof(struct SRS_IF_USER_ENTRY))

//========================
// == ���[�U�폜 ==
//========================
struct	SRS_IF_USER_DELETE
{
	WORD	IF_UD_wUserID;			// ���[�U�h�c
	WORD	IF_UD_wReserved;		// ���U�[�u
};
#define SRS_IF_USER_DELETE_SZ	(sizeof(struct SRS_IF_USER_DELETE))

//========================
//  ���r�[���[�U�Q�� 
//========================
struct	SRS_IF_LOBBY_ENTRY
{
	WORD	IF_LE_wUserID;			// �Q�����[�U�h�c
	WORD	IF_LE_wLobbyID;			// �Q�����r�[�h�c
};
#define SRS_IF_LOBBY_ENTRY_SZ	(sizeof(struct SRS_IF_LOBBY_ENTRY))

//========================
//  ���r�[���[�U�E�� 
//========================
struct	SRS_IF_LOBBY_DEFECT
{
	WORD	IF_LD_wUserID;			// �E�ރ��[�U�h�c
	WORD	IF_LD_wLobbyID;			// �E�ރ��r�[�h�c
};
#define SRS_IF_LOBBY_DEFECT_SZ	(sizeof(struct SRS_IF_LOBBY_DEFECT))

//========================
//  �O���[�v�쐬 
//========================
struct	SRS_IF_GROUP_MAKE
{
	WORD	IF_GM_wUserID;							// ��Ã��[�U�h�c
	WORD	IF_GMwReserved;							// ���U�[�u
	char	IF_GM_cGroupName[SRS_MAX_NAME_GROUP];	// �쐬�O���[�v��
};
#define SRS_IF_GROUP_MAKE_SZ	(sizeof(struct SRS_IF_GROUP_MAKE))

//========================
//  �O���[�v�Q�� 
//========================
struct	SRS_IF_GROUP_JOIN
{
	WORD	IF_GJ_wUserID;			// �Q�����[�U�h�c
	WORD	IF_GJ_wGroupID;			// �Q���O���[�v�h�c
	WORD	IF_GJ_wCondition;		// �Q������
	WORD	IF_GJ_wReserved;		// ���U�[�u
};
#define SRS_IF_GROUP_JOIN_SZ	(sizeof(struct SRS_IF_GROUP_JOIN))

//========================
//  �O���[�v�E�� 
//========================
struct	SRS_IF_GROUP_DEFECT
{
	WORD	IF_GD_wUserID;			// �E�ރ��[�U�h�c
	WORD	IF_GD_wGroupID;			// �E�ރO���[�v�h�c
};
#define SRS_IF_GROUP_DEFECT_SZ	(sizeof(struct SRS_IF_GROUP_DEFECT))

//========================
//  �Q�[���Q�� 
//========================
struct	SRS_IF_GAME_JOIN
{
	WORD	IF_GJ_wUserID;			// �Q�����[�U�h�c
	WORD	IF_GJ_wGameID;			// �Q���Q�[���h�c
	WORD	IF_GJ_wCondition;		// �Q������
	WORD	IF_GJ_wReserved;		// ���U�[�u
};
#define SRS_IF_GAME_JOIN_SZ	(sizeof(struct SRS_IF_GAME_JOIN))

//========================
//  �Q�[���E�� 
//========================
struct	SRS_IF_GAME_DEFECT
{
	WORD	IF_GD_wUserID;			// �E�ރ��[�U�h�c
	WORD	IF_GD_wGameID;			// �E�ރQ�[���h�c
};
#define SRS_IF_GAME_DEFECT_SZ	(sizeof(struct SRS_IF_GAME_DEFECT))

//========================
//  �Q�[���J�n 
//========================
struct	SRS_IF_GAME_START
{
	WORD	IF_GS_wUserID;			// ���[�U�h�c
	WORD	Reserve;				// ���U�[�u
};
#define SRS_IF_GAME_START_SZ	(sizeof(struct SRS_IF_GAME_START))

//========================
//  �Q�[���I�� 
//========================
struct	SRS_IF_GAME_END
{
	WORD	IF_GE_wUserID;			// ���[�U�h�c
	WORD	IF_GE_Reserve;			// ���U�[�u
};
#define SRS_IF_GAME_END_SZ	(sizeof(struct SRS_IF_GAME_END))

//========================
//  �ŗL���Ҕ� 
//========================
struct	SRS_IF_SAVE_INFO
{
	WORD	IF_SI_wUserID;					// ���[�U�h�c
	WORD	IF_SI_Reserve;					// ���U�[�u
	char	IF_SI_cInfo[SRS_MAX_USERAREA];	// �ŗL���
};
#define SRS_IF_SAVE_INFO_SZ	(sizeof(struct SRS_IF_SAVE_INFO))

//========================
//  ���X�V 
//========================
struct	SRS_IF_RENEWAL
{
	WORD	IF_RE_wUserInfoNo;		// �X�V���[�U���
	WORD	IF_RE_wLobbyInfoNo;		// �X�V���r�[���
	WORD	IF_RE_wGroupInfoNo;		// �X�V�O���[�v���
	WORD	IF_RE_wGameInfoNo;		// �X�V�Q�[�����
	char	IF_RE_cInfo[4];			// �Ǘ�����
};
#define SRS_IF_RENEWAL_SZ	(sizeof(struct SRS_IF_RENEWAL) - 4 )

//========================
// ���b�Z�[�W���M�^��M 
//========================
struct	SRS_IF_MESSAGE
{
	WORD	IF_MS_wUserID;						// ���M�����[�U�h�c
	WORD	IF_MS_wMessageCode;					// ���b�Z�[�W�R�[�h
	char	IF_MS_cMessage[SRS_MAX_MESSAGE];	// ���b�Z�[�W�{��
};
#define SRS_IF_MESSAGE_SZ	(sizeof(struct SRS_IF_MESSAGE))

#endif
