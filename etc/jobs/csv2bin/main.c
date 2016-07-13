/* WAIS START */
/*
 *	$Header: /usr/external/CVS/nas2000/tools/csv2bin/main.c,v 1.2 2003/05/15 16:08:06 mizutani Exp $
 *	$Log: main.c,v $
 *	Revision 1.2  2003/05/15 16:08:06  mizutani
 *	float、double型のデータも扱えるようにした。
 *	
 *	Revision 1.1.1.1  1999/12/26 07:03:14  sakurada
 *	1st try to import from VSS
 *	
 * 
 * 10    98/04/28 17:21 Sakurada
 * 
 * 5     98/02/12 14:05 Sakurada
 * WAIS検索キーワード追加
 * 
 * 4     98/01/14 11:24 Sakurada
 * 入力データの行末にコメントを入れるようにする
 * 
 * 1     97/10/31 16:11 Sakurada
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "getopt.h"

extern int optind;
extern char *optarg;
int		bflag = 0;	/*	binary conver	*/
int		vflag = 0;	/*	verbose flag	*/

char	*delim = ",\t\n ";
char	csvfile[32];
char	binfile[32];
char	buf[1024 * 5];
char	rec_dat[1024];

#define MOTOROLA 0
#define INTEL	1



unsigned char	phome = MOTOROLA;
unsigned long	record_num = 1;
unsigned long	record_siz = 4;
unsigned long	count = 0;
FILE *fcsv, *fbin;

struct st {
	short	type;
#define C	1	/*	char	*/
#define S	2	/*	short	*/
#define L	3	/*	long	*/
#define UC	4	/*	unsigned char	*/
#define US	5	/*	unsigned short	*/
#define	UL	6	/*	unsigned long	*/
#define FL	7	/*  float */
#define	DB	8	/*  double */
#define XC	0x81	/* hex char */
#define XS	0x82	/* hex short */
#define XL	0x83	/* hex long */
	short offset;
	short	size;
	short pos;
	struct st *next;
};

struct st *pfirst;

struct _type {
	char	*str;
	short	type;
} type[] = {
	{ "c", C},
	{ "s", S},
	{ "l", L},
	{ "uc", UC},
	{ "us", US},
	{ "ul", UL},
	{ "fl", FL},
	{ "db", DB},
	{ "xc", XC},
	{ "xs", XS},
	{ "xl", XL}
};

int num_type = (sizeof(type) / sizeof(struct _type));

void usage(void)
{
	printf("usage: csv2bin [-b csvfile] [-m binfile] [-i][-v]\n");
	printf("\n");
	printf(" -b csvfile  ... 変換元CSVファイル\n");
	printf(" -m binfile  ... 出力先BINファイル(省略可)\n");
	printf(" -i          ... リトルエンディアンで出力\n");
	printf(" -v          ... verbose mode\n");
	printf("\n");

	exit(1);
}


int atoi_x(char s[], int xsw)
{
	int		i,n,w;
	char	*p;

	if(s[1] == 'x' || s[1] == 'X')
		p = &s[2];
	else
		p = &s[0];

	if (xsw == 0) return (atoi(s));

	n = 0;
	for(i=0; i<16 ;++i) {
		if		(('0' <= p[i]) && (p[i] <= '9')) w = p[i] - '0';
		else if (('A' <= p[i]) && (p[i] <= 'F')) w = p[i] - 'A' + 10;
		else if (('a' <= p[i]) && (p[i] <= 'f')) w = p[i] - 'a' + 10;
		else break;

		n = n * 0x10 + w;
	}

	return n;

}

long htonl(long x)
{
	char a[4], b;
	long y;

	memcpy(a, &x, 4);

	b = a[0];
	a[0] = a[3];
	a[3] = b;

	b = a[1];
	a[1] = a[2];
	a[2] = b;

	y = *(long *)&a[0];

	return y;
}

short htons(short x)
{
	char a[2], b;
	short y;

	memcpy(a, &x, 2);

	b = a[0];
	a[0] = a[1];
	a[1] = b;

	y = *(short *)&a[0];

	return y;
}

int rec_set (void *s, short offset, short size)
{
	short			i;
	unsigned char	*sdat;

	sdat = (unsigned char *)s;

	for(i=0; i<size; i++) {
		rec_dat[offset+i] |= sdat[i];
	}

	return (0);
}

/*	１行読んで、データをセット	*/
int read_data(void)
{
	char *p;
	struct st *pst;
	char c[32];
	unsigned char uc[32];
	short s;
	unsigned short us;
	long l;
	unsigned long ul;
	float fl;
	double	db;
	
	int n;
	short	size;
	int wtype, xsw;	

	pst = pfirst;
	memset(rec_dat, 0, sizeof(rec_dat));
	

	/*	行末またはデータメンバがなくなるまでループ	*/
	while((p = strtok(NULL, delim)) != NULL && pst->next != 0)
	{
		wtype = pst->type & 0x7f;
		xsw = pst->type & 0x80;

		switch(wtype)
		{
		case C:
			if(pst->size > 1)
			{
				memset(c, 0, sizeof(c));
				size = min(sizeof(c), strlen(p));
				memcpy(c, p, size);
			}
			else
			{
				c[0] = (char)atoi_x(p,xsw) << pst->pos;
			}
			n = rec_set (&c[0], pst->offset, pst->size);
			break;
		case UC:
			if(pst->size > 1)
			{
				memset(uc, 0, sizeof(uc));
				size = min(sizeof(uc), strlen(p));
				memcpy(uc, p, size);
			}
			else
			{
				uc[0] = (unsigned char)atoi_x(p,xsw) << pst->pos;
			}
			n = rec_set (&uc[0], pst->offset, pst->size);
			break;
		case S:
			if(phome == INTEL)
				s = (atoi_x(p,xsw) << pst->pos);
			else
				s = htons((short)(atoi_x(p,xsw) << pst->pos));
			n = rec_set (&s, pst->offset, pst->size);
			break;
		case US:
			if(phome == INTEL)
				us = (unsigned short)(atoi_x(p,xsw) << pst->pos);
			else
				us = htons((unsigned short)(atoi_x(p,xsw) << pst->pos));
			n = rec_set (&us, pst->offset, pst->size);
			break;
		case L:
			if(phome == INTEL)
				l = (long)(atoi_x(p,xsw) << pst->pos);
			else
				l = htonl((long)(atoi_x(p,xsw) << pst->pos));
			n = rec_set (&l, pst->offset, pst->size);
		/*	n = fwrite(&l, 4, 1, fbin);	*/
			break;
		case UL:
			if(phome == INTEL)
				ul = (unsigned long)(atoi_x(p,xsw) << pst->pos);
			else
				ul = htonl((unsigned long)(atoi_x(p,xsw) << pst->pos));
			n = rec_set (&ul, pst->offset, pst->size);
			break;
		case FL:
			if(phome == INTEL)
				/* fl = (float)(atoi_x(p,xsw) << pst->pos); */
				   fl = (float)atof(p);
			else
				fl = (float)atof(p);
			n = rec_set (&fl, pst->offset, pst->size);
			break;
		
		case DB:
			if(phome == INTEL)
				db = (double)atof(p);
			else
				db = (double)atof(p);
			n = rec_set (&db, pst->offset, pst->size);
			break;
			
		default:
			if(vflag)
				printf("set_data: type mismatch (%d)\n", pst->type);
			return -1;
		}
		pst = pst->next;
	}

	n = fwrite(rec_dat, 1, record_siz, fbin);

	return 0;
}

int set_type(void)
{
	int i;
	char *p;
	struct st *pst;

	pst = pfirst;

	p = strtok(NULL, delim);

	while((p = strtok(NULL, delim)) != NULL)
	{
		for(i = 0; i < num_type; i++)
		{
			if(strcmp(p, type[i].str) == 0)
			{
				pst->type = type[i].type;
				if(pst->next == 0)
				{
					pst->next = (struct st *)malloc(sizeof(struct st));
					pst = pst->next;
					pst->pos = 0;
					pst->next = 0;
				}
				else
				{
					pst = pst->next;
				}
				
				goto next;
			}
		}
		/*	type mismatch	*/
		if(vflag)
			printf("set_type: type mismatch\n");
			
		return -1;
next:
		;	/*	next str	*/
	}

	return 0;
}

int set_offset(void)
{
	int i;
	char *p;
	struct st *pst;

	pst = pfirst;

	p = strtok(NULL, delim);

	while((p = strtok(NULL, delim)) != NULL)
	{
		for(i = 0; i < num_type; i++)
		{
			pst->offset = (short)atoi(p);

			if(pst->next == 0)
			{
				pst->next = (struct st *)malloc(sizeof(struct st));
				pst = pst->next;
				pst->pos = 0;
				pst->next = 0;
			}
			else
			{
				pst = pst->next;
			}

			goto next;
		}
		/*	offset mismatch	*/
		if(vflag)
			printf("set_offset: type mismatch\n");

		return -1;
next:
		;	/*	next str	*/
	}

	return 0;
}

int set_size(void)
{
	int i;
	char *p;
	struct st *pst;

	pst = pfirst;

	p = strtok(NULL, delim);

	while((p = strtok(NULL, delim)) != NULL)
	{
		for(i = 0; i < num_type; i++)
		{
			pst->size = (short)atoi(p);

			if(pst->next == 0)
			{
				pst->next = (struct st *)malloc(sizeof(struct st));
				pst = pst->next;
				pst->pos = 0;
				pst->next = 0;
			}
			else
			{
				pst = pst->next;
			}
				
			goto next;
		}
		/*	type mismatch	*/
		if(vflag)
			printf("set_size: type mismatch\n");
			
		return -1;
next:
		;	/*	next str	*/
	}

	return 0;
}

int set_pos(void)
{
	int i;
	char *p;
	struct st *pst;

	pst = pfirst;

	p = strtok(NULL, delim);

	while((p = strtok(NULL, delim)) != NULL)
	{
		for(i = 0; i < num_type; i++)
		{
			pst->pos = (short)atoi(p);

			if(pst->next == 0)
			{
				pst->next = (struct st *)malloc(sizeof(struct st));
				pst = pst->next;
				pst->next = 0;
			}
			else
			{
				pst = pst->next;
			}

			goto next;
		}
		/*	pos mismatch	*/
		if(vflag)
			printf("set_pos: type mismatch\n");

		return -1;
next:
		;	/*	next str	*/
	}

	return 0;
}



int csv2bin(void)
{
	char	*p;
	int ret = 0, n;

	fcsv = fopen(csvfile, "r");
	if(fcsv == NULL)
	{
		printf("csv file not found\n");
		return -1;
	}

	fbin = fopen(binfile, "w+b");
	if(fbin == NULL)
	{
		printf("bin file open error\n");
		return -1;
	}


	pfirst = (struct st *)malloc(sizeof(struct st));
	pfirst->next = 0;
	pfirst->pos = 0;

	count = 0;

	while(!feof(fcsv))
	{
		ret = 0;
		memset(buf, 0, sizeof(buf));
		
		p = fgets(buf, sizeof(buf), fcsv);
		if(p == NULL)
		{
			printf("file read error\n");
			return -1;
		}

		p = strtok(buf, delim);
		switch(*p)
		{
		case '#':	/*	コマンド	*/
			p++;
			switch(*p)
			{
			case 'C':	/*	コメント	*/
				break;
			case 'F':	/*	ファイル名	*/
				break;
			case 'N':	/*	件数・サイズ */
				strtok(NULL, delim);
				p = strtok(NULL, delim);
				record_num = atoi(p);
				p = strtok(NULL, delim);
				record_siz = atoi(p);
				break;
			case 'O':	/*	オフセット	*/
				ret = set_offset();
				if(ret < 0)
					goto bad;
				break;
			case 'T':	/*	データ型	*/
				ret = set_type();
				if(ret < 0)
					goto bad;
				break;
			case 'S':
				ret = set_size();
				if(ret < 0)
					goto bad;
				break;
			case 'P':	/* ﾋﾞｯﾄ位置･ｼﾌﾄ数 */
				ret = set_pos();
				if(ret < 0)
					goto bad;
				break;
			case 'D':	/*	データ	*/
				ret = read_data();
				if(ret < 0)
					goto bad;
				count++;
				if(record_num <= count)	goto bad;
				break;
			case 'E':	/*	終了	*/
				memset(rec_dat, 0, record_siz);
				for(;count<record_num;count++){
					n = fwrite(rec_dat, 1, record_siz, fbin);
				}
				goto bad;
			default:
				;
			}
			break;
		default:
			;
		}
	}

bad:
	fclose(fcsv);
	fclose(fbin);

	return ret;
}

void main(int argc, char **argv)
{
	int	ch;
	int	ret;

	while((ch = getopt(argc, argv, "b:m:ivh?")) != EOF)
	{
		switch(ch)
		{
		
		case 'b':
			if(optarg != NULL)
			{
				strcpy(csvfile, optarg);
				if(!strstr(csvfile, "."))
					strcat(csvfile, ".csv");
				strcpy(binfile, optarg);
				strcat(binfile, ".bin");
				bflag = 1;
			}
			else
				usage();
			break;
		case 'i':
			phome = INTEL;
			if(optarg != NULL)
			{
				memset(binfile, 0, sizeof(binfile));
				strcpy(binfile, optarg);
				strcat(binfile, ".bin");
			}
			break;
			
		case 'm':
			if(optarg != NULL)
			{
				memset(binfile, 0, sizeof(binfile));
				strcpy(binfile, optarg);
				strcat(binfile, ".bin");
			}
			break;
		
		case 'v':	/*	verbose	*/
			vflag = 1;
			break;

		default:
			usage();
		}
	}

	argv += optind;
	argc -= optind;

	if(bflag)
	{
		ret = csv2bin();
	}

	if(ret < 0)
		printf("execution not completed\n");
}

/* WAIS END */
