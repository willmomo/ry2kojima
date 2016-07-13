#include "rklib.h"



//=============================================================================
// ディレクトリやサブディレクトリを通じて、作成、移動、および列挙するための
// 静的メソッドを公開します。
//=============================================================================
//-----------------------------------------------------------------------------
// 既存のフォルダを新しいフォルダにコピーします。
//-----------------------------------------------------------------------------
void rk::Directory::Copy(const rk::string& sourceDirName, const rk::string& destDirName, bool overwrite/*=false*/) {

	rk::Directory::CreateFolder(destDirName);

	strings files = rk::Directory::GetFiles(sourceDirName);

	for (strings::size_type i = 0; i < files.size(); i++) {
		rk::File::Copy(files[i], rk::Path::Combine(destDirName, rk::Path::GetFileName(files[i])), overwrite);
	}

	strings dirs = rk::Directory::GetDirectories(sourceDirName);

	for (strings::size_type i = 0; i < dirs.size(); i++) {
		rk::Directory::Copy(dirs[i], rk::Path::Combine(destDirName, rk::Path::GetFileName(dirs[i])), overwrite);
	}
}

//-----------------------------------------------------------------------------
// path で指定したすべてのディレクトリとサブディレクトリを作成します。
// 関数名が CreateDirectory だと API とかぶるので、CreateFolder を選択。
//-----------------------------------------------------------------------------
rk::DirectoryInfo rk::Directory::CreateFolder(const rk::string& path) {
	if (rk::Directory::Exists(path) == false) {
		rk::Directory::CreateFolder(rk::Path::GetDirectoryName(path));

		::SetLastError(0);
		BOOL bRet = ::CreateDirectory(path.c_str(), NULL);
		if (bRet == FALSE) {
			DWORD dwErr = ::GetLastError();
			switch (dwErr) {
			case ERROR_ACCESS_DENIED:
				throw rk::UnauthorizedAccessException(dwErr, rk::SystemException::mkmsg(dwErr, path));

			default:
				throw rk::SystemException(dwErr, rk::SystemException::mkmsg(dwErr, path));
			}
		}
	}

	return rk::DirectoryInfo(path);
}

//-----------------------------------------------------------------------------
// 指定したパスがディスク上の既存のディレクトリを参照しているかどうかを確認します。
//-----------------------------------------------------------------------------
bool rk::Directory::Exists(const rk::string& path) {
	return (::PathFileExists(path.c_str()) != FALSE) ? true : false;
}

//-----------------------------------------------------------------------------
// 指定したディレクトリ内のサブディレクトリの名前を取得します。
//-----------------------------------------------------------------------------
rk::strings rk::Directory::GetDirectories(const rk::string& path) {
	rk::strings result;

	WIN32_FIND_DATA data;
	HANDLE hFind = ::FindFirstFile(rk::Path::Combine(path, _T("*.*")).c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if ((::lstrcmp(data.cFileName, _T(".")) != 0) && (::lstrcmp(data.cFileName, _T("..")) != 0)) {
					result.push_back(rk::Path::Combine(path, data.cFileName));
				}
			}
		} while (::FindNextFile(hFind, &data));

		::FindClose(hFind);
	}

	return result;
}

//-----------------------------------------------------------------------------
// 指定したディレクトリ内のファイル名を返します。
//-----------------------------------------------------------------------------
rk::strings rk::Directory::GetFiles(const rk::string& path) {
	rk::strings result;

	WIN32_FIND_DATA data;
	HANDLE hFind = ::FindFirstFile(rk::Path::Combine(path, _T("*.*")).c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
				result.push_back(rk::Path::Combine(path, data.cFileName));
			}
		} while (::FindNextFile(hFind, &data));

		::FindClose(hFind);
	}

	return result;
}


//=============================================================================
// ファイルの作成、コピー、削除、移動、オープンのための静的メソッドを提供し
// ます。
//=============================================================================
//-----------------------------------------------------------------------------
// 既存のファイルを新しいファイルにコピーします。
//-----------------------------------------------------------------------------
void rk::File::Copy(const rk::string& sourceFileName, const rk::string& destFileName, bool overwrite/*=false*/) {

	::SetLastError(0);
	BOOL bRet = ::CopyFile(sourceFileName.c_str(), destFileName.c_str(), (overwrite) ? FALSE : TRUE);
	DWORD dwErr = ::GetLastError();

	_tout << sourceFileName << _T(" → ") << destFileName << _T(" : ") << bRet << _T(" : ") << dwErr << _T("\n");

	if (bRet == FALSE) {
		switch (dwErr) {
		case ERROR_ACCESS_DENIED:
			throw rk::UnauthorizedAccessException(dwErr, rk::SystemException::mkmsg(dwErr, destFileName));

		case ERROR_SHARING_VIOLATION:
			throw rk::IOException(dwErr, rk::SystemException::mkmsg(dwErr, sourceFileName));

		case ERROR_FILE_EXISTS:
			throw rk::IOException(dwErr, rk::SystemException::mkmsg(dwErr, destFileName));

		default:
			throw rk::SystemException(dwErr, rk::SystemException::mkmsg(dwErr));
		}
	}
}


//=============================================================================
// ファイルまたはディレクトリのパス情報を格納する string インスタンスで操作
// を実行します。これらの操作は、プラットフォーム間で実行されます。
//=============================================================================
//-----------------------------------------------------------------------------
// 2 つのパス文字列を結合します。
//-----------------------------------------------------------------------------
rk::string rk::Path::Combine(const rk::string& path1, const rk::string& path2) {
	TCHAR work[MAX_PATH];

	::PathAppend(::lstrcpy(work, path1.c_str()), path2.c_str());

	return work;
}

//-----------------------------------------------------------------------------
// 指定したパス文字列のディレクトリ情報を返します。
//-----------------------------------------------------------------------------
rk::string rk::Path::GetDirectoryName(const rk::string& path) {
	TCHAR work[MAX_PATH];

	::PathRemoveFileSpec(::lstrcpy(work, path.c_str()));

	return work;
}

//-----------------------------------------------------------------------------
// 指定したパス文字列のファイル名と拡張子を返します。
//-----------------------------------------------------------------------------
rk::string rk::Path::GetFileName(const rk::string& path) {
	return ::PathFindFileName(path.c_str());
}


//=============================================================================
// 例外の基本クラスを定義します。
//=============================================================================
//-----------------------------------------------------------------------------
// エラーコードとパラメータから一般的なエラーメッセージを作成する。
//-----------------------------------------------------------------------------
rk::string rk::SystemException::mkmsg(DWORD dwErr, const rk::string& p1/*=_T("")*/) {
	rk::_tostringstream oss;

	switch (dwErr) {
	case ERROR_ACCESS_DENIED:
		oss << _T("パス '") << p1 << _T("' へのアクセスが拒否されました。");
		break;

	case ERROR_SHARING_VIOLATION:
		oss << _T("別のプロセスで使用されているため、プロセスはファイル '") << p1 << _T("' にアクセスできません。");
		break;

	case ERROR_FILE_EXISTS:
		oss << _T("ファイル '") << p1 << _T("' は既に存在します。");
		break;

	default:
		oss << _T("");
		break;
	}

	return oss.str();
}


int _tmain(int argc, TCHAR** argv) {
	std::locale::global(std::locale("japanese"));

	try {
		rk::Directory::Copy(argv[1], argv[2]);
	} catch (rk::IOException& e) {
		_tout << _T("Code: ") << e.get_Code() << _T("\n");
		_tout << _T("Msg : ") << e.get_Message() << _T("\n");
	} catch (rk::UnauthorizedAccessException& e) {
		_tout << _T("Code: ") << e.get_Code() << _T("\n");
		_tout << _T("Msg : ") << e.get_Message() << _T("\n");
	}

	return 0;
}
