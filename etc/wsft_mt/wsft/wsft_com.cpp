/**************************************************************************************/
/***																				***/
/***								wsft.cpp										***/
/***																				***/
/**************************************************************************************/
#include <afx.h>
#include <assert.h>
#include <winsock2.h>


#include "wsft.h"
#include "wsft_def.h"
#include "wsft_err.h"
#include "tracex.h"
//#include "../wssasup/wssasup.h"

#include <string>
std::string GetExeName();	// adhoc �v���g�^�C�v

__declspec(thread) DWORD g_dwTOCount[2];		//[0]:�S��M���s�� [1]:�^�C���A�E�g������

extern LONG g_Suspended;

//	2001/09/27�� �ǉ� --->
/*---------------------------------------------------------------------------------/
								IP�A�h���X���擾����

	[�֐���]
		getIPAddr

	[����]
		LPCSTR		*lpName		:	�z�X�g��

	[�߂�l]
		DWORD		:	�h�o�A�h���X

	[���l]
		�l�b�g���[�N�o�C�g�I�[�_�ł̒l��߂�l�Ƃ���
/---------------------------------------------------------------------------------*/
DWORD getIPAddr( LPCSTR lpName )
{
	if (lpName[0] == '\0')	return INADDR_ANY;

	// addr��0x00000000�ŏ������@**FUMIYA
	DWORD	addr = INADDR_ANY;
	// ���̊֐��Ŏg���|�C���^�ϐ��Ɉ����̃A�h���X���i�[
	LPCSTR trg_name = lpName;
	struct hostent	*hent;
	// �o�C�i���̃C���^�l�b�g�A�h���X�ɕϊ��@**FUMIYA
	addr = inet_addr( trg_name );
	// trg_name��"192.16.255.255"�̂悤�Ȑ��l������ł͂Ȃ������Ƃ�**FUMIYA
	if( addr == INADDR_NONE ){
		// trg_name��"www.idgbooks.com"�̂悤�ȷ�׸�������̂Ƃ�**FUMIYA
		hent = gethostbyname( trg_name );
		if( hent != NULL ){
			// �l�b�g���[�N�A�h���X���擾(�擪�̃|�C���^�̒��g�Ɋi�[)**FUMIYA
			addr = *((DWORD *)(hent->h_addr_list[0]));
		}
		else{
			// addr��0x00000000�ŏ��������Ȃ����@**FUMIYA
			addr = INADDR_ANY;
		}
	}
	//return	ntohl( addr );	//osako
	return	addr;
}
//	<--- �����܂�

//�T�[�o�[����
//	�w�肳�ꂽIP�A�h���X�l����̃T�[�o�[����
// **FUMIYA�@>>
//	�@�\�F�Y���T�[�o�[�̗L���𒲂ׂ�֐�
//	�Y���T�[�o�[����Fret�@=�@0
//	�Y���T�[�o�[�Ȃ��Fret�@=�@WSFTERR_NOT_SERVER
// **FUMIYA�@<<
int GetHost( unsigned long addr ){
	LPHOSTENT lpHostEnt;		//hostent�\���̂�typedef�@**FUMIYA
	struct in_addr iaHC;		//�A�h���X�\���́@**FUMIYA
	char	logstr[256] ;

	//�ύX���ꂽserverAddr�̒l�𑗐M��A�h���X�Ƃ��Đݒ肷��
	iaHC.s_addr = addr ;
	// �����Ŏ󂯎�����A�h���X����hostent�\���̎擾**FUMIYA
	lpHostEnt = gethostbyaddr( (const char * )&iaHC, sizeof( struct in_addr ), AF_INET );

	if( lpHostEnt == NULL ){
		//�T�[�o�̏�񂪎擾�ł��Ȃ�����
		wsprintf( logstr, "GetHost() �F �w�肳�ꂽ�T�[�o�[��������܂���B�A�h���X�F%s", inet_ntoa( iaHC ) );
		TraceF(logstr );
		return WSFTERR_NOT_SERVER ;
	}

	return 0;
}

/*-----------------------------------------------------------------------------------------/
										�\�P�b�g�����
/-----------------------------------------------------------------------------------------*/
void CloseSocket(St_SocketInfo *lpInfo)
{
	//	�\�P�b�g�����
	// **FUMIYA >>
	// ���葤�̃\�P�b�g�ɒʒm���o���ă\�P�b�g�����
	// SD_RECEIVE:�\�P�b�g�ɑ΂����M�𖳌��ɂ���*FUMIYA
	// **FUMIYA <<
	shutdown( lpInfo->sock, SD_RECEIVE );
	//�\�P�b�g������x����@**FUMIYA
	closesocket( lpInfo->sock );
}

//---------------------------------------------------------------------
// �\�P�b�g�̍쐬

int SetServer(St_SocketInfo	*lpInfo, u_short port)
{
	//UDP�\�P�b�g�L�q�q���쐬����B
	lpInfo->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//�\�P�b�g�쐬���s�I add comment by FUMIYA 02.01.17
	if ( lpInfo->sock == INVALID_SOCKET ){
		TraceE("ConnectHost() : UDP�\�P�b�g�̍쐬�Ɏ��s���܂����B" ) ;
		return WSFTERR_INIT_SOCKET ;
	}
	//�A�h���X�\���̕��������i�[ add comment by FUMIYA 02.01.17
	lpInfo->len = sizeof(SOCKADDR);

//	TraceD("SetServer() : �T�[�o�[ [%ld] �|�[�g[%ld] �ɐڑ����܂��B",
//		lpInfo->saAddr, port) ;
	TraceD("SetServer() : �T�[�o�[ [%d.%d.%d.%d] �|�[�g[%ld] �ɐڑ����܂��B",
		(lpInfo->saAddr & 0xff), ((lpInfo->saAddr >> 8) & 0xff),
		((lpInfo->saAddr >> 16) & 0xff), ((lpInfo->saAddr >> 24) & 0xff),
		port) ;

	//	�T�[�o�[�̐ݒ�
	lpInfo->saServer.sin_family = AF_INET;
	lpInfo->saServer.sin_addr.S_un.S_addr = lpInfo->saAddr ;
	lpInfo->saServer.sin_port = htons(port);

	// �^�C���A�E�g�̐ݒ�
	int to = (lpInfo->timeout > 500) ? lpInfo->timeout - 500 : 1;
	int ret = setsockopt(lpInfo->sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&to, sizeof(to));
	if (ret == SOCKET_ERROR) {
		TraceF("%s(%d): SetServer(): �^�C���A�E�g�̐ݒ�ŃG���[���������܂����B(ERR: %u)",
			__FILE__, __LINE__, WSAGetLastError());
		return WSFTERR_OPTION_TIMEOUT;
	}

	return ret;
}

/*-----------------------------------------------------------------------------------------/
						��M�����G���[�d���̓��e���o�͂���
/-----------------------------------------------------------------------------------------*/
int OutputErrorLog(ST_DATA req, ST_COMMAND *anscmdheader)
{

	int		ret = htons(anscmdheader->info[0]) ;

	switch( htons(anscmdheader->info[0]) ){
	case WSFTERR_BUSY_I :	//	���@�킩��̏������ݒ�
		/*TraceF("CheckCommonHeader():���@�킩�珑���ݔr�����B�t�@�C���ԍ�=%ld, �����݋@��IP�A�h���X=%d.%d.*.*",
			htons(anscmdheader->info[1]),
			(htons(anscmdheader->info[2]) & 0xff00) >> 8,
			htons(anscmdheader->info[2]) & 0x00ff) ;*/
		TraceF("CheckCommonHeader():���@�킩�珑���ݔr�����Binfo[0]=0x%04x, info[1]=0x%04x, info[2]=0x%04x, info[3]=0x%04x",
			htons(anscmdheader->info[0]), htons(anscmdheader->info[1]), htons(anscmdheader->info[2]),
			htons(anscmdheader->info[3]));
		//	���M�����R�}���h�ɂ��߂�l��ύX����
		switch( req.cmdheader.command ){
		case 'I' :	//	�t�@�C�����擾�v����
			ret = WSFTERR_BUSY_I ;
			break ;
		case 'R' :	//	�ǂݍ��ݗv����
			ret = WSFTERR_BUSY_R ;
			break ;
		case 'w' :	//	�j���ݏ����v����
			ret = WSFTERR_BUSY_S ;
			break ;
		case 'D' :	//	�������ݗv����
			ret = WSFTERR_BUSY_W ;
			break ;
		case 'W' :	//	�������݊����v����
			ret = WSFTERR_BUSY_E ;
			break ;
		}
		break ;
	case WSFTERR_BUFF :	//	�o�b�t�@�m�ێ��s
		TraceF("CheckCommonHeader():�������ݏ����v����̃o�b�t�@�m�ۃG���[�B" ) ;
		break ;
	case WSFTERR_FRAME_SKIP :	//	�t���[������
		TraceF("CheckCommonHeader():�t���[�������G���[�B %ld, %ld, %ld", htons(anscmdheader->info[1]), htons(anscmdheader->info[2]), htons(anscmdheader->info[3]) ) ;
		break ;
	case WSFTERR_REC_TIMEOUT :	//	�������ݎ��^�C���A�E�g
		TraceF("CheckCommonHeader():�������ݎ�M�^�C���A�E�g�B �ŏI��M�t���[��=%ld, ��M�t���[�����v=%ld, �K�v�t���[�����v=%ld", htons(anscmdheader->info[1]), htons(anscmdheader->info[2]), htons(anscmdheader->info[3]) ) ;
		break ;
	case WSFTERR_NO_SUPPORT_SMF :	//	�r�l�e�ɖ����t�@�C���ԍ�
		TraceF("CheckCommonHeader():SMF�ɖ����t�@�C���ԍ��ł��B" ) ;
		break ;
	case WSFTERR_ADDRESS :			//	�A�h���X
		TraceF("CheckCommonHeader():�A�N�Z�X�֎~�̈�̃A�h���X���w�肳��܂����B" ) ;
		break ;
	case WSFTERR_RECORD :			//	���R�[�h
		TraceF("CheckCommonHeader():�͈͊O�̃��R�[�h���w�肳��܂����B" ) ;
		break ;
	case WSFTERR_FRAME :			//	�t���[��
		TraceF("CheckCommonHeader():���肦�Ȃ��t���[���ԍ��ŃA�N�Z�X����܂����B" ) ;
		break ;
	case WSFTERR_COMMAND :			//	�R�}���h
		TraceF("CheckCommonHeader():�K��R�}���h�ȊO���w�肳��܂����B �R�}���h=%ld", htons(anscmdheader->info[1]) ) ;
		break ;
	case WSFTERR_VERSION :			//	�o�[�W����
		TraceF("CheckCommonHeader():�v���g�R���o�[�W�������Ԉ���Ă��܂��B" ) ;
		break ;
	default :						//	���̑�
		TraceF("CheckCommonHeader():�T�|�[�g����Ă��Ȃ��G���[�R�[�h�ł��B �G���[�R�[�h=0x%04X", htons(anscmdheader->info[0]) ) ;
		break ;
	}

	return ret ;
}

/*-----------------------------------------------------------------------------------------/
									�d�����ʕ��̃`�F�b�N

	[����]
		ST_DATA			req				:	�v���d��
		ST_APP_HEADER	*ansappheader	:	�����d���̃w�b�_�[��
		ST_COMMAND		*anscmdheader	:	�����d���̃w�b�_�[��

	[�߂�l]
		== 0							:	�G���[�Ȃ�
		!= 0							:	�G���[�R�[�h

/-----------------------------------------------------------------------------------------*/
inline int CheckCommonHeader(	ST_DATA	req	,ST_APP_HEADER *ansappheader	,ST_COMMAND *anscmdheader	)
{
	char	anscmd ;
	short	i = 0 ;

	//	�G���[���A���Ă����Ƃ�
	if( anscmdheader->command == 'E' ){
		return OutputErrorLog( req, anscmdheader ) ;
	}

	//	�v�������d���ɊY�����鉞���R�}���h�̐ݒ�
	switch( req.cmdheader.command ){
	case 'R' :
		anscmd = 'D' ;
		break ;
	case 'f' :
		anscmd = 'D' ;
		break ;
	case 'D' :
		anscmd = 'A' ;
		break ;
	default :
		//	�c��͗v���d���Ɠ����R�}���h�ŕԂ��Ă���
		anscmd = req.cmdheader.command ;
		break ;
	}

	//	�v���������̂ƈႤ�d������M�����Ƃ��͖�������
	if( req.appheader.seqnum != ansappheader->seqnum ){
		TraceE("CheckCommonHeader() : �قȂ�V�[�P���X�ԍ��̃f�[�^����M���܂����B�v��=%u, ����=%u",
			req.appheader.seqnum & 0xff, ansappheader->seqnum & 0xff ) ;
		return -2 ;
	}
	//	�����d���̃R�}���h�����҂��Ă�����̂��ǂ����̃`�F�b�N
	if( anscmd != anscmdheader->command ){
		TraceE("CheckCommonHeader() : �قȂ�R�}���h�̃f�[�^����M���܂����B�v��=%c, ����=%c", req.cmdheader.command, anscmdheader->command ) ;
		return -1 ;
	}
	//	���M�����t�@�C���ԍ��܂��̓A�h���X�����̂܂ܕԂ��Ă��Ă��邩
	if( req.cmdheader.addr != anscmdheader->addr ){
		TraceE("CheckCommonHeader() : �قȂ�t�@�C���ԍ��̃f�[�^����M���܂����B�v��=%d, ����=%d", htonl(req.cmdheader.addr), htonl(anscmdheader->addr) ) ;
		return -1 ;
	}
	//	�v���������R�[�h�ԍ��̒l���Ԃ��Ă��Ă��邩
	if( req.cmdheader.rec != anscmdheader->rec ){
		TraceE("CheckCommonHeader() : �قȂ郌�R�[�h�̃f�[�^����M���܂����B�v��=%d, ����=%d", htonl(req.cmdheader.rec), htonl(anscmdheader->rec) ) ;
		return -1 ;
	}

	//	���M�����d���ɂ�肳��Ƀ`�F�b�N���鍀�ڂ�����Ƃ�
	switch( req.cmdheader.command ){
	case 'D' :
	case 'w' :
	case 'W' :
		for( i=0 ; i<4 ; i++ ){
			if( req.cmdheader.info[i] != anscmdheader->info[i] ){
				TraceE("CheckCommonHeader() : Info[%ld] �̒l����v���܂���B�v��=%ld, ����=%ld", i, htons(req.cmdheader.info[i]), htons(anscmdheader->info[i]) ) ;
				return -1 ;
			}
		}
		break ;
	}

	return 0 ;
}

/////////////////////// KOJIMA //////////////////////////////

int SendAndRecv(
	St_SocketInfo *lpInfo
	,char *req
	,unsigned short reqsize
	,char *recvbuff
	,unsigned short anssize
	)
{
	ST_DATA		req1 ;			//	�v���d���p�\����
	int			ret = 0 ;		//	�߂�l�p
	int			cnt = 0 ;		//	���g���C�񐔃J�E���g�p
	int			size = 0 ;		//	����M�p�߂�l
	long		stime ;			//	��M�J�n����
	long		etime ;			//	��M�I������
	ST_APP_HEADER	*app ;		//	�w�b�_�[�\����
	ST_COMMAND		*cmd ;		//	�w�b�_�[�\����

	// g_Suspended == FALSE �̎��A�d���𑗐M���Ȃ��ŏI������
	if (g_Suspended != TRUE) {
		Winssa_Log(LOGC_WSFT, LOGLV_INFO, "(%s)SendAndRecv(): g_Suspended = FALSE �̂��߁A���M���܂���B",
			GetExeName().c_str());
		return -1;
	}

	sockaddr_in sock_addr;	//**02.02.27 misako**
	int iAddress = sizeof(sock_addr);	//**02.02.27 misako**
	//	2001/09/26�� �ǉ� --->
//	if( GetHost( lpInfo->saAddr ) != 0 ){
		//	�T�[�o�[��������Ȃ�
//		TraceF("�ݒ肳��Ă���T�[�o�[[%ld]��������܂���B", lpInfo->saAddr ) ;
//		return WSFTERR_NOT_SERVER ;
//	}
	//	<--- �����܂�

	//	�ݒ�񐔃��g���C����
	//	2001/09/20�� �C�� --->
	//	���g���C�񐔂��P�񑽂�����
	//for( cnt=0 ; cnt<=lpInfo->retrycount; cnt++ ){
	for( cnt=0 ; cnt<lpInfo->retrycount; cnt++ ){
	//	<--- �����܂�
		//	�v���d���p�o�b�t�@�̃N���A
//		memset( &req1, 0, sizeof(req1) ) ;
		//	�����d���p�o�b�t�@�̃N���A
//		memset( recvbuff, 0, anssize ) ;
		//	�V�[�P���X�ԍ��̉��Z
		if( lpInfo->seqnum >= 255 ){
			lpInfo->seqnum = 1 ;
		}else{
			lpInfo->seqnum++ ;
		}
		//	�w�b�_���擾
		memcpy( &req1, req, sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ) ;
		//	�V�[�P���X�ԍ����擾
		req1.appheader.seqnum = lpInfo->seqnum ;
		memcpy( req, &req1, sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ) ;
		//	�ēx���M����Ƃ��͖߂�l�𐳏�I���̒l�ɂ��Ă���
		ret = 0 ;
		//	�����œn���ꂽ�̗v���d���𑗐M
		//size = sendto( lpInfo->sock, (char*)&req1, reqsize, 0, (LPSOCKADDR)&lpInfo->saServer, lpInfo->len ) ;
		size = sendto( lpInfo->sock, req, reqsize, 0, (LPSOCKADDR)&lpInfo->saServer, lpInfo->len ) ;

		// ���O���c���݂̂ŁA����͑��M�G���[���̏����͓��ɒǉ����Ȃ�
		if( size == SOCKET_ERROR ){
			ret = WSAGetLastError();
			if( ret == WSAETIMEDOUT ){
				TraceE( "SendAndRecv()�F���s%d�F�^�C���A�E�g���ԓ��ɑ��M�ł��܂���ł����B(WSAE:%d)", cnt+1, ret );
			}
			else{
				TraceE( "SendAndRecv()�F���s%d�F�d���̑��M�Ɏ��s���܂����B(WSAE:%d)", cnt+1, ret );
			}
		}
		else if( size != reqsize ){
			TraceE( "SendAndRecv()�F���s%d�F�d������x�ɑ��M���؂�܂���ł����B", cnt+1 );
		}

		//	��M
		stime = GetTickCount() ;
		do{
recv_retry:
			ret = 0 ;

			//	��M
			//size = recvfrom( lpInfo->sock, recvbuff, anssize, 0, (LPSOCKADDR)&lpInfo->saServer, &lpInfo->len );
			g_dwTOCount[0]++;
			size = recvfrom( lpInfo->sock, recvbuff, anssize, 0, (LPSOCKADDR)&sock_addr, &iAddress ); //**02.02.27 misako**

			//	�G���[�������͑���M�I��
			if( size == SOCKET_ERROR ){
				break ;
			}

			//	��M�f�[�^������Ƃ�
			if ( size != 0 ){
				app = (ST_APP_HEADER*)recvbuff ;
				cmd = (ST_COMMAND*)&recvbuff[sizeof(ST_APP_HEADER)] ;

				//	�d�����ʕ��̃`�F�b�N
				ret = CheckCommonHeader( req1, app, cmd ) ;
				if (ret == -2)			// �V�[�P���XNo�s��v�̂Ƃ���
					goto recv_retry;	// ��M�̂݃��g���C

				break;
			}

			//	���Ɏ�M�J�n����^�C���A�E�g�b�o�߂��Ă���Ƃ��͖������ŏo��
			//  �����ł̓\�P�b�g�̃^�C���A�E�g�ƃv���O�����ł̃^�C���A�E�g��
			//�@����Ă���*****02.01.21 FUMIYA*****
			etime = GetTickCount() ;
			if( etime - stime > (lpInfo->timeout) ){
				break ;
			}

			//	���ʓd���̕s��v�̂Ƃ��͎�M�𑱂���
			if( ret != -1 ){
				//	�G���[�R�}���h����M�����Ƃ��͎�M�͏I��(���ʓd���̕s��v�̂Ƃ��͎�M�𑱂���)
				if( ret != 0 || size != 0 ){
					break ;
				}
			}
		}while(1) ;

		// ����ȓd������M�ł��Ȃ������Ƃ�
		if( ret != 0 ){
			//	�t�@�C���擾�̂Ƃ��ȊO�̓G���[�R�}���h���Ԃ��Ă��Ă����g���C����
			//	�t���[�������̂Ƃ��͒��f
			//	�t���[�������G���[�̂Ƃ��͒��f
			if( ret != WSFTERR_FRAME_SKIP ){
				Sleep(150);	// ���g���C����̂�150ms�܂��Ă�����s
				continue ;
			//	�t�@�C���擾�ŃG���[�d�����Ԃ��Ă����Ƃ��͒��f����
			}else{
				break ;
			}
		}

		//	��M����
		if( size != 0 && size != SOCKET_ERROR ){
			break ;
		}else{
			if( WSAGetLastError() == WSAETIMEDOUT ){
				g_dwTOCount[1]++;
				switch( req1.cmdheader.command ){
				case 'I' :
					ret = WSFTERR_TIMEOUT_I ;
					break ;
				case 'R' :
					ret = WSFTERR_TIMEOUT_R ;
					break ;
				case 'w' :
					ret = WSFTERR_TIMEOUT_S ;
					break ;
				case 'D' :
					ret = WSFTERR_TIMEOUT_W ;
					break ;
				case 'W' :
					ret = WSFTERR_TIMEOUT_E ;
					break ;
				default :
					//	�G���[�R�[�h�ݒ�
					//	�T�|�[�g���Ă��Ȃ��R�}���h�̂Ƃ�
					ret = WSFTERR_TIMEOUT_I ;
					break ;
				}

				continue ;
			}else{
				//	��M�Ɏ��s���܂����B
				TraceF("SendAndRecv() : ��M�Ɏ��s���܂����B �߂�l %d (%d) ", size, WSAGetLastError() ) ;
				//	��M�G���[�̂Ƃ��̃G���[�R�[�h��ݒ�
				switch( req1.cmdheader.command ){
				case 'I' :
					ret = WSFTERR_REC_I ;
					break ;
				case 'R' :
					ret = WSFTERR_REC_R ;
					break ;
				case 'w' :
					ret = WSFTERR_REC_S ;
					break ;
				case 'D' :
					ret = WSFTERR_REC_W ;
					break ;
				case 'W' :
					ret = WSFTERR_REC_E ;
					break ;
				default :
					//	�T�|�[�g���Ă��Ȃ��R�}���h�̂Ƃ�
					ret = WSFTERR_REC_I ;
					break ;
				}
				//	2001/09/26�� �ǉ� --->
				//	�^�C���A�E�g�b�҂�
				Sleep( lpInfo->timeout ) ;
				//	<--- �����܂�
				continue ;
			}
		}
	}

	return ret ;

}


/*-----------------------------------------------------------------------------------------/
									�t�@�C�����擾

	[�֐���]
		com_infofile

	[����]
		St_SocketInfo	*lpInfo		:	�ʐM�̐ݒ�
		unsigned long	fno			:	�����擾����t�@�C����smf�t�@�C���ԍ�
		unsigned short	rec			:	�����擾���郌�R�[�h�ԍ�(0�̂Ƃ��̓t�@�C��)
		unsigned long	*fsize		:	�t�@�C���T�C�Y
		unsigned long	*fr			:	�t���[����
		unsigned long	*flag		:	�t���O	0x0001:�������ݒ� 0x0002:���U�[�u��
		char			FileorAddr	:	'F' or 'A'

	[�߂�l]
		== 0						:	����
		!= 0						:	�G���[(�G���[�R�[�h)

/-----------------------------------------------------------------------------------------*/
int com_infofile(St_SocketInfo *lpInfo, u_long fno, u_long rec,
				 u_long *fsize, u_short *fr, u_short *flag, char FileorAddr)
{
	ST_DATA		req ;			//	�v���d���p�\����
	ST_DATA		ans ;			//	�����d����M�p�\����
	int			ret = 0 ;		//	�߂�l
	unsigned short	reqsize ;	//	�v���d���T�C�Y

	memset(&req, 0, sizeof(req));

	//	�A�v���w�b�_��ݒ�
	//	�v���g�R���o�[�W����
	req.appheader.ver = htons(lpInfo->ver);
	//	�v���d���T�C�Y
	reqsize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
	req.appheader.size = htons(reqsize) ;

	//	�R�}���h��ݒ�
	req.cmdheader.command = 'I' ;
	//	�t�@�C���ԍ� or �A�h���X �̎w�� ( 'F' or 'A' )
	req.cmdheader.fileoraddr = FileorAddr ;
	//	�t�@�C���ԍ� or �A�h���X
	req.cmdheader.addr = htonl(fno) ;
	//	���R�[�h�ԍ� or �T�C�Y
	req.cmdheader.rec = htonl(rec) ;

	//	����M
	ret = SendAndRecv( lpInfo, (char*)&req, reqsize, (char*)&ans, reqsize ) ;

	if (ret == 0) {
		if (fsize) {	// �t�@�C���T�C�Y
			*fsize = ntohl(*(u_long*)ans.cmdheader.info);
		}
		if (flag) {		// �t���O
			*flag = ntohs(ans.cmdheader.info[2]);
		}
		if (fr) {		// �t���[����
			*fr = ntohs(ans.cmdheader.info[3]);
		}
	}

	TraceD("com_infofile() : com_infofile�֐���߂�l (%d) �ŏI�����܂��B", ret ) ;

	return ret ;

}


/*-----------------------------------------------------------------------------------------/
									�t�@�C���ǂݍ���

	[�֐���]
		com_readfile

	[����]
		St_SocketInfo	*lpInfo		:	�ʐM�ݒ�
		unsigned short	addr		:	�擾����t�@�C����smf�t�@�C���ԍ��܂��̓A�h���X
		unsigned short	rec			;	�擾���郌�R�[�h(�O�̂Ƃ��̓t�@�C��)
		char			*buf		:	�擾�����t�@�C���̃f�[�^���i�[����o�b�t�@
		unsigned long	bufsize		:	�o�b�t�@�T�C�Y
		char			FileorAddr	:	'F' : �t�@�C���ԍ��Ŏ擾�B 'A' : �A�h���X�Ŏ擾

	[�߂�l]
		== 0						:	����
		!= 1						:	�G���[(�G���[�R�[�h)

/-----------------------------------------------------------------------------------------*/
int com_readfile(
	St_SocketInfo *lpInfo
	,unsigned long	addr
	,unsigned long rec
	,char *buf
	,unsigned long bufsize
	,char FileorAddr
	)
{
	ST_DATA		req ;			//	�v���d���o�b�t�@
	int			ret = 0 ;		//	�߂�l
	unsigned long fsize ;		//	�t�@�C���T�C�Y
	unsigned short fr ;			//	�t���[����
	unsigned short flag ;		//	�t���O
	unsigned short reqsize ;	//	�v���d���o�b�t�@�T�C�Y
	unsigned short anssize ;	//	�����d���o�b�t�@�T�C�Y
	unsigned short frno ;		//	�t���[���ԍ�
	unsigned long total ;		//	�g�[�^���f�[�^�T�C�Y
	char *tmpbuff ;				//	�e���|�����o�b�t�@
	char *back ;
	unsigned short rsize = 0 ;	//	��M�f�[�^�T�C�Y
	char *rbuf ;				//	��M�o�b�t�@
	unsigned short maxdatasize = 0 ;	//	�P��Ŏ�M�ł���ő�o�b�t�@�T�C�Y(�f�[�^���̂�)

	//	�t�@�C�������擾����
	ret = com_infofile( lpInfo, addr, rec, &fsize, &fr, &flag, FileorAddr ) ;
	if( ret != 0 ){
		TraceD("com_readfile() : com_readfile�֐���߂�l (%d) �ŏI�����܂��B", ret ) ;
		return ret ;
	}

	TraceD("com_readfile() : fno=%ld, fsize=%ld, framecount=%ld, flag=%ld", addr, fsize, fr, flag ) ;

	//	�t�@�C�����̃t���O�����ď������s���ꍇ�͈ȉ��̃R�[�h��L���ɂ���
	switch( flag ){
	case WSFT_INFO_FILE_FLAG_WRITE :
		TraceW("com_readfile() : �w�肳�ꂽ�t�@�C���͌��ݏ������ݒ��ł��B �t�@�C���ԍ�=%ld", addr ) ;
		break ;
	case WSFT_INFO_FILE_FLAG_RESERVE :
		TraceW("com_readfile() : �w�肳�ꂽ�t�@�C���͌��݃��U�[�u���ł��B �t�@�C���ԍ�=%ld", addr ) ;
		break ;
	}

	//	�o�b�t�@�T�C�Y�ƃt�@�C���T�C�Y�̔�r
	//if( fsize != bufsize ){
	//	�����ł킽���ꂽ�o�b�t�@�̃T�C�Y���ʐM����t�@�C���̃T�C�Y���傫���Ƃ��ɃG���[�Ƃ��Ē��f����
	//���d�l�װ�̂��߁A�����œn���ꂽ�ޯ̧��̧�ق̻��ނ���v���Ȃ��ƴװ�ɂ���B**02.03.08 misako**
	if( bufsize != fsize ){
		TraceF("com_readfile() : �ʐM�t�@�C���T�C�Y�ƈ�v���Ȃ��o�b�t�@���w�肳��Ă��܂��B ��M�o�b�t�@�T�C�Y=%ld�A�ʐM�t�@�C���T�C�Y=%ld", bufsize, fsize ) ;
		ret = WSFTERR_REC_SIZE ;
		TraceD("com_readfile() : com_readfile�֐���߂�l (%d) �ŏI�����܂��B", ret ) ;
		return ret ;
	}

	//	�擾����t�@�C���T�C�Y�������m�ۂ���(�o�b�t�@�T�C�Y�ł͂Ȃ�)
	tmpbuff = (char*)malloc(fsize) ;
	if( tmpbuff == NULL ){
		TraceF("com_readfile() : �������̊m�ۂɎ��s���܂����B" ) ;
		ret = WSFTERR_INIT_MALLOC ;
		TraceD("com_readfile() : com_readfile�֐���߂�l (%d) �ŏI�����܂��B", ret ) ;
		return ret ;
	}

	//	�P��̒ʐM�ő���M�ł���ő�̃o�b�t�@���m�ۂ���
	rbuf = (char*)malloc(lpInfo->maxsize) ;
	if( rbuf == NULL ){
		TraceF("com_readfile() : �������̊m�ۂɎ��s���܂����B" ) ;
		free( tmpbuff ) ;
		ret = WSFTERR_INIT_MALLOC ;
		TraceI("com_readfile() : com_readfile�֐���߂�l (%d) �ŏI�����܂��B", ret ) ;
		return ret ;
	}

	//	���ƂŖ߂����߂ɐ擪�A�h���X���o���Ă���
	back = tmpbuff ;

	//	�v���d���̃o�b�t�@�T�C�Y���v�Z����
	reqsize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
	//	�g�[�^���̓d���T�C�Y
	total = fsize ;
	// �P��Ŏ󂯎��f�[�^�����̍ő�o�b�t�@�T�C�Y
	maxdatasize = lpInfo->maxsize - reqsize ;

	//	�P�t���[������(�t���[���m���͂P����)
	for( frno=1 ; frno<=fr ; frno++ ){
		if( total > maxdatasize ){
			rsize = maxdatasize ;
		}else{
			rsize = (unsigned short)total ;
		}
		total = total - rsize ;

		//	�v���g�R���o�[�W����
		req.appheader.ver = htons(lpInfo->ver);
		//	�v���d���T�C�Y
		req.appheader.size = htons(reqsize) ;
		//	�v���R�}���h
		req.cmdheader.command = 'R' ;
		//	�w����@ ( 'F' or 'A' )
		req.cmdheader.fileoraddr = FileorAddr ;
		//	�t�@�C���ԍ� or �A�h���X
		req.cmdheader.addr = htonl(addr) ;
		//	�w�背�R�[�h or �T�C�Y
		req.cmdheader.rec = htonl(rec) ;
		//	�t���[���m��
		req.cmdheader.info[0] = htons(frno) ;
		//	��M�f�[�^�T�C�Y
		anssize = reqsize + rsize ;

		//	�擾�f�[�^
		TraceD("com_readfile() : �t�@�C���ԍ�(�A�h���X)=%ld, ���R�[�h(�T�C�Y)=%ld, �t���[��No=%ld", addr, rec, frno ) ;

		//	�t�@�C���̓ǂݍ���
		ret = SendAndRecv( lpInfo, (char*)&req, reqsize, rbuf, anssize ) ;

	 	//	����ɒʐM���s��ꂽ�Ƃ��̓o�b�t�@�Ɋi�[
		if( ret == 0 ){
			//	��M�T�C�Y���R�s�[
			memcpy( tmpbuff, &rbuf[reqsize], rsize ) ;
			//	���̏������݈ʒu�Ɉړ�
			tmpbuff += rsize ;
		}else{
			break ;
		}
	}

	//	�擪�ɖ߂�
	tmpbuff = back ;

	//	�e���|�����o�b�t�@����R�s�[
	if( ret == 0 ){
		memcpy( buf, tmpbuff, bufsize ) ;
	}

	//	�J��
	free( tmpbuff ) ;
	free( rbuf ) ;

	TraceD("com_readfile() : com_readfile�֐���߂�l (%d) �ŏI�����܂��B", ret ) ;

	return ret ;

}

/////////////////////////////////////////////////////////////////////////////
// �t�@�C���ǂݍ���2
//
// [���l]
// �w�背�R�[�h����w�背�R�[�h���ǂݍ��ށB�t�@�C���S�̂�ǂݍ��ނƂ��́A
// com_readfile���g�p����B

int com_readfile2(
	St_SocketInfo *lpInfo
	,unsigned long	addr
	,unsigned long rec
	,size_t rec_size
	,size_t rec_count
	,char *buf
	,unsigned long bufsize
	,char FileorAddr
	)
{
	ST_DATA		req ;			//	�v���d���o�b�t�@
	int			ret = 0 ;		//	�߂�l
	unsigned long fsize ;		//	�t�@�C���T�C�Y
	unsigned short fr ;			//	�t���[����
	unsigned short flag ;		//	�t���O
	unsigned short reqsize ;	//	�v���d���o�b�t�@�T�C�Y
	unsigned short anssize ;	//	�����d���o�b�t�@�T�C�Y
	unsigned short frno ;		//	�t���[���ԍ�
	unsigned long total ;		//	�g�[�^���f�[�^�T�C�Y
	char *tmpbuff ;				//	�e���|�����o�b�t�@
	char *back ;
	unsigned short rsize = 0 ;	//	��M�f�[�^�T�C�Y
	char *rbuf ;				//	��M�o�b�t�@
	unsigned short maxdatasize = 0 ;	//	�P��Ŏ�M�ł���ő�o�b�t�@�T�C�Y(�f�[�^���̂�)

	//	�t�@�C�������擾����
	ret = com_infofile( lpInfo, addr, 0, &fsize, &fr, &flag, FileorAddr ) ;
	if( ret != 0 ){
		TraceD("com_readfile2() : com_readfile2�֐���߂�l (%d) �ŏI�����܂��B", ret ) ;
		return ret ;
	}

	TraceD("com_readfile2() : fno=%ld, fsize=%ld, framecount=%ld, flag=%ld", addr, fsize, fr, flag ) ;

	//	�t�@�C�����̃t���O�����ď������s���ꍇ�͈ȉ��̃R�[�h��L���ɂ���
	switch( flag ){
	case WSFT_INFO_FILE_FLAG_WRITE :
		TraceW("com_readfile2() : �w�肳�ꂽ�t�@�C���͌��ݏ������ݒ��ł��B �t�@�C���ԍ�=%ld", addr ) ;
		break ;
	case WSFT_INFO_FILE_FLAG_RESERVE :
		TraceW("com_readfile2() : �w�肳�ꂽ�t�@�C���͌��݃��U�[�u���ł��B �t�@�C���ԍ�=%ld", addr ) ;
		break ;
	}

	//	�o�b�t�@�T�C�Y�ƃt�@�C���T�C�Y�̔�r
	//if( fsize != bufsize ){
	//	�����ł킽���ꂽ�o�b�t�@�̃T�C�Y���ʐM����t�@�C���̃T�C�Y���傫���Ƃ��ɃG���[�Ƃ��Ē��f����
	//���d�l�װ�̂��߁A�����œn���ꂽ�ޯ̧��̧�ق̻��ނ���v���Ȃ��ƴװ�ɂ���B**02.03.08 misako**
//	if( bufsize != fsize ){
//		TraceF("com_readfile2() : �ʐM�t�@�C���T�C�Y�ƈ�v���Ȃ��o�b�t�@���w�肳��Ă��܂��B ��M�o�b�t�@�T�C�Y=%ld�A�ʐM�t�@�C���T�C�Y=%ld", bufsize, fsize ) ;
//		ret = WSFTERR_REC_SIZE ;
//		TraceD("com_readfile2() : com_readfile2�֐���߂�l (%d) �ŏI�����܂��B", ret ) ;
//		return ret ;
//	}

	//	�擾����t�@�C���T�C�Y�������m�ۂ���(�o�b�t�@�T�C�Y�ł͂Ȃ�)
	tmpbuff = (char*)malloc(fsize) ;
	if( tmpbuff == NULL ){
		TraceF("com_readfile2() : �������̊m�ۂɎ��s���܂����B" ) ;
		ret = WSFTERR_INIT_MALLOC ;
		TraceD("com_readfile2() : com_readfile2�֐���߂�l (%d) �ŏI�����܂��B", ret ) ;
		return ret ;
	}

	//	�P��̒ʐM�ő���M�ł���ő�̃o�b�t�@���m�ۂ���
	rbuf = (char*)malloc(lpInfo->maxsize) ;
	if( rbuf == NULL ){
		TraceF("com_readfile2() : �������̊m�ۂɎ��s���܂����B" ) ;
		free( tmpbuff ) ;
		ret = WSFTERR_INIT_MALLOC ;
		TraceI("com_readfile2() : com_readfile2�֐���߂�l (%d) �ŏI�����܂��B", ret ) ;
		return ret ;
	}

	//	���ƂŖ߂����߂ɐ擪�A�h���X���o���Ă���
	back = tmpbuff ;

	//	�v���d���̃o�b�t�@�T�C�Y���v�Z����
	reqsize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
	//	�g�[�^���̓d���T�C�Y
	total = fsize ;
	// �P��Ŏ󂯎��f�[�^�����̍ő�o�b�t�@�T�C�Y
	maxdatasize = lpInfo->maxsize - reqsize ;

	int start_frame = ((rec_size * (rec - 1)) / maxdatasize) + 1;				// �J�n�t���[��
	int end_frame = ((rec_size * (rec + rec_count - 1) - 1) / maxdatasize) + 1;	// �I���t���[��
	total -= maxdatasize * (start_frame - 1);	// �J�n�t���[���܂œǂݍ��񂾏�ԂŃX�^�[�g

	//	�P�t���[������(�t���[���m���͂P����)
	for( frno=start_frame ; frno<=end_frame ; frno++ ){
		if( total > maxdatasize ){
			rsize = maxdatasize ;
		}else{
			rsize = (unsigned short)total ;
		}
		total = total - rsize ;

		//	�v���g�R���o�[�W����
		req.appheader.ver = htons(lpInfo->ver);
		//	�v���d���T�C�Y
		req.appheader.size = htons(reqsize) ;
		//	�v���R�}���h
		req.cmdheader.command = 'R' ;
		//	�w����@ ( 'F' or 'A' )
		req.cmdheader.fileoraddr = FileorAddr ;
		//	�t�@�C���ԍ� or �A�h���X
		req.cmdheader.addr = htonl(addr) ;
		//	�w�背�R�[�h or �T�C�Y
		req.cmdheader.rec = htonl(0) ;
		//	�t���[���m��
		req.cmdheader.info[0] = htons(frno) ;
		//	��M�f�[�^�T�C�Y
		anssize = reqsize + rsize ;

		//	�擾�f�[�^
		TraceD("com_readfile2() : �t�@�C���ԍ�(�A�h���X)=%ld, ���R�[�h(�T�C�Y)=%ld, �t���[��No=%ld", addr, rec, frno ) ;

		//	�t�@�C���̓ǂݍ���
		ret = SendAndRecv( lpInfo, (char*)&req, reqsize, rbuf, anssize ) ;

	 	//	����ɒʐM���s��ꂽ�Ƃ��̓o�b�t�@�Ɋi�[
		if( ret == 0 ){
			if (frno == start_frame) {	// �v���擪�t���[���̂Ƃ�
				int ofs = ((rec - 1) * rec_size) - ((frno - 1) * maxdatasize);
				memmove(tmpbuff, &rbuf[reqsize + ofs], rsize - ofs);
				tmpbuff += rsize - ofs;
			} else if (frno == end_frame) {	// �v���ŏI�t���[���̂Ƃ�
				memmove(tmpbuff, &rbuf[reqsize],
					((rec + rec_count - 1) * rec_size) - ((frno - 1) * maxdatasize));
				tmpbuff += ((rec + rec_count - 1) * rec_size) - ((frno - 1) * maxdatasize);
			} else {
				//	��M�T�C�Y���R�s�[
				memcpy( tmpbuff, &rbuf[reqsize], rsize ) ;
				//	���̏������݈ʒu�Ɉړ�
				tmpbuff += rsize ;
			}
		}else{
			break ;
		}
	}

	//	�擪�ɖ߂�
	tmpbuff = back ;

	//	�e���|�����o�b�t�@����R�s�[
	if( ret == 0 ){
		memcpy( buf, tmpbuff, bufsize ) ;
	}

	//	�J��
	free( tmpbuff ) ;
	free( rbuf ) ;

	TraceD("com_readfile2() : com_readfile2�֐���߂�l (%d) �ŏI�����܂��B", ret ) ;

	return ret ;

}

/*-----------------------------------------------------------------------------------------/
									�t�@�C����������

	[�֐���]
		com_writefile

	[����]
		St_SocketInfo	*lpInfo		:	�ʐM�ݒ�
		unsigned long	fno			:	�擾����t�@�C����smf�t�@�C���ԍ�
		unsigned short	rec			:	�������݂��s�����R�[�h�ԍ�(0�̓t�@�C��)
		char			*buf		:	�擾�����t�@�C���̃f�[�^���i�[����o�b�t�@
		unsigned long			bufsize		:	�o�b�t�@�T�C�Y
		char			FileorAddr	:	'F' : �t�@�C���ԍ��ŏ������� 'A' : �A�h���X�ŏ�������

	[�߂�l]
		== 0						:	����
		!= 1						:	�G���[(�G���[�R�[�h)

/-----------------------------------------------------------------------------------------*/
int com_writefile(
	St_SocketInfo *lpInfo
	,unsigned long	fno
	,unsigned long rec
	,char *buf
	,unsigned long bufsize
	,char FileorAddr
	)
{
	ST_DATA			req ;				//	�v���d���p�\����
	ST_DATA			ans ;				//	�����d����M�p�\����
	int				ret = 0 ;			//	�߂�l
	unsigned long	fsize ;				//	�t�@�C���T�C�Y
	unsigned short	fr ;				//	�t���[����
	unsigned short	flag ;				//	�t���O
	unsigned short	reqsize ;			//	�v���d���T�C�Y
	unsigned short	anssize ;			//	�����d���T�C�Y
	unsigned short	frno ;				//	�t���[��No
	unsigned long	total ;				//	�g�[�^���f�[�^�T�C�Y
	unsigned long	p ;					//	�������݃f�[�^�ʒu
	char			*reqbuf ;			//	�v���d���p�o�b�t�@
	unsigned long	maxdatasize = 0 ;	//	�P��ő���M���s����ő�o�b�t�@�T�C�Y(�f�[�^���̂�)
	short			retrycount = 0 ;	//	���g���C��(�t���[�������̍ď������ݎ�)
	int				i = 0 ;				//	���[�v�p�J�E���^
	int				j = 0 ;				//	���[�v�p
	//	2001/09/27�� �C�� --->
	//	unsigned �ɂ��Ă��܂��Ă�������
	//unsigned short			thisskipfr[3] ;		//	�t���[�������d���ŕԂ��Ă����t���[��No
	short			thisskipfr[3] ;		//	�t���[�������d���ŕԂ��Ă����t���[��No
	//unsigned short			skipfr[3] ;			//	�O��̃t���[�������ŋA���Ă����t���[��No
	short			skipfr[3] ;			//	�O��̃t���[�������ŋA���Ă����t���[��No
	//	<--- �����܂�
	short			f_retrycount[3] ;	//	�t���[�������̍ď������݉�(�t���[��No����)
	short			thisretrycount[3] ;	//

	//	�t�@�C�������擾����
	ret = com_infofile( lpInfo, fno, rec, &fsize, &fr, &flag, FileorAddr ) ;
	if( ret != 0 ){
		return ret ;
	}

	//	�o�b�t�@�T�C�Y����v���Ȃ��Ƃ����܂���
	if( bufsize != fsize ){
		TraceF("com_writefile() : �������ރo�b�t�@�̃T�C�Y�ƃt�@�C���̃f�[�^�T�C�Y����v���܂���B �o�b�t�@�T�C�Y=%ld�A�t�@�C���T�C�Y=%ld", bufsize, fsize ) ;
		ret = WSFTERR_BUFFSIZE ;
		TraceD("com_writefile() : com_writefile�֐���߂�l (%d) �ŏI�����܂��B", ret ) ;
		return ret ;
	}

	//	�v���d���Ɖ����d���̃T�C�Y
	reqsize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
	anssize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;

	//	�v���d���p�o�b�t�@�̊m��
	reqbuf = (char*)malloc(lpInfo->maxsize) ;
	if( reqbuf == NULL ){
		ret = WSFTERR_INIT_MALLOC ;
		return ret ;
	}

	//	�������ݏ�������M
	//	�w�b�_�[�̐ݒ�
	//	�v���g�R���o�[�W����
	req.appheader.ver = htons(lpInfo->ver);
	//	�v���d���T�C�Y
	req.appheader.size = htons(reqsize) ;
	//	�R�}���h
	req.cmdheader.command = 'w' ;
	//	�t�@�C���ԍ� or �A�h���X �̎w�� ( 'F' or 'A' )
	req.cmdheader.fileoraddr = FileorAddr ;
	//	�t�@�C���ԍ� or �A�h���X
	req.cmdheader.addr = htonl(fno) ;
	//	���R�[�h�ԍ� or �T�C�Y
	req.cmdheader.rec = htonl(rec) ;
	//	�t���[����
	req.cmdheader.info[0] = htons(fr) ;
	//	����M
	ret = SendAndRecv( lpInfo, (char*)&req, reqsize, (char*)&ans, anssize ) ;
	//	�S���M�T�C�Y
	total = bufsize ;
	//	�P��ő��M����T�C�Y�̃f�[�^�����̃T�C�Y
	maxdatasize = lpInfo->maxsize - reqsize ;

	p = 0 ;
	if( ret == 0 ){
		//	�t���[���m���͂P����
		for( frno=1 ; frno<=fr ; frno++ ){
			//	����̑��M�T�C�Y�̌v�Z
			if( total > maxdatasize ){
				fsize = maxdatasize ;
			}else{
				fsize = total ;
			}
			total = total - fsize ;
			//	��������
			reqsize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) + fsize ;
			anssize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
			//	�w�b�_�[�̐ݒ�
			//	�v���g�R���o�[�W����
			req.appheader.ver = htons(lpInfo->ver);
			//	�v���d���T�C�Y
			req.appheader.size = htons(reqsize) ;
			//	�R�}���h
			req.cmdheader.command = 'D' ;
			//	�t�@�C���ԍ� or �A�h���X �̎w�� ( 'F' or 'A' )
			req.cmdheader.fileoraddr = FileorAddr ;
			//	�t�@�C���ԍ� or �A�h���X
			req.cmdheader.addr = htonl(fno) ;
			//	���R�[�h�ԍ� or �T�C�Y
			req.cmdheader.rec = htonl(rec) ;
			//	�t���[��No
			req.cmdheader.info[0] = htons(frno) ;
			//	������
			memset( reqbuf, 0, lpInfo->maxsize ) ;
			//	�w�b�_�����R�s�[
			memcpy( reqbuf, &req, anssize ) ;
			//	�������݃f�[�^���R�s�[
			memcpy( &reqbuf[anssize], &buf[p], fsize ) ;
			//	����M
			ret = SendAndRecv( lpInfo, reqbuf, reqsize, (char*)&ans, anssize ) ;

			//	���s�����Ƃ��͑��o��
			if( ret != 0 ){
				break ;
			}
			//	���̏������݃f�[�^�ʒu�Ɉړ�
			p = p + fsize ;
		}
	}

	//	�������݊���
	if( ret == 0 ){
		do{
			//	�v���d���Ǝ�M�d���̃T�C�Y���v�Z
			reqsize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
			anssize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
			//	�w�b�_�[�̐ݒ�
			//	�v���g�R���o�[�W����
			req.appheader.ver = htons(lpInfo->ver);
			//	�v���d���T�C�Y
			req.appheader.size = htons(reqsize) ;
			//	�R�}���h
			req.cmdheader.command = 'W' ;
			//	�t�@�C���ԍ� or �A�h���X �̎w�� ( 'F' or 'A' )
			req.cmdheader.fileoraddr = FileorAddr ;
			//	�t�@�C���ԍ� or �A�h���X
			req.cmdheader.addr = htonl(fno) ;
			//	���R�[�h�ԍ� or �T�C�Y
			req.cmdheader.rec = htonl(rec) ;
			//	�t���[����
			req.cmdheader.info[0] = htons(fr) ;
			//	����M
			// FROM�A�N�Z�X�̉\��������̂Ń^�C���A�E�g��15sec��悹
			// 2097152�o�C�g��FROM�t�@�C���������݂ɖ�10sec�K�v�B
			lpInfo->timeout += 15000;
			int to = (lpInfo->timeout > 500) ? lpInfo->timeout - 500 : 1;
			int ret = setsockopt(lpInfo->sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&to, sizeof(to));
			// SetServer���ĂԂƁA�\�P�b�g��j�������ɍč쐬���Ă��܂����ߍ폜�B
//			SetServer(lpInfo, lpInfo->wPort);
			ret = SendAndRecv( lpInfo, (char*)&req, reqsize, (char*)&ans, anssize ) ;
			lpInfo->timeout -= 15000;	// ���ɔ����Ă������Ɩ߂��Ă���
			// SetServer���ĂԂƁA�\�P�b�g��j�������ɍč쐬���Ă��܂����ߍ폜�B
//			SetServer(lpInfo, lpInfo->wPort);
			to = (lpInfo->timeout > 500) ? lpInfo->timeout - 500 : 1;
			ret = setsockopt(lpInfo->sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&to, sizeof(to));

			//	�t���[�������̃G���[���Ԃ��Ă����Ƃ�
			if( ret == WSFTERR_FRAME_SKIP ){
				//	�G���[�R�[�h��߂��܂�
				ret = 0 ;

				//	�����Ă����t���[���ԍ����擾����(�ő�R��)
				for( i=1 ; i<4 ; i++ ){
					thisskipfr[i-1] = -1 ;
					//	�L���ȃt���[���m���̂Ƃ�
					if( htons(ans.cmdheader.info[i]) > 0 && htons(ans.cmdheader.info[i]) <= fr ){
						thisskipfr[i-1] = htons(ans.cmdheader.info[i]) ;
					}else{
						break ;
					}
				}

				//	�����Ă����t���[���̓��e���ēx���M���܂�
				for( i=0 ; i<3 ; i++ ){
					//	�L���ȃt���[���m���̂Ƃ�
					if( thisskipfr[i] != -1 ){
						//	�����Ă����t���[��No�̎擾
						//frno = htons( ans.cmdheader.info[i] ) ;
						frno = thisskipfr[i] ;
						//	����̑��M�T�C�Y�̌v�Z
						if( (frno * maxdatasize) > bufsize ){
							fsize = bufsize - ((frno-1)*maxdatasize) ;
						}else{
							fsize = maxdatasize ;
						}
						//	���M����t���[���̃f�[�^�ʒu
						p = (frno-1)* maxdatasize ;

						//	�������ݗp�̗v���d���Ɖ����d���̃T�C�Y���v�Z
						reqsize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) + fsize ;
						anssize = sizeof(ST_APP_HEADER) + sizeof(ST_COMMAND) ;
						//	�w�b�_�[�̐ݒ�
						//	�v���g�R���o�[�W����
						req.appheader.ver = htons(lpInfo->ver);
						//	�v���d���T�C�Y
						req.appheader.size = htons(reqsize) ;
						//	�R�}���h
						req.cmdheader.command = 'D' ;
						//	�t�@�C���ԍ� or �A�h���X �̎w�� ( 'F' or 'A' )
						req.cmdheader.fileoraddr = FileorAddr ;
						//	�t�@�C���ԍ� or �A�h���X
						req.cmdheader.addr = htonl(fno) ;
						//	���R�[�h�ԍ� or �T�C�Y
						req.cmdheader.rec = htonl(rec) ;
						//	�t���[��No
						req.cmdheader.info[0] = htons(frno) ;
						//	������
						memset( reqbuf, 0, lpInfo->maxsize ) ;
						//	�w�b�_�����R�s�[
						memcpy( reqbuf, &req, anssize ) ;
						//	�������݃f�[�^���R�s�[
						memcpy( &reqbuf[anssize], &buf[p], fsize ) ;
						//	����M
						ret = SendAndRecv( lpInfo, reqbuf, reqsize, (char*)&ans, anssize ) ;

						//	���s�����Ƃ��͑��o��
						if( ret != 0 ){
							break ;
						}
					}else{
						break ;
					}
				}

				if( ret == 0 ){
					//	����̃t���[��������
					for( i=0 ; i<3 ; i++ ){
						thisretrycount[i] = 0 ;
						for( j=0 ; j<3 ; j++ ){
							if( skipfr[j] == thisskipfr[i] ){
								thisretrycount[i] = f_retrycount[j] ;
							}
						}
						thisretrycount[i]++ ;
					}

					retrycount = 0 ;
					for( i=0 ; i<3 ; i++ ){
						skipfr[i] = thisskipfr[i] ;
						f_retrycount[i] = thisretrycount[i] ;
						if( retrycount < f_retrycount[i] ){
							retrycount = f_retrycount[i] ;
						}
					}
					ret = WSFTERR_FRAME_SKIP ;
					//	�܂����g���C�񐔂��ݒ�񐔈ȉ��̂Ƃ��͖߂�l��߂�
					//	2001/09/20�� �C�� --->
					//	���g���C�񐔂��P�񑽂�����
					//if( retrycount <= lpInfo->retrycount ){
					if( retrycount < lpInfo->retrycount ){
					//	<--- �����܂�
						ret = 0 ;
					}
				}
			//	�t���[�������������ꍇ�͏I��
			}else{
				break ;
			}
		}while( ret == 0 ) ;
	}

	//	�m�ۂ����������̉��
	free( reqbuf ) ;

	TraceD("com_writefile() : com_writefile�֐���߂�l (%d) �ŏI�����܂��B", ret ) ;

	return ret ;

}
