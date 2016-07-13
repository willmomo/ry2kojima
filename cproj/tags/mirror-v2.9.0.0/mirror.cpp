//---------------------------------------------------------------------
// �~���[�����O���s���A�v���P�[�V����
//
// 2015/05/01 ���O�̕ۑ��ʂ�ύX(1�t�@�C��1M->2M,9->99����)
//---------------------------------------------------------------------

#pragma warning(disable : 4786)

#include "mirror.h"

//---------------------------------------------------------------------
// �O���[�o���ϐ�
//---------------------------------------------------------------------
kjm::log g_log;


//---------------------------------------------------------------------
// �R�s�[�i���\��
//---------------------------------------------------------------------
DWORD CALLBACK copyProgress(LARGE_INTEGER TotalFileSize,
							LARGE_INTEGER TotalBytesTransferred,
							LARGE_INTEGER StreamSize,
							LARGE_INTEGER StreamBytesTransferred,
							DWORD dwStreamNumber,
							DWORD dwCallbackReason,
							HANDLE hSourceFile,
							HANDLE hDestinationFile,
							LPVOID lpData)
{
	static DWORD s_prevPrint;	// �O��\������
	static DWORD s_t1;

	if (dwCallbackReason == CALLBACK_STREAM_SWITCH) {
		s_t1 = GetTickCount();
	}

	// (�O��\���X�V����A��b�ȏ�o��) or (�t�@�C���̐V�����o�͊J�n) or (�t�@�C���̏������ݏI��)�ŁA�\���X�V
	if (((GetTickCount() - s_prevPrint) > 1000) || (dwCallbackReason == CALLBACK_STREAM_SWITCH) || (StreamSize.QuadPart == StreamBytesTransferred.QuadPart)) {
		DWORD t = (s_prevPrint = GetTickCount()) - s_t1;

		_tprintf(_T("%3I64d%% %I64d/%I64d "),
			(TotalFileSize.QuadPart == 0) ? 100 : (TotalBytesTransferred.QuadPart * 100) / TotalFileSize.QuadPart,
			TotalBytesTransferred.QuadPart,
			TotalFileSize.QuadPart);

		// �R�s�[���Ԃ���b�𒴂�����]�����x��\��
		if (t > 1000) {
			double mbps = ((double)TotalBytesTransferred.QuadPart / ((double)t / 1000.0)) / 1024.0 / 1024.0;
			if (mbps > 0) {
				_tprintf(_T("%6.3fmbps"), mbps);
			}
		}
	}

	_tprintf(_T("\r"));

	return PROGRESS_CONTINUE;
}

//---------------------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//---------------------------------------------------------------------------
BackupMirror::BackupMirror() {
	// �����o�[�̏�����
	m_mode = modeBackupOnly;
	m_fTest = false;
	m_fArchive = false;
	m_fTimeGup = false;
	m_fNoEmptyFolder = false;
	m_warningSec = WARNING_SEC;
	m_fVerbose = false;
	m_fReverse = false;
	m_fIgnoreDate = false;
	m_fIgnoreSize = false;
	m_fNoRecurse = false;
	m_fStrict = false;

	m_defaultExcludeFolders.push_back(_T("$recycle.bin"));
	m_defaultExcludeFolders.push_back(_T("recycler"));
	m_defaultExcludeFolders.push_back(_T("system volume information"));

	m_defaultExcludeFiles.push_back(_T("thumbs.db"));
}

//---------------------------------------------------------------------
// �t�@�C���̃R�s�[
//
// @arg orgBase  ���΃p�X��\������Ƃ��̊�ɂȂ�t�H���_�B
//---------------------------------------------------------------------
bool BackupMirror::copy_file(const kjm::fileInfo& fiOrg, const _tstring& strMir, const _tstring& orgBase) {
	kjm::tickCounter tc;
	_tstring strMode;

	// �A�[�J�C�u�̂݃��[�h�̎��́A�R�s�[���̑������`�F�b�N����
	if (this->get_ArchiveOnlyMode() && !fiOrg.isArchive()) {
		return true;
	}

	// �R�s�[��t�@�C���̗L�����m�F����
	kjm::fileInfo mirFile(strMir);
	if (mirFile.exists()) {

#if 0	// �f�o�b�O�p�̏��o��
		g_log.writee(_T("<%s>"), fiOrg.fullName().c_str());
		g_log.writee(_T("�쐬�����@�@�F%s %s"),
			fiOrg.creationTime().toString().c_str(), mirFile.creationTime().toString().c_str());
		g_log.writee(_T("�A�N�Z�X�����F%s %s"),
			fiOrg.lastAccessTime().toString().c_str(), mirFile.lastAccessTime().toString().c_str());
		g_log.writee(_T("�X�V�����@�@�F%s %s"),
			fiOrg.lastWriteTime().toString().c_str(), mirFile.lastWriteTime().toString().c_str());
#endif
		// �T�C�Y�̍����L�^
		__int64 diffSize = fiOrg.length() - mirFile.length();

		// ���Ԃ̍����L�^
		__int64 diff = (__int64)(fiOrg.lastWriteTime() - mirFile.lastWriteTime()).totalSeconds();

		if (this->get_IgnoreDate() == false && this->get_IgnoreSize() == false) {
			// ���t�ƃT�C�Y�A�ǂ��炪����Ă��Ă��t�@�C���̓R�s�[�����i�ʏ�j
#if _MSC_VER <= 1200	// VC6 �ȉ�
			if (diffSize == 0 && abs((int)diff) <= 2) return true;
#else
			if (diffSize == 0 && _abs64(diff) <= 2) return true;
#endif
		} else if (this->get_IgnoreDate() != false && this->get_IgnoreSize() == false) {
			// �T�C�Y�����Ⴂ�̔��f��Ƃ��郂�[�h
			if (diffSize == 0) return true;
		} else if (this->get_IgnoreDate() == false && this->get_IgnoreSize() != false) {
			// ���t�����Ⴂ�̔��f��Ƃ��郂�[�h
#if _MSC_VER <= 1200	// VC6 �ȉ�
			if (abs((int)diff) <= 2) return true;
#else
			if (_abs64(diff) <= 2) return true;
#endif
		} else {
			// ���t���T�C�Y���Ⴂ�̔��f��Ƃ��Ȃ��i���݂��Ȃ��t�@�C�������������̑ΏۂɂȂ�I�j
			return true;
		}

		//if (diff != 0) g_log.writee(_T("����: %I64d"), diff);

		strMode = _T("�X�V");
		g_log.writee(_T("%s [%s] %c (%s) %5s"),
			strMode.c_str(),
			fiOrg.lastWriteTime().toString().c_str(),
			(fiOrg.lastWriteTime() > mirFile.lastWriteTime() ? '>' : '<'),
			mirFile.lastWriteTime().toString().c_str(),
			kjm::util::toHumanReadable(fiOrg.length()).c_str());

		if (!get_TestMode()) {
			// �R�s�[��� readonly/system/hidden �����Ă���Ƃ��͗��Ƃ��B
			DWORD dwAttr = kjm::file::getAttributes(strMir);
			if (dwAttr != 0xffffffff) {
				/*** �����ύX�̃��O�͏o���Ȃ��悤�ɂ���B
				if ((dwAttr & kjm::readOnly) != 0) {
					g_log.writee(_T("'%s' �� Read-only ���O���܂��B"), strMir.c_str());
				}
				if ((dwAttr & kjm::system) != 0) {
					g_log.writee(_T("'%s' �� System    ���O���܂��B"), strMir.c_str());
				}
				if ((dwAttr & kjm::hidden) != 0) {
					g_log.writee(_T("'%s' �� Hidden    ���O���܂��B"), strMir.c_str());
				}
				***/

				if ((dwAttr & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY)) != 0) {
					kjm::file::setAttributes(strMir, (dwAttr & ~(FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY)));
				}
			} else {
				g_log.writee(_T("'%s' �̑������擾�ł��܂���ł����B"), strMir.c_str());
			}
		}
	} else {
		// �R�s�[��t�@�C�����������́A�R�s�[���ėǂ��B
		strMode = _T("�ǉ�");
		g_log.writee(_T("%s                         [%s] %5s"),
			strMode.c_str(), fiOrg.lastWriteTime().toString().c_str(),
			kjm::util::toHumanReadable(fiOrg.length()).c_str());
	}

	g_log.writee(_T("�� - %c%c%c%c %s"),
		(fiOrg.isArchive()  ? 'A' : ' '),
		(fiOrg.isSystem()   ? 'S' : ' '),
		(fiOrg.isHidden()   ? 'H' : ' '),
		(fiOrg.isReadonly() ? 'R' : ' '),
		fiOrg.fullName().c_str());
	g_log.writee(_T("�� - %c%c%c%c %s"),
		'_', '_', '_', '_', strMir.c_str());

	if (!get_TestMode()) {
		DWORD t1 = GetTickCount();

		// �o�b�N�A�b�v��t�H���_�́A��ɍ쐬����
		try {
			kjm::directory::createDirectory(kjm::path::removeFileSpec(strMir));
		} catch (kjm::exception e) {
			g_log.writee(_T("error %d: %s"), e.m_errCode, e.m_errMsg.c_str());
			return false;
		}
		
		//int ret = kjm::util::copyFile2(fiOrg.fullName().c_str(), strMir.c_str(), FALSE);
		BOOL bCancel = FALSE;
		BOOL ret = kjm::util::copyFileEx(fiOrg.fullName().c_str(), strMir.c_str(), copyProgress, NULL, &bCancel, 0);
		_tprintf(_T("\n"));

		DWORD t2 = GetTickCount();
		if (ret == FALSE) {
			DWORD dwErr = GetLastError();

			_tstring temp = kjm::util::chomp(kjm::util::formatMessageBySystem(dwErr));

			g_log.writee(_T("copyFile error %d [ %s ]"), dwErr, kjm::util::chomp(kjm::util::formatMessageBySystem(dwErr)).c_str());
			g_log.writee(_T("ret = %d"), ret);
			g_log.write(_T("BATCMD: copy \"%s\" \"%s\""), fiOrg.fullName().c_str(), strMir.c_str());

			if (dwErr != ERROR_SHARING_VIOLATION && dwErr != ERROR_ACCESS_DENIED) {
				return false;
			} else {
				if (this->get_Strict()) {
					// �������[�h�̂Ƃ��́A�r���n�G���[���G���[�����ɂ��܂��B
					return false;
				} else {
					_tout << _T("�������p�����܂��B\n");
					g_log.write(_T("�������p�����܂��B"));
				}
			}
		}
	}

	// �A�[�J�C�u�̂݃��[�h�̎��́A�R�s�[���̑����𗎂Ƃ�
	if (this->get_ArchiveOnlyMode()) {
		DWORD dwAttr = kjm::file::getAttributes(fiOrg.fullName());
		if (dwAttr != 0xffffffff) {
			kjm::file::setAttributes(fiOrg.fullName(), (dwAttr & ~FILE_ATTRIBUTE_ARCHIVE));
		} else {
			g_log.writee(_T("'%s' �̑������擾�ł��܂���ł����B"), fiOrg.fullName().c_str());
		}
	}

	tc.stop();
	if (tc.elapseMin() >= 1) {
		_tout << _T("(�R�s�[���x ") << ((double)(fiOrg.length() / tc.elapseSec()) / 1024.0 / 1024.0) << _T(" mbps)\n");
	}

	return true;
}

//-----------------------------------------------------------------------------
// �t�@�C�������A���O���閼�O���ǂ������ׂ�
//
// [���l]
// ���O���X�g�́A[��������]���ĕۑ�����Ă���̂ŁA�t�@�C�����������������Ĕ�r
// ����B
//-----------------------------------------------------------------------------
bool BackupMirror::is_exclude(const _tstring& strFname) {
	_tstring lowerFname = kjm::util::toLower(strFname);

	for (std::vector<_tstring>::size_type i = 0; i < m_excludeList.size(); i++) {
		if (lowerFname.find(m_excludeList[i]) != _tstring::npos) {
			if (this->get_Verbose()) {
				g_log.writee(_T("'%s' �́A���O�t�@�C���ł��B[%s] �Ƀ}�b�`"), strFname.c_str(), m_excludeList[i].c_str());
			}
			return true;
		}
	}

	return false;
}

static bool name_desc(const kjm::folderInfo& left, const kjm::folderInfo& right) {
	return left.getFileName() > right.getFileName();
}

//---------------------------------------------------------------------------
// ������z��(s1)�ɕ�����(s2)���A���邩�ǂ����𒲂ׂ�
//---------------------------------------------------------------------------
static bool isInclude(const kjm::_tstrings& s1, const kjm::_tstring& s2) {
	struct compareFunc {
		kjm::_tstring m_s2;
		compareFunc(const kjm::_tstring& s) : m_s2(s) {}
		bool operator()(const kjm::_tstring& s) const { return (_tcsicmp(s.c_str(), m_s2.c_str()) == 0); }
	};
	return (std::find_if(s1.begin(), s1.end(), compareFunc(s2)) != s1.end());
}

//---------------------------------------------------------------------
// �o�b�N�A�b�v
//---------------------------------------------------------------------
bool BackupMirror::backup(const _tstring& strOrg, const _tstring& strMir) {
	static _tstring s_orgBase;
	bool bRet = true;

	//g_log.writee(_T("{DEBUG} BackupMirror::backup 1(%s, %s)"), strOrg.c_str(), strMir.c_str());

	if (s_orgBase.empty()) s_orgBase = strOrg;

	//g_log.writee(_T("{DEBUG} BackupMirror::backup 2(%s, %s)"), strOrg.c_str(), strMir.c_str());

	if (!get_TestMode()) {
		// �o�b�N�A�b�v��t�H���_�́A��ɍ쐬����
		if (this->get_NoEmptyFolder() == false) {
			try {
				kjm::directory::createDirectory( kjm::util::removeBackslash( strMir ) );
			} catch (kjm::exception e) {
				g_log.writee(_T("error %d: %s"), e.m_errCode, e.m_errMsg.c_str());
				return false;
			}
		}
	}

	//g_log.writee(_T("{DEBUG} BackupMirror::backup 3(%s, %s)"), strOrg.c_str(), strMir.c_str());

	// --norecurse���w�肳��Ă��Ȃ��Ƃ��́A�T�u�t�H���_�̏������s���B
	if (this->get_NoRecurse() == false) {
		std::vector<kjm::folderInfo> subFolders = kjm::folderInfo(strOrg).getSubFolders();	// �T�u�t�H���_���

		// �������T�u�t�H���_����A�W�����O�t�H���_����菜��
		std::vector<kjm::folderInfo>::iterator it = subFolders.begin();
		while (it != subFolders.end()) {
			if (isInclude(m_defaultExcludeFolders, it->getFileName())) {
				it = subFolders.erase(it);
			} else {
				++it;
			}
		}

		if (this->get_Reverse()) {
			std::sort(subFolders.begin(), subFolders.end(), name_desc);
		}

		//g_log.writee(_T("{DEBUG} BackupMirror::backup 4 %d(%s, %s)"), subFolders.size(), strOrg.c_str(), strMir.c_str());

		for (std::vector<kjm::folderInfo>::size_type j = 0; j < subFolders.size(); j++) {
			if (!is_exclude(subFolders[j].getFullPath())) {
				try {
					if (!backup(kjm::path::append(strOrg, subFolders[j].getFileName()), kjm::path::append(strMir, subFolders[j].getFileName()))) {
						bRet = false;
						break;
					}
				} catch (kjm::exception e) {
					g_log.writee(_T("%s(%d) : error %d: %s"), _T__FILE__, __LINE__, e.m_errCode, e.m_errMsg.c_str());
					bRet = false;
					break;
				}
			}
		}
	}

	// ���ʃt�H���_�̃R�s�[���������ς񂾎������A�����̃t�@�C�����R�s�[����B
	if (bRet) {
		std::vector<kjm::fileInfo> files = kjm::folderInfo(strOrg).getFiles();

		// �������t�@�C������A�W�����O�t�@�C������菜��
		std::vector<kjm::fileInfo>::iterator it = files.begin();
		while (it != files.end()) {
			if (isInclude(m_defaultExcludeFiles, it->name())) {
				it = files.erase(it);
			} else {
				++it;
			}
		}

		// �t�H���_���̃t�@�C�����o�b�N�A�b�v
		for (std::vector<kjm::fileInfo>::size_type i = 0; i < files.size(); i++) {
			if (!is_exclude(files[i].fullName())) {
				try {
					if (!copy_file(files[i], kjm::path::append(strMir, files[i].name()), s_orgBase)) {
						bRet = false;
						break;
					}
				} catch (kjm::exception e) {
					g_log.writee(_T("%s(%d) : error %d: %s"), _T__FILE__, __LINE__, e.m_errCode, e.m_errMsg.c_str());
					bRet = false;
					break;
				}
			}
		}
	}

	return bRet;
}

//---------------------------------------------------------------------
// �~���[�����O
//---------------------------------------------------------------------
bool BackupMirror::mirror(const _tstring& strOrg, const _tstring& strMir) {
	bool bRet = true;
	kjm::folderInfo mirFolder(strMir);
	std::vector<kjm::fileInfo> files = mirFolder.getFiles();

	bool fFileExists;

	// --norecurse���w�肳��Ă��Ȃ��Ƃ��́A�T�u�t�H���_�̏������s���B
	if (this->get_NoRecurse() == false) {
		std::vector<kjm::folderInfo> subFolders = mirFolder.getSubFolders();
		for (std::vector<kjm::folderInfo>::size_type j = 0; j < subFolders.size(); j++) {
			// ��{�I�ɁA�I���W�i�����Ƀt�@�C�����Ȃ��Ƃ��A�~���[�����폜����B
			// �t�@�C�����Ȃ��Ƃ������f���
			// file_exists �� false ����
			// �G���[�R�[�h���A ERROR_FILE_NOT_FOUND or ERROR_PATH_NOT_FOUND
			// �ƁA����I�ɂ���B
			// �I���W�i������̃l�b�g���[�N�s�ǂł��Afile_exists�́Afalse�ɂȂ邽�߂̑΍�B
			SetLastError(NOERROR);
			try {
				fFileExists = kjm::path::fileExists(kjm::path::append(strOrg, subFolders[j].getFileName()));
			} catch (kjm::exception e) {
				g_log.writee(_T("%s(%d) : error %d: %s"), _T__FILE__, __LINE__, e.m_errCode, e.m_errMsg.c_str());
				bRet = false;
				break;
			}
			DWORD dwErr = GetLastError();

			if (is_exclude(subFolders[j].getFullPath()) || (fFileExists == false && (dwErr == ERROR_FILE_NOT_FOUND || dwErr == ERROR_PATH_NOT_FOUND))) {

				g_log.writee(_T("�폜�t�H���_ %s"), 
					//kjm::path::relativePathTo(strMir, FILE_ATTRIBUTE_DIRECTORY, subFolders[j].getFullPath(), FILE_ATTRIBUTE_DIRECTORY).c_str());
					subFolders[j].getFullPath().c_str());

				if (!get_TestMode()) {
					try {
						kjm::util::removeFolder(subFolders[j].getFullPath());
					} catch (kjm::exception e) {
						_tout << _T("\n");

						g_log.writee(_T("removeFolder error %d: %s"), e.m_errCode, e.m_errMsg.c_str());
						g_log.writee(_T("�R�s�[�� - %s"), strOrg.c_str());
						g_log.writee(_T("�R�s�[�� - %s"), strMir.c_str());

						bRet = false;
						break;
					}
				}
			} else {
				try {
					if (mirror(kjm::path::append(strOrg, subFolders[j].getFileName()), kjm::path::append(strMir, subFolders[j].getFileName())) == false) {
						bRet = false;
						break;
					}
				} catch (kjm::exception e) {
					g_log.writee(_T("%s(%d) : error %d: %s"), _T__FILE__, __LINE__, e.m_errCode, e.m_errMsg.c_str());
					bRet = false;
					break;
				}
			}
		}
	}

	// �t�H���_���̃t�@�C�����~���[�����O
	for (std::vector<kjm::fileInfo>::size_type i = 0; i < files.size(); i++) {
		SetLastError(NOERROR);
		try {
			fFileExists = kjm::path::fileExists(kjm::path::append(strOrg, files[i].name()));
		} catch (kjm::exception e) {
			g_log.writee(_T("%s(%d) : error %d: %s"), _T__FILE__, __LINE__, e.m_errCode, e.m_errMsg.c_str());
			bRet = false;
			break;
		}
		DWORD dwErr = GetLastError();

		if (is_exclude(files[i].fullName()) || (fFileExists == false && (dwErr == ERROR_FILE_NOT_FOUND || dwErr == ERROR_PATH_NOT_FOUND))) {

			//_tout << _T("�폜����t�@�C�� - ") << kjm::path::relativePathTo(strMir, FILE_ATTRIBUTE_DIRECTORY, files[i].fullName(), 0);
			//g_log.write(_T("�폜 %s"), kjm::path::relativePathTo(strMir, FILE_ATTRIBUTE_DIRECTORY, files[i].fullName(), 0).c_str());

			_tout << _T("�폜����t�@�C�� - ") << files[i].fullName();
			g_log.write(_T("�폜 %s"), files[i].fullName().c_str());

			if (!get_TestMode()) {
				try {
					kjm::file::deleteFile(files[i].fullName(), true);
				} catch (kjm::exception e) {
					g_log.writee(_T("error %d: %s"), e.m_errCode, e.m_errMsg.c_str());
					bRet = false;
					break;
				}
			}

			_tout << _T("\n");
		}
	}
	return bRet;
}

//---------------------------------------------------------------------
// �o�b�N�A�b�v�J�n
//---------------------------------------------------------------------
bool BackupMirror::StartBackup() {
	bool bRet = false;
	kjm::tickCounter tc;

	tc.start();
	g_log.writee(_T("�o�b�N�A�b�v�J�n"));

	//g_log.writee(_T("{DEBUG} BackupMirror::mirror(%s, %s)"), m_strOrg.c_str(), m_strMir.c_str());
	bRet = backup(m_strOrg, m_strMir);
	if (bRet == false) {
		g_log.writee(_T("�o�b�N�A�b�v�Ɏ��s���܂����B"));
	}

	//g_log.writee(_T("�o�b�N�A�b�v���v����: %d �b"), tc.elapseSec());
	
	return bRet;
}

//---------------------------------------------------------------------
// �~���[�����O�J�n
//---------------------------------------------------------------------
bool BackupMirror::StartMirror() {
	bool bRet = false;
	kjm::tickCounter tc;

	tc.start();
	g_log.writee(_T("�~���[�����O�J�n"));

	bRet = mirror(m_strOrg, m_strMir);
	if (bRet == false) {
		g_log.writee(_T("�~���[�����O�Ɏ��s���܂����B"));
	}

	//g_log.writee(_T("�~���[�����O���v����: %d �b"), tc.elapseSec());

	return bRet;
}

//---------------------------------------------------------------------
// �o�b�N�A�b�v�E�~���[�����O�����̎��s
//---------------------------------------------------------------------
int BackupMirror::doMain() {
	switch (m_mode) {
	case BackupMirror::modeBackupOnly:	// �o�b�N�A�b�v����
		if (StartBackup() == false) return -1;
		break;

	case BackupMirror::modeMirrorOnly:	// �~���[�����O����
		if (StartMirror() == false) return -1;
		break;

	case BackupMirror::modeBackupMirror:	// �o�b�N�A�b�v���~���[�����O
		if (StartBackup() == false) return -1;
		if (StartMirror() == false) return -1;
		break;

	case BackupMirror::modeMirrorBackup:	// �~���[�����O���o�b�N�A�b�v
		if (StartMirror() == false) return -1;
		if (StartBackup() == false) return -1;
		break;
	}

	return 0;
}


//---------------------------------------------------------------------
// ���O���X�g�������ꂽ�t�@�C����ǂݍ���
//
// [���l]
// ���O���X�g�ɂ́A[��������]���āA�L�^�����B
//---------------------------------------------------------------------
void load_exclude_list(const _tstring& strFname, std::vector<_tstring>& excludeList) {
	_tout << _T("���O��`�Ǎ���: '" << strFname << "'.\n");

	_tstring dst = kjm::environment::expandEnvironmentVariables(strFname);

	_tifstream ifs(dst.c_str());
	if (!ifs.is_open()) {
		return;
	}

	_tstring line;
	ifs.imbue(std::locale("japanese"));
	while (!std::getline(ifs, line).fail()) {
		if (line.empty() == false) {
			_tout << _T("���O [") << line << _T("]\n");
			excludeList.push_back(kjm::util::toLower(line));
		}
	}

	ifs.close();
}

//---------------------------------------------------------------------
// �o�[�W��������\��
//---------------------------------------------------------------------
void version() {
	_tout
		<< _T("�o�[�W���� ")
		<< kjm::util::getFileVersion(kjm::util::getModuleFileName() )
		<< _T(" [build ") << kjm::util::getMscVerText() << _T("] ")
		<< _T("\n");
	_tout << _T(" ���p���O�V�X�e��: [") << g_log.get_MyName() << _T("]\n");
}

//---------------------------------------------------------------------
// �g�p�@��\��
//---------------------------------------------------------------------
void usage() {
	_tout << kjm::util::getModuleFileName() << _T(" ");
	version();
	_tout
		<< _T("Usage: mirror [OPTIONS] <�I���W�i��> <����>\n")
		<< _T("  --mode=<mode> backup-only (default)\n")
		<< _T("                mirror-only\n")
		<< _T("                backup-mirror\n")
		<< _T("                mirror-backup\n")
		<< _T("  --test        �e�X�g���[�h\n")
		<< _T("  --exclude=<���O�t�@�C���ꗗ>\n")
		<< _T("                ���O�t�@�C���̈ꗗ\n")
		<< _T("  --log-prefix=<���O�t�@�C���̖��O>\n")
		<< _T("                ���O�t�@�C���̐擪�ɕt���閼�O\n")
		<< _T("  --logpath=<���O�t�@�C���̍쐬�ꏊ>\n")
		<< _T("                ���O�t�@�C���̍쐬�ꏊ\n")
		<< _T("  --archive     �A�[�J�C�u�����̃t�@�C��������Ώۂɂ���\n")
		<< _T("  --time-gap    NTFS��FAT�̎������l��(2�b�ȓ��́A����Ƃ݂Ȃ�\n")
		<< _T("  --no-empty-folder ��t�H���_���쐬���Ȃ�\n")
		<< _T("  --norecurse   �ċA�������s��Ȃ�\n")
		<< _T("  --reverse     �t�H���_�𖼑O�~���ŏ�������B\n")
		<< _T("  --strict      ���L�ᔽ�n�G���[���G���[�I�������Ƃ���B\n")
		<< _T("                �t�H���_�������t�̏ꍇ�A�V�������t���D��I�ɏ��������B\n")
		<< _T("  --verbose     ���O�o�𑽂͂߂ɂ���\n")
		<< _T("  --ignore-date ���t������Ă��Ă��R�s�[/�폜�̑ΏۂƂ��Ȃ�\n")
		<< _T("  --ignore-size �T�C�Y������Ă��Ă��R�s�[/�폜�̑ΏۂƂ��Ȃ�\n")
		<< _T("  -?, --help    ���̃w���v��\��\n")
		<< _T("  -v, --version �o�[�W��������\��\n")
		<< _T("  --warning-sec=<�x���b��>\n")
		<< _T("                �񋓂ɂ����鎞�Ԃ��������ꍇ�x�����o��(default:") << WARNING_SEC << _T(" �b)\n")
		;
}

//---------------------------------------------------------------------
// �v���O�����X�^�[�g
//---------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	bool logStopFlag = true;
	g_log.set_log_path(kjm::path::append(kjm::util::getTempPath(), _T("kjmlog")));
	g_log.set_maxLogSize(2*1024*1024);
	g_log.set_RotateCount(99);

	kjm::tickCounter totalTick;
	int prevSize = 0;

	_tsetlocale(LC_ALL, _T("japanese"));
	std::locale loc("japanese");
	_tout.imbue(loc);

	g_log.set_LogNameType(0);	// ���O�t�@�C������ 'YYYYMMDD.log' �ɕύX�B

	kjm::commandLine cl;

	cl.parse(argc, argv, _T("?v"));

	std::vector<_tstring> opts = cl.getOptList();

	BackupMirror bakMir;
	
	std::vector<_tstring> excludeList;

	for (std::vector<_tstring>::size_type i = 0; i < opts.size(); i++) {
		if (opts[i] == _T("mode")) {
			_tstring optArg = cl.getOptArg(opts[i]);
			if (lstrcmpi(&optArg[0], _T("backup-only")) == 0) {
				bakMir.set_Mode(BackupMirror::modeBackupOnly);
			} else if (lstrcmpi(&optArg[0], _T("mirror-only")) == 0) {
				bakMir.set_Mode(BackupMirror::modeMirrorOnly);
			} else if (lstrcmpi(&optArg[0], _T("backup-mirror")) == 0) {
				bakMir.set_Mode(BackupMirror::modeBackupMirror);
			} else if (lstrcmpi(&optArg[0], _T("mirror-backup")) == 0) {
				bakMir.set_Mode(BackupMirror::modeMirrorBackup);
			} else {
				usage();
				return 0;
			}
		} else if (opts[i] == _T("exclude")) {
			// ���O���X�g�������ꂽ�t�@�C����ǂݍ���
			load_exclude_list(cl.getOptArg(opts[i]), excludeList);
		} else if (opts[i] == _T("log-prefix")) {
			g_log.set_prefix(cl.getOptArg(opts[i]));
		} else if (opts[i] == _T("logpath")) {
			g_log.set_log_path(cl.getOptArg(opts[i]));
			logStopFlag = false;
		} else if (opts[i] == _T("test")) {
			bakMir.set_TestMode(true);
		} else if (opts[i] == _T("archive")) {
			bakMir.set_ArchiveOnlyMode(true);
		} else if (opts[i] == _T("time-gup")) {
			bakMir.set_TimeGup(true);
		} else if (opts[i] == _T("no-empty-folder")) {
			bakMir.set_NoEmptyFolder(true);
		} else if (opts[i] == _T("verbose")) {
			bakMir.set_Verbose(true);
		} else if (opts[i] == _T("reverse")) {
			bakMir.set_Reverse(true);
		} else if (opts[i] == _T("ignore-date")) {
			bakMir.set_IgnoreDate(true);
		} else if (opts[i] == _T("ignore-size")) {
			bakMir.set_IgnoreSize(true);
		} else if (opts[i] == _T("norecurse")) {
			bakMir.set_NoRecurse(true);
		} else if (opts[i] == _T("strict")) {
			bakMir.set_Strict(true);
		} else if (opts[i] == _T("help") || opts[i] == _T("?")) {
			usage();
			return 0;
		} else if (opts[i] == _T("version") || opts[i] == _T("v")) {
			version();
			return 0;
		} else if (opts[i] == _T("warning-sec")) {
			_tstring optArg = cl.getOptArg(opts[i]);
			if (optArg.empty()) {
				bakMir.set_WarningSec(WARNING_SEC);
			} else {
				bakMir.set_WarningSec(_ttoi(optArg.c_str()));
			}
		}
	}

	if (cl.getArgListCount() != 2) {
		usage();
		return -1;
	}

	g_log.set_Stop(logStopFlag);
	g_log.write(_T("----- %s start -----"), kjm::util::getModuleFileName().c_str());
	g_log.writeMyName();

	g_log.deleteOldLog();

	switch (bakMir.get_Mode()) {
	case BackupMirror::modeBackupOnly:	g_log.writee(_T("����: �o�b�N�A�b�v�̂�"));	break;
	case BackupMirror::modeMirrorOnly:	g_log.writee(_T("����: �~���[�����O�̂�"));	break;
	case BackupMirror::modeBackupMirror:	g_log.writee(_T("����: �o�b�N�A�b�v���Ă���A�~���[�����O"));	break;
	case BackupMirror::modeMirrorBackup:	g_log.writee(_T("����: �~���[�����O���Ă���A�o�b�N�A�b�v"));	break;
	default:	g_log.writee(_T("����: ���[�h�ُ�"));	return -1;
	}

	bakMir.set_ExcludeList(excludeList);

	for (std::vector<_tstring>::size_type j = 0; j < excludeList.size(); j++) {
		_tout << _T("���O: ") << excludeList[j] << _T("\n");
		g_log.write(_T("���O: %s"), excludeList[j].c_str());
	}

	bakMir.set_OriginalPath(cl.getArgList()[0]);
	bakMir.set_MirrorPath(cl.getArgList()[1]);

	g_log.writee(_T("�ċA: %s"), (bakMir.get_NoRecurse() ? _T("���Ȃ�") : _T("����")));
	g_log.writee(_T("����: %s"), (bakMir.get_Strict() ? _T("����") : _T("���Ȃ�")));
	g_log.writee(_T("���{: %s"), bakMir.get_OriginalPath().c_str());
	g_log.writee(_T("����: %s"), bakMir.get_MirrorPath().c_str());
	//g_log.writee(_T("�x��: %d �b"), bakMir.get_WarningSec());

	// �V�����I�v�V���������O�ɏo��
	if (bakMir.get_Verbose()) {
		if (bakMir.get_IgnoreDate()) {
			g_log.writee(_T("���t�̈Ⴂ�𖳎�����(--ignore-date ON)�B"));
		}
		if (bakMir.get_IgnoreSize()) {
			g_log.writee(_T("�T�C�Y�̈Ⴂ�𖳎�����(--ignore-size ON)�B"));
		}
	}

	int result = 0;

	// ���{�̃p�X�����݂��Ȃ��Ƃ��́A�~���[���S�������Ă��܂��̂ŁA
	// �G���[�����ŏI������B
	if (kjm::path::fileExists(bakMir.get_OriginalPath()) == false) {
		g_log.writee(_T("\"%s\" ���A���݂��܂���B���������ɁA�I�����܂��B"), bakMir.get_OriginalPath().c_str());
		result = 1;
		goto theEnd;
	}

	result = (bakMir.doMain() != 0) ? 1 : 0;	// �o�b�N�A�b�v�E�~���[�����O�����̎��s

theEnd:
	g_log.writee(_T("�S�̏��v����: %d �b <result code : %d>"), totalTick.elapseSec(), result);
	g_log.write(_T("----- %s end -----"), kjm::util::getModuleFileName().c_str());

	_tout << _T("���O�t�@�C���� '") << g_log.get_log_path() << _T("' �ɍ쐬����܂����B\n");

	return result;
}
