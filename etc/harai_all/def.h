#ifndef _task_id_h_
#define _task_id_h_


/*****************************************************************************/
/* 	�萔��`		�i�b��I �� �̂��Ɉ����֕ύX����j						 */
/*****************************************************************************/
#define	SA		0x31
#define	ENQ		0x05
#define	SYN		0x16
#define	EOT		0x04
#define	ACK		0x06
#define	NAK		0x15
#define	STX		0x02
#define	M_STX	'S'
#define	ETX		0x03
#define	M_ETX	'E'
#define	CR		0x0d
#define	LF		0x0a
#define NO_REC	0x07
#define	ERR		0x08
#define	STATUS1	0xa0
#define	STATUS2	0xa1
#define	HARAI	0xbb
#define	CLR		0xcc
#define	CAL		0xdd
#define	SKIP	0xff

#define	RECEIVE		0x00
#define	SEND		0x01
#define	DISP		0x00
#define	LOG			0x01

#define	LOOP_NONE	0x00
#define	LOOP_ST		0x01
#define	LOOP_ED		0x02

#define	H_GLORY_OLD	1
#define	H_GLORY_NEW	2
#define	H_SANKYO	3
#define	H_MARS		4
#define	H_HIROTAC	5
#define	H_SUBARU	6
#define	H_TMI		7

#define	TOKUSHU_MAX	4	/* ����i�i��ސ� */
#define	LANE_MAX	8	/* �O���A�}�[�X�̃��[���ő吔 */

/* �v���g�^�C�v�錾 */
typedef struct tagExtraOfferBall {
	unsigned long tdai;
	unsigned long dai;
	unsigned long chu;
	unsigned long syo;
} T_EXTRA_OFFER_BALL;

typedef struct tagExtraOffer {
	unsigned long	total,
					dai,
					chu,
					syo;
} T_EXTRA_OFFER;

/* main.cpp */
static int fname_cnt = 0;
static char rece_err_flag = FALSE;
static int ptr_count = 0;

int csv_read_func( void );
int read_csv_harai( void );
int read_csv_open( void );
int get_send_csv(char *buf, int cnt);
int data_init( void );
int send_sa( void );
int send_syn( void );
int send_ack( void );	/* ACK���M */
int send_nak( void );	/* NAK���M */
int send_eot( void );	/* EOT���M */
int send_enq( void );	/* ENQ���M */
int send_cmd( void );	/* ����ޑ��M */
int send_data(char n);	/* �L�q���e���̂܂ܑ��M */
int send_mtanka( char *data );
int send_status( char mode );	/* ��ԃ`�F�b�N���� */
int send_harai( void );	/* ���o�v������ */
int send_clr( void );	/* �N���A���� */
int send_cal( void );	/* �J�����_�ݒ艞�� */
void disp_log_cmd(char *data, char mode1, char mode2, int size);
void disp_log( char data );
void disp_harai( char *data );
void disp_cal( char *data );
void readini(void);
void make_harai_sankyo( char *data );
void make_tanka_mars( char *data );
void make_harai_mars( char *data, int cbSize );
unsigned long AutoOffer(unsigned long left_ball,
						T_EXTRA_OFFER_BALL *OfferBall,
						T_EXTRA_OFFER *Offer);
int get_lcm(int ball_1, int ball_2);
int get_gcd(int ball_1, int ball_2);

/* pdpctrl.cpp */
HANDLE OpenComm();
int SendData(HANDLE hComm, const char* pszCmd);
int RecvData(HANDLE hComm, char* pszAns, size_t bufSize);



/* main.cpp */
extern char com[8];	/* OPEN����COM */
extern int maker;	/* OPEN����COM */
#endif	/* _task_id_h_ */
