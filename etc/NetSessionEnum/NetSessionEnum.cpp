#pragma warning(disable:4786)

#include <windows.h>
#include <lm.h>

#include <iostream>
#include <iomanip>
#include <strstream>

#include <time.h>

#include "getopt.h"
#include "sub.h"

//---------------------------------------------------------------------
//	�O���[�o���ϐ�

string g_strServerName;
string g_strClientName;
string g_strUserName;
DWORD g_dwInterval;

//---------------------------------------------------------------------
//[�T�v]
//	�g�p�@��\�����ďI��

void usage(int status)
{
	cout <<
		"NetSessionEnum ver.0.01 build(" << __DATE__ << ")\n"
		"usage: NetSessionEnum [-s servername] [-c clientname] [-u username] [-i interval]" <<
		endl;

	exit(status);
}

//---------------------------------------------------------------------
//[�T�v]
//	�ݐϕb��H:M:S�ɕ���

string SecToHMS(DWORD sec)
{
	ostrstream ss;

	ss << setw(2) << (sec / 3600) << ":";	// ��
	sec %= 3600;
	ss << setfill('0');						// non-zero-suppress
	ss << setw(2) << (sec / 60) << ":";		// ��
	sec %= 60;
	ss << setw(2) << sec << ends;			// �b

	return string(ss.str());
}

//---------------------------------------------------------------------
//[�T�v]
//	SESSION_INFO_2�̃^�C�g���\��

void PrintSessionInfo2Title()
{
	cout << setw(8) << left << "���[�U�[" << " ";
	cout << setw(15) << left << "�R���s���[�^" << " ";
	cout << setw(4) << right << "̧��" << " ";
	cout << setw(8) << right << "�ڑ�����" << " ";
	cout << setw(8) << right << "�A�C�h��" << " ";
	cout << setw(5) << right << "Guest" << " ";
	cout << setw(6) << right << "��Í�" << " ";
	cout << left << "�^�C�v";
	cout << endl;
}

//---------------------------------------------------------------------
//[�T�v]
//	SESSION_INFO_2�̕\��

void PrintSessionInfo2(SESSION_INFO_2* psi2)
{
	cout << setw(8) << left << WStrToStr((wchar_t*)psi2->sesi2_username) << " ";
	cout << setw(15) << left << WStrToStr((wchar_t*)psi2->sesi2_cname) << " ";
	cout << setw(4) << right << psi2->sesi2_num_opens << " ";
	cout << setw(8) << right << SecToHMS(psi2->sesi2_time) << " ";
	cout << setw(8) << right << SecToHMS(psi2->sesi2_idle_time) << " ";
	cout << setw(5) << right << ((psi2->sesi2_user_flags & SESS_GUEST) ? "Yes" : "") << " ";
	cout << setw(6) << right << ((psi2->sesi2_user_flags & SESS_NOENCRYPTION) ? "Yes" : "") << " ";
	cout << left << WStrToStr((wchar_t*)psi2->sesi2_cltype_name);
	cout << endl;
}

//---------------------------------------------------------------------
//[�T�v]
//	�v���O������X�^�[�g

int main(int argc, char** argv)
{
	// �I�v�V�����̊m�F
	int c;
	while ((c = getopt(argc, argv, "s:c:u:i:h?")) != -1) {
		switch (c) {
		case 's':
			g_strServerName.assign(optarg);
			break;
		case 'c':
			g_strClientName.assign(optarg);
			break;
		case 'u':
			g_strUserName.assign(optarg);
			break;
		case 'i':
			g_dwInterval = strtoul(optarg, NULL, 0);
			break;
		case 'h':
		case '?':
			usage(EXIT_SUCCESS);
			break;
		default:
			usage(EXIT_FAILURE);
			break;
		}
	}

	do {
		if (g_dwInterval) {
			time_t t = time(NULL);
			cout << ctime(&t);
		}

		PrintSessionInfo2Title();

		SESSION_INFO_2* pbuf;
		NET_API_STATUS ret;
		DWORD dwRead, dwTotal, dwHandle = 0, dwTotalSession = 0;
		do {
#if _MSC_VER <= 1200	// VC++6�ȉ�
			// �w�b�_�̐錾�̊֌W�ŁAchar* �ɃL���X�g����K�v��������
			ret = NetSessionEnum(
				(char*)(g_strServerName.empty() ? NULL : StrToWStr(g_strServerName.c_str()).c_str()),
				(char*)(g_strClientName.empty() ? NULL : StrToWStr(g_strClientName.c_str()).c_str()),
				(char*)(g_strUserName.empty() ? NULL : StrToWStr(g_strUserName.c_str()).c_str()),
				2, (u_char**)&pbuf, MAX_PREFERRED_LENGTH,
				&dwRead, &dwTotal, &dwHandle);
#else
			ret = NetSessionEnum(
				(LPWSTR)(g_strServerName.empty() ? NULL : StrToWStr(g_strServerName.c_str()).c_str()),
				(LPWSTR)(g_strClientName.empty() ? NULL : StrToWStr(g_strClientName.c_str()).c_str()),
				(LPWSTR)(g_strUserName.empty() ? NULL : StrToWStr(g_strUserName.c_str()).c_str()),
				2, (u_char**)&pbuf, MAX_PREFERRED_LENGTH,
				&dwRead, &dwTotal, &dwHandle);
#endif
			// �f�[�^���擾�ł����H
			if (ret == NERR_Success || ret == ERROR_MORE_DATA) {
				for (DWORD n = 0; n < dwRead; n++)
					PrintSessionInfo2(&pbuf[n]);

				NetApiBufferFree(pbuf);
			} else {
				cerr << GetSystemMessage(ret) << endl;
				return -1;
			}

			dwTotalSession += dwRead;
		} while (ret == ERROR_MORE_DATA);

		cout << "�S�Z�b�V������: " << dwTotalSession << "\n" << endl;

		if (g_dwInterval != 0)
			Sleep(g_dwInterval * 1000);

	} while (g_dwInterval);

	return 0;
}
