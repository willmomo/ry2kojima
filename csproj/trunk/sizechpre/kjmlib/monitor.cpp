//
// ���j�^�[���Ɋւ���R�[�h
//
// monitor : Version 0.2015.12.23
//
// r.kojima
//
#include "monitor.h"

//=====================================================================
//
//  monitorInfo �N���X�̎���
//
//=====================================================================

// ������Z�q
//
kjm::monitorInfo& kjm::monitorInfo::operator=(const kjm::monitorInfo& src) {
	if (this != &src) {
		this->m_info = src.m_info;
	}
	return *this;
}

// ���j�^�[���̎擾
//
BOOL kjm::monitorInfo::getMonitorInfo(HMONITOR hMonitor) {
	m_info.cbSize = sizeof(m_info);
	return GetMonitorInfo(hMonitor, &m_info);
}

//=====================================================================
//
//  monitors �N���X�̎���
//
//=====================================================================

// ������Z�q
kjm::monitors& kjm::monitors::operator=(const kjm::monitors& src) {
	if (this != &src) {
		this->m_monitors = src.m_monitors;
	}
	return *this;
}

// ���j�^�[���
//
BOOL kjm::monitors::enumDisplayMonitors(HDC hdc, LPCRECT lprcClip) {
	return EnumDisplayMonitors(hdc, lprcClip, kjm::monitors::monitorEnumProc, (LPARAM)this);
}

// �v���C�}���[�f�B�X�v���C���j�^�[�̃C���f�b�N�X���擾
int kjm::monitors::findPrimaryDisplayMonitor() const {
	for (MONITORS::size_type i = 0; i < m_monitors.size(); i++) {
		if (m_monitors[i].m_info.dwFlags & MONITORINFOF_PRIMARY) {
			return i;
		}
	}
	return -1;
}

// ���j�^�[�񋓊֐�
//
BOOL CALLBACK kjm::monitors::monitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	kjm::monitors* p = (kjm::monitors*)dwData;
	
	kjm::monitorInfo info;
	info.getMonitorInfo(hMonitor);
	p->m_monitors.push_back(info);

	return TRUE;
}
