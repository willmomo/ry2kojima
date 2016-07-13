#include "tinylookup.h"
#include "__os__.h"

#if defined(_WIN32)

#pragma warning(disable:4996)

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#elif defined(__linux__)

#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define WSAGetLastError()	(errno)
#define closesocket	close
#define INVALID_SOCKET (-1)
typedef int SOCKET;
typedef unsigned long DWORD;

#elif defined(_OS9000)

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <modes.h>
#include <UNIX\os9def.h>
#include <ctype.h>
#include <stdlib.h>
#include <sg_codes.h>
#include <spf\spf.h>
#include <net\if.h>
#include <sys\sockio.h>

#else
#error unknown system.
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>

#if defined(_WIN32)
#pragma pack(1)
#endif

typedef struct _NBT_HEADER {
	unsigned short m_Transaction_ID;
	unsigned short m_Flags; /* 0x0110 */
	unsigned short m_Questions;
	unsigned short ANCOUNT;
	unsigned short NSCOUNT;
	unsigned short ARCOUNT;
	unsigned char buf[1 + 0x20 + 1 + 4];
} NBT_HEADER;


/*
   Character   ASCII Code    Hex Code
   ----------------------------------

   A            EB            45 42
   B            EC            45 43
   C            ED            45 44
   D            EE            45 45
   E            EF            45 46
   F            EG            45 47
   G            EH            45 48
   H            EI            45 49
   I            EJ            45 4A
   J            EK            45 4B
   K            EL            45 4C
   L            EM            45 4D
   M            EN            45 4E
   N            EO            45 4F
   O            EP            45 50
   P            FA            46 41
   Q            FB            46 42
   R            FC            46 43
   S            FD            46 44
   T            FE            46 45
   U            FF            46 46
   V            FG            46 47
   W            FH            46 48
   X            FI            46 49
   Y            FJ            46 4A
   Z            FK            46 4B

   0            DA            44 41
   1            DB            44 42
   2            DC            44 43
   3            DD            44 44
   4            DE            44 45
   5            DF            44 46
   6            DG            44 47
   7            DH            44 48
   8            DI            44 49
   9            DJ            44 4A

   <space>      CA            43 41
   !            CB            43 42
   "            CC            43 43
   #            CD            43 44
   $            CE            43 45
   %            CF            43 46
   &            CG            43 47
   '            CH            43 48
   (            CI            43 49
   )            CJ            43 4A
   *            CK            43 4B
   +            CL            43 4C
   ,(comma)     CM            43 4D
   -(hyphen)    CN            43 4E
   .(period)    CO            43 4F
   =            DN            44 4E
   :(colon)     DK            44 4B
   ;(semicolon) DL            44 4C
   @            EA            45 41
   ^            FO            46 4F
   _(underscore)FP            46 50
   {            HL            48 4C
   }            HN            48 4E
   ~            HO            48 4F
				
*/
void netbios_encode(const char* psrc, char* pdst) {
	const char* pt;
	unsigned short pp;
	char* qq;

	qq = pdst;
	pt = psrc;
	while (*pt != 0) {
		if ('A' <= *pt && *pt <= 'O') {
			pp = 0x4542 + (*pt - 'A');
		} else if ('P' <= *pt && *pt <= 'Z') {
			pp = 0x4641 + (*pt - 'P');
		} else if ('0' <= *pt && *pt <= '9') {
			pp = 0x4441 + (*pt - '0');
		} else if (' ' <= *pt && *pt <= '.') {
			pp = 0x4341 + (*pt - ' ');
		} else if (*pt == '=') {
			pp = 0x444e;
		} else if (*pt == ':') {
			pp = 0x444b;
		} else if (*pt == ';') {
			pp = 0x444c;
		} else if (*pt == '@') {
			pp = 0x4541;
		} else if (*pt == '^') {
			pp = 0x464f;
		} else if (*pt == '_') {
			pp = 0x4650;
		} else if (*pt == '{') {
			pp = 0x484c;
		} else if (*pt == '}') {
			pp = 0x484e;
		} else if (*pt == '~') {
			pp = 0x484f;
		} else {
			pp = 0;
		}
		/*pp = htons(pp);*/

		qq[0] = (char)((pp & 0xFF00) >> 8);
		qq[1] = (char)(pp & 0x00FF);

		qq+=2;
		++pt;
	}
}

/*---------------------------------------------------------------------
	ホスト名から、IPアドレスを取得する
---------------------------------------------------------------------*/
static unsigned long get_ipaddr(const char *hostname) {
	unsigned long result;
	struct hostent* phe;

	result = inet_addr(hostname);
	if (result == INADDR_NONE) {
		phe = gethostbyname(hostname);
		if (phe) {
			result = *(unsigned long*)phe->h_addr;
		}
	}

	return result;
}

/*---------------------------------------------------------------------
	受信タイムアウトの設定
---------------------------------------------------------------------*/
static void _setrecvtimeo(SOCKET sock, DWORD to) {
#if defined(_WIN32)
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&to, sizeof(to));
#elif defined(__linux__)
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&to, sizeof(to));
#elif defined(_OS9000)
	struct spf_popts opt;
	u_int32 opt_size;

	opt_size = sizeof(opt);
	_os_gs_popt(sock, &opt_size, &opt);
	opt.pd_iotime = to / 10;		/* 受信タイムアウト値(×10ms)を設定 */
	_os_ss_popt(sock, opt_size, &opt);
#else
#error unknown system.
#endif
}

int getmyaddr() {
#if defined(_OS9000)
	int                s;
	struct sockaddr_in *sin;
	struct ifreq       ifr;

	s = socket(AF_INET, SOCK_RAW, 0);
	if(s == -1){
		exit(1);
	}

	strncpy(ifr.ifr_name, "enet0", sizeof (ifr.ifr_name));
	ioctl(s, SIOCGIFADDR, (caddr_t)&ifr);
	_os_close(s);

	sin = (struct sockaddr_in *)&ifr.ifr_addr;

	return(sin->sin_addr.s_addr);
#else
    char ac[80];
	struct hostent *phe;
	int i;
    struct in_addr addr;

    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		printf("Error %d when getting local host name.\n", WSAGetLastError());
        return 1;
    }
	printf("Host name is %s.\n", ac);

    phe = gethostbyname(ac);
    if (phe == 0) {
		printf("Yow! Bad host lookup.\n");
        return 1;
    }

    for (i = 0; phe->h_addr_list[i] != 0; ++i) {
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		printf("Address %d: %s\n", i, inet_ntoa(addr));

		return addr.S_un.S_addr;
    }
    
    return 0;
#endif
}

void parse_NBNS(void* p, struct sockaddr_in* f) {
#if 0
	printf("return from %d.%d.%d.%d\n", f->sin_addr.S_un.S_un_b.s_b1, f->sin_addr.S_un.S_un_b.s_b2, f->sin_addr.S_un.S_un_b.s_b3, f->sin_addr.S_un.S_un_b.s_b4);
#else
	printf("return from %d.%d.%d.%d\n",
		((f->sin_addr.s_addr & 0xFF000000) >> 24), ((f->sin_addr.s_addr & 0x00FF0000) >> 16), ((f->sin_addr.s_addr & 0x0000FF00) >> 8), (f->sin_addr.s_addr & 0x000000FF));
#endif
	return;
}

int tinylookup_main(SOCKET s, char* name) {
	int i;
	int result = 0;
	struct sockaddr_in my;
	struct sockaddr_in to;
	struct sockaddr_in from;
	socklen_t fromlen = sizeof(from);
	int ret;
	char rbuf[8192];
	void *p = NULL;
	NBT_HEADER *pp;
	u_long host_addr;
	u_long net_mask;
	u_long net_addr;
	u_long dir_bcast_addr;

	memset(&my, 0, sizeof(my));
	my.sin_family = AF_INET;
	my.sin_addr.s_addr = htonl(INADDR_ANY);
	my.sin_port = htons(137);
	/*bind(s, (struct sockaddr*)&my, sizeof(my));*/

	/*host_addr = inet_addr("192.168.120.3");*/
	host_addr = getmyaddr();
	net_mask = inet_addr("255.255.255.0");
	net_addr = host_addr & net_mask;
	dir_bcast_addr = net_addr | (~net_mask);

	memset(&to, 0, sizeof(to));
	to.sin_family = AF_INET;
	to.sin_addr.s_addr = dir_bcast_addr;
	to.sin_port = htons(137);

	p = malloc(sizeof(NBT_HEADER));
	memset(p, 0, sizeof(NBT_HEADER));

	pp = (NBT_HEADER*)p;
	pp->m_Transaction_ID = (unsigned short)(GetTickCount() & 0xffff);
	pp->m_Flags = htons(0x0110);
	pp->m_Questions = htons(1);

	pp->buf[0] = 0x20;
	for (i = 0; i < 0x20; i += 2) {
		pp->buf[1 + i + 0] = 0x43;
		pp->buf[1 + i + 1] = 0x41;
	}
	netbios_encode(name, (char*)&pp->buf[1]);
	pp->buf[0x1f] = 'A';
	pp->buf[0x20] = 'A';
	pp->buf[33] = 0x00;
	pp->buf[34] = 0x00;
	pp->buf[35] = 0x20;
	pp->buf[36] = 0x00;
	pp->buf[37] = 0x01;
	sendto(s, (char*)p, sizeof(NBT_HEADER), 0, (struct sockaddr*)&to, sizeof(to));

	/* タイムアウト時間を設定する */
	_setrecvtimeo(s, 1000);

	memset(&from, 0, sizeof(from));
	fromlen = sizeof(from);
	ret = recvfrom(s, (char*)&rbuf, sizeof(rbuf), 0, (struct sockaddr*)&from, &fromlen);
	if (ret > 0) {
		parse_NBNS(rbuf, &from);
	} else {
		printf("受信タイムアウト\n");
		result = -10;
	}

	free(p);

	return result;
}

int tinylookup() {
	int result = 0;
	SOCKET s;
	int option;

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) != INVALID_SOCKET) {
		option = 1;
		if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &option, sizeof(option)) != -1){
			/*result = tinylookup_main(s, "LANDISK-B7C184");*/
			result = tinylookup_main(s, "WSS");
			closesocket(s);
		}else{
			printf("setsockopt error: %d\n", WSAGetLastError());
			result = -1;
		}
	} else {
		printf("socket error: %d\n", WSAGetLastError());
		result = -1;
	}

	return result;
}

void socket_startup() {
#if defined(_WIN32)
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif
}

void socket_cleanup() {
#if defined(_WIN32)
	WSACleanup();
#endif
}

int main() {
	socket_startup();

	tinylookup();

	socket_cleanup();
	return 0;
}
