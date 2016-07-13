#include <windows.h>
#include <lmaccess.h>
#include <lmerr.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef struct {
	int option;
	string parm;
} OPTION;

class COpt
{
private:
	vector<OPTION> m_options;
	vector<string> m_nonoptions;

public:
	COpt(){};
	COpt(int argc, char** argv, char* optstring) {
		Parse(argc, argv, optstring);
	};
	virtual ~COpt(){};

	int Parse(int argc, char** argv, char* optstring);
	bool Get(int option);
	bool Get(int option, string& parm);
	bool GetNonOption(int index, string& parm);
};

//---------------------------------------------------------------------
//[�T�v]	�I�v�V�����̎擾
//[�߂�l]	true: �I�v�V�������w�肳��Ă���
//			false: �I�v�V�����͎w�肳��Ă��Ȃ�

bool COpt::Get(int option)
{
	vector<OPTION>::iterator i;
	for (i = m_options.begin(); i != m_options.end(); i++) {
		if (i->option == option) {
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------
//[�T�v]	�I�v�V�����̎擾(�p�����[�^�擾�t��)
//[�߂�l]	true: �I�v�V�������w�肳��Ă���
//			false: �I�v�V�����͎w�肳��Ă��Ȃ�

bool COpt::Get(int option, string& parm)
{
	vector<OPTION>::iterator i;
	for (i = m_options.begin(); i != m_options.end(); i++) {
		if (i->option == option) {
			parm = i->parm;
			return true;
		}
	}

	parm.erase();
	return false;
}

//---------------------------------------------------------------------
//[�T�v]	non�I�v�V�����̎擾

bool COpt::GetNonOption(int index, string& parm)
{
	if (index >= m_nonoptions.size())
		return false;

	parm = m_nonoptions[index];
	return true;
}

//---------------------------------------------------------------------
//[�T�v]	�I�v�V�����̉��

int COpt::Parse(int argc, char** argv, char* optstring)
{
	OPTION temp;
	int option;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-' || argv[i][0] == '/') {
			// option�̏���
			for (int j = 1; argv[i][j] != '\0'; j++) {
				option = argv[i][j];
				char* p = strchr(optstring, option);

				if (p == NULL) {
					cerr << "�I�v�V���� -" << (char*)option << " �́A�����ł��B" << endl;
					return -1;
				}

				if (p[1] == '=') {
					if (argv[i][j+1] == '\0') {
						++i;
						if (i < argc) {
							temp.option = option;
							temp.parm = string(argv[i]);
							m_options.push_back(temp);
							break;
						} else {
							cerr << "�I�v�V���� -"<< (char)option << " �ɂ́A�p�����[�^���K�v�ł��B" << endl;
							return -1;
						}
					} else if (argv[i][j+1] == '=') {
						temp.option = option;
						temp.parm = string(&argv[i][j+2]);
						m_options.push_back(temp);
						break;
					} else {
						temp.option = option;
						temp.parm = string(&argv[i][j+1]);
						m_options.push_back(temp);
						break;
					}
				} else {
					temp.option = option;
					temp.parm.erase();
					m_options.push_back(temp);
				}
			}
		} else {
			// non-option�̏���
			m_nonoptions.push_back(string(argv[i]));
		}
	}

	return 0;
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
//[�T�v]	�G���[������Q

typedef struct {
	int code;
	char* errstr;
} ERR_STR_LIST;

ERR_STR_LIST errList[] = {
	{ERROR_ACCESS_DENIED, "The user does not have access to the requested information."},
	{NERR_InvalidComputer, "The computer name is invalid."},
	{NERR_NotPrimary, "The operation is allowed only on the primary domain controller of the domain."},
	{NERR_UserNotFound, "The user name could not be found."},
};

//---------------------------------------------------------------------
//[�T�v]	�G���[��������擾

char* GetErrString(int code)
{
	for (int i = 0; i < sizeof(errList) / sizeof(ERR_STR_LIST); i++) {
		if (errList[i].code == code) {
			return errList[i].errstr;
		}
	}

	return "(unknown error)";
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
