#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <i86.h>
#include <ctype.h>

#define KIRO		1024l
#define MEGA		(KIRO * KIRO)

#define DEF_FROM_SIZE	(16l * MEGA)
#define DEF_SRAM_SIZE	(8l * MEGA)

#define IOADDR		0x320
#define MEMADDR		MK_FP(0xd000, 0x0000)
#define BANK_SIZE	(32l * KIRO)
#define FROM_TOP_VADDR	0l
#define FROM_SIZE	g_lFromSize
#define FROM_END_VADDR	(FROM_TOP_VADDR+FROM_SIZE)
#define SRAM_TOP_VADDR	0x1000000l
#define SRAM_SIZE	g_lSramSize
#define SRAM_END_VADDR	(SRAM_TOP_VADDR+SRAM_SIZE)
#define FROM_TOP_BANK	(FROM_TOP_VADDR/BANK_SIZE)
#define FROM_END_BANK	(FROM_END_VADDR/BANK_SIZE)
#define SRAM_TOP_BANK	(SRAM_TOP_VADDR/BANK_SIZE)
#define SRAM_END_BANK	(SRAM_END_VADDR/BANK_SIZE)

#define	FROM_BLK_SIZE	0x10000l	// FROMブロックサイズ(64KB)
#define	FROM_ERASE_CNT	3000000l	// Eraseウェイトカウント数
#define	FROM_WRITE_CNT	1000		// Writeウェイトカウント数

long g_lFromSize = DEF_FROM_SIZE;
long g_lSramSize = DEF_SRAM_SIZE;

//////////////////////////////////////////////////////////////////////////
// 仮想アドレスからバンクの切り替えとfarポインタの取得

unsigned char __far* ChgBank(long lVAddr)
{
	printf("VADDR: %08lx ", lVAddr);
  int nBank = (int)(lVAddr / BANK_SIZE);	// バンク番号
  int nOffs = (int)(lVAddr % BANK_SIZE);	// バンク内のオフセット

  outp(IOADDR + 0, nBank & 0xff);		// バンク番号の下位1バイト
  outp(IOADDR + 1, 0x20 | (nBank >> 8));	// バンク番号の上位1バイト
  return (unsigned char __far *)MK_FP(0xd000, nOffs);
}

//////////////////////////////////////////////////////////////////////////
// FROMクリア処理

int _from_erase(unsigned short __far* adrs)
{
	unsigned long cnt;
	volatile unsigned short __far* adr = adrs;

	*adr = 0x2020;	// シングル・ブロック・イレーズ
	*adr = 0xd0d0;	// コンファーム/レジューム

	for (cnt = 0; (cnt < FROM_ERASE_CNT) && ((*adr & 0x8080) != 0x8080); cnt++)
		;

	if ((*adr & 0x8080) != 0x8080) {	// WSM Busy?
		*adr = 0x5050;		// クリア・ステータス・レジスタ
		printf("ERASE WSM Error\n");
		return -1;
	}

	if (*adr & 0x2020) {		// Erase Error?
		*adr = 0x5050;		// クリア・ステータス・レジスタ
		printf("Erase Error Code=$%04x\n", *adr);
		return -2;
	}

	*adr = 0x5050;		// クリア・ステータス・レジスタ
	printf("read = 0x%08lx: 0x%04x\n", (unsigned long)adr, (unsigned short)*adr);
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// FROM書き込み処理

int _from_write(unsigned char __far* adrs, unsigned char val) {
  long cnt;
  volatile unsigned char __far *adr = adrs;

  *adr = 0x10;			/* ワード/バイトライト */
  *adr = val;
  for (cnt = 0; (cnt < FROM_WRITE_CNT) && !(*adr & 0x80); cnt++)
    ;

  if (!(*adr &0x80)) {	/* WSM Busy ? */
    *adr = 0x50;		/* クリア・ステータス・レジスタ */
    printf("Write WSM Error\n");
    return -1;
  }

  if (*adr & 0x10) {	/* Erase Error ?	*/
    *adr = 0x50;		/* クリア・ステータス・レジスタ */
    printf("Write Error\n");
    return -2;
  }

  /* Normal Term */
  *adr = 0x50;			/* クリア・ステータス・レジスタ */
  
	printf("write = 0x%08lx: 0x%04x\n", (unsigned long)adr, *(unsigned short*)adr);
  return 0;
}

//////////////////////////////////////////////////////////////////////////
// FROMをFFクリアする

void FromClear()
{
  long lVAddr;

  printf("FROM:FF clear ... ");
  fflush(stdout);
  for (lVAddr = FROM_TOP_VADDR; 
       lVAddr < FROM_END_VADDR; lVAddr += (FROM_BLK_SIZE * 2))
    _from_erase((unsigned short __far*)ChgBank(lVAddr));
  puts("end");
}

//////////////////////////////////////////////////////////////////////////
// FROMデータテスト

int FromDataTest(unsigned char bData)
{
  long lVAddr;
  volatile unsigned char __far* p;

  printf("FROM data line test:$%02x:$%08lx～$%08lx:", 
	 bData, FROM_TOP_VADDR, FROM_END_VADDR);

  for (lVAddr = FROM_TOP_VADDR; lVAddr < FROM_END_VADDR; lVAddr++) {
    if ((lVAddr % (512l * KIRO)) == 0) {
      printf("#");
      fflush(stdout);
    }

    _from_write((unsigned char __far*)(p = ChgBank(lVAddr)), bData);
    if (*p != bData) {
      printf("\nData line error $%08lx ($%02x)\n", lVAddr, *p);
      return -1;
    }
  }
  puts("");

  return 0;
}

//////////////////////////////////////////////////////////////////////////
// FROMアドレステスト

int FromAddrTest()
{
  long n = 1;
  long al;

  printf("FROM address line test:$%08lx～$%08lx\n", 
	 FROM_TOP_VADDR, FROM_END_VADDR);

  for (al = 0l; 
       al < 32 && (FROM_TOP_VADDR + (1l << al)) < FROM_END_VADDR; al++) {

    long testAddr = FROM_TOP_VADDR + (1l << al);
    printf("  Address 0x%08lx:", testAddr);
    _from_write(ChgBank(testAddr), (unsigned char)al);

    // アドレスラインが機能していることを確認する
    if (*ChgBank(FROM_TOP_VADDR) == (unsigned char)al) {
      printf("\nSame data, $%08lx and $%08lx(1)\n", 
	     testAddr, FROM_TOP_VADDR);
      return -1;
    }

    for (n = 1; n < FROM_SIZE; n++) {
      if ((n % (512l * KIRO)) == 0) {
	printf("#");
	fflush(stdout);
      }

      long cmpAddr = testAddr + (n << (al + 1));
      if (cmpAddr >= FROM_END_VADDR)
	break;

      if (*ChgBank(cmpAddr) == (unsigned char)al) {
	printf("\nSame data, $%08lx and $%08lx(2)\n", 
	       testAddr, cmpAddr);
	return -1;
      }
    }
    puts("");
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////
// SRAMを0クリアする

void SramClear()
{
  long lVAddr;

  printf("SRAM:0 clear ... ");
  fflush(stdout);
  for (lVAddr = SRAM_TOP_VADDR; lVAddr < SRAM_END_VADDR; lVAddr += BANK_SIZE)
    _fmemset(ChgBank(lVAddr), 0, BANK_SIZE);
  puts("end");
}

//////////////////////////////////////////////////////////////////////////
// データテスト

int SramDataTest(unsigned char bData)
{
  long lVAddr;
  volatile unsigned char __far* p;

  printf("SRAM data line test:$%02x:$%08lx～$%08lx:", 
	 bData, SRAM_TOP_VADDR, SRAM_END_VADDR);

  for (lVAddr = SRAM_TOP_VADDR; lVAddr < SRAM_END_VADDR; lVAddr++) {
    if ((lVAddr % (512l * KIRO)) == 0) {
      printf("#");
      fflush(stdout);
    }

    *(p = ChgBank(lVAddr)) = bData;	// バンク切り替え&データ書き込み
    if (*p != bData) {
      printf("\nData error, $%08lx ($%02x)\n", lVAddr, *p);
      return -1;
    }
  }
  puts("");

  return 0;
}

//////////////////////////////////////////////////////////////////////////
// アドレステスト

int SramAddrTest()
{
  long n = 1;
  long al;

  printf("SRAM Address line test:$%08lx～$%08lx\n", 
	 SRAM_TOP_VADDR, SRAM_END_VADDR);

  for (al = 0l; 
       al < 32 && (SRAM_TOP_VADDR + (1l << al)) < SRAM_END_VADDR; al++) {

    long testAddr = SRAM_TOP_VADDR + (1l << al);
    printf("  Address 0x%08lx:", testAddr);
    *ChgBank(testAddr) = 0xff;

    // アドレスラインが機能していることを確認する
    if (*ChgBank(SRAM_TOP_VADDR) == 0xff) {
      printf("\nSame data, $%08lx and $%08lx(1)\n", 
	     testAddr, SRAM_TOP_VADDR);
      return -1;
    }

    for (n = 1; n < SRAM_SIZE; n++) {
      if ((n % (512l * KIRO)) == 0) {
	printf("#");
	fflush(stdout);
      }

      long cmpAddr = testAddr + (n << (al + 1));
      if (cmpAddr >= SRAM_END_VADDR)
	break;

      if (*ChgBank(cmpAddr) == 0xff) {
	printf("\nSame data, $%08lx and $%08lx(2)\n", 
	       testAddr, cmpAddr);
	return -1;
      }
    }
    puts("");

    *ChgBank(testAddr) = 0;
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////
// 情報表示

void PrintInfo()
{
  printf("FROM address:$%08lx(size:", FROM_TOP_VADDR);
  if (FROM_SIZE >= MEGA && (FROM_SIZE % MEGA) == 0)
    printf("%ldM)\n", FROM_SIZE / MEGA);
  else if (FROM_SIZE >= KIRO && (FROM_SIZE % KIRO) == 0)
    printf("%ldK)\n", FROM_SIZE / KIRO);
  else
    printf("%ldB)\n", FROM_SIZE);

  printf("SRAM address:$%08lx(size:", SRAM_TOP_VADDR);
  if (SRAM_SIZE >= MEGA && (SRAM_SIZE % MEGA) == 0)
    printf("%ldM)\n", SRAM_SIZE / MEGA);
  else if (SRAM_SIZE >= KIRO && (SRAM_SIZE % KIRO) == 0)
    printf("%ldK)\n", SRAM_SIZE / KIRO);
  else
    printf("%ldB)\n", SRAM_SIZE);

  puts("");
}

//////////////////////////////////////////////////////////////////////////
// 使用方法

void usage()
{
  printf("IM01SELF Ver.1.00\n");
  printf("usage : im01self\n");
  printf("/FS <size>\tfrom size\n");
  printf("/SS <size>\tsram size\n");

  exit(0);
}

//////////////////////////////////////////////////////////////////////////
// オプションのチェック

void chkarg(int argc, char** argv)
{
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' || argv[i][0] == '/') {
      if (toupper(argv[i][1]) == 'S') {
	if (toupper(argv[i][2]) == 'S') {
	  if (argv[i][3] != '\0') {
	    char* endp;

	    g_lSramSize = strtol(&argv[i][3], &endp, 0);
	    if (toupper(*endp) == 'K') {
	      g_lSramSize *= 1024l;
	    } else if (toupper(*endp) == 'M') {
	      g_lSramSize *= (1024l * 1024l);
	    } else if (*endp != '\0') {
	      usage();
	    }
	  } else {
	    if (i + 1 < argc) {
	      char* endp;
	     
	      ++i;
	      g_lSramSize = strtol(&argv[i][0], &endp, 0);
	      if (toupper(*endp) == 'K') {
		g_lSramSize *= 1024l;
	      } else if (toupper(*endp) == 'M') {
		g_lSramSize *= (1024l * 1024l);
	      } else if (*endp != '\0') {
		usage();
	      }
	    } else {
	      usage();
	    }
	  }
	} else {
	  usage();
	}
      } else if (toupper(argv[i][1]) == 'F') {
	if (toupper(argv[i][2]) == 'S') {
	  if (argv[i][3] != '\0') {
	    char* endp;

	    g_lFromSize = strtol(&argv[i][3], &endp, 0);
	    if (toupper(*endp) == 'K') {
	      g_lFromSize *= 1024l;
	    } else if (toupper(*endp) == 'M') {
	      g_lFromSize *= (1024l * 1024l);
	    } else if (*endp != '\0') {
	      usage();
	    }
	  } else {
	    if (i + 1 < argc) {
	      char* endp;
	     
	      ++i;
	      g_lFromSize = strtol(&argv[i][0], &endp, 0);
	      if (toupper(*endp) == 'K') {
		g_lFromSize *= 1024l;
	      } else if (toupper(*endp) == 'M') {
		g_lFromSize *= (1024l * 1024l);
	      } else if (*endp != '\0') {
		usage();
	      }
	    } else {
	      usage();
	    }
	  }
	} else {
	  usage();
	}
      } else {
	usage();
      }
    } else {
      usage();
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// プログラム スタート

int main(int argc, char** argv)
{
  volatile unsigned char __far* p;
	_from_erase((unsigned short __far*)ChgBank(0x400000));
    _from_write((unsigned char __far*)(p = ChgBank(0x400000)), 0x43);
    _from_write((unsigned char __far*)(p = ChgBank(0x400001)), 0x21);
	_from_erase((unsigned short __far*)ChgBank(0x400000));
	return 0;
	
  chkarg(argc, argv);
  PrintInfo();

  // FRPM系チェック

  FromClear();

  if (FromDataTest(0x55) != 0)
    return -1;

  FromClear();

  if (FromDataTest(0xaa) != 0)
    return -1;

  FromClear();

  if (FromAddrTest() != 0)
    return -1;

  printf("FROM check OK\n");

  // SRAM系チェック

  SramClear();

  if (SramDataTest(0x55) != 0)
    return -1;
  if (SramDataTest(0xaa) != 0)
    return -1;

  SramClear();

  if (SramAddrTest() != 0)
    return -1;

  printf("SRAM check OK\n");

  return 0;
}
