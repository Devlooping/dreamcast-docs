/*
 *   GDFS Streaming sample
 *   
 *   1999/01/19
 */


/***** constants *****/


#define STR_SCTSIZE  2048
#define STR_BYTE2SCT(x)  ((x) >> 11)
#define STR_SCT2BYTE(x)  ((x) << 11)


enum fStrErr {
	STR_ERR_INIT = -50,
	STR_ERR_MEMOVER,
	STR_ERR_ILGHNDL,
	STR_ERR_SEEK,
	STR_ERR_EOF,
	STR_OK = 0
};


/***** structures *****/


typedef struct fStrBuf {
	Sint32 total; /* �o�b�t�@�����O����ő�Z�N�^�� */
					/* max sectors for buffering */
	Sint32 back; /* ��߂�\�Z�N�^�� */
					/* sectors for backtrack */
	Sint32 readlmt; /* �ǂݍ��݋����E */
					/* limit for pre-loading */
	void *buf; /* �o�b�t�@�ւ̃|�C���^ (32 bytes align) */
				/* pointer of buffer */
} StrBuf;


typedef struct fStrHandle {
	Sint32 total; /* �o�b�t�@�����O����ő�Z�N�^�� */
					/* max sectors for buffering */
	Sint32 back; /* ��߂�\�Z�N�^�� */
					/* sectors for backtrack */
	Sint32 readlmt; /* �ǂݍ��݋����E */
					/* limit for pre-loading */
	void *buf; /* �o�b�t�@�ւ̃|�C���^ (32 bytes align) */
				/* pointer of buffer */
	Sint32 bufsize; /* �o�b�t�@�̃o�C�g�T�C�Y */
				/* buffer size */
	Sint32 filesize; /* �t�@�C���T�C�Y */
				/* file size */
	Sint32 cur; /* ���݂̃o�b�t�@����̓ǂݏo���ʒu */
				/* current seek point */
	Sint32 top; /* �o�b�t�@�̐擪 */
				/* top of buffer */
	Sint32 bottom; /* �o�b�t�@�̍Ō� */
					/* end of buffer */
	GDFS hn; /* �ǂݍ��ݗp�n���h�� */
				/* handle */
	Sint32 rdsct; /* �ǂݍ��ݒ��̃Z�N�^�� */
					/* sectors on reading */
	Sint32 trnsed; /* �]���ς݃o�C�g�� */
					/* transferred size */
} StrHandle;

typedef StrHandle *STRHN;


typedef struct fStrWork {
	Sint32 max_stream; /* �ő哯���I�[�v���� */
						/* max open numbers */
	StrHandle *strhandle;
} StrWork;


/*** APIs ***/


Sint32 StrInit(Sint32 max_stream, StrBuf *strbuf);
void StrFinish(void);
STRHN StrOpen(const char *fname, GDFS_DIRREC dirrec);
STRHN StrOpenNum(Sint32 strnum, const char *fname, GDFS_DIRREC dirrec);
Sint32 StrFread(void *ptr, Sint32 size, Sint32 n, STRHN hn);
Sint32 StrFseek(STRHN hn, Sint32 offset, Sint32 whence);
Sint32 StrFeof(STRHN hn);
Sint32 StrClose(STRHN hn);

void StrServer(void);

void *StrGetPtr(STRHN hn);
Sint32 StrGetContSize(STRHN hn);


/* eof */
