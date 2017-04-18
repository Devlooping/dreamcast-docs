/******************************************************************************
 *
 * name:    sock_test.c
 *
 * purpose: SOCKET LIB TEST
 *
 * socket library��test routine�ł���B
 * ��sock_test1.c�`sock_test19.c��sock_test.c�Ƃ���
 * ���������B 1998.11.14 by kiryu
 *
 *
 *      Copyright (C) 1993-1998 by ACCESS CO.,LTD.
 *
 *****************************************************************************/


#include <types.h>
#include <socket.h>
#include <sock_errno.h>
#include "usrUtl.h"


/* �ŗL�w�b�_�̈��p */
#include <aveppp.h>         /* ppp��API��`����荞�� */

#include <vsyncpro.h>       /* �Ȉ�V-SYNC�X���b�h�}�l�[�W���̃w�b�_ */

/* �W���w�b�_�̈��p */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <shinobi.h>

#include <dns.h>            /* DNS��API��`����荞�� */

void DebugPrint(char *in_format, ...);  /* From dbg.c. */


/******************************************************************************
 * �{�t�@�C�����ŋ��L�����ϐ��̒�`�Z�N�V����
 *****************************************************************************/

/* ���O�C�����邽�߂̃X�N���v�g */
static const char login_script[] =
    "send\"AT&F\\r\"\n"         /* 'AT&F'�𑗂��čH��o�׎��̐ݒ�ɖ߂� */
    "wait\"OK\\r\\n\",5\n"      /* 'OK'���Ԃ��Ă���̂�҂� */
    "send \"ATX3&C1&D2\\r\"\n"  /* 'ATX3&C1&D2'�𑗂� */
    "wait\"OK\\r\\n\",5\n";     /* 'OK'���Ԃ��Ă���̂�҂� */

/* �_�C�A�����O�̂��߂̃X�N���v�g */
static const char dialing_script[] =
    "dial\"%t\"\n";             /* �d�b�ԍ����_�C�A������ */

/* ���O�C���Ŏg����d�b�ԍ� */
static char telephon_no[64];

/* ���O�C���Ŏg���郆�[�U�[�� */
static char login_user[64];

/* ���O�C���Ŏg����p�X���[�h */
static char login_password[64];

/* DNS�̃h���C���� */
static wave_char DnsName[128];

/* DNS�T�[�o��IP�A�h���X */
static wave_32bit PPP_DnsServer1 = 0;


/******************************************************************************
 * �{�t�@�C���Ń��[�J����TCP/IP�Ăяo���֐��̒�`�Z�N�V����
 *****************************************************************************/

/* TCP/IP�v���g�R���X�^�b�N������������ */
static short avetcp_initialize(void)
{
    AT_init(0);
    return 0;
}


/* �v���g�R���X�^�b�N���I������ */
void avetcp_terminate(void)
{
    AT_disp(0);
}


/******************************************************************************
 * PPP���g���֐��̒�`�Z�N�V����
 *****************************************************************************/

/* �V���A���̒ʐM�d�l�����肷�� */
static void
setup_serial_spec(AvepppSerialParam *serial)
{
    serial->speed         = 5;    // speed 0-6 : 2400,4800,9600,19200,38400,57600(,115200)
    serial->stop          = 0;    // stop bit 0:1bit, 1:2bit
    serial->parity        = 2;    // parity 0:even, 1:odd, 2:none
    serial->flow          = 2;    // flow control 0:non, 1:RTS, 2:DTR
    serial->databit       = 0;    // data size 0:8bit, 1:7bit
}

/* �d�b�ԍ��d�l�����肷�� */
static void
setup_telephone_spec(AvepppTelephoneParam *telephone)
{
    telephone->dialtype       = 0;        // Dial type 0:TONE, 1:PULSE
    telephone->outside_line   = 0;        // Use Outside line (no)
    telephone->timeout        = 60;       // timeout
    telephone->dial_retry     = 0;        // redial tries (none)
    telephone->dial_interval  = 30;       // redial interval
    telephone->outside_number = UUGetOutsideLine();       // outside line number ("0,")
}

/* PPP�̐ڑ��d�l�����肷�� */
static void
setup_connection_spec(AvepppConnectionParam *pppconection)
{
    pppconection->recognize        = 4;            // user authorization, 0:PAP,1:chat,2:CHAP
    pppconection->mru              = 1500;         // maximum receiving bytes
    pppconection->magic_number     = 0x12345678;   // magic number
    pppconection->acfcomp          = 0;            // HDLC header compression (enable)
    pppconection->protocomp        = 0;            // PPP header compression (enable)
    pppconection->vjcomp           = 0;            // IPCP IP-TCP header compression (enable)
}

/* PPP�̃I�[�v���p�����[�^��ݒ肷�� */
static void
setup_ppp(AvepppOpenParam *iparam)
{
    /* Call function to ISP information. */
    UUInitISPInfo(UU_ACCESS);

    //strcpy(telephon_no, "5552112");
    //strcpy(login_user, "sega");
    //strcpy(login_password, "dreamcast");

    iparam->login_script1 = (char *) login_script;
    iparam->login_script2 = (char *) dialing_script;
    iparam->telephone_number = (char *) UUGetPrimaryNumber();
    iparam->login = (char *) UUGetLoginID();
    iparam->password = (char *) UUGetPassword();
}

/* �v���g�R�����������ĉ��(PPP�w)��ڑ����� */
static int
dialing(void)
{
    AvepppOpenParam iparam;         /* �I�[�v���̃p�����[�^ */

    AvepppSerialParam serpara;      /* �V���A���̐ݒ� */
    AvepppTelephoneParam telepara;  /* �d�b�ԍ��̐ݒ� */
    AvepppConnectionParam cnepara;  /* �ڑ��̐ݒ� */

    AvepppStatus statpara;          /* PPP�̃X�e�[�^�X */

    /* �V���A���̐ݒ���쐬 */
    setup_serial_spec(&serpara);

    /* �d�b�ԍ��̐ݒ���쐬 */
    setup_telephone_spec(&telepara);

    /* �ڑ��̐ݒ���쐬 */
    setup_connection_spec(&cnepara);

    /* PPP�̃I�[�v���p�����[�^���쐬 */
    iparam.serial = &serpara;
    iparam.telephone = &telepara;
    iparam.pppconection = &cnepara;
    setup_ppp(&iparam);

    /* PPP������������ */
    AvepppInitialize();

    /* TCP/IP������������ */
    avetcp_initialize();

    /* DNS�̐ݒ� */
    strcpy(DnsName, "");
    PPP_DnsServer1 = ((202 << 0) + (232 << 8) + (2 << 16) + (38 << 24));

    /* DNS������������ */
    ADNS_Initialize(DnsName, PPP_DnsServer1, PPP_DnsServer1);

    /* PPP��ڑ�����(�_�C�A�����O���J�n�����) */
    AvepppOpen(&iparam);

    /* PPP�̐ڑ������܂ő҂� */
    for (;;)
    {
        vsWaitVSync(100);

        /* PPP�̏�Ԃ𓾂� */
        AvepppGetStatus(&statpara);

        /* �ڑ������Ȃ�΃��[�v��E�o */
        if (statpara.phase == AvepppConnectEstablished) break;

        /* �ڑ��Ɏ��s������G���[������ */
        if (statpara.phase == AvepppConnectFail
            || statpara.phase == AvepppConnectDisconnected) goto error;
    }
    return 0;

error:
    return -1;
}

/* PPP�̐ؒf���� */
void
disconnect(void)
{
    AvepppStatus statpara;

    /* PPP��ؒf������ */
    AvepppClose(0);

    /* PPP�̐ؒf���m�F�����܂ő҂�*/
    for (;;)
    {
        vsWaitVSync(100);

        /* PPP�̏�Ԃ𓾂āA�ؒf��ԂɂȂ����烋�[�v��E�o */
        AvepppGetStatus(&statpara);
        if (statpara.phase == AvepppConnectUnused) break;
    }

    /* DNS�̏I���������s�� */
    ADNS_Finalize();

    /* TCP�̏I���������s�� */
    avetcp_terminate();

    /* PPP�̏I���������s�� */
    AvepppTerminate();
}


/******************************************************************************
 * �E���C�u�����Ƃ̃C���^�[�t�F�[�X
 *****************************************************************************/

/* �E���C�u�����̏����� */
void njUserInit(void)
{
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
    njSetBackColor( 0xff0000ff,0xff0000ff,0xff000000 );
    njInitVertexBuffer(800000, 0, 200000, 0, 0);
    njInitPrint(NULL, 0, 0);
	njPrintSize(16);
}

/* �E���C�u�����̏I������ */
void njUserExit(void)
{
    njExitPrint();

    sbExitSystem();

    /* �߂��Ă����Ƃ��ɔ����Ė������[�v�Œ�~����R�[�h��u�� */
    for (;;)
        ;
}

/******************************************************************************
 * �O�����J����֐��̒�`�Z�N�V����
 *****************************************************************************/

/*extern void sock_test1(void);*/

/* �e�X�g�̃��C�����[�`�� */
Sint32 njUserMain(void)
{
    int i;
    extern void debug();
    extern void td_main();

    unsigned int    count1;
    unsigned int    count2;
    unsigned int    count;
    unsigned int    micro;

    void *vSyncCallbackProc(void);

    /* Serial debug interface. */
    dbInit();

    /*vsInitVSyncProc(0);*/
    vsInitVSyncProc(vSyncCallbackProc);

    DebugPrint ("\n*** START TEST PROGRAM ***\n");

    vsWaitVSync(1);

    /*return 0;*/

    /*debug();*/

    /* �_�C�A�����O����PPP��ڑ���Ԃɂ��� */
    if (dialing() < 0)
    {
        DebugPrint ("\n*** Error occur: can't connect PPP\n");
        goto error;
    }
    DebugPrint ("\n*** PPP CONNECTED ***\n");

    /* socket���C�u����test */

    DebugPrint ("\n");

    debug();
    /*td_main();*/

    DebugPrint ("\n*** TCP/IP TESTED ***\n");

 error:
    /* PPP�̐ڑ������� */
    disconnect();

    DebugPrint ("\n*** PPP DIS-CONNECTED ***\n");

    return NJD_USER_EXIT;
}

/*
 * PPP/TCP���u���b�N�������s���Ă���Ƃ��Ăяo������
 */
void *vSyncCallbackProc(void)
{
    unsigned int    count;
    SYS_RTC_DATE    date;

    /* �^�C�}�\�� */
    count = syTmrGetCount();
    njPrint(NJM_LOCATION(4, 8), "Timer is %08x", count);

    njPrintC(NJM_LOCATION(4,10),"Connect Set5 serial output to PC");
    njPrintC(NJM_LOCATION(4,11),"to control socket test remotely.");

    njPrintC(NJM_LOCATION(4,13),"Set TERMINAL to 57600 bps");
    njPrintC(NJM_LOCATION(4,14),"  (8-N-1 HW Flow)");
    njPrintC(NJM_LOCATION(4,16),"Type TEST when connected.");

    /* ���ݎ����\�� */
    syRtcGetDate(&date);
    njPrint(NJM_LOCATION(4, 18), "%4d/%02d/%02d %2d:%02d:%02d",
        date.year, date.month, date.day, date.hour, date.minute,
        date.second);

    njWaitVSync();

}