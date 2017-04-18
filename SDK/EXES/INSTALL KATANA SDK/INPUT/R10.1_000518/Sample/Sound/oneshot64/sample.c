/****************************************************
 *													*
 *	Dreamcast Sound Library							*
 *			Sample Program							*
 *													*
 *		Copyright 1997-1999 (C)SEGA Enterprises		*
 *			Programmed By T.Jokagi					*
 *													*
 ****************************************************/

/********************************************************************
 *
 *	�T���v����	�����V���b�g�Đ��D
 *	���e		�T�E���h�V�X�e���̏��������s�������V���b�g���Đ��D
 *	���l		
 *
 */

#include	<shinobi.h>
#include	<ninja.h>
#include	"sg_sd.h"

/*
 *	���̃T���v���Ŏg�p����t�@�C��
 */
#define	SMPD_SND_DRV_FILE_NAME	"MANATEE.DRV"
#define	SMPD_MLT_FILE_NAME		"ONESHOT64.MLT"

/*
 *	�e�������Ƃɐݒ肵�����ő唭����
 */
#define	SMPD_MIDI_SLOT_MAX	(0)
#define	SMPD_SHOT_SLOT_MAX	(64)
#define	SMPD_PSTM_SLOT_MAX	(0)

/*
 *	���ۂɎg�p�ł���ő�|�[�g��
 *	MIDI
 *		MIDI�����͍ő唭�����Ɋ֌W�Ȃ�SDD_MIDI_PORT_MAX�|�[�g�g�p�o����D
 *	OneShot
 *		OneShot������1�X���b�g=1�|�[�g�Ȃ̂ōő唭����=�ő�|�[�g���ɂȂ�D
 *	PCM Stream
 *		PCM Stream������1�|�[�g�ɂ�4�X���b�g�܂Ŏg�p�ł���D�����1�|�[�g��
 *		��R�̃X���b�g�����蓖�Ă�قǓ����Ɏg�p�ł���ő�|�[�g�������邱�ƂɂȂ�
 *		�i�܂�g�p�@�ɂ����ł͂Ȃ��j
 */
#define	SMPD_MIDI_PORT_MAX	( SDD_MIDI_PORT_MAX)
#define	SMPD_SHOT_PORT_MAX	( SMPD_SHOT_SLOT_MAX)
#define	SMPD_PSTM_PORT_MAX	( SMPD_PSTM_SLOT_MAX)

Uint32	gSpeedCount;
Uint32	gFrameCount;
Uint32	gVintCount;
Bool	gNjUserMainLoopFlg;

SDSHOT	gShotHandle[ SMPD_SHOT_PORT_MAX];
Sint32	gShotHandleCounter;

Void	njUserInit( Void);
Sint32	njUserMain( Void);
Void	njUserFinish( Void);
Void	VintFunction( Void);
Void	Peripheral( Void);

/********************************************************************
 *
 *	�֐�	Sint32 njUserMain( Void);
 *	�@�\	�v���O�����̏����������D
 *	����	
 *	�Ԃ�l	
 *	���l	
 *
 */
Void	njUserInit( Void)
{
	Sint32	i;

	gFrameCount = 0;
	gVintCount = 0;
	gSpeedCount = 120;
	gNjUserMainLoopFlg = true;

    /* Check the cable for NTSC/PAL or VGA.. works properly for SCART. */
    switch (syCblCheck())
	{
        /* Initialize the display device and set the frame buffer based on the video mode. */
        case SYE_CBL_NTSC:  /* U.S./North America NTSC (60Hz) and Brazil PAL-M (60Hz). */
            sbInitSystem (NJD_RESOLUTION_640x480_NTSCNI, NJD_FRAMEBUFFER_MODE_RGB565, 1);
            break;
        case SYE_CBL_PAL:   /* Europe PAL (50Hz) and Argentina PAL-N (50Hz). */
            sbInitSystem (NJD_RESOLUTION_640x544_PALNI, NJD_FRAMEBUFFER_MODE_RGB565, 1);
            break;
        case SYE_CBL_VGA:   /* Standard VGA. */
            sbInitSystem (NJD_RESOLUTION_VGA, NJD_FRAMEBUFFER_MODE_RGB565, 1);
            break;
        default:
            syBtExit();     /* Unknown video type, return to Dreamcast BootROM. */
	}
    njSetBorderColor( 0x00000000 ) ;
    njInitVertexBuffer( 1000000, 0, 200000, 0, 0);
    njInitPrint(NULL, 0, 0);
    njPrintSize(14);
	njSetBackColor( 0x00000000, 0x00000000, 0x0000C060);

	for ( i = 0; i < SMPD_SHOT_PORT_MAX; i++)
		gShotHandle[ i] = NULL;
	gShotHandleCounter = 0;

	njSetVSyncFunction( VintFunction);

	/*
	 *	�T�E���h�V�X�e���̏�����
	 */
	/*	�T�E���h���C�u�����̏�����  */
	sdLibInit( NULL, 0, 0);

	/*	�T�E���h�h���C�o�t�@�C���̓ǂݍ���  */
	sdDrvDownloadFromFile( SMPD_SND_DRV_FILE_NAME, SDD_MEMBLK_LOCAL_ALLOCATE);
	/*	SDD_MEMBLK_LOCAL_ALLOCATE��API������syMalloc���g�p���܂��D���ӂ��Ă�������  */

	/*
	 *	�}���`���j�b�g�t�@�C���̓ǂݍ���
	 */
	/*	�}���`���j�b�g�t�@�C���̓ǂݍ���  */
	sdMultiUnitDownloadFromFile( SMPD_MLT_FILE_NAME, SDD_MEMBLK_LOCAL_ALLOCATE);
	/*	SDD_MEMBLK_LOCAL_ALLOCATE��API������syMalloc���g�p���܂��D���ӂ��Ă�������  */

	/*	�e�������Ƃ̍ő剹���̐ݒ�D�T�E���h�h���C�o�̃_�E�����[�h�̂��ƂɎ��s���Ȃ���΂Ȃ�Ȃ�  */
	sdSysSetSlotMax( SMPD_MIDI_SLOT_MAX, SMPD_SHOT_PORT_MAX, SMPD_PSTM_SLOT_MAX);

	for ( i = 0; i < SMPD_SHOT_PORT_MAX; i++)
		sdShotOpenPort( &gShotHandle[ i]);

}

/********************************************************************
 *
 *	�֐�	Sint32 njUserMain( Void);
 *	�@�\	�v���O�����̒��j�D
 *	����	
 *	�Ԃ�l	��Ƃ𑱂���Ȃ�NJD_USER_CONTINUE�D�I������Ȃ�NJD_USER_EXIT�D
 *	���l	
 *
 */
Sint32 njUserMain( Void)
{
	static Bool	flg = false;
	gFrameCount++;

	Peripheral();


	/*	gSpeedCount�l V-int ���ƂɍĐ�  */
	if ( ( gFrameCount % gSpeedCount) == 0){
		sdShotPlay( gShotHandle[ gShotHandleCounter], 0, 0, 0);
//		gShotHandleCounter++ = gShotHandleCounter % SMPD_SHOT_PORT_MAX;
		gShotHandleCounter++;
		gShotHandleCounter = gShotHandleCounter % SMPD_SHOT_PORT_MAX;
	}

/* Display Contents */
	njPrintC( NJM_LOCATION(  0, 1), "V-Int        :          (  :  :  )");
	njPrintH( NJM_LOCATION( 14, 1), gVintCount, 8);
	njPrintD( NJM_LOCATION( 25, 1), ( gVintCount / ( 60 * 60)) % 60, 2);
	njPrintD( NJM_LOCATION( 28, 1), ( gVintCount / 60        ) % 60, 2);
	njPrintD( NJM_LOCATION( 31, 1), ( gVintCount             ) % 60, 2);

	sdLibShowStat( 0, 2, 0xFFFFFFFF);
	sdDrvShowStat( 0, 3, 0xFFFFFFFF);

	njPrintC( NJM_LOCATION(  0, 7), "Shot Port Num:");
	njPrintH( NJM_LOCATION( 14, 7), gShotHandleCounter, 8);
	sdShotShowStat( gShotHandle[ gShotHandleCounter], 0, 8, 0xFFFFFFFF);

	njPrintC( NJM_LOCATION(  0, 16), "Shot Speed   :        v-int");
	njPrintD( NJM_LOCATION( 14, 16), gSpeedCount, 8);

	return gNjUserMainLoopFlg == true? NJD_USER_CONTINUE: NJD_USER_EXIT;
}

/********************************************************************
 *
 *	�֐�	Void	njUserExit( Void);
 *	�@�\	�v���O�����̏I���D
 *	����	
 *	�Ԃ�l	
 *	���l	
 *
 */
Void	njUserExit( Void)
{
	Sint32	i;

	for ( i = 0; i < SMPD_SHOT_PORT_MAX; i++)
		sdShotClosePort( gShotHandle[ i]);

	set_imask(15);
	sdSysFinish();
	njExitTexture();
	sbExitSystem();
}

/********************************************************************
 *
 *	�֐�	Void	VintFunction( Void);
 *	�@�\	V���荞�݂̏����D
 *	����	
 *	�Ԃ�l	
 *	���l	
 *
 */
Void	VintFunction( Void)
{
	gVintCount++;
	sdSysServer();
}

/********************************************************************
 *
 *	�֐�	Void	Peripheral( Void);
 *	�@�\	�p�b�h�ł̃R���g���[���D
 *	����	
 *	�Ԃ�l	
 *	���l	
 *
 */
Void	Peripheral( Void)
{
	PDS_PERIPHERAL	*per;

	per = pdGetPeripheral( NJD_PORT_A0);

	/*
	 *	[A][B][X][Y][START]�ł̃��Z�b�g
	 */
	if ( per->on & PDD_DGT_TA
	  && per->on & PDD_DGT_TB
	  && per->on & PDD_DGT_TX
	  && per->on & PDD_DGT_TY
	  && per->on & PDD_DGT_ST)
		gNjUserMainLoopFlg = false;

	if ( per->on & PDD_DGT_KU ){
		if(gSpeedCount <= 300){
			gSpeedCount++;
		}
	}
	if ( per->on & PDD_DGT_KD ){
		if(gSpeedCount > 0){
			gSpeedCount--;
		}
	}

}

/*EOF*/
