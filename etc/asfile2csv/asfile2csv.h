#pragma once

#include <winsock2.h>
#include <iostream>
#include <fstream>

#define __STDC__	1
#include "getopt.h"
#include "regex.h"
#undef __STDC__

#include "rklib.h"

#include "asheader/sys_hed.h"
#include "asheader/set_sys.h"
#include "asheader/set_gyou.h"
#include "asheader/dat_file.h"

typedef struct {
	int fno;
	int (*conv_csv)(const char*, bool, int);
} CONV_FUNC;

typedef struct {
	bool raw;
	int endian;		// 0=little 1=big
	int fno;
} ASFILE2CSV_OPTION;

// プロトタイプ

// asfile2csv.cpp
short ntoh(short n);
unsigned short ntoh(unsigned short n);
int ntoh(int n);
unsigned int ntoh(unsigned int n);
long ntoh(long n);
unsigned long ntoh(unsigned long n);

// conv1000.cpp
int conv1001(const char* fname, bool raw, int endian);
int conv1002(const char* fname, bool raw, int endian);

// conv1100.cpp
int conv1103(const char* fname, bool raw, int endian);
int conv1106(const char* fname, bool raw, int endian);

// conv4100.cpp
int conv4103(const char* fname, bool raw, int endian);
