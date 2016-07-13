int GetProcessHandleCount2(DWORD procId) {
	typedef struct _SYSTEM_PROCESS_INFORMATION {
		ULONG NextEntryOffset;
		BYTE Reserved1[52];
		PVOID Reserved2[3];
		HANDLE UniqueProcessId;
		PVOID Reserved3;
		ULONG HandleCount;
		BYTE Reserved4[4];
		PVOID Reserved5[11];
		SIZE_T PeakPagefileUsage;
		SIZE_T PrivatePageCount;
		LARGE_INTEGER Reserved6[6];
	} SYSTEM_PROCESS_INFORMATION;

	#define STATUS_SUCCESS                    0
	#define STATUS_INFO_LENGTH_MISMATCH        0xC0000004
	#define SystemProcessInformation        5

	typedef NTSTATUS (WINAPI *NTQSI)(int, PVOID, ULONG, PULONG);

	int hc = -1;
	kjm::library lib;

	if (lib.LoadLibraryEx(_T("ntdll"), NULL, 0)) {
		NTQSI func = (NTQSI)GetProcAddress(lib.getInstance(), "NtQuerySystemInformation");
		if (func == NULL) {
			return -1;
		}
		
		DWORD bufSize = 384 * 1024;
		std::vector<BYTE> buf(bufSize);
		NTSTATUS ret;

		while (1) {
			DWORD sizeNeeded = buf.size();
			ret = func(SystemProcessInformation, &buf[0], buf.size(), &sizeNeeded);
			if (ret == STATUS_SUCCESS) {
				break;
			} else if (ret == STATUS_INFO_LENGTH_MISMATCH) {
				// win2k �́A���ł��K�v�T�C�Y�� 0 �Ŗ߂��Ă���̂ŁA
				// �����o�b�t�@ * 2 �ɂ��Ȃ���A����I������̂�҂�
				if (sizeNeeded == 0) {
					bufSize *= 2;
					buf.resize(bufSize);
				} else {
					bufSize *= 2;
					buf.resize(bufSize);
				}
			} else {
				return -1;
			}
		}

		SYSTEM_PROCESS_INFORMATION* p = (SYSTEM_PROCESS_INFORMATION*)&buf[0];
	
		while (p) {
			// �ړI�̃v���Z�X����������A�n���h������߂�l�ɂ��ă��[�v�𔲂���
			if ((DWORD)(p->UniqueProcessId) == procId) {
				hc = p->HandleCount;
				break;
			}
			
			// �v���Z�X���X�g���`�F�[�����Ă���
			if (p->NextEntryOffset == 0) {
				p = NULL;
			} else {
				p = (SYSTEM_PROCESS_INFORMATION*)((BYTE*)p + p->NextEntryOffset);
			}
		}
	}

	return hc;
}
