/*
 *  debug.c
 *  Copyright 1997,98 ACCESS CO.,LTD.
 *  All rights reserved.
 */

#include    <shinobi.h>

#include    <stdio.h>
#include    <stdarg.h>

/*
 * SH4 SCIF(FIFO�����V���A���E�R�~���j�P�[�V�����E�C���^�[�t�F�C�X)
 * �V���A���E�f�o�b�O���^�o�̓h���C�o
 *
 */

#define CR      0x0d                /* �L�����b�W�E���^�[�� */
#define LF      0x0a                /* ���C���E�t�B�[�h */
#define BS      0x08                /* �o�b�N�E�X�y�[�X */

void DebugPrint(char *in_format, ...);
void debug(void);
void command_exec(char *msg);

static void set_date(char *msg);
static void date(void);

#define MAX_MESSAGE     128
char            msg[MAX_MESSAGE];
void debug(void)
{
    char            rcv_data;
    char            send_data[3];
    int             i;

    send_data[1] = 0;

    i = 0;

    DebugPrint("\r\nDebug TASK Ver. 1.00 start !!!");
    DebugPrint("\r\nCopyright (C) 1998 ACCESS CO., LTD.");
    DebugPrint("\r\n> ");
    while (1) {
        if (scif_isget()) {                     /* ��M�f�[�^����? */
            if (i < MAX_MESSAGE) {
                msg[i] = (char)scif_get();      /* ��M�f�[�^�R�s�[ */
                rcv_data = msg[i];              /* ���b�Z�[�W�i�[ */
                switch (rcv_data) {
                    case (int)CR: {
                        msg[i] = 0;
                        DebugPrint("\r\n");     /* �G�R�[�o�b�N */
                        command_exec(msg);
                        send_data[0] = 0;
                        send_data[1] = 0;
                        i = 0;
                        break;
                    }
                    case (int)BS: {
                        if (i > 0) {
                            send_data[0] = BS;
                            send_data[1] = ' ';
                            send_data[2] = BS;
                            send_data[3] = 0;
                            DebugPrint(send_data);
                            send_data[0] = 0;
                            send_data[1] = 0;
                            send_data[2] = 0;
                            send_data[3] = 0;
                            i--;
                        }
                        break;
                    }
                    default: {
                        send_data[0] = rcv_data;
                        DebugPrint(send_data);          /* �G�R�[�o�b�N */
                        i++;
                    }
                }
            }
        }
        else {
            /* �Ȃɂ����Ȃ� */
            vsWaitVSync(1);
        }
    }
}

void command_exec(char *msg)
{
    int     ret;
    extern void test(void);

    if (strcmp(msg, "test") == 0) {
        test();
    }

    if (strcmp(msg, "date") == 0) {
        date();
    }

    else if (strncmp(msg, "date 1", 6) == 0) {
        set_date(msg);
    }

    else {
        DebugPrint("not found\r\n");
    }
    DebugPrint("> ");
}

/******************************************************************************
 * �{�t�@�C���Ń��[�J���ȃf�o�b�O�p�֐��̒�`�Z�N�V����
 *****************************************************************************/

/* �w�肳�ꂽin_format�̏����w��ɏ]���āA���̌�ɂ���ψ����̃p�����[
   �^��]�����ăV���A���|�[�g�ɏo�͂��� */
void
DebugPrint(char *in_format, ...)
{
    static char msg[8192];
    va_list ap;
    char *p;

    /* printf�̏����ɂ��������ďo�͕�������쐬���� */
    va_start(ap, in_format);
    vsprintf(msg, in_format, ap);
    va_end(ap);

    /* �o��(���s���������͓��ʂ�CR/LF���o��)���� */
    for (p = msg; *p != 0; p++)
    {
        if (*p == '\n')
        {
            scif_putq('\r');
        }
        scif_putq(*p);
    }
}

static void date(void)
{
    SYS_RTC_DATE    date;

    /* ���ݎ����\�� */
    syRtcGetDate(&date);
    DebugPrint("%4d/%02d/%02d %2d:%02d:%02d\n",
        date.year, date.month, date.day, date.hour, date.minute,
        date.second);
}

static void set_date(char *msg)
{
    int     year, month, day, hour, minute, second;
    SYS_RTC_DATE    date;

    sscanf((msg + 5), "%d %d %d %d %d %d", &year, &month, &day, &hour, &minute,
        &second);

    date.year = year;
    date.month = month;
    date.day = day;
    date.hour = hour;
    date.minute = minute;
    date.second = second;

    syRtcSetDate(&date);

}
