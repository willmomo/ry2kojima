#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "resource.h"

#define LINE_MAX	512
#define SEPARATOR	"\t "

static FILE *fp;
static char dataBin[LINE_MAX];
static unsigned int dataLen;

static char *command[] = {"w", "s", "r", 0x00};


/*
 * Public Declare Function scriptOpen Lib "HaraiDasiDll.dll" (ByVal fname As String) As Long
 */
int __stdcall scriptOpen(char *fname)
{
	fp = 0;
	fp = fopen(fname, "r");
	if(fp == 0) {
		return 1;
	}

	return 0;
}


/*
 * Public Declare Function scriptClose Lib "HaraiDasiDll.dll" () As Long
 */
int __stdcall scriptClose(void)
{
	if(fp == 0) return 0;
	
	if(fclose(fp) != 0) {
		return 1;
	}
	
	return 0;
}


/*
 * Public Declare Function scriptGetLine Lib "HaraiDasiDll.dll" (ByRef cmd As Byte, ByRef param As Byte, ByRef data As Byte) As Long
 */
int __stdcall scriptGetLine(char *cmd, char *param, char *data)
{
	unsigned int i;
	unsigned int stat;
	char *lp;
	char lineBuf[LINE_MAX];
	char dataString[LINE_MAX];

	memset(dataString, 0, sizeof(dataString));
	dataLen = 0;

	while(1) {
		memset(lineBuf, 0, sizeof(lineBuf));
		if(fgets(lineBuf, LINE_MAX, fp) == NULL) return 1;

		lp = strtok(lineBuf, SEPARATOR);
		if(lp == NULL) continue;
		if(*lp == '\n') continue;
		if(memcmp(lp, "//", 2) == 0) continue;
		break;
	}

//	lp = strtok(lineBuf, SEPARATOR);
//	if(lp == NULL) return 1;
	strcpy(cmd, lp);
	strlwr(cmd);
	i = 0;
	stat = 1;
	while(command[i] != 0) {
		if(strcmp(cmd, command[i]) == 0) {
			stat = 0;
			break;
		}
		i++;
	}
	if(stat != 0) {
		*cmd = 0;
		return 1;
	}


	lp = strtok(NULL, SEPARATOR);
	if(lp == NULL) return 1;
	*param = (char)strtol(lp, 0, 0);
	if(*param == 0) return 1;

	lp = strtok(NULL, SEPARATOR);
	if(lp == NULL) {
		*data = 0;
		return 0;
	}
	strcpy(dataString, lp);
	i = 0;
	lp = strtok(dataString, ",");
	dataBin[i] = data[i] = (char)strtol(lp, 0, 0);
	i++;

	while (lp != NULL) {
		lp = strtok(NULL, ",");
		if(lp != NULL) {
			if(memcmp(lp, "//", 2) == 0) break;

			dataBin[i] = data[i] = (char)strtol(lp, 0, 0);
			i++;
		}
	}

	dataLen = i;

	return 0;
}


/*
 * Public Declare Function scriptCompare Lib "HaraiDasiDll.dll" (ByRef cmd As Byte, ByRef param As Byte, ByRef data As Byte) As Long
 */
int __stdcall scriptCompare(char *cmd, char *param, char *data)
{
	unsigned int i;

	for(i = 0; i < dataLen; i++) {
		if(data[i] != dataBin[i]) return 1;
	}

	return 0;
}

/*
void main(void)
{
	unsigned char cmd, param;
	unsigned char data[64];
	int i;	
	scriptOpen("C:\\PROJECTS\\POS_TOOL\\src\\test\\Debug\\test.txt");

	i = 0;
	while(1) {
		memset(data, 0, sizeof(data));
		if(scriptGetLine(&cmd, &param, data) != 0) break;
		if(scriptCompare(&cmd, &param, data) != 0) break;
		i++;
	}

	scriptClose();
}
*/

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return TRUE;
}
