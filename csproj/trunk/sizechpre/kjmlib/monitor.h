//
// ���j�^�[���Ɋւ���R�[�h
//
// monitor : Version 0.2015.12.23
//
// r.kojima
//
#pragma once

#include <Windows.h>
#include <vector>

namespace kjm {
	//
	// ���j�^�[����ێ�����N���X
	//
	class monitorInfo {
	public:
		MONITORINFOEX m_info;

	public:
		monitorInfo(){}
		monitorInfo(const monitorInfo& src) : m_info(src.m_info) {}
		monitorInfo& operator=(const monitorInfo& src);
		~monitorInfo() {}

	public:
		BOOL getMonitorInfo(HMONITOR hMonitor);
	};

	//
	// ���j�^�[�ꗗ��ێ�����N���X
	//
	class monitors {
	public:
		typedef std::vector<monitorInfo> MONITORS;
		MONITORS m_monitors;

	public:
		monitors(){}
		monitors(const monitors& src) : m_monitors(src.m_monitors) {}
		monitors& operator=(const monitors& src);
		~monitors(){}

	public:
		BOOL enumDisplayMonitors(HDC hdc=NULL, LPCRECT lprcClip=NULL);

	public:
		int get_MonitorCount() const { return m_monitors.size(); }

		// �v���C�}���[�f�B�X�v���C���j�^�[�̃C���f�b�N�X���擾
		// ������Ȃ��ꍇ�́A-1��Ԃ�
		int findPrimaryDisplayMonitor() const;

	private:
		// enumDisplayMonitors �Ŏg�p����񋓊֐�
		// dwData �� this ��n���Ă���
		static BOOL CALLBACK monitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
	};
};
