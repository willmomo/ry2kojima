#include <windows.h>
#include <lmaccess.h>
#include <lmerr.h>
#include <iostream>
#include <string>
#include <vector>
#include "..\lib\opt\opt.h"

using namespace std;

//---------------------------------------------------------------------
//[�T�v]	������I�[��'\n'���폜

char* chomp(char* str)
{
	for (int i = strlen(str) - 1; i > 0; i--) {
		if (str[i] == '\n')
			str[i] = '\0';
		else
			break;
	}

	return str;
}

//---------------------------------------------------------------------
//[�T�v]	string��wstring�ɕϊ�

wstring StringToWString(const string& str)
{
	wstring wstr;	// �߂蕶����
	wstr.empty();	// �܂���ɂ���

	// �ϊ��ɕK�v�ȃo�b�t�@�T�C�Y(������(�܂�\0))���擾
	int nSizeNeeded = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	if (nSizeNeeded == 0)
		return wstr;

	// �o�b�t�@���m��
	wchar_t* pwsz = (wchar_t*)malloc(sizeof(wchar_t) * nSizeNeeded);
	if (pwsz == NULL)
		return wstr;

	// �ϊ�
	int ret = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pwsz, nSizeNeeded);
	if (ret == 0) {
		free(pwsz);
		return wstr;
	}

	wstr.assign(pwsz);
	free(pwsz);

	return wstr;
}

//---------------------------------------------------------------------
//[�T�v]	wstring��string�ɕϊ�

string WStringToString(const wstring& wstr)
{
	string str;		// �߂蕶����
	str.empty();	// �܂���ɂ���

	// �ϊ��ɕK�v�ȃo�b�t�@�T�C�Y(������(�܂�\0))���擾
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0)
		return str;

	// �o�b�t�@���m��
	char* psz = (char*)malloc(sizeof(char) * nSizeNeeded);
	if (psz == NULL)
		return str;

	// �ϊ�
	int ret = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		free(psz);
		return str;
	}

	str.assign(psz);
	free(psz);

	return str;
}

//---------------------------------------------------------------------
//[�T�v]	�G���[��������擾

string GetErrString(int code)
{
	static string msg;
	char* pmsg;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
		NULL,
		code,
		0,
		(char*)&pmsg,
		256,
		NULL);
	chomp(pmsg);
	msg = string(pmsg);
	LocalFree(pmsg);

	return msg;
}

//---------------------------------------------------------------------
//[�T�v]	�g�p�@�̕\�� and �I��

void usage(int status)
{
	cout << "�g�p�@�Fuserdel [-m server] <user [...]>" << endl;
	exit(status);
}

//---------------------------------------------------------------------
//[�T�v]	���[�U�[�̍폜
//[����]	machine�Ŏw�肳�ꂽPC����username�Ŏw�肳�ꂽ���[�U���폜

int UserDel(string& machine, string& username)
{
	wstring wstrMachine = StringToWString(machine);
	wstring wstrUsername = StringToWString(username);

	NET_API_STATUS ret;
	ret = NetUserDel(wstrMachine.c_str(), wstrUsername.c_str());
	if (ret != NERR_Success) {
		cerr << "error " << ret << ":" << GetErrString(ret) << endl;
	}

	return 0;
}

//---------------------------------------------------------------------
//[�T�v]	�v���O�����E�X�^�[�g

int main(int argc, char** argv)
{
	COpt opt;

	if (opt.Parse(argc, argv, "hm=") != 0) {
		usage(EXIT_FAILURE);
	}

	if (opt.Get('h')) {
		usage(EXIT_SUCCESS);
	}

	string machine;
	string username;

	opt.Get('m', machine);
	for (int i = 0; opt.GetNonOption(i, username); i++) {
		UserDel(machine, username);
	}

	return 0;
}
