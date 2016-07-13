#include <windows.h>
#include <tchar.h>


/////////////////////////////////////////////////////////////////////////
// �萔

#define TOTALBYTES		43008
#define BYTEINCREMENT	1024

/////////////////////////////////////////////////////////////////////////
// �O���[�o���ϐ�

static OSVERSIONINFO s_osvi;

/////////////////////////////////////////////////////////////////////////
// �p�t�H�[�}���X�f�[�^�擾�J�n

static BOOL StartPerf95()
{
	LONG rc;
	HKEY hkey;

	// �p�t�H�[�}���X�E�f�[�^�̎��W��L���ɂ���
	rc = RegOpenKeyEx(HKEY_DYN_DATA, _T("PerfStats\\StartStat"), 0, KEY_READ, &hkey);
	if (rc == ERROR_SUCCESS) {
		DWORD dwType, cbData;
		LPBYTE pByte;
		rc = RegQueryValueEx(hkey, _T("KERNEL\\CPUUsage"), NULL, &dwType, NULL, &cbData);
		if (rc == ERROR_SUCCESS) {
			pByte = (LPBYTE)LocalAlloc(LPTR, cbData);
			rc = RegQueryValueEx(hkey, _T("KERNEL\\CPUUsage"), NULL, &dwType, pByte, &cbData);
			if (rc != ERROR_SUCCESS) {
				TCHAR szBuffer[128];
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					rc,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
			}
			LocalFree(pByte);
		} else {
			TCHAR szBuffer[128];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
				rc,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
		}
		RegCloseKey(hkey);
	} else {
		TCHAR szBuffer[128];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			rc,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
	}

	return TRUE;
}

static BOOL StartPerfNT()
{
	return TRUE;
}

BOOL StartPerf()
{
	s_osvi.dwOSVersionInfoSize = sizeof(s_osvi);
	GetVersionEx(&s_osvi);

	if (s_osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		return StartPerf95();
	else if (s_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		return StartPerfNT();

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////
// �p�t�H�[�}���X�f�[�^�擾

static INT GetCPUUsage95()
{
	LONG rc;
	HKEY hkey;
	INT inData = 0;

	// object\counter�y�A�̃p�t�H�[�}���X�E�f�[�^����������
	rc = RegOpenKeyEx(HKEY_DYN_DATA, _T("PerfStats\\StatData"), 0, KEY_READ, &hkey);
	if (rc == ERROR_SUCCESS) {
		DWORD dwType, dwData, cbData = sizeof(DWORD);
		rc = RegQueryValueEx(hkey, _T("KERNEL\\CPUUsage"), NULL, &dwType, (LPBYTE)&dwData, &cbData);
		if (rc == ERROR_SUCCESS) {
			inData = dwData;
		} else {
			TCHAR szBuffer[128];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
				rc,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
		}
		RegCloseKey(hkey);
	} else {
		TCHAR szBuffer[128];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			rc,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
	}

	return inData;
}

static PVOID CounterDataPtr(PPERF_DATA_BLOCK pDataBlock, DWORD dwObjectNameTitleIndex, WCHAR* pszInstanceName, DWORD dwCounterNameTitleIndex)
{
	PPERF_OBJECT_TYPE pObjectType = (PPERF_OBJECT_TYPE)((PBYTE)pDataBlock + pDataBlock->HeaderLength);
	for (DWORD i = 0; i < pDataBlock->NumObjectTypes; i++) {
		if (pObjectType->ObjectNameTitleIndex == dwObjectNameTitleIndex) {
			if (pObjectType->NumInstances < 0) {
				PPERF_COUNTER_BLOCK pCounterBlock = (PPERF_COUNTER_BLOCK)((PBYTE)pObjectType + pObjectType->DefinitionLength);
				PPERF_COUNTER_DEFINITION pCounterDefinition = (PPERF_COUNTER_DEFINITION)((PBYTE)pObjectType + pObjectType->HeaderLength);
				for (DWORD j = 0; j < pObjectType->NumCounters; j++) {
					if (pCounterDefinition->CounterNameTitleIndex == dwCounterNameTitleIndex) {
						return (PVOID)((PBYTE)pCounterBlock + pCounterDefinition->CounterOffset);
					}

					pCounterDefinition = (PPERF_COUNTER_DEFINITION)((PBYTE)pCounterDefinition + pCounterDefinition->ByteLength);
				}

				return NULL;
			} else {
				WCHAR* pszDebug = NULL;
				PPERF_INSTANCE_DEFINITION pInstanceDefinition = (PPERF_INSTANCE_DEFINITION)((PBYTE)pObjectType + pObjectType->DefinitionLength);
				for (int k = 0; k < pObjectType->NumInstances; k++) {
					PPERF_COUNTER_BLOCK pCounterBlock = (PPERF_COUNTER_BLOCK)((PBYTE)pInstanceDefinition + pInstanceDefinition->ByteLength);

					pszDebug = (WCHAR*)((PBYTE)pInstanceDefinition + pInstanceDefinition->NameOffset);
					if (wcscmp((WCHAR*)((PBYTE)pInstanceDefinition + pInstanceDefinition->NameOffset), pszInstanceName) == 0) {
						PPERF_COUNTER_DEFINITION pCounterDefinition = (PPERF_COUNTER_DEFINITION)((PBYTE)pObjectType + pObjectType->HeaderLength);
						for (DWORD j = 0; j < pObjectType->NumCounters; j++) {
							if (pCounterDefinition->CounterNameTitleIndex == dwCounterNameTitleIndex) {
								return (PVOID)((PBYTE)pCounterBlock + pCounterDefinition->CounterOffset);
							}

							pCounterDefinition = (PPERF_COUNTER_DEFINITION)((PBYTE)pCounterDefinition + pCounterDefinition->ByteLength);
						}

						return NULL;
					}

					pInstanceDefinition = (PPERF_INSTANCE_DEFINITION)((PBYTE)pCounterBlock + pCounterBlock->ByteLength);
				}

				return NULL;
			}
		}

		pObjectType = (PPERF_OBJECT_TYPE)((PBYTE)pObjectType + pObjectType->TotalByteLength);
	}

	return NULL;
}

static INT GetCPUUsageNT()
{
	PPERF_DATA_BLOCK PerfData = NULL;
	DWORD BufferSize = TOTALBYTES;
	static LARGE_INTEGER liOld;
	static LARGE_INTEGER PerfTime100nSecOld;

//	PerfData = (PPERF_DATA_BLOCK)malloc(BufferSize);	// �o�b�t�@���m��
	PerfData = (PPERF_DATA_BLOCK)LocalAlloc(LPTR, BufferSize);	// �o�b�t�@���m��
	if (PerfData == NULL)
		return -1;

//	while (RegQueryValueEx(HKEY_PERFORMANCE_DATA, "Global", NULL, NULL, (LPBYTE)PerfData, &BufferSize) == ERROR_MORE_DATA) {
	while (RegQueryValueEx(HKEY_PERFORMANCE_DATA, _T("238"), NULL, NULL, (LPBYTE)PerfData, &BufferSize) == ERROR_MORE_DATA) {
		BufferSize += BYTEINCREMENT;	// �\���ɑ傫�ȃo�b�t�@���m��
//		PerfData = (PPERF_DATA_BLOCK)realloc(PerfData, BufferSize);
		PerfData = (PPERF_DATA_BLOCK)LocalReAlloc((HGLOBAL)PerfData, BufferSize, 0);
		if (PerfData == NULL)
			return -1;
	}

	LARGE_INTEGER* pli = (LARGE_INTEGER*)CounterDataPtr(PerfData, 238, L"0", 6);

	INT C = (INT)(pli->QuadPart - liOld.QuadPart);
	INT T = (INT)(PerfData->PerfTime100nSec.QuadPart - PerfTime100nSecOld.QuadPart);

	PerfTime100nSecOld.QuadPart = PerfData->PerfTime100nSec.QuadPart;
	liOld.QuadPart = pli->QuadPart;

//	free(PerfData);
	if (LocalFree((HGLOBAL)PerfData) != NULL)
		return -1;

	INT ret = (INT)((1.0 - ((double)C / (double)T)) * 100.0);
	return (ret < 0) ? 0 : ret;
}

INT GetCPUUsage()
{
	if (s_osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		return GetCPUUsage95();
	else if (s_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		return GetCPUUsageNT();

	SetLastError((DWORD)CO_E_WRONGOSFORAPP);	// �o�[�W�����G���[

	return -1;
}

/////////////////////////////////////////////////////////////////////////
// �p�t�H�[�}���X�f�[�^�擾�I��

static BOOL StopPerf95()
{
	LONG rc;
	HKEY hkey;

	// �p�t�H�[�}���X�E�f�[�^�̎��W�𖳌��ɂ���
	rc = RegOpenKeyEx(HKEY_DYN_DATA, _T("PerfStats\\StopStat"), 0, KEY_READ, &hkey);
	if (rc == ERROR_SUCCESS) {
		DWORD dwType, cbData;
		LPBYTE pByte;
		rc = RegQueryValueEx(hkey, _T("KERNEL\\CPUUsage"), NULL, &dwType, NULL, &cbData);
		if (rc == ERROR_SUCCESS) {
			pByte = (LPBYTE)LocalAlloc(LPTR, cbData);
			rc = RegQueryValueEx(hkey, _T("KERNEL\\CPUUsage"), NULL, &dwType, pByte, &cbData);
			if (rc != ERROR_SUCCESS) {
				TCHAR szBuffer[128];
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					rc,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
			}
			LocalFree(pByte);
		} else {
			TCHAR szBuffer[128];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
				rc,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
		}
		RegCloseKey(hkey);
	} else {
		TCHAR szBuffer[128];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			rc,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
	}

	return TRUE;
}

static BOOL StopPerfNT()
{
	return TRUE;
}

BOOL StopPerf()
{
	if (s_osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		return StopPerf95();
	else if (s_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		return StopPerfNT();

	return FALSE;
}
