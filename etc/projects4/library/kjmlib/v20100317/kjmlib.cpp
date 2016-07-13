//=====================================================================
// ���x������Ă� codesnipet
//=====================================================================
#include "kjmlib.h"
#include <time.h>
#include <assert.h>


//---------------------------------------------------------------------
// ���S�� sprintf 
//
// [����]
// ��Ԗڂ̈������Avector<char>�ւ̎Q�ƂɂȂ��Ă���ȊO�A
// sprintf�֐��Ɠ����ł��B
//---------------------------------------------------------------------
int sprintf_vec(std::vector<char>& buf, const char* format, ...) {
	va_list marker;

	// �o�b�t�@���w�肳��Ă��Ȃ����A256�o�C�g���蓖�ĂĂ���
	if (buf.size() == 0) {
		buf.resize(256);
	}

	va_start(marker, format);
	while (1) {

		// vsnprintf �ŕ�������쐬�B
		// �s�\���ȃo�b�t�@��Ƀf�[�^���쐬���Ă���ԁA���[�v���J��Ԃ��B
		int n = _vsnprintf(buf.begin(), buf.size(), format, marker);
		if (n != -1 && n != buf.size()) {
			break;
		}

		buf.resize(buf.size() + 256);
	}
	va_end(marker);

	return buf.size();
}


//---------------------------------------------------------------------
// __int64 ���o�͉\�ɂ��� ostream �w���p
//---------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, __int64 i) {
	TCHAR buf[32];
	_snprintf(buf, sizeof(buf), "%I64d", i);
	os << buf;
	return os;
}


//=====================================================================
// ���ԃN���X
//=====================================================================

//---------------------------------------------------------------------
// 1970/1/1 0:00:00 ����̌o�ߎ��Ԃŏ���������R���X�g���N�^
//---------------------------------------------------------------------
kjm::time::time(DWORD src) {
	struct tm* ptm = gmtime((time_t*)&src);
	if (ptm) {
		SYSTEMTIME st;
		memset(&st, 0, sizeof(st));
		st.wYear = ptm->tm_year + 1900;
		st.wMonth = ptm->tm_mon + 1;
		st.wDay = ptm->tm_mday;
		st.wHour = ptm->tm_hour;
		st.wMinute = ptm->tm_min;
		st.wSecond = ptm->tm_sec;

		SystemTimeToFileTime(&st, &m_ft);
	} else {
		memset(&m_ft, 0, sizeof(m_ft));
	}
}

//---------------------------------------------------------------------
// �������P���� YYYY/MM/DD HH:MM:SS ������ɕϊ����܂��B
//---------------------------------------------------------------------
_tstring kjm::time::to_string() {
	std::vector<TCHAR> temp_v(80);
	SYSTEMTIME st;

	FileTimeToSystemTime(&m_ft, &st);
	sprintf_vec(temp_v, _T("%04d/%02d/%02d %02d:%02d:%02d"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	return temp_v.begin();
}

//---------------------------------------------------------------------
// UTC���Ԃ����[�J�����Ԃɕϊ����܂��B
//---------------------------------------------------------------------
kjm::time& kjm::time::to_local_time() {
	FILETIME newFt;

	FileTimeToLocalFileTime(&m_ft, &newFt);

	m_ft = newFt;
	return *this;
}


//---------------------------------------------------------------------
// ���[�J�����Ԃ�UTC���Ԃɕϊ����܂��B
//---------------------------------------------------------------------
kjm::time& kjm::time::to_UTC_time() {
	FILETIME newFt;

	LocalFileTimeToFileTime(&m_ft, &newFt);

	m_ft = newFt;
	return *this;
}


//---------------------------------------------------------------------
// GetLocalTime
// ���݂̃��[�J���������擾���܂��B
//---------------------------------------------------------------------
kjm::time kjm::time::get_local_time() {
	kjm::time t;
	SYSTEMTIME st;

	::GetLocalTime(&st);
	::SystemTimeToFileTime(&st, &t.m_ft);

	return t;
}


//=====================================================================
// �C�x���g���O�ɃA�N�Z�X����N���X
//=====================================================================

//---------------------------------------------------------------------
// �C�x���g���O���J��
//---------------------------------------------------------------------
bool kjm::eventlog::open_event_log(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName) {
	// ���O�����`�F�b�N
	assert(m_hEventLog == NULL);

	m_hEventLog = OpenEventLog(lpUNCServerName, lpSourceName);
	return (m_hEventLog != NULL);
}


//---------------------------------------------------------------------
// �ۑ����ꂽ�C�x���g���O���J��
//---------------------------------------------------------------------
bool kjm::eventlog::open_backup_event_log(LPCTSTR lpUNCServerName, LPCTSTR lpFileName) {
	// ���O�����`�F�b�N
	assert(m_hEventLog == NULL);

	m_hEventLog = OpenBackupEventLog(lpUNCServerName, lpFileName);
	return (m_hEventLog != NULL);
}


//---------------------------------------------------------------------
// �C�x���g���O�����
//---------------------------------------------------------------------
bool kjm::eventlog::close_event_log() {
	BOOL bRet = TRUE;
	if (m_hEventLog != NULL) {
		bRet = CloseEventLog(m_hEventLog);
		m_hEventLog = NULL;
	}
	return (bRet != FALSE);
}


//---------------------------------------------------------------------
// �C�x���g���O��ǂ�
//
// [�߂�l]
//  1 ���R�[�h�ǂݍ��񂾁B
//  0 ���R�[�h���Ȃ�����(EOF)�B
// -1 �G���[�����������B
//---------------------------------------------------------------------
int kjm::eventlog::read_event_log(DWORD dwReadFlags, DWORD dwRecordOffset, kjm::eventlogrecord& evlogrec) {
	int ret = 0;
	DWORD nBytesRead;
	DWORD nMinNumberOfBytesNeeded;
	std::vector<char> work(1);

	while (1) {
		BOOL bRet = ReadEventLog(m_hEventLog, dwReadFlags, dwRecordOffset, work.begin(), work.size(), &nBytesRead, &nMinNumberOfBytesNeeded);
		if (bRet != FALSE) {
			ret = 1;	// ����̂Ƃ��́A���R�[�h����ۑ����ă��[�v�I��
			evlogrec.assign(work);
			break;
		}
		
		DWORD dwErr = GetLastError();
		if (dwErr == ERROR_HANDLE_EOF) {
			ret = 0;	// ���R�[�h���Ȃ��̂Ń��[�v�I��
			break;
		} else if (dwErr != ERROR_INSUFFICIENT_BUFFER) {
			ret = -1;	// �o�b�t�@������Ȃ��ȊO�̃G���[�́A�ُ�I��
			break;
		}

		work.resize(nMinNumberOfBytesNeeded);
	}

	return ret;
}


//=====================================================================
// �C�x���g���O�̈ꃌ�R�[�h��\������N���X
//=====================================================================

//---------------------------------------------------------------------
// �C�x���g�\�[�X�̖��O���擾
//---------------------------------------------------------------------
std::string kjm::eventlogrecord::get_source_name() {
	char* pSourceName = &m_evlogrec[sizeof(EVENTLOGRECORD)];
	return pSourceName;
}


//---------------------------------------------------------------------
// ���R�[�h�f�[�^���A�T�C������
//---------------------------------------------------------------------
void kjm::eventlogrecord::assign(const std::vector<char>& data) {
	m_evlogrec = data;

	EVENTLOGRECORD* p = (EVENTLOGRECORD*)m_evlogrec.begin();
}
