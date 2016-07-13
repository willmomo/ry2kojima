//---------------------------------------------------------------------
// 厳密チェック、ログつき関数群

#pragma once

//---------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

//---------------------------------------------------------------------

#define HS_LOG0(fmt)	printf("Program: %s, "##fmt##", file %s, line %d\n", hs_GetModuleFileName(), __FILE__, __LINE__)
#define HS_LOG1(fmt, arg1)	printf("Program: %s, "##fmt##", file %s, line %d\n", hs_GetModuleFileName(), (arg1), __FILE__, __LINE__)
#define HS_LOG2(fmt, arg1, arg2)	printf("Program: %s, "##fmt##", file %s, line %d\n", hs_GetModuleFileName(), (arg1), (arg2), __FILE__, __LINE__)

#define HS_ERRLOG0(fmt)	printf("Program: %s, "##fmt##" error %d: %s, file %s, line %d\n", hs_GetModuleFileName(), errno, strerror(errno), __FILE__, __LINE__)
#define HS_ERRLOG1(fmt, arg1)	printf("Program: %s, "##fmt##" error %d: %s, file %s, line %d\n", hs_GetModuleFileName(), (arg1), errno, strerror(errno), __FILE__, __LINE__)
#define HS_ERRLOG2(fmt, arg1, arg2)	printf("Program: %s, "##fmt##" error %d: %s, file %s, line %d\n", hs_GetModuleFileName(), (arg1), (arg2), errno, strerror(errno), __FILE__, __LINE__)

//---------------------------------------------------------------------

#if !defined(HS_IMPREMENT)

#define fopen	hs_fopen
#define fputs	hs_fputs
#define fclose	hs_fclose

#endif

//---------------------------------------------------------------------

FILE *hs_fopen(const char *filename, const char *mode);
int hs_fputs(const char *string, FILE *stream);
int hs_fclose(FILE *stream);
