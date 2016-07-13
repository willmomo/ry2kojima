#pragma once

#include "kjm_base.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

//=========================================================================
// パス名に関する処理
//=========================================================================
namespace kjm {
	class path {
	public:
		static kjm::_tstring addBackslash(const kjm::_tstring& path) {
			TCHAR temp[MAX_PATH];
			::PathAddBackslash(lstrcpy(temp, path.c_str()));
			return temp;
		}

		// 指定したパス文字列の拡張子(ピリオドを含む)を返す。
		static kjm::_tstring getExtension(const kjm::_tstring& path);

		// 2つの文字列を1つのパスに結合します。
		static kjm::_tstring combine(const kjm::_tstring& path1, const kjm::_tstring& path2);

		// 2つの文字列を1つのパスに結合します。
		static kjm::_tstring append(const kjm::_tstring& path1, const kjm::_tstring& path2) {
			return kjm::path::combine(path1, path2);
		}

		// フルパスからファイル名を取り除く
		static kjm::_tstring removeFileSpec(const kjm::_tstring& strPath) {
			TCHAR szTemp[MAX_PATH];
			PathRemoveFileSpec(lstrcpy(szTemp, strPath.c_str()));
			return szTemp;
		}

		// フルパスからファイル名を変更する
		static kjm::_tstring renameFileSpec(const kjm::_tstring& strPath, const kjm::_tstring& strFileSpec) {
			return kjm::path::append(kjm::path::removeFileSpec(strPath), strFileSpec);
		}

		// ファイルが存在するかどうか調べる
		static bool fileExists(const kjm::_tstring& strPath) {
			return (PathFileExists(strPath.c_str()) != FALSE);
		}

		// ファイル名部分を取得
		static kjm::_tstring getFileName(const kjm::_tstring& path);
		
		// フルパスから、ファイル名を取り出す
		static kjm::_tstring findFileName(const kjm::_tstring& strPath) {
			return PathFindFileName(strPath.c_str());
		}

		// ディレクトリかどうか調べる
		static bool isDirectory(const _tstring& strPath) {
			return (PathIsDirectory(strPath.c_str()) != FALSE);
		}

		// 相対パスを作成する
		static _tstring relativePathTo(const _tstring& from, DWORD attrFrom, const _tstring& to, DWORD attrTo);

		// PathRemoveExtension 関数のラッパー
		static _tstring removeExtension(const _tstring& strPath);

		// 指定したパス文字列のディレクトリ情報を返します。
		//
		// [戻り値]
		// path がルート ディレクトリを示す場合、空文字列を返す。
		static _tstring getDirectoryName(const _tstring& path) {
			TCHAR work[MAX_PATH];
			::PathRemoveFileSpec(::lstrcpy(work, path.c_str()));

			// 変換後の文字列に変化がないときは、ルートディレクトリと判断して、空文字列を返す。
			if (::lstrcmpi(path.c_str(), work) == 0) return kjm::_tstring();

			return work;
		}

		// パスのルート情報以外の部分をすべて取り除く
		static _tsting stripToRoot(const _tstring& path);
		
	private:
		path() {}
		~path() {}
	};
};
