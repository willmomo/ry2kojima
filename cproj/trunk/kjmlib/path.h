//---------------------------------------------------------------------
// kjmpath : Version 0.2015.12.8
//
// プリコンパイル済みヘッダを使用しないように設定してください。
//
// r.kojima
//---------------------------------------------------------------------

#pragma once

#pragma warning(disable : 4786)
#pragma warning(disable : 4996)

#include "kjmbase.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace kjm {
	//
	// パス名に関する処理
	//
	class path {
	public:

		// ファイルが存在するかどうか調べる
		static bool fileExists(const kjm::_tstring& strPath);

		// \\?\D:\very long path, \\?\UNC\server\share\very long path に変換する
		static kjm::_tstring makeVeryLongPath(const kjm::_tstring& path);

		// \\?\ または \\?\UNC を取り除く
		static kjm::_tstring removeVeryLongPath(const kjm::_tstring& path);

		// 末尾に \ を追加
		static kjm::_tstring addBackslash(const kjm::_tstring& path);

		// 指定したパス文字列の拡張子(ピリオドを含む)を返す。
		static kjm::_tstring getExtension(const kjm::_tstring& path);

		// 2つの文字列を1つのパスに結合します。
		static kjm::_tstring combine(const kjm::_tstring& path1, const kjm::_tstring& path2);

		// 2つの文字列を1つのパスに結合します。
		static kjm::_tstring append(const kjm::_tstring& path1, const kjm::_tstring& path2) {
			return kjm::path::combine(path1, path2);
		}

		// フルパスからファイル名を取り除く
		static kjm::_tstring removeFileSpec(const kjm::_tstring& strPath);

		// フルパスからファイル名を変更する
		static kjm::_tstring renameFileSpec(const kjm::_tstring& strPath, const kjm::_tstring& strFileSpec) {
			return kjm::path::append(kjm::path::removeFileSpec(strPath), strFileSpec);
		}

		// ファイル名部分を取得
		static kjm::_tstring getFileName(const kjm::_tstring& path);
		
		// フルパスから、ファイル名を取り出す
		static kjm::_tstring findFileName(const kjm::_tstring& strPath) {
			return PathFindFileName(strPath.c_str());
		}

		// ディレクトリかどうか調べる
		static bool isDirectory(const kjm::_tstring& strPath) {
			return (PathIsDirectory(strPath.c_str()) != FALSE);
		}

		// 相対パスを作成する
		static kjm::_tstring relativePathTo(const kjm::_tstring& from, DWORD attrFrom, const kjm::_tstring& to, DWORD attrTo);

		// PathRemoveExtension 関数のラッパー
		static kjm::_tstring removeExtension(const kjm::_tstring& strPath);

		// 指定したパス文字列のディレクトリ情報を返します。
		//
		// [戻り値]
		// path がルート ディレクトリを示す場合、空文字列を返す。
		static kjm::_tstring getDirectoryName(const kjm::_tstring& path);

	private:
		path() {}
		~path() {}
	};
};
