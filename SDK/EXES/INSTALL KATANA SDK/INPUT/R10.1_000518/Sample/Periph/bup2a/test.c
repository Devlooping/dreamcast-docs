/*
    Ninja Sample

    COPYRIGHT (C) SEGA ENTERSRISES,LTD.

    Backup Sample #2A

    1999.5.31
*/

#include <shinobi.h>
#include "backup.h"
#include <nindows.h>

extern Uint16 T009[];

#define OBJECT  object_tea_Teapot1_Teapot1
extern NJS_OBJECT OBJECT[];

NJS_TEXNAME texname[1];
NJS_TEXLIST tlist1 = {texname,1};
NJS_TEXINFO info;

extern NJS_MATRIX   matrix[8];
extern NJS_VERTEX_BUF   vbuf[4096];

NJS_TEXMEMLIST  tex[10];

NJS_VIEW    view;
NJS_LIGHT   light;

Sint32  xx,yy,zz = 0;


#define DRIVE 0

#define S_NOT_READY 0
#define S_READY     1
#define S_SAVE      2
#define S_LOAD      3
#define S_DELETE    4
#define S_COMPLETE  5

#define FNAME "SAVEDATA_001"
#define SAVEBUFFER SaveData
#define SAVEBLOCKS 4

Sint32 save_file(void);
Sint32 load_file(void);
Sint32 delete_file(void);

Sint32 gFrameCount;
Uint32 gState;

extern Uint8 SaveData[];

void DispBackupRAMInfo(Sint32 x, Sint32 y, Sint32 drive);
void InitTeapot(void);
void DrawTeapot(void);

void njUserInit(void)
{
	gFrameCount = 0;
	gState = 0;

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
    njInitVertexBuffer( 800000, 0, -1, 0, 0);
    njInitPrint(NULL, 0, 0);
    njPrintSize(14);
	njSetBackColor(0x00000000, 0x00000000, 0x0000C060);

	InitTeapot();

	BupInit();
}

void InitTeapot(void)
{
	njInitMatrix(matrix, 8, 0);
	njInit3D(vbuf, 4096);
	njInitView(&view);
	njCreateLight(&light, NJD_PHONG_DIR);
	njSetLightIntensity(&light, 1.5f, 1.0f, 0.7f);
	njSetLightAngle(&light, NJM_RAD_ANG(NJD_PI / 30.f),
										NJM_RAD_ANG(NJD_PI / 4.f));
	njSetLightDirection(&light, 0, 0.707f, -0.707f);
	njSetView(&view);

	njInitTexture(tex, 10);
	nwInitSystem(10, NJD_PORT_B0);
	njSetTextureInfo(&info, T009, NJD_TEXFMT_RGB_565 | NJD_TEXFMT_TWIDDLED,
							128, 128);
	njSetTextureName(&texname[0], &info, 0,
						NJD_TEXATTR_TYPE_MEMORY | NJD_TEXATTR_GLOBALINDEX);
	njLoadTexture(&tlist1);
	njSetTexture(&tlist1);

	njControl3D(NJD_CONTROL_3D_CONSTANT_ATTR);
	njSetConstantAttr(0xffffffff,
			NJD_FLAG_USE_TEXTURE | NJD_FLAG_USE_ENV | NJD_FILTER_BILINEAR);
}

Sint32 njUserMain(void)
{
	const PDS_PERIPHERAL* per;
	const BACKUPINFO *binfo;

	njPrintC(NJM_LOCATION(2, 2), "BACKUP SAMPLE #2A");
	njPrintH(NJM_LOCATION(30, 2), gFrameCount++, 8);

	per = pdGetPeripheral(PDD_PORT_A0);

	binfo = BupGetInfo(DRIVE);

	DispBackupRAMInfo(2, 22, 0);

	switch (gState) {
		case S_NOT_READY:
			njPrintC(NJM_LOCATION(4, 4), "CONNECT MEMORY CARD TO PORT A-1");
			if (binfo->Ready) gState = S_READY;
			break;
		case S_READY:
			njPrintC(NJM_LOCATION(4, 4), "PRESS A BUTTON TO SAVE FILE");
			njPrintC(NJM_LOCATION(4, 5), "PRESS B BUTTON TO LOAD FILE");
			njPrintC(NJM_LOCATION(4, 6), "PRESS X BUTTON TO DELETE FILE");
			if (per->press & NJD_DGT_TA) {
				BupSave(DRIVE, FNAME, SAVEBUFFER, SAVEBLOCKS);
				gState = S_SAVE;
			}
			if (per->press & NJD_DGT_TB) {
				BupLoad(DRIVE, FNAME, SAVEBUFFER);
				gState = S_LOAD;
			}
			if (per->press & NJD_DGT_TX) {
				BupDelete(DRIVE, FNAME);
				gState = S_DELETE;
			}
			if (!binfo->Ready) gState = S_NOT_READY;
			break;
		case S_SAVE:
			njPrint(NJM_LOCATION(4, 8), "NOW SAVING...(%03d%%)",
			binfo->ProgressCount * 100 / binfo->ProgressMax);
			if (buStat(DRIVE) == BUD_STAT_READY) {
				gState = S_COMPLETE;
			}
			break;
		case S_LOAD:
			njPrint(NJM_LOCATION(4, 8), "NOW LOADING...(%03d%%)",
			binfo->ProgressCount * 100 / binfo->ProgressMax);
			if (buStat(DRIVE) == BUD_STAT_READY) {
				gState = S_COMPLETE;
			}
			break;
		case S_DELETE:
			njPrint(NJM_LOCATION(4, 8), "NOW DELETING...(%03d%%)",
			binfo->ProgressCount * 100 / binfo->ProgressMax);
			if (buStat(DRIVE) == BUD_STAT_READY) {
				gState = S_COMPLETE;
			}
			break;
		case S_COMPLETE:
			njPrint(NJM_LOCATION(4, 9), "COMPLETE[%s]",
			BupGetErrorString(buGetLastError(DRIVE)));
			njPrintC(NJM_LOCATION(4, 10), "PRESS BUTTON");
			if (per->press) {
				gState = S_NOT_READY;
			}
			break;
		default:
			break;
	}


	DrawTeapot();

	return nwExecute();
}

void DrawTeapot(void)
{
	njClearMatrix();
	njTranslate(NULL, 0.f, 0.f, -12.f);
	njRotateXYZ(NULL, xx, yy, zz);
	njDrawObject(OBJECT);

	xx += 257;
	yy += 179;
	zz += 193;
}

void njUserExit(void)
{
	BupExit();

	nwExitSystem();
	sbExitSystem();
	syBtExit();
}


/*===============================================================*/
/* �������[�J�[�h�̏�Ԃ�\������                                */
/* �ڑ�����Ă��Ȃ���΁u---�v���A�t�H�[�}�b�g����Ă��Ȃ����   */
/* �uUNFORMAT�v���A�t�H�[�}�b�g����Ă���ꍇ�͋󂫗e�ʂƍő�    */
/* �e�ʂ�\�����܂��B                                            */
/* Display information of memory card.                           */
/* If memory card is not connected, "---" is displayed.          */
/* If memory card is not formatted, "UNFORMAT", is displayed.    */
/* if memory card is formatted, free volume and maximum volume   */
/* is displayed.                                                 */
/*===============================================================*/


void DispBackupRAMInfo(Sint32 x, Sint32 y, Sint32 drive)
{
	const BACKUPINFO* binfo;
	BUS_DISKINFO* diskinfo;

	binfo = BupGetInfo(drive);
	diskinfo = &binfo->DiskInfo;

	njPrint(NJM_LOCATION(x, y), "MEMORY CARD #%d",drive);

	if (binfo->Ready) {
		if (binfo->IsFormat) {
			njPrint(NJM_LOCATION(x + 15, y), "%04d/%04d BLOCKS",
			diskinfo->free_user_blocks,
			diskinfo->total_user_blocks);
		} else {
			njPrint(NJM_LOCATION(x + 15, y), "UNFORMAT        ");
		}
	} else {
		if (binfo->Operation == BUD_OP_MOUNT) {
			njPrint(NJM_LOCATION(x + 15, y), "CHECKING...     ");
		} else {
			njPrint(NJM_LOCATION(x + 15, y), "----------------");
		}
	}
}


