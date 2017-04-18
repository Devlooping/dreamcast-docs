/*
 *
 *		Middleware Player Sample
 *
 */

/***************************************************************************
 *      �C���N���[�h�t�@�C��
 *      Include file
 ***************************************************************************/
#include <shinobi.h>
#include <sg_sd.h>
#include <sg_mw.h>
#include <ginsu.h>


/***************************************************************************
 *      �萔�}�N��
 *      MACRO CONSTANT
 ***************************************************************************/

/*	�\�����[�h		*/
/*	Display mode	*/
#define SYS_MODE		NJD_RESOLUTION_640x480_NTSCI
#define SYS_FRAME		NJD_FRAMEBUFFER_MODE_ARGB8888
#define SYS_COUNT		1

/*	���_�o�b�t�@�T�C�Y(1���}�C�i�X�ɂ����2V���[�e���V���[�h)			*/
/*	Vertex buffer size(2V latency mode is when a valiable is minus.)	*/
#define SFD_VB_OP		-500000
#define SFD_VB_OM		0
#define SFD_VB_TP		20000
#define SFD_VB_TM		0
#define SFD_VB_PT		0

#define TM_VB_OP		100000
#define TM_VB_OM		0
#define TM_VB_TP		100000
#define TM_VB_TM		0
#define TM_VB_PT		0

/*
 * �����Đ��̎w��
 *   ON   :�������蓮��t�@�C���Ȃ�Ή����Đ����s���B�f�t�H���g�B
 *   OFF  :�����Đ���}�~����B�������蓮��t�@�C���ł��f���������Đ��B
 * Setting play audio
 *   ON   :Play audio when the animation file is interleaved audio(default)
 *   OFF  :Not play audio
 */
#define AUDIO_SW		ON

/*	NINJA			*/
#define	TEX_NUM			100
#define	TEXNAME_NUM		2
#define	TEX_BUFSIZE		0x20800

/*	�G���[���b�Z�[�W��		*/
/*	Length of error message	*/
#define ERRMSG_LEN		256

/*	�I�[�f�B�I�Đ��p�o�b�t�@�T�C�Y		*/
/*	Buffer size for play Audio Decoder	*/
#define	AUDIO_BUF_SIZE	MWD_PLY_CALC_AWORK(MWD_PLY_MIN_AWORK)
#define	MPA_BUF_SIZE	AUDIO_BUF_SIZE
#define WAV_BUF_SIZE	AUDIO_BUF_SIZE


/***************************************************************************
 *      �����}�N��
 *      Process MACRO
 ***************************************************************************/

/*	�t���OON/OFF�̔��]		*/
/*	Turn over flag(ON/OFF)	*/
#define NOT_FLAG(flag)		(((flag) == OFF) ? ON : OFF)


/***************************************************************************
 *      �ϐ���`
 *		Variable Declaration
 ***************************************************************************/

/*	�G���[������		*/
/*	The number of error	*/
Sint32 errcnt = 0;

/*	�G���[���b�Z�[�W	*/
/*	Error message		*/
char errmsg[ERRMSG_LEN] = "(OK)";

/*	������\���t���O				*/
/*	Flag for a show of character	*/
Sint32 disp_flag = ON;

/*	�|�[�Y���		*/
/*	Pause status	*/
Sint32 pause = OFF;

/*	NINJA				*/
NJS_TEXMEMLIST	tex[TEX_NUM];
NJS_TEXINFO		info[TEXNAME_NUM];
NJS_TEXNAME		texname[TEXNAME_NUM];
NJS_TEXLIST		texlist = {texname, TEXNAME_NUM};
Sint8			texbuf[TEX_BUFSIZE];
Uint16			framebuf[1024*512*2];

/*	WAVE�Đ��p�o�b�t�@			*/
/*	Buffer for play WAVE file	*/
/* char play_wav_buf[WAV_BUF_SIZE]; */


/***************************************************************************
 *      �֐���`
 *      Function Declaration
 ***************************************************************************/

/***
 *		�T�E���h���̏�����
 *		Initialize sound section
 ***/

/*	�T�E���h�h���C�o�̓]���E������					*/
/*	Initialize sound library and load sound driver	*/
void SoundInit(void *sndptr, void *mltptr)
{
	SDMEMBLK	memblk = NULL;

	/*	Initialize Sound library	 */
	sdLibInit(NULL, 0, 0);		/*	This api's argument is dummy now.  */

	/* 	Download and Initialize Sound driver.	 */
	/*	Create Memory block handle  */
	sdMemBlkCreate(&memblk);
	/*  Setting Memory block handle  */
	sdMemBlkSetPrm(memblk, sndptr, 0, SDD_MEMBLK_SYNC_FUNC, NULL);
	/*	Initialize Sound driver  */
	sdDrvInit(memblk);

	/*	Download and Initialize Multi unit	*/
	/*	Setting Memory block handle	*/
	sdMemBlkSetPrm(memblk, mltptr, 0, SDD_MEMBLK_SYNC_FUNC, NULL);
	sdDownloadMultiUnit(memblk);

	sdMemBlkDestroy(memblk);

	/*	Truemotion Only CPU mode	*/
#if	0
	/*	�]�����[�h�̐ݒ�		*/
	/*	Setting transfer mode	*/
	sdMemBlkSetTransferMode(SDE_MEMBLK_TRANSFER_MODE_DMA);
#endif

	return;
}

/***
 *			���[�e�B���e�B�֐�
 *			Utility function
 ***/

/*	�f�[�^�̓ǂݍ���	*/
/*	Load data to memory	*/
void *LoadFile(char *fname, long *len)
{
	GDFS			gdfs;
	long			fsize, nsct;
	void			*dat;

	gdfs = gdFsOpen(fname, NULL);
	if (gdfs == NULL) {
		for (;;) ;
	}
	gdFsGetFileSize(gdfs, &fsize);
	nsct = (fsize + 2047) / 2048;
	dat = syMalloc(nsct * 2048);
	if (dat == NULL) {
		for (;;) ;
	}
	gdFsRead(gdfs, nsct, dat);
	gdFsClose(gdfs);
	if (len != NULL) {
		*len = fsize;
	}

	return dat;
}

/*	msec -> time(hour,min,sec,frame)	*/
void Msec2Time(long mstime, long *hh, long *mm, long *ss, long *ff)
{
	*hh = mstime / (60*60*1000);
	*mm = (mstime - *hh*60*60*1000) / (60*1000);
	*ss = (mstime - *hh*60*60*1000 - *mm*60*1000) / 1000;
	*ff = mstime % 1000;

	return;
}

/***
 *			�~�h���E�F�A�̍Đ�
 *			Playing middleware
 ***/

/*	�~�h���E�F�A�̃G���[�������ɋN������֐�			*/
/*	Callback function when an error occur in middleware	*/
void UsrMwErrFunc(void *obj, char *ecode)
{
	errcnt++;
	strncpy(errmsg, ecode, ERRMSG_LEN);

	return;
}

/*	���̕\��			*/
/*	Display infomation	*/
void DispInfo(MWPLY ply, char *fname)
{
//	long			time, tunit, hh, mm, ss, ff, time_ms;
//	float			timef;
//
//	mwPlyGetTime(ply, &time, &tunit);
//	timef = ((float)time / (float)tunit) * 1000.0;
//	time_ms = (long)timef;
//	Msec2Time(time_ms, &hh, &mm, &ss, &ff);
//	ff = ff*30/1000;
//	njPrint(NJM_LOCATION(0, 3), "File: %s  Time:%02d:%02d:%02d.%02d", 
//							fname, hh, mm, ss, ff);
//	njPrint(NJM_LOCATION(0, 4), "Error%2d:%s", errcnt, errmsg);
//
	return;
}

/*	�~�h���E�F�A�̍Đ�	*/
/*	Playing middleware	*/
void play_main(MWPLY ply, char *fname)
{
	MWE_PLY_STAT			stat;
	const PDS_PERIPHERAL	*per;

	mwPlyEntryErrFunc((void *)UsrMwErrFunc, &ply);
	mwPlyPause(ply, pause);				/*	�O��Đ��̃|�[�Y��Ԃ��ێ�		*/
										/*	Keep the last time pause status	*/
	mwPlyStartFname(ply, fname);		/*	�Đ��J�n						*/
										/*	Start to play middleware		*/
	while (1) {
		njWaitVSync();
		mwPlyStartFrame();
		per = pdGetPeripheral(PDD_PORT_A0);
		if (per->press & PDD_DGT_TA) {
			disp_flag = NOT_FLAG(disp_flag);
		}
		if (per->press & PDD_DGT_TX) {
			pause = ON;
			mwPlyPause(ply, ON);		/*	�ꎞ��~				*/
										/*	Pause					*/
		}
		if (per->press & PDD_DGT_TY) {
			pause = OFF;
			mwPlyPause(ply, OFF);		/*	�ꎞ��~����			*/
										/*	Release pause status	*/
		}
		if (per->press & PDD_DGT_ST) {
			break;
		}
		stat = mwPlyGetStat(ply);
		if ((stat == MWE_PLY_STAT_PLAYEND) || (stat == MWE_PLY_STAT_ERROR)) {
			break;
		}
		if (disp_flag == ON) {
			DispInfo(ply, fname);
		}
		mwPlyExecServer();
	}
	mwPlyStop(ply);						/*	�Đ���~ 					*/
										/*	Stop to play middleware		*/
	njWaitVSync();						/*	�p�b�h�̏�Ԏ擾�̂���		*/
										/*	for getting status of PAD	*/
	mwPlyDestroy(ply);					/*	�n���h������				*/
										/*	Destroy handle				*/

	return;
}

/***
 *			MPEG Sofdec �̍Đ�
 *			Play MPEG Sofdec
 ***/

/*	SFD�t�@�C���̍Đ�	*/
/*	Playing SFD file	*/
void play_sfd(char *fname)
{
	MWPLY					ply;
	MWS_PLY_CPRM_SFD		cprm;

	/*	�n���h���̐���	*/
	/*	Create handle	*/
	memset(&cprm, 0, sizeof(cprm));	/*	�\�񃁃��o�̃[���ݒ�̂��߂ɕK�v	*/
									/*	To initialize reserved member		*/
	cprm.ftype			= MWD_PLY_FTYPE_SFD;
	cprm.dtype			= MWD_PLY_DTYPE_AUTO;
	cprm.max_bps = 450*1024*8;
	cprm.max_width = 320;
	cprm.max_height = 480;
	cprm.nfrm_pool_wk = 3;
	cprm.wksize = mwPlyCalcWorkSofdec(
								cprm.ftype,
								cprm.max_bps,
								cprm.max_width,
								cprm.max_height,
								cprm.nfrm_pool_wk);
	cprm.work = syMalloc(cprm.wksize);
	ply = mwPlyCreateSofdec(&cprm);
	if (ply == NULL) {
		for (;;) ;
	}
	mwPlySetAudioSw(ply, AUDIO_SW);
	play_main(ply, fname);
	syFree(cprm.work);

	return;
}

/***
 *			TrueMotion �̍Đ�
 *			Play TrueMotion
 ***/

/*	AVI�t�@�C���̍Đ�	*/
/*	Playing AVI file	*/
void play_tm(char *fname)
{
	MWPLY				ply;
	MWS_PLY_CPRM_TM		cprm;

	/*	NINJA������			*/
	/*	Initialize NINJA	*/
	njInitVertexBuffer(TM_VB_OP, TM_VB_OM, TM_VB_TP, TM_VB_TM, TM_VB_PT);
	njInitPrint(NULL, 0, NJD_TEXFMT_ARGB_8888);
	njPrintSize(12);
	njInitTexture(tex, TEX_NUM);
	njInitTextureBuffer(texbuf, TEX_BUFSIZE);
	njSetTextureInfo(&info[0], framebuf,
					(NJD_TEXFMT_YUV_422 | NJD_TEXFMT_STRIDE), 1024, 512);
	njSetTextureInfo(&info[1], framebuf + 1024 * 512,
					(NJD_TEXFMT_YUV_422 | NJD_TEXFMT_STRIDE), 1024, 512);
	njSetTextureName(&texname[0], &info[0], 0,
					(NJD_TEXATTR_TYPE_MEMORY | NJD_TEXATTR_GLOBALINDEX));
	njSetTextureName(&texname[1], &info[1], 1,
					(NJD_TEXATTR_TYPE_MEMORY | NJD_TEXATTR_GLOBALINDEX));
	njLoadTexture(&texlist);
	njSetTexture(&texlist);

	/*	�n���h���̐���	*/
	/*	Create handle	*/
	ply = mwPlyCreateTM(&cprm);
	if (ply == NULL) {
		for (;;) ;
	}
	play_main(ply, fname);

	/*	NINJA���		*/
	/*	Finish NINJA	*/
	njReleaseTextureAll();
	njExitTexture();

	/*	NINJA������			*/
	/*	Initialize NINJA	*/
	njInitVertexBuffer(SFD_VB_OP, SFD_VB_OM, SFD_VB_TP, SFD_VB_TM, SFD_VB_PT);
	njInitPrint(NULL, 0, NJD_TEXFMT_ARGB_8888);
	njPrintSize(12);

	return;
}

/***
 *			MPEG/Audio �̍Đ�
 *			Play MPEG/Audio
 ***/

/*	MP2�t�@�C���̍Đ�	*/
/*	Playing MP2 file	*/
void play_mpa(char *fname)
{
	MWPLY			ply;
	MWS_PLY_CPRM	cprm;

	/*	�n���h���̐���	*/
	/*	Create handle	*/
	cprm.buf     = syMalloc(MPA_BUF_SIZE);
	cprm.size    = MPA_BUF_SIZE;
	cprm.libwork = syMalloc(MWD_MPA_CALC_WORK(1));
	ply = mwPlyCreateMpa(&cprm);
	if (ply == NULL) {
		for (;;) ;
	}
	play_main(ply, fname);
	syFree(cprm.libwork);
	syFree(cprm.buf);

	return;
}

/***
 *			WAVE�̍Đ�
 *			Play WAVE
 ***/

/*	WAVE�t�@�C���̍Đ�	*/
/*	Playing WAVE file	*/
void play_wav(char *fname)
{
	MWPLY			ply;
	MWS_PLY_CPRM	cprm;

	/*	�n���h���̐���	*/
	/*	Create handle	*/
	cprm.buf     = syMalloc(WAV_BUF_SIZE);
	cprm.size    = WAV_BUF_SIZE;
	cprm.libwork = NULL;
	ply = mwPlyCreateWav(&cprm);
	if (ply == NULL) {
		for (;;) ;
	}
	play_main(ply, fname);
	syFree(cprm.buf);

	return;
}

/***
 *			���C����
 *			MAIN
 ***/

/*	�A�v���P�[�V�����I������	*/
/*	Finalize application		*/
void UsrExit(void)
{
	/*	���C�u�����I������					*/
	/*	Finalize Middleware					*/
	mwPlyFinishSofdec();
	mwPlyFinishTM();
	mwPlyFinishMpa();
	mwPlyFinishWav();

	sbExitSystem();			/*	�E���C�u�����̏I������				*/
							/*	Finalize SHINOBI					*/
//	syBtExit();				/*	�V���v���v���C���փW�����v			*/
							/*	Jump to Simple player				*/
}

/*	GD�t�@�C���V�X�e���̃G���[�������ɋN������֐�			*/
/*	Callback function when an error occur in GD file system	*/
void UsrGdErrFunc(void *obj, Sint32 errcode)
{
	if (errcode == GDD_ERR_TRAYOPEND || errcode == GDD_ERR_UNITATTENT) {
		UsrExit();					/*	�A�v���P�[�V�����I������	*/
									/*	Finalize application		*/
	}

	return;
}

/*	���[�U��V-SYNC���荞�݂ɓo�^����֐�	*/
/*	Callback function when V-sync interrupt	*/
void UsrVsyncFunc(void)
{
	Sint32 dstat;

    /*	�T�E���h���C�u�����̃T�[�o�֐�		*/
	/*	Server function of sound library	*/
	sdSysServer();

	/*	�h�A�I�[�v���`�F�b�N	*/
	/*	Check door open			*/
	dstat = gdFsGetDrvStat();
	if (dstat == GDD_DRVSTAT_OPEN || dstat == GDD_DRVSTAT_BUSY) {
		gdFsReqDrvStat();
	}

	return;
}

void main(void)
{
	MWS_PLY_INIT_SFD		iprm;
	void 					*snddrv;
	void					*mltfile;
	const NJS_PERIPHERAL	*per;

#ifdef __GNUC__
	shinobi_workaround();
#endif
    gsInit();
    
	/*	�~�h���E�F�A�̂��߂̃V�X�e�������� (sbInitSystem�̑O�ɒu��)	*/
	/*	Initialize system for middleware(call before sbInitSystem)	*/
	mwPlyPreInitSofdec();

	/*	�E���C�u�����̏�����	*/
	/*	Initialize SHINOBI		*/
    njSetBorderColor(0x00000000);
    
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


#if 0
// this line is needed if DDS is not used, ie GDFS version < 1.06
    gdFsChangeDir(gsGetBasePath());
#endif    
    
	njInitVertexBuffer(SFD_VB_OP, SFD_VB_OM, SFD_VB_TP, SFD_VB_TM, SFD_VB_PT);
	njInitPrint(NULL, 0, NJD_TEXFMT_ARGB_8888);
	njPrintSize(12);
	/*	GD�t�@�C���V�X�e���G���[�R�[���o�b�N�֐��̓o�^	*/
	/*	Entry callback function of GD file system error	*/
	gdFsEntryErrFuncAll((void *)UsrGdErrFunc, NULL);

	/*	�T�E���h���C�u�����̏���	*/
	/*	Prepare sound library		*/
	snddrv  = LoadFile("manatee.drv", NULL);
	mltfile = LoadFile("stream.mlt", NULL);
	SoundInit(snddrv, mltfile);

	njSetVSyncFunction(UsrVsyncFunc);

	/*	�~�h���E�F�A���C�u����������	*/
	/*	Initialize middleware			*/
	memset(&iprm, 0, sizeof(iprm));	/*	�\�񃁃��o�̃[���ݒ�̂��߂ɕK�v	*/
									/*	To initialize reserved member		*/

	iprm.mode		= SYS_MODE;
	iprm.frame		= SYS_FRAME;
	iprm.count		= SYS_COUNT;
	iprm.latency	= MWD_PLY_LATENCY(SFD_VB_OP, SFD_VB_OM,
											SFD_VB_TP, SFD_VB_TM, SFD_VB_PT);
	mwPlyInitSofdec(&iprm);
	mwPlyInitTM();
	mwPlyInitMpa(MWD_PLY_SVR_VSYNC);
	mwPlyInitWav(MWD_PLY_SVR_VSYNC);

    if (gsGetArgC()==3)
    {
        if (!strcmp("MPEG",gsGetArgV(2)))
        {
        	play_sfd(gsGetArgV(1));
        }
    
        if (!strcmp("DUCK",gsGetArgV(2)))
        {
        	play_tm(gsGetArgV(1));
        }
    }
    
//	for (;;) {
//		per = njGetPeripheral(NJD_PORT_A0);
//		njPrint(NJM_LOCATION(15, 3), 
//					"Middleware Player Sample No.1");
//		njPrint(NJM_LOCATION(5,  6), "A     : Play MPEG Sofdec");
//		njPrint(NJM_LOCATION(5,  8), "B     : Play TrueMotion");
//		njPrint(NJM_LOCATION(5, 10), "X     : Play MPEG/Audio");
//		njPrint(NJM_LOCATION(5, 12), "Y     : Play WAVE");
//		njPrint(NJM_LOCATION(5, 14), "START : Stop");
//		if (per->press & NJD_DGT_TA) {
//			play_sfd("SAMPLE.SFD");
//		}
//		if (per->press & NJD_DGT_TB) {
//			play_tm("SAMPLE.AVI");
//		}
//		if (per->press & NJD_DGT_TX) {
//			play_mpa("SAMPLE.MP2");
//		}
//		if (per->press & NJD_DGT_TY) {
//			play_wav("SAMPLE.WAV");
//		}
//		njWaitVSync();
//	}

	UsrExit();						/*	�A�v���P�[�V�����I������	*/
									/*	Finalize application		*/
    gsExit();
}


/* end of file */
