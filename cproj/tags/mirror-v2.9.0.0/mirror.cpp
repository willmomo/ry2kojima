//---------------------------------------------------------------------
// ミラーリングを行うアプリケーション
//
// 2015/05/01 ログの保存量を変更(1ファイル1M->2M,9->99世代)
//---------------------------------------------------------------------

#pragma warning(disable : 4786)

#include "mirror.h"

//---------------------------------------------------------------------
// グローバル変数
//---------------------------------------------------------------------
kjm::log g_log;


//---------------------------------------------------------------------
// コピー進捗表示
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
	static DWORD s_prevPrint;	// 前回表示時間
	static DWORD s_t1;

	if (dwCallbackReason == CALLBACK_STREAM_SWITCH) {
		s_t1 = GetTickCount();
	}

	// (前回表示更新から、一秒以上経過) or (ファイルの新しい出力開始) or (ファイルの書き込み終了)で、表示更新
	if (((GetTickCount() - s_prevPrint) > 1000) || (dwCallbackReason == CALLBACK_STREAM_SWITCH) || (StreamSize.QuadPart == StreamBytesTransferred.QuadPart)) {
		DWORD t = (s_prevPrint = GetTickCount()) - s_t1;

		_tprintf(_T("%3I64d%% %I64d/%I64d "),
			(TotalFileSize.QuadPart == 0) ? 100 : (TotalBytesTransferred.QuadPart * 100) / TotalFileSize.QuadPart,
			TotalBytesTransferred.QuadPart,
			TotalFileSize.QuadPart);

		// コピー時間が一秒を超えたら転送速度を表示
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
// デフォルトコンストラクタ
//---------------------------------------------------------------------------
BackupMirror::BackupMirror() {
	// メンバーの初期化
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
// ファイルのコピー
//
// @arg orgBase  相対パスを表示するときの基準になるフォルダ。
//---------------------------------------------------------------------
bool BackupMirror::copy_file(const kjm::fileInfo& fiOrg, const _tstring& strMir, const _tstring& orgBase) {
	kjm::tickCounter tc;
	_tstring strMode;

	// アーカイブのみモードの時は、コピー元の属性をチェックする
	if (this->get_ArchiveOnlyMode() && !fiOrg.isArchive()) {
		return true;
	}

	// コピー先ファイルの有無を確認する
	kjm::fileInfo mirFile(strMir);
	if (mirFile.exists()) {

#if 0	// デバッグ用の情報出力
		g_log.writee(_T("<%s>"), fiOrg.fullName().c_str());
		g_log.writee(_T("作成日時　　：%s %s"),
			fiOrg.creationTime().toString().c_str(), mirFile.creationTime().toString().c_str());
		g_log.writee(_T("アクセス日時：%s %s"),
			fiOrg.lastAccessTime().toString().c_str(), mirFile.lastAccessTime().toString().c_str());
		g_log.writee(_T("更新日時　　：%s %s"),
			fiOrg.lastWriteTime().toString().c_str(), mirFile.lastWriteTime().toString().c_str());
#endif
		// サイズの差を記録
		__int64 diffSize = fiOrg.length() - mirFile.length();

		// 時間の差を記録
		__int64 diff = (__int64)(fiOrg.lastWriteTime() - mirFile.lastWriteTime()).totalSeconds();

		if (this->get_IgnoreDate() == false && this->get_IgnoreSize() == false) {
			// 日付とサイズ、どちらが違っていてもファイルはコピーされる（通常）
#if _MSC_VER <= 1200	// VC6 以下
			if (diffSize == 0 && abs((int)diff) <= 2) return true;
#else
			if (diffSize == 0 && _abs64(diff) <= 2) return true;
#endif
		} else if (this->get_IgnoreDate() != false && this->get_IgnoreSize() == false) {
			// サイズだけ違いの判断基準とするモード
			if (diffSize == 0) return true;
		} else if (this->get_IgnoreDate() == false && this->get_IgnoreSize() != false) {
			// 日付だけ違いの判断基準とするモード
#if _MSC_VER <= 1200	// VC6 以下
			if (abs((int)diff) <= 2) return true;
#else
			if (_abs64(diff) <= 2) return true;
#endif
		} else {
			// 日付もサイズも違いの判断基準としない（存在しないファイルだけが処理の対象になる！）
			return true;
		}

		//if (diff != 0) g_log.writee(_T("時差: %I64d"), diff);

		strMode = _T("更新");
		g_log.writee(_T("%s [%s] %c (%s) %5s"),
			strMode.c_str(),
			fiOrg.lastWriteTime().toString().c_str(),
			(fiOrg.lastWriteTime() > mirFile.lastWriteTime() ? '>' : '<'),
			mirFile.lastWriteTime().toString().c_str(),
			kjm::util::toHumanReadable(fiOrg.length()).c_str());

		if (!get_TestMode()) {
			// コピー先に readonly/system/hidden がついているときは落とす。
			DWORD dwAttr = kjm::file::getAttributes(strMir);
			if (dwAttr != 0xffffffff) {
				/*** 属性変更のログは出さないようにする。
				if ((dwAttr & kjm::readOnly) != 0) {
					g_log.writee(_T("'%s' の Read-only を外します。"), strMir.c_str());
				}
				if ((dwAttr & kjm::system) != 0) {
					g_log.writee(_T("'%s' の System    を外します。"), strMir.c_str());
				}
				if ((dwAttr & kjm::hidden) != 0) {
					g_log.writee(_T("'%s' の Hidden    を外します。"), strMir.c_str());
				}
				***/

				if ((dwAttr & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY)) != 0) {
					kjm::file::setAttributes(strMir, (dwAttr & ~(FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY)));
				}
			} else {
				g_log.writee(_T("'%s' の属性を取得できませんでした。"), strMir.c_str());
			}
		}
	} else {
		// コピー先ファイルが無い時は、コピーして良い。
		strMode = _T("追加");
		g_log.writee(_T("%s                         [%s] %5s"),
			strMode.c_str(), fiOrg.lastWriteTime().toString().c_str(),
			kjm::util::toHumanReadable(fiOrg.length()).c_str());
	}

	g_log.writee(_T("元 - %c%c%c%c %s"),
		(fiOrg.isArchive()  ? 'A' : ' '),
		(fiOrg.isSystem()   ? 'S' : ' '),
		(fiOrg.isHidden()   ? 'H' : ' '),
		(fiOrg.isReadonly() ? 'R' : ' '),
		fiOrg.fullName().c_str());
	g_log.writee(_T("先 - %c%c%c%c %s"),
		'_', '_', '_', '_', strMir.c_str());

	if (!get_TestMode()) {
		DWORD t1 = GetTickCount();

		// バックアップ先フォルダは、常に作成する
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
					// 厳密モードのときは、排他系エラーもエラー扱いにします。
					return false;
				} else {
					_tout << _T("処理を継続します。\n");
					g_log.write(_T("処理を継続します。"));
				}
			}
		}
	}

	// アーカイブのみモードの時は、コピー元の属性を落とす
	if (this->get_ArchiveOnlyMode()) {
		DWORD dwAttr = kjm::file::getAttributes(fiOrg.fullName());
		if (dwAttr != 0xffffffff) {
			kjm::file::setAttributes(fiOrg.fullName(), (dwAttr & ~FILE_ATTRIBUTE_ARCHIVE));
		} else {
			g_log.writee(_T("'%s' の属性を取得できませんでした。"), fiOrg.fullName().c_str());
		}
	}

	tc.stop();
	if (tc.elapseMin() >= 1) {
		_tout << _T("(コピー速度 ") << ((double)(fiOrg.length() / tc.elapseSec()) / 1024.0 / 1024.0) << _T(" mbps)\n");
	}

	return true;
}

//-----------------------------------------------------------------------------
// ファイル名が、除外する名前かどうか調べる
//
// [備考]
// 除外リストは、[小文字化]して保存されているので、ファイル名も小文字化して比較
// する。
//-----------------------------------------------------------------------------
bool BackupMirror::is_exclude(const _tstring& strFname) {
	_tstring lowerFname = kjm::util::toLower(strFname);

	for (std::vector<_tstring>::size_type i = 0; i < m_excludeList.size(); i++) {
		if (lowerFname.find(m_excludeList[i]) != _tstring::npos) {
			if (this->get_Verbose()) {
				g_log.writee(_T("'%s' は、除外ファイルです。[%s] にマッチ"), strFname.c_str(), m_excludeList[i].c_str());
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
// 文字列配列(s1)に文字列(s2)が、あるかどうかを調べる
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
// バックアップ
//---------------------------------------------------------------------
bool BackupMirror::backup(const _tstring& strOrg, const _tstring& strMir) {
	static _tstring s_orgBase;
	bool bRet = true;

	//g_log.writee(_T("{DEBUG} BackupMirror::backup 1(%s, %s)"), strOrg.c_str(), strMir.c_str());

	if (s_orgBase.empty()) s_orgBase = strOrg;

	//g_log.writee(_T("{DEBUG} BackupMirror::backup 2(%s, %s)"), strOrg.c_str(), strMir.c_str());

	if (!get_TestMode()) {
		// バックアップ先フォルダは、常に作成する
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

	// --norecurseが指定されていないときは、サブフォルダの処理を行う。
	if (this->get_NoRecurse() == false) {
		std::vector<kjm::folderInfo> subFolders = kjm::folderInfo(strOrg).getSubFolders();	// サブフォルダを列挙

		// 見つけたサブフォルダから、標準除外フォルダを取り除く
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

	// 下位フォルダのコピーが正しく済んだ時だけ、直下のファイルをコピーする。
	if (bRet) {
		std::vector<kjm::fileInfo> files = kjm::folderInfo(strOrg).getFiles();

		// 見つけたファイルから、標準除外ファイルを取り除く
		std::vector<kjm::fileInfo>::iterator it = files.begin();
		while (it != files.end()) {
			if (isInclude(m_defaultExcludeFiles, it->name())) {
				it = files.erase(it);
			} else {
				++it;
			}
		}

		// フォルダ内のファイルをバックアップ
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
// ミラーリング
//---------------------------------------------------------------------
bool BackupMirror::mirror(const _tstring& strOrg, const _tstring& strMir) {
	bool bRet = true;
	kjm::folderInfo mirFolder(strMir);
	std::vector<kjm::fileInfo> files = mirFolder.getFiles();

	bool fFileExists;

	// --norecurseが指定されていないときは、サブフォルダの処理を行う。
	if (this->get_NoRecurse() == false) {
		std::vector<kjm::folderInfo> subFolders = mirFolder.getSubFolders();
		for (std::vector<kjm::folderInfo>::size_type j = 0; j < subFolders.size(); j++) {
			// 基本的に、オリジナル側にファイルがないとき、ミラー側を削除する。
			// ファイルがないという判断基準を
			// file_exists が false かつ
			// エラーコードが、 ERROR_FILE_NOT_FOUND or ERROR_PATH_NOT_FOUND
			// と、限定的にする。
			// オリジナルがわのネットワーク不良でも、file_existsは、falseになるための対策。
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

				g_log.writee(_T("削除フォルダ %s"), 
					//kjm::path::relativePathTo(strMir, FILE_ATTRIBUTE_DIRECTORY, subFolders[j].getFullPath(), FILE_ATTRIBUTE_DIRECTORY).c_str());
					subFolders[j].getFullPath().c_str());

				if (!get_TestMode()) {
					try {
						kjm::util::removeFolder(subFolders[j].getFullPath());
					} catch (kjm::exception e) {
						_tout << _T("\n");

						g_log.writee(_T("removeFolder error %d: %s"), e.m_errCode, e.m_errMsg.c_str());
						g_log.writee(_T("コピー元 - %s"), strOrg.c_str());
						g_log.writee(_T("コピー先 - %s"), strMir.c_str());

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

	// フォルダ内のファイルをミラーリング
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

			//_tout << _T("削除するファイル - ") << kjm::path::relativePathTo(strMir, FILE_ATTRIBUTE_DIRECTORY, files[i].fullName(), 0);
			//g_log.write(_T("削除 %s"), kjm::path::relativePathTo(strMir, FILE_ATTRIBUTE_DIRECTORY, files[i].fullName(), 0).c_str());

			_tout << _T("削除するファイル - ") << files[i].fullName();
			g_log.write(_T("削除 %s"), files[i].fullName().c_str());

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
// バックアップ開始
//---------------------------------------------------------------------
bool BackupMirror::StartBackup() {
	bool bRet = false;
	kjm::tickCounter tc;

	tc.start();
	g_log.writee(_T("バックアップ開始"));

	//g_log.writee(_T("{DEBUG} BackupMirror::mirror(%s, %s)"), m_strOrg.c_str(), m_strMir.c_str());
	bRet = backup(m_strOrg, m_strMir);
	if (bRet == false) {
		g_log.writee(_T("バックアップに失敗しました。"));
	}

	//g_log.writee(_T("バックアップ所要時間: %d 秒"), tc.elapseSec());
	
	return bRet;
}

//---------------------------------------------------------------------
// ミラーリング開始
//---------------------------------------------------------------------
bool BackupMirror::StartMirror() {
	bool bRet = false;
	kjm::tickCounter tc;

	tc.start();
	g_log.writee(_T("ミラーリング開始"));

	bRet = mirror(m_strOrg, m_strMir);
	if (bRet == false) {
		g_log.writee(_T("ミラーリングに失敗しました。"));
	}

	//g_log.writee(_T("ミラーリング所要時間: %d 秒"), tc.elapseSec());

	return bRet;
}

//---------------------------------------------------------------------
// バックアップ・ミラーリング処理の実行
//---------------------------------------------------------------------
int BackupMirror::doMain() {
	switch (m_mode) {
	case BackupMirror::modeBackupOnly:	// バックアップだけ
		if (StartBackup() == false) return -1;
		break;

	case BackupMirror::modeMirrorOnly:	// ミラーリングだけ
		if (StartMirror() == false) return -1;
		break;

	case BackupMirror::modeBackupMirror:	// バックアップ→ミラーリング
		if (StartBackup() == false) return -1;
		if (StartMirror() == false) return -1;
		break;

	case BackupMirror::modeMirrorBackup:	// ミラーリング→バックアップ
		if (StartMirror() == false) return -1;
		if (StartBackup() == false) return -1;
		break;
	}

	return 0;
}


//---------------------------------------------------------------------
// 除外リストが書かれたファイルを読み込む
//
// [備考]
// 除外リストには、[小文字化]して、記録される。
//---------------------------------------------------------------------
void load_exclude_list(const _tstring& strFname, std::vector<_tstring>& excludeList) {
	_tout << _T("除外定義読込み: '" << strFname << "'.\n");

	_tstring dst = kjm::environment::expandEnvironmentVariables(strFname);

	_tifstream ifs(dst.c_str());
	if (!ifs.is_open()) {
		return;
	}

	_tstring line;
	ifs.imbue(std::locale("japanese"));
	while (!std::getline(ifs, line).fail()) {
		if (line.empty() == false) {
			_tout << _T("除外 [") << line << _T("]\n");
			excludeList.push_back(kjm::util::toLower(line));
		}
	}

	ifs.close();
}

//---------------------------------------------------------------------
// バージョン情報を表示
//---------------------------------------------------------------------
void version() {
	_tout
		<< _T("バージョン ")
		<< kjm::util::getFileVersion(kjm::util::getModuleFileName() )
		<< _T(" [build ") << kjm::util::getMscVerText() << _T("] ")
		<< _T("\n");
	_tout << _T(" 利用ログシステム: [") << g_log.get_MyName() << _T("]\n");
}

//---------------------------------------------------------------------
// 使用法を表示
//---------------------------------------------------------------------
void usage() {
	_tout << kjm::util::getModuleFileName() << _T(" ");
	version();
	_tout
		<< _T("Usage: mirror [OPTIONS] <オリジナル> <複製>\n")
		<< _T("  --mode=<mode> backup-only (default)\n")
		<< _T("                mirror-only\n")
		<< _T("                backup-mirror\n")
		<< _T("                mirror-backup\n")
		<< _T("  --test        テストモード\n")
		<< _T("  --exclude=<除外ファイル一覧>\n")
		<< _T("                除外ファイルの一覧\n")
		<< _T("  --log-prefix=<ログファイルの名前>\n")
		<< _T("                ログファイルの先頭に付ける名前\n")
		<< _T("  --logpath=<ログファイルの作成場所>\n")
		<< _T("                ログファイルの作成場所\n")
		<< _T("  --archive     アーカイブ属性のファイルだけを対象にする\n")
		<< _T("  --time-gap    NTFSとFATの時差を考慮(2秒以内は、同一とみなす\n")
		<< _T("  --no-empty-folder 空フォルダを作成しない\n")
		<< _T("  --norecurse   再帰処理を行わない\n")
		<< _T("  --reverse     フォルダを名前降順で処理する。\n")
		<< _T("  --strict      共有違反系エラーをエラー終了扱いとする。\n")
		<< _T("                フォルダ名が日付の場合、新しい日付が優先的に処理される。\n")
		<< _T("  --verbose     ログ出力を多めにする\n")
		<< _T("  --ignore-date 日付が違っていてもコピー/削除の対象としない\n")
		<< _T("  --ignore-size サイズが違っていてもコピー/削除の対象としない\n")
		<< _T("  -?, --help    このヘルプを表示\n")
		<< _T("  -v, --version バージョン情報を表示\n")
		<< _T("  --warning-sec=<警告秒数>\n")
		<< _T("                列挙にかかる時間が超えた場合警告を出す(default:") << WARNING_SEC << _T(" 秒)\n")
		;
}

//---------------------------------------------------------------------
// プログラムスタート
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

	g_log.set_LogNameType(0);	// ログファイル名を 'YYYYMMDD.log' に変更。

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
			// 除外リストが書かれたファイルを読み込む
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
	case BackupMirror::modeBackupOnly:	g_log.writee(_T("動作: バックアップのみ"));	break;
	case BackupMirror::modeMirrorOnly:	g_log.writee(_T("動作: ミラーリングのみ"));	break;
	case BackupMirror::modeBackupMirror:	g_log.writee(_T("動作: バックアップしてから、ミラーリング"));	break;
	case BackupMirror::modeMirrorBackup:	g_log.writee(_T("動作: ミラーリングしてから、バックアップ"));	break;
	default:	g_log.writee(_T("動作: モード異常"));	return -1;
	}

	bakMir.set_ExcludeList(excludeList);

	for (std::vector<_tstring>::size_type j = 0; j < excludeList.size(); j++) {
		_tout << _T("除外: ") << excludeList[j] << _T("\n");
		g_log.write(_T("除外: %s"), excludeList[j].c_str());
	}

	bakMir.set_OriginalPath(cl.getArgList()[0]);
	bakMir.set_MirrorPath(cl.getArgList()[1]);

	g_log.writee(_T("再帰: %s"), (bakMir.get_NoRecurse() ? _T("しない") : _T("する")));
	g_log.writee(_T("厳密: %s"), (bakMir.get_Strict() ? _T("する") : _T("しない")));
	g_log.writee(_T("原本: %s"), bakMir.get_OriginalPath().c_str());
	g_log.writee(_T("複製: %s"), bakMir.get_MirrorPath().c_str());
	//g_log.writee(_T("警告: %d 秒"), bakMir.get_WarningSec());

	// 新しいオプションをログに出す
	if (bakMir.get_Verbose()) {
		if (bakMir.get_IgnoreDate()) {
			g_log.writee(_T("日付の違いを無視する(--ignore-date ON)。"));
		}
		if (bakMir.get_IgnoreSize()) {
			g_log.writee(_T("サイズの違いを無視する(--ignore-size ON)。"));
		}
	}

	int result = 0;

	// 原本のパスが存在しないときは、ミラーが全部消えてしまうので、
	// エラー扱いで終了する。
	if (kjm::path::fileExists(bakMir.get_OriginalPath()) == false) {
		g_log.writee(_T("\"%s\" が、存在しません。何もせずに、終了します。"), bakMir.get_OriginalPath().c_str());
		result = 1;
		goto theEnd;
	}

	result = (bakMir.doMain() != 0) ? 1 : 0;	// バックアップ・ミラーリング処理の実行

theEnd:
	g_log.writee(_T("全体所要時間: %d 秒 <result code : %d>"), totalTick.elapseSec(), result);
	g_log.write(_T("----- %s end -----"), kjm::util::getModuleFileName().c_str());

	_tout << _T("ログファイルは '") << g_log.get_log_path() << _T("' に作成されました。\n");

	return result;
}
