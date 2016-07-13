// ������`�F�b�J�[
//

#if defined(_UNICODE)
#define UNICODE
#endif

#include <windows.h>
#include <lm.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <tchar.h>
#include <conio.h>

// �z��N���X
//
template <class T> class Array {
private:
	T* m_p;			// �o�b�t�@�ւ̃|�C���^
	size_t m_size;	// �o�b�t�@�̗v�f��

public:
	// �f�t�H���g �R���X�g���N�^
	//
	Array() : m_p(NULL), m_size(0) {};

	// �R�s�[�R���X�g���N�^
	//
	Array(const Array<T>& src) {
		// ��Array�̎��́A��ɂ��ďI��
		if (src.GetSize() == 0) {
			m_p = NULL;
			m_size = 0;
			return;
		}

		m_p = new T[m_size = src.GetSize()];
		for (size_t i = 0; i < m_size; i++) {
			m_p[i] = src.GetAt(i);
		}
	};

	// �����T�C�Y�w�� �R���X�g���N�^
	//
	Array(size_t size) : m_p(NULL), m_size(size) {
		if (m_size > 0) {
			m_p = new T[m_size];
		}
	};

	// �z��w�肵�� �R���X�g���N�^
	Array(const T* src, size_t size) : m_p(NULL), m_size(size) {
		if (m_size > 0) {
			m_p = new T[m_size];
			for (size_t i = 0; i < m_size; i++)
				m_p[i] = src[i];
		}
	};

	// �f�X�g���N�^
	//
	~Array() { RemoveAll(); };

public:
	// �v�f�����擾
	//
	size_t GetSize() const { return m_size; };

	// �v�f����ݒ�
	//
	void SetSize(size_t nNewSize) {
		if (m_size == nNewSize)	return;	// �T�C�Y�ɕύX������
		
		if (nNewSize == 0) {	// ��ɂ���(RemoveAll����)
			RemoveAll();
			return;
		}

		T* temp = new T[nNewSize];
		for (size_t i = 0; i < min(m_size, nNewSize); i++) {
			temp[i] = m_p[i];
		}
		if (m_p)	delete[] m_p;
		m_p = temp;
		m_size = nNewSize;
	};

	// �v�f����0�ɂ���
	//
	void RemoveAll() {
		if (m_p)	delete[] m_p;
		m_p = NULL;
		m_size = 0;
	};

	// �w�肵���ʒu�̗v�f��Ԃ�
	//
	T GetAt(int nIndex) const { return m_p[nIndex]; };

	// �w�肵���ʒu�ɗv�f��ݒ肷��
	//
	void SetAt(int nIndex, const T& newElement) { m_p[nIndex] = newElement; };

	// �v�f�ւ̎Q�Ƃ�Ԃ�
	//
	T& ElementAt(int nIndex) { return m_p[nIndex]; };

	// �z��̐擪�A�h���X��Ԃ�
	//
	const T* GetData() const { return m_p; };
	T* GetData() { return m_p; };

	// �z��̌��ɗv�f�̒ǉ�
	//
	int Add(const T& newElement) {
		
		// �z����ЂƂg�����āA�g�������ꏊ�ɒl����
		SetSize(GetSize() + 1);
		m_p[GetSize() - 1] = newElement;

		return GetSize() - 1;
	};

	// �w�肵���ʒu�̗v�f�A�܂��́A�Q�Ƃ�Ԃ�
	//
	T operator [](int nIndex) const { return m_p[nIndex]; };
	T& operator [](int nIndex) { return m_p[nIndex]; };

	// �^�ϊ�
	//
	operator const T*() const { return m_p; };
	operator T*() { return m_p; };

	// ������Z�q
	//
	Array<T>& operator =(const Array<T>& src) {
		if (this == src)	return *this;	// ����R�s�[��h�~

		RemoveAll();	// ��������ɂ���

		if (src.GetSize() == 0)	return *this;	// src����̂Ƃ��͂����ŏI��

		m_p = new T[m_size = src.GetSize()];
		for (int i = 0; i < m_size; i++) {
			m_p[i] = src[i];
		}
	};
};

// �e��z��^
//
typedef Array<char>		ar_char;
typedef Array<wchar_t>	ar_wchar;
typedef Array<_TCHAR>	ar_tchar;

// ���g�̎��s�t�@�C�������擾����
//
ar_tchar GetExeName()
{
	ar_tchar buf(MAX_PATH);

	while (GetModuleFileName(NULL, buf, buf.GetSize()) == buf.GetSize())
		buf.SetSize(buf.GetSize() * 2);

	return buf;
}

// �g���q��u��������
//
ar_tchar RenameExt(const _TCHAR* src, const _TCHAR* ext)
{
	ar_tchar src_buf(src, _tcslen(src) + 1);
	ar_tchar dst_buf(MAX_PATH);
	ar_tchar drive(_MAX_DRIVE), dir(_MAX_DIR), fname(_MAX_FNAME);

	_tsplitpath(src_buf, drive, dir, fname, NULL);
	_tmakepath(dst_buf, drive, dir, fname, ext);

	return dst_buf;
}

// char�ȕ������wchar_t������ɕϊ�
//
ar_wchar StrToWStr(const char *src)
{
	size_t needed = mbstowcs(NULL, src, strlen(src));

	ar_wchar dst(needed);

	mbstowcs(dst, src, strlen(src));

	dst.Add(L'\0');	// ������I�[��ǉ�

	return dst;
}

// wchar_t�ȕ������char������ɕϊ�
//
ar_char WStrToStr(const wchar_t* src)
{
	size_t needed = wcstombs(NULL, src, wcslen(src));

	ar_char dst(needed);

	wcstombs(dst, src, wcslen(src));

	dst.Add('\0');	// ������I�[��ǉ�

	return dst;
}

// �w�胆�[�U�[�Ń��O�C���\���ǂ����̃`�F�b�N
//
DWORD LoginCheck(const _TCHAR* name, const _TCHAR* pass)
{
	HANDLE hToken;

	if (LogonUser((LPTSTR)name, NULL, (LPTSTR)pass, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hToken)) {
		CloseHandle(hToken);
	}

	return GetLastError();
}

// ���[�U�[���`�F�b�N
//
//  -1: ���[�U�[�����o�^
//  -2: �p�X���[�h���������ł͂Ȃ�
//  -3: ���[�U�[���ɃA�N�Z�X���錠��������
//  -4: �R���s���[�^�[�����Ԉ���Ă���
// -99: ���̑��̃G���[
//
int UserInfo(const _TCHAR* name)
{
	int ret = 0;
	PUSER_INFO_3 pui;

#if !defined(_UNICODE)
	NET_API_STATUS nret = NetUserGetInfo(NULL, StrToWStr(name), 3, (LPBYTE*)&pui);
#else
	NET_API_STATUS nret = NetUserGetInfo(NULL, name, 3, (LPBYTE*)&pui);
#endif

	switch (nret) {
	case NERR_Success:

		if ((pui->usri3_flags & UF_DONT_EXPIRE_PASSWD) == 0) {
			ret = -2;
		}

		NetApiBufferFree(pui);
		break;

	case NERR_UserNotFound:
		ret = -1;
		break;

	case ERROR_ACCESS_DENIED:
		ret = -3;
		break;

	case NERR_InvalidComputer:
		ret = -4;
		break;

	default:
		ret = -99;
		break;
	}

	return ret;
}

// ���C�����[�v
//
void do_main(int ac, _TCHAR **av)
{
	DWORD dwRet;
	int err_cnt = 0;
	ar_tchar ini = RenameExt(GetExeName(), _T("ini"));
	ar_tchar key(32);
	ar_tchar tmp(96);
	ar_tchar buf(96);
	ar_tchar pas(96);
	_TCHAR* p;

	for (int i = 1;; i++) {
		_sntprintf(key, key.GetSize(), _T("user%d"), i);
		GetPrivateProfileString(_T("users"), key, _T(""), tmp, tmp.GetSize(), ini);

		if (_tcscmp(tmp, _T("")) == 0)
			break;

		p = _tcstok(tmp, ",");
		if (p)	_tcscpy(buf, p);

		p = _tcstok(NULL, ",");
		if (p)	_tcscpy(pas, p);

		_tprintf(_T("%s ... "), buf);

		int ret = UserInfo(buf);
		if (ret != 0)
			++err_cnt;

		switch (ret) {
		case 0:

			dwRet = LoginCheck(buf, pas);
			if (dwRet != 0) {
				++err_cnt;
				_tprintf(_T("ERROR (���O�C���ł��܂���(error=%u))\n"), dwRet);
			} else {
				_putts(_T("OK"));
			}
			break;
		case -1:
			_putts(_T("ERROR (���[�U�[�����o�^)"));
			break;
		case -2:
			_putts(_T("ERROR (�p�X���[�h���������ł͂Ȃ�)"));
			break;
		case -3:
			_putts(_T("ERROR (���[�U�[���ɃA�N�Z�X���錠��������)"));
			break;
		case -4:
			_putts(_T("ERROR (�R���s���[�^�[�����Ԉ���Ă���)"));
			break;
		default:
			_putts(_T("ERROR (���̑�)"));
			break;
		}
	}

	if (err_cnt == 0) {
		_putts(_T("���[�U�[���͐���ł��B"));
	} else {
		_tprintf(_T("%d ���̃G���[������܂��B�ݒ���������ēx�`�F�b�N�����Ă��������B\n"), err_cnt);
	}

	_tprintf(_T("�����L�[�������Ă��������E�E�E"));

	_getch();
}

// �v���O�����E�X�^�[�g
//
int _tmain(int argc, _TCHAR **argv)
{
	_tsetlocale(LC_ALL, _T("jpn"));	// ���{�ꃍ�P�[���ɕύX

	do_main(argc, argv);

	return 0;
}
