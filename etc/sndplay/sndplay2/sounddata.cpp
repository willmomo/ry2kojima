
#include "sounddata.h"
#pragma comment(lib, "winmm.lib")

// コンストラクタ
//
CSoundData::CSoundData(HWND hwnd, const char* soundFile)
{
	m_hwnd = hwnd;
	m_soundFile = soundFile;
	m_deviceID = 0;

	// MCI_OPENでサウンドファイルを開く
	MCI_OPEN_PARMS mciOpenParms;
	memset(&mciOpenParms, 0, sizeof(mciOpenParms));
	mciOpenParms.lpstrElementName = m_soundFile.c_str();
	MCIERROR mcierror = mciSendCommand(0, MCI_OPEN, MCI_WAIT | MCI_OPEN_ELEMENT, (DWORD)&mciOpenParms);
	if (mcierror == 0) {
		m_deviceID = mciOpenParms.wDeviceID;
		m_mciLastErrorText = "";
	} else {
		char errText[256];
		if (mciGetErrorString(mcierror, errText, sizeof(errText))) {
			m_mciLastErrorText = errText;
		} else {
			wsprintf(errText, "unknown error code: %d", mcierror);
			m_mciLastErrorText = errText;
		}
	}
}

// デストラクタ
//
CSoundData::~CSoundData()
{
	Close();
}

// 再生
//
bool CSoundData::Play()
{
	if (m_deviceID == 0)	return true;

	MCI_PLAY_PARMS mciPlayParms;
	memset(&mciPlayParms, 0, sizeof(mciPlayParms));
	mciPlayParms.dwCallback = (DWORD)m_hwnd;
	mciPlayParms.dwFrom = 0;

	MCIERROR mcierror = mciSendCommand(m_deviceID, MCI_PLAY, MCI_FROM | MCI_NOTIFY, (DWORD)&mciPlayParms);
	if (mcierror == 0) {
		m_mciLastErrorText = "";
	} else {
		char errText[256];
		if (mciGetErrorString(mcierror, errText, sizeof(errText))) {
			m_mciLastErrorText = errText;
		} else {
			wsprintf(errText, "unknown error code: %d", mcierror);
			m_mciLastErrorText = errText;
		}
	}

	return (mcierror == 0);
}

// 停止
//
bool CSoundData::Stop()
{
	if (m_deviceID == 0)	return true;

	MCIERROR mcierror = mciSendCommand(m_deviceID, MCI_STOP, MCI_WAIT, 0);
	if (mcierror == 0) {
		m_mciLastErrorText = "";
	} else {
		char errText[256];
		if (mciGetErrorString(mcierror, errText, sizeof(errText))) {
			m_mciLastErrorText = errText;
		} else {
			wsprintf(errText, "unknown error code: %d", mcierror);
			m_mciLastErrorText = errText;
		}
	}

	return (mcierror == 0);
}

// 閉じる
//
bool CSoundData::Close()
{
	if (m_deviceID == 0)	return true;

	MCIERROR mcierror = mciSendCommand(m_deviceID, MCI_CLOSE, MCI_WAIT, 0);
	if (mcierror == 0) {
		m_mciLastErrorText = "";
	} else {
		char errText[256];
		if (mciGetErrorString(mcierror, errText, sizeof(errText))) {
			m_mciLastErrorText = errText;
		} else {
			wsprintf(errText, "unknown error code: %d", mcierror);
			m_mciLastErrorText = errText;
		}
	}

	m_deviceID = 0;

	return (mcierror == 0);
}
