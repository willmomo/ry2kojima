//---------------------------------------------------------------------
// ���x������Ă� codesnipet
//---------------------------------------------------------------------
#include "kjmlib.h"


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
// ���ԃN���X
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//---------------------------------------------------------------------
kjm::time::time() {
	memset(&m_ft, 0, sizeof(m_ft));
}

//---------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------
kjm::time::~time() {
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