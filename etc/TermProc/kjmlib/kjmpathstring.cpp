//---------------------------------------------------------------------
// kjm::PathString �̎����t�@�C��
//---------------------------------------------------------------------
#include "kjmlib.h"
#include <shlwapi.h>


#if !defined(UNICODE)
#include <mbctype.h>
#endif


//---------------------------------------------------------------------
// \\?\ \\?\UNC ��ǉ������������Ԃ�
//---------------------------------------------------------------------
kjm::_tstring kjm::pathString::GetOverMaxPath() {
	kjm::_tstring result;

	if (PathIsUNC(m_str.c_str())) {
		result = _T("\\\\?\\UNC\\") + kjm::_tstring(&m_str[2]);
	} else {
		result = _T("\\\\?\\") + m_str;
	}

	return result;
}


//---------------------------------------------------------------------
// �p�X������̌��� '\' ��ǉ�����
//---------------------------------------------------------------------
kjm::pathString& kjm::pathString::AddBackslash() {
	// �󕶎���̎��́A�������Ȃ�
	if (m_str.empty()) {
		return *this;
	}

	// 1�����̎��́A'\' �Ɣ�r���ĒP���ɒǉ��������s��
	if (m_str.length() == 1) {
		if (m_str[0] != '\\') {
			m_str += '\\';
		}
		return *this;
	}

	// 2�����ȏ�̎�
#if defined(UNICODE)
	// UNICODE�̎��́A�P���ɍŌ�̈ꕶ�����`�F�b�N����΂悢
	if (m_str[m_str.length() - 1] != '\\') {
		m_str += '\\';
	}
#else
	if (_ismbstrail((unsigned char*)&m_str[0], (unsigned char*)&m_str[m_str.length() - 1]) != 0) {
		// �Ō�̈ꕶ�����A�}���`�o�C�g�̌㑱�o�C�g�̂Ƃ��́A�L�������킳�� '\' ���������Ă��ǂ�
		m_str += '\\';
	} else if (m_str[m_str.length() - 1] != '\\') {
		// �Ō�̈ꕶ�����A�}���`�o�C�g�̌㑱�o�C�g�łȂ���΁A1�o�C�g�����Ȃ̂ŁA
		// '\' �łȂ���΁A'\' ����������B
		m_str += '\\';
	}
#endif

	return *this;
}


//---------------------------------------------------------------------
// �p�X������̌���
//---------------------------------------------------------------------
kjm::pathString& kjm::pathString::Append(const kjm::_tstring& pMore) {
	// �܂��A�A���̂��߂� '\' ��ǉ�����B
	AddBackslash();

	// ���ɗ^����ꂽ���������������
	m_str += pMore;

	return *this;
}
