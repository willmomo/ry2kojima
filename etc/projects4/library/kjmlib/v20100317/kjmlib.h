//---------------------------------------------------------------------
// ���x������Ă� codesnipet
//---------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>

#if defined(UNICODE)
typedef std::wstring	_tstring;
#else
typedef std::string		_tstring;
#endif

// ���S�� sprintf 
int sprintf_vec(std::vector<char>& buf, const char* format, ...);

// __int64 ���o�͉\�ɂ��� ostream �w���p
std::ostream& operator<<(std::ostream& os, __int64 i);


namespace kjm {
	//=================================================================
	// ���ԃN���X
	//=================================================================
#define FT_USEC		((__int64)10)		// ����Ŋ���ƁA1 usec
#define FT_MSEC		(FT_USEC * 1000)	// ����Ŋ���ƁA1 msec
#define FT_SEC		(FT_MSEC * 1000)	// ����Ŋ���ƁA1 sec
#define FT_MINUTE	(FT_SEC * 60)		// ����Ŋ���ƁA1 minute
#define FT_HOUR		(FT_MINUTE * 60)	// ����Ŋ���ƁA1 hour
#define FT_DAY		(FT_HOUR * 24)		// ����Ŋ���ƁA1 day

	class time {
	public:
		// �f�t�H���g�R���X�g���N�^
		time() { memset(&m_ft, 0, sizeof(m_ft)); };

		// 1970/1/1 0:00:00 ����̌o�ߎ��Ԃŏ���������R���X�g���N�^
		time(DWORD src);

		// �f�X�g���N�^
		~time() {};

		// �������P���� YYYY/MM/DD HH:MM:SS ������ɕϊ����܂��B
		_tstring to_string();

		// ���ݎ��Ԃ�100ns�P�ʂŕԂ�
		__int64 get_time() const { return *(__int64*)&m_ft; };

		// UTC���Ԃ����[�J�����Ԃɕϊ����܂��B
		time& to_local_time();

		// ���[�J�����Ԃ�UTC���Ԃɕϊ����܂��B
		time& to_UTC_time();

		__int64 operator-(const time& rhs) { return this->get_time() - rhs.get_time(); };

		// GetLocalTime
		// ���݂̃��[�J���������擾���܂��B
		static kjm::time get_local_time();

	private:
		FILETIME m_ft;
	};

	//=================================================================
	// �C�x���g���O�̈ꃌ�R�[�h��\������N���X
	//=================================================================
	class eventlogrecord {
	public:
		// �f�t�H���g�R���X�g���N�^
		eventlogrecord() {};

		// �f�X�g���N�^
		~eventlogrecord() {};

		// ���R�[�h�f�[�^���A�T�C������
		void assign(const std::vector<char>& data);

		// TimeGenerated ���擾
		time get_time_generated() { return time(((EVENTLOGRECORD*)m_evlogrec.begin())->TimeGenerated); };

		// TimeWritten ���擾
		time get_time_written() { return time(((EVENTLOGRECORD*)m_evlogrec.begin())->TimeWritten); };

		// �C�x���g�^�C�v���擾
		int get_event_type() { return ((EVENTLOGRECORD*)m_evlogrec.begin())->EventType; };

		// �C�x���g�\�[�X�̖��O���擾
		std::string get_source_name();

	private:
		// �R�s�[���֎~����
		eventlogrecord(const eventlogrecord& src) {};
		eventlogrecord& operator=(const eventlogrecord& rhs) { return *this; };

	private:
		std::vector<char> m_evlogrec;
	};

	//=================================================================
	// �C�x���g���O�ɃA�N�Z�X����N���X
	//=================================================================
	class eventlog {
	public:
		// �f�t�H���g�R���X�g���N�^
		eventlog() : m_hEventLog(NULL) {};

		// �f�X�g���N�^
		~eventlog() { close_event_log(); };

		// �C�x���g���O���J��
		bool open_event_log(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName);

		// �ۑ����ꂽ�C�x���g���O���J��
		bool open_backup_event_log(LPCTSTR lpUNCServerName, LPCTSTR lpFileName);

		// �C�x���g���O�����
		bool close_event_log();

		// �C�x���g���O��ǂ�
		//
		// [�߂�l]
		//  1 ���R�[�h�ǂݍ��񂾁B
		//  0 ���R�[�h���Ȃ�����(EOF)�B
		// -1 �G���[�����������B
		int read_event_log(DWORD dwReadFlags, DWORD dwRecordOffset, eventlogrecord& evlogrec);

	private:
		// �R�s�[���֎~����
		eventlog(const eventlog& src) {};
		eventlog& operator=(const eventlog& rhs) { return *this; };

	private:
		HANDLE m_hEventLog;
	};
};
