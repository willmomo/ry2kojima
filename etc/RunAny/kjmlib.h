//---------------------------------------------------------------------
// wsslog ���g�p�ł���Ƃ��́AUSE_WSSLOG ���`���Ă��������B
//---------------------------------------------------------------------
#pragma once


//---------------------------------------------------------------------
// �w�b�_�t�@�C��
//---------------------------------------------------------------------
#pragma warning(disable : 4786)

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <string>
#include <assert.h>
#include <vector>
#include <map>
#include <strstream>
#pragma comment(lib, "version.lib")

#if defined(USE_WSSLOG)
#include "wsslog.h"
#pragma comment(lib, "wsslog.lib")
#endif


namespace kjm {
	//-----------------------------------------------------------------
	// �֐����Ŕ�������Win32�G���[���O�Ƃ��ė����N���X
	//-----------------------------------------------------------------
	class Win32Exception {
	public:
		Win32Exception(DWORD dwError) : m_dwError(dwError) {};
		virtual ~Win32Exception() {};

	public:
		DWORD m_dwError;
	};

	//-----------------------------------------------------------------
	// �p�X������
	//-----------------------------------------------------------------
	class PathString {
	public:
		//-------------------------------------------------------------
		// �f�t�H���g�R���X�g���N�^
		//-------------------------------------------------------------
		PathString() : m_str("") {};

		//-------------------------------------------------------------
		// �R�s�[ �R���X�g���N�^
		//-------------------------------------------------------------
		PathString(const PathString& src) : m_str(src.m_str) {};

		//-------------------------------------------------------------
		// std::string���󂯎��R���X�g���N�^
		//-------------------------------------------------------------
		PathString(const std::string& src) : m_str(src) {};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		virtual ~PathString() {};

		//-------------------------------------------------------------
		// ������Z�q
		//-------------------------------------------------------------
		PathString& operator=(const PathString& rhs) {
			if (this == &rhs) return *this;
			m_str = rhs.m_str;
			return *this;
		};

		//-------------------------------------------------------------
		// c_str() �ւ̃A�N�Z�T
		//-------------------------------------------------------------
		const char* c_str() { return m_str.c_str(); };

		//-------------------------------------------------------------
		// const char* �ւ̃L���X�g
		//-------------------------------------------------------------
		operator const char *() { return m_str.c_str(); };

		//-------------------------------------------------------------
		// PathAppend �̃��b�p�[
		//-------------------------------------------------------------
		PathString& Append(const std::string& strMore) {
			std::vector<char> work(MAX_PATH);
			PathAppend(lstrcpy(work.begin(), m_str.c_str()), strMore.c_str());
			m_str = work.begin();
			return *this;
		};

		//-------------------------------------------------------------
		// PathRemoveFileSpec �̃��b�p�[
		//-------------------------------------------------------------
		PathString& RemoveFileSpec() {
			std::vector<char> work(MAX_PATH);
			::PathRemoveFileSpec(lstrcpy(work.begin(), m_str.c_str()));
			m_str = work.begin();
			return *this;
		};

		//-------------------------------------------------------------
		// ���[�g�f�B���N�g�����擾����
		//-------------------------------------------------------------
		std::string GetRoot() {
			std::vector<char> work(MAX_PATH);
			lstrcpy(work.begin(), this->m_str.c_str());

			while (PathIsRoot(work.begin()) == FALSE) {
				if (!PathRemoveFileSpec(work.begin())) {
					return "";
				}
			}

			return work.begin();
		};

	private:
		std::string m_str;
	};


	class Mutex {
	public:
		//-------------------------------------------------------------
		// �f�t�H���g �R���X�g���N�^
		//-------------------------------------------------------------
		Mutex() : m_hMutex(NULL), m_alreadyExists(false) {};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		virtual ~Mutex() { Close(); };

		//-------------------------------------------------------------
		// �~���[�e�b�N�X�̍쐬
		//-------------------------------------------------------------
		bool Create(const std::string& name) {
			m_hMutex = ::CreateMutex(NULL, FALSE, name.c_str());
			if (GetLastError() == ERROR_ALREADY_EXISTS) {
				m_alreadyExists = true;
			}
			return (m_hMutex != NULL);
		};

		//-------------------------------------------------------------
		// �~���[�e�b�N�X�̔j��
		//-------------------------------------------------------------
		BOOL Close() {
			BOOL bRet = TRUE;
			if (m_hMutex) {
				bRet = CloseHandle(m_hMutex);
				m_hMutex = NULL;
			}
			return bRet;
		};

	private:
		//-------------------------------------------------------------
		// �R�s�[�Ƒ�����֎~
		//-------------------------------------------------------------
		Mutex(const Mutex& src) {};
		void operator=(const Mutex& rhs) {};

	public:
		//-------------------------------------------------------------
		// �~���[�e�b�N�X�����ɑ��݂��Ă������ǂ����𒲂ׂ�
		//-------------------------------------------------------------
		bool IsAlreadyExists() { return m_alreadyExists; };

	private:
		HANDLE m_hMutex;
		bool m_alreadyExists;
	};


	class Process {
	public:
		//-------------------------------------------------------------
		// �f�t�H���g �R���X�g���N�^
		//-------------------------------------------------------------
		Process() {
			memset(&m_startupInfo, 0, sizeof(m_startupInfo));
			memset(&m_processInformation, 0, sizeof(m_processInformation));
		};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		virtual ~Process() { Close(); };

		//-------------------------------------------------------------
		// �v���Z�X����
		//-------------------------------------------------------------
		BOOL Create(const std::string& commandLine) {
			m_startupInfo.cb = sizeof(m_startupInfo);

			TCHAR cmd[MAX_PATH];
			lstrcpy(cmd, commandLine.c_str());

			return CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &m_startupInfo, &m_processInformation);
		};

		//-------------------------------------------------------------
		// �v���Z�X�n���h���N���[�Y
		//-------------------------------------------------------------
		BOOL Close() {
			BOOL bRet = TRUE;
			if (m_processInformation.hThread) {
				if (!CloseHandle(m_processInformation.hThread)) {
					bRet = FALSE;
				}
			}
			if (m_processInformation.hProcess) {
				if (!CloseHandle(m_processInformation.hProcess)) {
					bRet = FALSE;
				}
			}
			memset(&m_startupInfo, 0, sizeof(m_startupInfo));
			memset(&m_processInformation, 0, sizeof(m_processInformation));
			return bRet;
		};

		//-------------------------------------------------------------
		// �v���Z�X�������I��
		//-------------------------------------------------------------
		BOOL Terminate(UINT uExitCode) { return ::TerminateProcess(m_processInformation.hProcess, uExitCode); };

		//-------------------------------------------------------------
		// �v���Z�X�I����҂�
		//-------------------------------------------------------------
		DWORD Wait(DWORD dwMilliseconds = INFINITE) { return WaitForSingleObject(m_processInformation.hProcess, dwMilliseconds); };

		//-------------------------------------------------------------
		// �I���R�[�h���擾����
		//-------------------------------------------------------------
		DWORD GetExitCodeProcess(LPDWORD lpExitCode) { return ::GetExitCodeProcess(m_processInformation.hProcess, lpExitCode); };

	private:
		//-------------------------------------------------------------
		// �R�s�[�Ƒ�����֎~
		//-------------------------------------------------------------
		Process(const Process& src) {};
		void operator=(const Process& rhs) {};

	public:
		//-------------------------------------------------------------
		// STARTUPINFO::wShowWindow �ւ̃A�N�Z�T
		//-------------------------------------------------------------
		WORD get_ShowWindow() { return m_startupInfo.wShowWindow; };
		void set_ShowWindow(WORD value) {
			m_startupInfo.dwFlags |= STARTF_USESHOWWINDOW;
			m_startupInfo.wShowWindow = value;
		};

	private:
		STARTUPINFO m_startupInfo;
		PROCESS_INFORMATION m_processInformation;
	};


	class FindFile {
	public:
		//-------------------------------------------------------------
		// �f�t�H���g �R���X�g���N�^
		//-------------------------------------------------------------
		FindFile() : m_hFindFile(INVALID_HANDLE_VALUE) {};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		virtual ~FindFile() { Close(); };

		//-------------------------------------------------------------
		// FindFirstFile�֐��̃��b�p�[
		//-------------------------------------------------------------
		bool FindFirst(const std::string& lpPathName, const std::string& lpWildcard) {
			assert(m_hFindFile == INVALID_HANDLE_VALUE);

			::PathAppend(::lstrcpy(m_fileName, lpPathName.c_str()), lpWildcard.c_str());
			//SetLastError(ERROR_SUCCESS);
			m_hFindFile = ::FindFirstFile(m_fileName, &m_findFileData);
			return (m_hFindFile != INVALID_HANDLE_VALUE);
		};

		//-------------------------------------------------------------
		// FindNextFile�֐��̃��b�p�[
		//-------------------------------------------------------------
		bool FindNext() { return (::FindNextFile(m_hFindFile, &m_findFileData) != FALSE); };

		//-------------------------------------------------------------
		// FindClose�֐��̃��b�p�[
		//-------------------------------------------------------------
		bool Close() {
			bool bResult = true;
			if (m_hFindFile != INVALID_HANDLE_VALUE) {
				bResult = (::FindClose(m_hFindFile) != FALSE);
				m_hFindFile = INVALID_HANDLE_VALUE;
			}
			return bResult;
		};

	private:
		//-------------------------------------------------------------
		// �R�s�[�Ƒ�����֎~
		//-------------------------------------------------------------
		FindFile(const FindFile& src) {};
		void operator=(const FindFile& rhs) {};

	public:
		//-------------------------------------------------------------
		// �����Ɏg���t���p�X�����擾
		//-------------------------------------------------------------
		std::string GetFindFileName() { return m_fileName; };

		//-------------------------------------------------------------
		// �t�@�C�����f�B���N�g�����ǂ������ׂ�
		//-------------------------------------------------------------
		bool IsDirectory() { return ((m_findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0); };

		//-------------------------------------------------------------
		// �t�@�C����"."�܂���".."���ǂ������ׂ�
		//-------------------------------------------------------------
		bool IsDots() { return (::lstrcmp(m_findFileData.cFileName, _T(".")) == 0) || (::lstrcmp(m_findFileData.cFileName, _T("..")) == 0); };

		//-------------------------------------------------------------
		// �t�@�C�������擾
		//-------------------------------------------------------------
		std::string GetFileName() { return m_findFileData.cFileName; };

	private:
		TCHAR m_fileName[MAX_PATH];
		HANDLE m_hFindFile;
		WIN32_FIND_DATA m_findFileData;
	};

	//-----------------------------------------------------------------
	// ���b�N�t�@�C�������S�Ɉ������߂̃N���X
	//-----------------------------------------------------------------
	class Lockfile {
	public:
		//-------------------------------------------------------------
		// �R���X�g���N�^
		//-------------------------------------------------------------
		Lockfile() : m_hFile(INVALID_HANDLE_VALUE) {};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		virtual ~Lockfile() { Unlock(); };

		//-------------------------------------------------------------
		// ���b�N����
		//-------------------------------------------------------------
		BOOL Lock(const std::string& strLockfile);

		//-------------------------------------------------------------
		// �A�����b�N����
		//-------------------------------------------------------------
		BOOL Unlock();

		// ���b�N�����ǂ������ׂ�
		bool IsLocked() { return (m_hFile != INVALID_HANDLE_VALUE); };

	private:
		//-------------------------------------------------------------
		// �R�s�[�Ƒ����F�߂Ȃ�
		//-------------------------------------------------------------
		Lockfile(const Lockfile& src) {};
		void operator=(const Lockfile& lhs) {};

		//-------------------------------------------------------------
		// �����o�[�ϐ�
		//-------------------------------------------------------------
		HANDLE m_hFile;
	};


	//-----------------------------------------------------------------
	// INI�t�@�C���������N���X
	//-----------------------------------------------------------------
	class IniFile {
	public:
		//-------------------------------------------------------------
		// �f�t�H���g �R���X�g���N�^
		//-------------------------------------------------------------
		IniFile() {
			char buf[MAX_PATH];
			::GetModuleFileName(NULL, buf, sizeof(buf));
			::PathRenameExtension(buf, ".ini");
			m_iniFile = buf;
		};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		virtual ~IniFile() {};

		//-------------------------------------------------------------
		// ��������擾����
		//-------------------------------------------------------------
		std::string GetString(const std::string& strSection, const std::string& strKey, const std::string& strDefault) const {
			char buf[8192];
			::GetPrivateProfileString(strSection.c_str(), strKey.c_str(), strDefault.c_str(), buf, sizeof(buf), m_iniFile.c_str());
			return buf;
		};

		//-------------------------------------------------------------
		// �l���擾����
		//-------------------------------------------------------------
		UINT GetUInt(const std::string& strSection, const std::string& strKey, UINT uDefault) const {
			return ::GetPrivateProfileInt(strSection.c_str(), strKey.c_str(), uDefault, m_iniFile.c_str());
		};

		//-------------------------------------------------------------
		// ���������������
		//-------------------------------------------------------------
		void WriteString(const std::string& strSection, const std::string& strKey, const std::string& strValue) {
			::WritePrivateProfileString(strSection.c_str(), strKey.c_str(), strValue.c_str(), m_iniFile.c_str());
		};

		//-------------------------------------------------------------
		// �l(UINT)����������
		//-------------------------------------------------------------
		void WriteUInt(const std::string& strSection, const std::string& strKey, UINT uValue) {
			char work[16];	// 4294967295(0xffffffff)���������ނ̂ɏ\���ȃo�b�t�@
			wsprintf(work, "%u", uValue);
			::WritePrivateProfileString(strSection.c_str(), strKey.c_str(), work, m_iniFile.c_str());
		};

		//-------------------------------------------------------------
		// INI �t�@�C���̖��O��ݒ肷��
		//-------------------------------------------------------------
		void set_IniFile(const std::string& strIniFile) { m_iniFile = strIniFile; };

		//-------------------------------------------------------------
		// INI �t�@�C���̖��O���擾����
		//-------------------------------------------------------------
		std::string get_IniFile() { return m_iniFile; };

	private:
		std::string m_iniFile;
	};


	//-----------------------------------------------------------------
	//-----------------------------------------------------------------
	class util {
	public:
		//-------------------------------------------------------------
		// �Œ�t�@�C����񂩂�t�@�C���o�[�W�������擾
		//-------------------------------------------------------------
		static std::string get_file_version() {
			DWORD handle = 0;
			std::vector<char> fname(MAX_PATH);
			
			// �o�[�W������񃊃\�[�X���擾
			GetModuleFileName(NULL, fname.begin(), fname.size());
			std::vector<char> buf(GetFileVersionInfoSize(fname.begin(), &handle));
			GetFileVersionInfo(fname.begin(), handle, buf.size(), buf.begin());

			VS_FIXEDFILEINFO* p;
			UINT size;
			VerQueryValue(buf.begin(), "\\", (LPVOID*)&p, &size);

			std::ostrstream os;
			os	<< ((p->dwFileVersionMS & 0xffff0000) >> 16) << "."
				<< (p->dwFileVersionMS & 0x0000ffff) << "."
				<< ((p->dwFileVersionLS & 0xffff0000) >> 16) << "."
				<< (p->dwFileVersionLS & 0x0000ffff);

			return std::string(os.str(), os.pcount());
		};

		//-------------------------------------------------------------
		// �������u������BVB��StrReplace�̂悤�Ȋ֐��B
		//-------------------------------------------------------------
		static std::string str_replace(const std::string& src, LPCTSTR pFind, LPCTSTR pReplace);

		//-------------------------------------------------------------
		// �t�H���_�̍쐬
		//-------------------------------------------------------------
		static BOOL MakeDirectory(LPCTSTR lpPathName);

		//-------------------------------------------------------------
		// GetModuleFileName �֐��̃��b�p�[
		//-------------------------------------------------------------
		static std::string get_module_file_name(HMODULE hModule) {
			std::vector<char> filename(MAX_PATH);
			if (::GetModuleFileName(hModule, filename.begin(), filename.size()) == FALSE) {
				DWORD dwErr = ::GetLastError();
				throw kjm::Win32Exception(::GetLastError());
			}
			return filename.begin();
		};

		//-------------------------------------------------------------
		// SHFileOperation �� FO_COPY ���g�����t�@�C���̃R�s�[
		//-------------------------------------------------------------
		static int FOCopy(LPCTSTR lpFrom, LPCTSTR lpTo) {
			// SHFileOperation�̕�����́A\0\0�ŏI���K�v��������ۂ�
			char szFrom[MAX_PATH], szTo[MAX_PATH];
			szFrom[::lstrlen(::lstrcpy(szFrom, lpFrom)) + 1] = '\0';
			szTo  [::lstrlen(::lstrcpy(szTo  , lpTo  )) + 1] = '\0';

			SHFILEOPSTRUCT fileOp;
			memset(&fileOp, 0, sizeof(fileOp));
			fileOp.wFunc = FO_COPY;
			fileOp.pFrom = szFrom;
			fileOp.pTo = szTo;
			fileOp.fFlags = FOF_SILENT;

			return SHFileOperation(&fileOp);
		}

		//-------------------------------------------------------------
		// SHFileOperation �� FO_DELETE ���g�����t�@�C���̍폜
		//-------------------------------------------------------------
		static int FODelete(LPCTSTR lpPathName) {
			// SHFileOperation�̕�����́A\0\0�ŏI���K�v��������ۂ�
			char szFrom[MAX_PATH];
			szFrom[::lstrlen(::lstrcpy(szFrom, lpPathName)) + 1] = '\0';

			SHFILEOPSTRUCT fileOp;
			memset(&fileOp, 0, sizeof(fileOp));
			fileOp.wFunc = FO_DELETE;
			fileOp.pFrom = szFrom;
			fileOp.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;

			return ::SHFileOperation(&fileOp);
		};

		//-------------------------------------------------------------
		// ExitWindowsEx �̃��b�p�[
		//-------------------------------------------------------------
		static BOOL exit_windows_ex(UINT uFlags) {
			HANDLE hProcess = GetCurrentProcess();

			HANDLE hToken = NULL;
			if (OpenProcessToken(hProcess, TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken) == FALSE) {
#if defined(USE_WSSLOG)
				Winssa_Log("kjmlib", LOGLV_INFO, "kjm::exit_windows_ex(): OpenProcessToken() error: %d", GetLastError());
#endif
				return FALSE;
			}

			LUID luid;
			if (LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid) == FALSE) {
#if defined(USE_WSSLOG)
				Winssa_Log("kjmlib", LOGLV_INFO, "kjm::exit_windows_ex(): LookupPrivilegeValue() error: %d", GetLastError());
#endif
				return FALSE;
			}

			TOKEN_PRIVILEGES token, preToken;
			token.PrivilegeCount = 1;
			token.Privileges[0].Luid = luid;
			token.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			DWORD dwRet;
			if (AdjustTokenPrivileges(hToken, FALSE, &token, sizeof(preToken), &preToken, &dwRet) == FALSE) {
#if defined(USE_WSSLOG)
				Winssa_Log("kjmlib", LOGLV_INFO, "kjm::exit_windows_ex(): AdjustTokenPrivileges() error: %d", GetLastError());
#endif
				return FALSE;
			}

#if defined(USE_WSSLOG)
			Winssa_Log("kjmlib", LOGLV_INFO, "kjm::exit_windows_ex(): ���̃R�}���h��OS���V���b�g�_�E�����܂�: ExitWindowsEx(%d)", uFlags);
#endif
			BOOL bRet = ExitWindowsEx(uFlags, 0);
			if (bRet == FALSE) {
#if defined(USE_WSSLOG)
				Winssa_Log("kjmlib", LOGLV_INFO, "kjm::exit_windows_ex(): ExitWindowsEx(%d) error: %d", uFlags, GetLastError());
#endif
			}

			return bRet;
		};

	private:
		// �R���X�g���N�^�ƃf�X�g���N�^���v���C�x�[�g�ɂ��āA
		// �C���X�^���X����s���Ƃ���
		util() {};
		~util() {};
	};

	//-----------------------------------------------------------------
	// RECT �N���X
	//-----------------------------------------------------------------
	class Rect : public RECT {
	public:
		//-------------------------------------------------------------
		// �f�t�H���g�R���X�g���N�^
		//-------------------------------------------------------------
		Rect() {};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		~Rect() {};

		//-------------------------------------------------------------
		// OffsetRect �̃��b�p�[
		//-------------------------------------------------------------
		BOOL OffsetRect(int dx, int dy) { return ::OffsetRect(this, dx, dy); };

		//-------------------------------------------------------------
		// ����Ԃ�
		//-------------------------------------------------------------
		int Width() { return this->right - this->left; };

		//-------------------------------------------------------------
		// ������Ԃ�
		//-------------------------------------------------------------
		int Height() { return this->bottom - this->top; };
	};

	//-----------------------------------------------------------------
	// �E�B���h�E�N���X
	//-----------------------------------------------------------------
	class Window {
	public:
		//-------------------------------------------------------------
		// �f�t�H���g�R���X�g���N�^
		//-------------------------------------------------------------
		Window() : m_hwnd(NULL) {};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		virtual ~Window() {};

		//-------------------------------------------------------------
		// �A�^�b�`
		//-------------------------------------------------------------
		void Attach(HWND hwnd) { m_hwnd = hwnd; };

		//-------------------------------------------------------------
		// �f�^�b�`
		//-------------------------------------------------------------
		void Detach() { m_hwnd = NULL; };

	private:
		HWND m_hwnd;	// �Ǘ�����E�B���h�E�̃n���h��
	};

	//-----------------------------------------------------------------
	// �_�C�A���O�N���X
	//-----------------------------------------------------------------
	class Dialog {
	public:
		//-------------------------------------------------------------
		// �f�t�H���g�R���X�g���N�^
		//-------------------------------------------------------------
		Dialog() : m_hwnd(NULL) {};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		virtual ~Dialog() {};

		//-------------------------------------------------------------
		// ���[�h���X�_�C�A���O�̍쐬
		//-------------------------------------------------------------
		bool ShowModelessDialog(HINSTANCE hInstance, UINT uID, HWND hwndParent) {
			assert(this->m_hwnd == NULL);
			m_bModal = false;

#if defined(USE_WSSLOG)
			Winssa_Log("kjmlib", LOGLV_INFO, "kjm::Dialog::ShowModelessDialog(): hwndParent = %p, uID = %d", hwndParent, uID);
#endif

			m_hwnd = ::CreateDialogParam(hInstance, MAKEINTRESOURCE(uID), hwndParent, Dialog::DialogProc, (LPARAM)this);
			return (m_hwnd != NULL);
		};

		//-------------------------------------------------------------
		// ���[�_���_�C�A���O�̍쐬
		//-------------------------------------------------------------
		BOOL ShowModalDialog(HINSTANCE hInstance, UINT uID, HWND hwndParent) {
			assert(this->m_hwnd == NULL);
			m_bModal = true;

#if defined(USE_WSSLOG)
			Winssa_Log("kjmlib", LOGLV_INFO, "kjm::Dialog::ShowModalDialog(): hwndParent = %p, uID = %d", hwndParent, uID);
#endif

			return ::DialogBoxParam(hInstance, MAKEINTRESOURCE(uID), hwndParent, Dialog::DialogProc, (LPARAM)this);
		};

		//-------------------------------------------------------------
		// �N���X�ŗL�̃_�C�A���O�v���V�[�W��
		//-------------------------------------------------------------
		virtual BOOL DlgProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
			switch (uMsg) {
			case WM_COMMAND:
				return OnCommand(HIWORD(wParam), LOWORD(wParam), (HWND)lParam);

			case WM_DESTROY:
				return OnDestroy();

			case WM_INITDIALOG:
				return OnInitDialog((HWND)wParam, lParam);

			case WM_TIMER:
				return OnTimer(wParam, (TIMERPROC*)lParam);
			}

			return FALSE;
		};

		//-------------------------------------------------------------
		// �ǂ��g�����b�Z�[�W�́Avirtual�֐��Ƃ��Ē�`���Ă����B
		//
		// WM_COMMAND ���b�Z�[�W����
		// WM_DESTROY ���b�Z�[�W����
		// WM_INITDIALOG ���b�Z�[�W����
		// WM_TIMER ���b�Z�[�W����
		//-------------------------------------------------------------
		virtual BOOL OnCommand(WORD wNotifyCode, WORD wID, HWND hwndCtl) { return FALSE; };
		virtual BOOL OnDestroy() { return FALSE; };
		virtual BOOL OnInitDialog(HWND hwndFocus, LPARAM lInitParam) { return FALSE; };
		virtual BOOL OnTimer(WPARAM wTimerID, TIMERPROC* tmprc) { return FALSE; };

	public:
		//-------------------------------------------------------------
		// �E�B���h�E�̃C���X�^���X���擾
		//-------------------------------------------------------------
		HINSTANCE get_Instance() { return GetWindowInstance(m_hwnd); };

		//-------------------------------------------------------------
		// �E�B���h�E�̃^�C�g����ݒ肷��
		//-------------------------------------------------------------
		BOOL set_WindowText(LPCTSTR lpString) { return ::SetWindowText(m_hwnd, lpString); };

		//-------------------------------------------------------------
		// �_�C�A���O��̃R���g���[���Ƀe�L�X�g��ݒ肷��
		//-------------------------------------------------------------
		BOOL SetDlgItemText(int nIDDlgItem, LPCTSTR lpString) {
			assert(this->m_hwnd != NULL);
			assert(lpString != NULL);
			return ::SetDlgItemText(this->m_hwnd, nIDDlgItem, lpString);
		};

		//-------------------------------------------------------------
		// �w�肳�ꂽ ID �����R���g���[���̃E�B���h�E�n���h�����擾���܂��B
		//-------------------------------------------------------------
		HWND get_dlg_item(int nIDDlgItem) {
			assert(this->m_hwnd != NULL);
			return ::GetDlgItem(this->m_hwnd, nIDDlgItem);
		};

		//-------------------------------------------------------------
		// �w�肳�ꂽ�����l�𕶎���ɕϊ����āA�_�C�A���O�{�b�N�X����
		// �R���g���[���Ƀe�L�X�g�Ƃ��Đݒ肵�܂��B
		//-------------------------------------------------------------
		BOOL set_dlg_item_int(int nIDDlgItem, INT nValue) {
			assert(this->m_hwnd != NULL);
			return ::SetDlgItemInt(this->m_hwnd, nIDDlgItem, nValue, TRUE);
		};

		//-------------------------------------------------------------
		// �w�肳�ꂽ�����l�𕶎���ɕϊ����āA�_�C�A���O�{�b�N�X����
		// �R���g���[���Ƀe�L�X�g�Ƃ��Đݒ肵�܂��B
		//-------------------------------------------------------------
		BOOL set_dlg_item_int(int nIDDlgItem, UINT uValue) {
			assert(this->m_hwnd != NULL);
			return ::SetDlgItemInt(this->m_hwnd, nIDDlgItem, uValue, FALSE);
		};

		//-------------------------------------------------------------
		// �_�C�A���O�{�b�N�X���̎w�肳�ꂽ�R���g���[���̃e�L�X�g���A
		// �����l�ɕϊ����܂��B
		//-------------------------------------------------------------
		UINT get_dlg_item_int(int nIDDlgItem, BOOL bSigned) {
			assert(this->m_hwnd != NULL);
			return ::GetDlgItemInt(this->m_hwnd, nIDDlgItem, NULL, bSigned);
		};

		//-------------------------------------------------------------
		// �^�C�}�[��ݒ肷��
		//-------------------------------------------------------------
		UINT SetTimer(UINT nIDEvent, UINT uElapse) { return ::SetTimer(m_hwnd, nIDEvent, uElapse, NULL); };

		//-------------------------------------------------------------
		// �^�C�}�[��j������
		//-------------------------------------------------------------
		BOOL KillTimer(UINT uIDEvent) { return ::KillTimer(m_hwnd, uIDEvent); };

		//-------------------------------------------------------------
		// �_�C�A���O�����
		//-------------------------------------------------------------
		BOOL EndDialog(int nResult) {
			assert(m_hwnd != NULL);
			assert(m_bModal != false);
			return ::EndDialog(m_hwnd, nResult);
		};

		//-------------------------------------------------------------
		// DestroyWindow �̃��b�p�[
		//-------------------------------------------------------------
		BOOL DestroyWindow() {
			assert(m_hwnd != NULL);
			return ::DestroyWindow(this->m_hwnd);
		};

		//-------------------------------------------------------------
		// �E�B���h�E�̊g���������擾
		//-------------------------------------------------------------
		LONG GetExStyle() {
			assert(this->m_hwnd != NULL);
			return ::GetWindowLong(this->m_hwnd, GWL_EXSTYLE);
		};

		//-------------------------------------------------------------
		// �E�B���h�E�̊g��������ݒ�
		//-------------------------------------------------------------
		LONG SetExStyle(LONG dwNewExStyle) {
			assert(this->m_hwnd != NULL);
			return ::SetWindowLong(this->m_hwnd, GWL_EXSTYLE, dwNewExStyle);
		};

		//-------------------------------------------------------------
		// �E�B���h�E�̈ʒu���Ȃǂ�ݒ�
		//-------------------------------------------------------------
		BOOL SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags) {
			assert(this->m_hwnd != NULL);
			return ::SetWindowPos(this->m_hwnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
		};

		//-------------------------------------------------------------
		// �E�B���h�E����ɍőO��(top-most)�ɂ���
		//-------------------------------------------------------------
		BOOL SetTopMost() {
			assert(this->m_hwnd != NULL);
			this->SetExStyle(this->GetExStyle() | WS_EX_TOPMOST);
			return this->SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
		};

		//-------------------------------------------------------------
		// GetWindowRect �֐��̃��b�p�[
		//-------------------------------------------------------------
		BOOL GetWindowRect(kjm::Rect& rect) {
			assert(this->m_hwnd != NULL);
			return ::GetWindowRect(this->m_hwnd, &rect);
		};

		//-------------------------------------------------------------
		// MoveWindow �̃��b�p�[
		//-------------------------------------------------------------
		BOOL MoveWindow(int X, int Y, int nWidth, int nHeight, BOOL bRepaint) {
			assert(this->m_hwnd != NULL);
			return ::MoveWindow(this->m_hwnd, X, Y, nWidth, nHeight, bRepaint);
		};

		//-------------------------------------------------------------
		// �E�B���h�E���L�����ǂ������ׂ�
		//-------------------------------------------------------------
		bool IsAvailable() { return (this->m_hwnd != NULL); };

	private:
		//-------------------------------------------------------------
		// �W���̃_�C�A���O�v���V�[�W��
		//-------------------------------------------------------------
		static BOOL CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			static std::map<HWND, kjm::Dialog*> s_dialogMap;
		
			kjm::Dialog* pDialog = NULL;

			if (uMsg == WM_INITDIALOG) {
				pDialog = (kjm::Dialog*)lParam;
				pDialog->m_hwnd = hwnd;
				s_dialogMap[hwnd] = pDialog;
			} else {
				pDialog = s_dialogMap[hwnd];
			}

			BOOL bRet = FALSE;
			if (pDialog != NULL) {
				bRet = pDialog->DlgProc(uMsg, wParam, lParam);
			}

			if (uMsg == WM_NCDESTROY) {
				if (pDialog != NULL) {
					pDialog->m_hwnd = NULL;
				}
				s_dialogMap.erase(hwnd);
			}
			return bRet;
		};

	protected:
		HWND m_hwnd;	// �_�C�A���O�ւ̃n���h��

	private:
		bool m_bModal;	// ���[�_���_�C�A���O�̂Ƃ��Atrue 
	};

	//-----------------------------------------------------------------
	// NOTIFYICONDATA �N���X
	//-----------------------------------------------------------------
	class NotifyIcon {
	public:
		//-------------------------------------------------------------
		// �f�t�H���g�R���X�g���N�^
		//-------------------------------------------------------------
		NotifyIcon() { memset(&m_nid, 0, sizeof(m_nid)); };

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		virtual ~NotifyIcon() {};
		
		//-------------------------------------------------------------
		// �A�C�R���̒ǉ�
		//-------------------------------------------------------------
		BOOL Add(HWND hwnd, UINT uID, HICON hIcon, UINT uCallbackMessage, LPCTSTR pszTip) {
			memset(&m_nid, 0, sizeof(m_nid));

			m_nid.cbSize = sizeof(m_nid);
			m_nid.hWnd = hwnd;
			m_nid.uID = uID;

			if (hIcon != NULL) {
				m_nid.hIcon = hIcon;
				m_nid.uFlags |= NIF_ICON;
			}

			if (uCallbackMessage != 0) {
				m_nid.uCallbackMessage = uCallbackMessage;
				m_nid.uFlags |= NIF_MESSAGE;
			}

			if (pszTip != NULL) {
				lstrcpy(m_nid.szTip, pszTip);
				m_nid.uFlags |= NIF_TIP;
			}

			return Shell_NotifyIcon(NIM_ADD, &m_nid);
		};

		//-------------------------------------------------------------
		// �A�C�R���̍폜
		//-------------------------------------------------------------
		BOOL Delete() { return ::Shell_NotifyIcon(NIM_DELETE, &m_nid); };

		//-------------------------------------------------------------
		// �A�C�R���̕ύX
		//-------------------------------------------------------------
		BOOL Modify(HICON hIcon, UINT uCallbackMessage, LPCTSTR pszTip) {
			m_nid.uFlags = 0;

			if (hIcon != NULL) {
				m_nid.hIcon = hIcon;
				m_nid.uFlags |= NIF_ICON;
			}

			if (uCallbackMessage != 0) {
				m_nid.uCallbackMessage = uCallbackMessage;
				m_nid.uFlags |= NIF_MESSAGE;
			}

			if (pszTip != NULL) {
				lstrcpy(m_nid.szTip, pszTip);
				m_nid.uFlags |= NIF_TIP;
			}

			return Shell_NotifyIcon(NIM_MODIFY, &m_nid);
		};

	public:
		NOTIFYICONDATA m_nid;	// �A�C�R�����
	};

	//-----------------------------------------------------------------
	// �N���e�B�J���Z�N�V�������Ǘ�����N���X
	//-----------------------------------------------------------------
	class CriticalSection : public CRITICAL_SECTION {
	public:
		//-------------------------------------------------------------
		// �R���X�g���N�^(�N���e�B�J���Z�N�V�����̏����������˂�)
		//-------------------------------------------------------------
		CriticalSection() {
			::InitializeCriticalSection(this);
		};

		//-------------------------------------------------------------
		// �f�X�g���N�^
		//-------------------------------------------------------------
		~CriticalSection() {
			::DeleteCriticalSection(this);
		};

		//-------------------------------------------------------------
		// ���L���̎擾
		//-------------------------------------------------------------
		VOID Enter() {
			::EnterCriticalSection(this);
		};

		//-------------------------------------------------------------
		// ���L���̔j��
		//-------------------------------------------------------------
		VOID Leave() {
			::LeaveCriticalSection(this);
		};
	};
};
