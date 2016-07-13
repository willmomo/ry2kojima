	/**************************************************************************************/
/***																				***/
/***									wsft_def.h									***/
/***																				***/
/**************************************************************************************/

//#include "..\include\tracelog.h"

#ifndef	_WSFT_DEF_H	//	{{
#define	_WSFT_DEF_H

//	�ݒ�t�@�C���̓��e
typedef struct{
	unsigned short	rPort ;
	unsigned short	wPort ;
	unsigned long	sAddr ;
	int				timeout ;
	int				retrycount ;
}St_IniInfo ;

//	�ʐM�p�\����
typedef struct{
	SOCKADDR_IN		saServer;
	int				len;
	SOCKET			sock;
	unsigned short	rPort;
	unsigned short	wPort;
	unsigned long	saAddr ;
	int				timeout ;
	int				retrycount ;
	unsigned char	seqnum ;
	unsigned short	maxsize ;
	//char			ver[10] ;
	unsigned short	ver ;
	unsigned short	tracever ;
//	ST_TRACE_VER2	traceinfo ;
	BOOL			StartFlag ;
	char			inifilename[MAX_PATH] ;
	St_IniInfo		IniInfo ;
	char			Addr[64] ;
} St_SocketInfo ;

//	�ʐM�p�֐�
void CloseSocket( St_SocketInfo *lpInfo ) ;
int SetServer( St_SocketInfo *lpInfo/*, char *HostName */,WORD port ) ;
//	�t�@�C�����̎擾
int com_infofile( St_SocketInfo *lpInfo, unsigned long fno, unsigned long rec, unsigned long *fsize, unsigned short *fr, unsigned short *flag, char FileorAddr ) ;
//	�t�@�C���̓ǂݍ���
int com_readfile( St_SocketInfo *lpInfo, unsigned long addr, unsigned long rec, char *buf, unsigned long bufsize, char FileorAddr ) ;
int com_readfile2(St_SocketInfo *lpInfo, unsigned long addr, unsigned long rec, size_t rec_size, size_t rec_count, char *buf, unsigned long bufsize, char FileorAddr);
//	�t�@�C���̏�������
int com_writefile( St_SocketInfo *lpInfo, unsigned long fno, unsigned long rec, char *buf, unsigned long bufsize, char FileorAddr ) ;

//	2001/09/27�� �ǉ� --->
DWORD getIPAddr( LPCSTR lpName ) ;
//	<--- �����܂�

//	�ʐM�p�f�[�^�\����
typedef struct{
	short			ver ;
	unsigned short	size ;
	char			seqnum ;
	char			dmy1[3] ;
	
}ST_APP_HEADER ;

typedef struct{
	char			command ;
	char			fileoraddr ;
	char			dmy2[2] ;
	unsigned long	addr ;
	unsigned long	rec ;
	unsigned short	info[4] ;
}ST_COMMAND ;

typedef struct{
	ST_APP_HEADER	appheader ;
	ST_COMMAND		cmdheader ;
	//char			data[4068] ;
	char			*data ;
}ST_DATA ;


#endif	//	_WSFT_DEF_H	}}
