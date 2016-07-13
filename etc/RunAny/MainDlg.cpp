/**
 */
#include "MainDlg.h"
#include "resource.h"
#include <string>


#define TIMER_1SEC	1


// WM_INITDIALOG ���b�Z�[�W����

BOOL MainDlg::OnInitDialog(HWND hwndFocus, LPARAM lInitParam) {

	kjm::IniFile iniFile;	// �f�t�H���g�ŁARunAny.ini ���Q��

	this->SetTopMost();

	std::string runCommand = iniFile.GetString("option", "RunCommand", "");
	this->m_procExp.Create(runCommand);
	//this->m_procExp.Create("exp gmc/mpcadmin@mpc file=expdat.dmp log=expdat.log");
	//this->m_procExp.Create("exp scott/tiger@mpc file=expdat.dmp log=expdat.log");

	this->m_dwStart = GetTickCount();

	this->SetTimer(TIMER_1SEC, 1000);

	return FALSE;
}


// WM_COMMAND ���b�Z�[�W����

BOOL MainDlg::OnCommand(WORD wNotifyCode, WORD wID, HWND hwndCtl) {

	switch (wID) {
	case IDOK:
	case IDCANCEL:
		{
			DWORD exitCode = 0;
			this->m_procExp.GetExitCodeProcess(&exitCode);
			if (exitCode == STILL_ACTIVE) {
				this->m_procExp.Terminate(1);
				this->m_procExp.Wait();
				this->m_procExp.Close();
			}
			this->EndDialog(0);
		}
		return TRUE;
	}

	return FALSE;
}

// WM_TIMER ���b�Z�[�W����

BOOL MainDlg::OnTimer(WPARAM wTimerID, TIMERPROC* tmprc) {

	// �o�ߎ��ԍX�V
	char buf[80];
	DWORD dwTime = (GetTickCount() - this->m_dwStart) / 1000;
	if (dwTime < 60) {
		wsprintf(buf, "%d �b�o��", dwTime);
	} else if (dwTime < 3600) {
		wsprintf(buf, "%d �� %d �b�o��", dwTime / 60, dwTime % 60);
	} else {
		wsprintf(buf, "%d ���� %d �� %d �b�o��", dwTime / 3600, (dwTime % 3600) / 60, (dwTime % 3600) % 60);
	}
	this->SetDlgItemText(IDC_TIME, buf);

	DWORD exitCode = 0;
	this->m_procExp.GetExitCodeProcess(&exitCode);

	if (exitCode == 0) {
		// ����I���ŏI������ꍇ

		this->KillTimer(TIMER_1SEC);

		this->m_procExp.Close();

		// ���k
		char szOutput[1024];
		szOutput[0] = '\0';
		int retLha = Unlha(this->m_hwnd, "a -gm1 -n expdat.lzh expdat.dmp", szOutput, sizeof(szOutput));
		if (retLha != 0) {
			// Winssa_Log(LOGC_COM, LOGLV_ERR, "[B1] BackupThrd_Compress(): ���k�t�@�C���̍쐬�Ɏ��s���܂���(���^�[���R�[�h : %d)�B%s", retLha, szOutput);
		}

		this->EndDialog(0);
	} else if (exitCode != STILL_ACTIVE) {
		// ����I���ȊO�ŏI������ꍇ

		this->KillTimer(TIMER_1SEC);

		this->m_procExp.Close();

		this->EndDialog(0);
	}

	return TRUE;
}
