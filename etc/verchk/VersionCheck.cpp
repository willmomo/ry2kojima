/*
	VersionCheck

	グローバル変数「VERSION」に、自分のバージョンを設定します
	引数で指定されたiniファイルの、指定セクション、指定キーを読み出し
	自分のバージョンと比較します

	[usage]
		VersionCheck IniFileName Section Key

			IniFileName:	フルパス指定
			Section:		セクション名
			Key:			キー名

		return code
			-1:	FatalError
					引数が足らない
					指定iniファイルが開けない
					指定セクションが見つからない
					指定キーが見つからない
			0:	Unknown
					バージョンエントリータイプが古い
			1:	Identical
					同一バージョン
			2:	OlderVersion
					自分より相手が、古いバージョン
			3:	NewerVersion
					自分より相手が、新しいバージョン
			4:	Identical version, But OlderPatch
					自分より相手が、同一バージョンだが、パッチが古い
			5:	Identical version, But NewerPatch
					自分より相手が、同一バージョンだが、パッチが新しい
*/
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <vector>
#include <string>

#include <stdio.h>
#include <io.h>

// 自分のバージョンを指定する
// ex.
//		パッチなし	"1.1.0.0"
//		パッチあり	"2.0.1.0 P1" or "2.0.1.0P1" or "2.0.1.0.1"
const static std::string VERSION = "2.1.0.0P1";


// 戻り値定義
const static int VERSION_PATCH_NEWER = 5;
const static int VERSION_PATCH_OLDER = 4;
const static int VERSION_NEWER = 3;
const static int VERSION_OLDER = 2;
const static int VERSION_IDENTICAL = 1;
const static int VERSION_UNKNOWN = 0;
const static int VERSION_FATAL = -1;


namespace soga
{
	class ClUtil {
		private:
			ClUtil() {}
			~ClUtil() {}

		public:
			// iniファイルから指定キーを読み出す
			static std::string GetIniKey(const std::string &section, const std::string &key, const std::string &def, const std::string &fileName)
			{
				std::vector<char> value(10);

				while(GetPrivateProfileString(section.c_str(), key.c_str(), def.c_str(), &value[0], value.size(), fileName.c_str()) >= (value.size() - 1)) value.resize(value.size() * 2);

				return std::string(&value[0]);
			}
	};


	class ClVersion {
		public:
			ClVersion() : m_status(FALSE), m_version(0), m_miner1(0), m_miner2(0), m_revision(0), m_patch(0) {}
			ClVersion(const std::string &value) : m_status(FALSE), m_version(0), m_miner1(0), m_miner2(0), m_revision(0), m_patch(0)
			{
				ParseString(value);
			}
			ClVersion(int version, int miner1, int miner2, int revision, int patch): m_status(TRUE), m_version(version), m_miner1(miner1), m_miner2(miner2), m_revision(revision), m_patch(patch) {}

			ClVersion(const ClVersion &src): m_status(src.m_status), m_version(src.m_version), m_miner1(src.m_miner1), m_miner2(src.m_miner2), m_revision(src.m_revision), m_patch(src.m_patch)
			{
				m_strValue = src.m_strValue;
			}
			ClVersion& operator=(const ClVersion &src)
			{
				if(this == &src) return *this;

				m_status = src.m_status;
				m_version = src.m_version;
				m_miner1 = src.m_miner1;
				m_miner2 = src.m_miner2;
				m_revision = src.m_revision;
				m_patch = src.m_patch;
				m_strValue = src.m_strValue;

				return *this;
			}

			virtual ~ClVersion() {}

			// バージョン文字列を分解
			bool ParseString(const std::string &value)
			{
				m_strValue = &value[0];
				m_status = TRUE;

				std::string work(value);
				while(work.find(" ", 0) != std::string::npos) work.erase(work.find(" ", 0), 1);	// スペース削除
				while(work.find("P", 0) != std::string::npos) work.replace(work.find("P", 0), 1, ".");	// "P"を"."に変換

				std::vector<int> version;
				work.push_back('.');
				while(work.find(".", 0) != std::string::npos) {	// "."区切りの数字を読み出す
					version.push_back(strtoul(std::string(work, 0, work.find(".", 0)).c_str(), 0, 0));
					work = work.substr(work.find(".", 0) + 1);
				}
				if(version.size() < 4) return FALSE;			// 列挙が足らない
				if(version.size() < 5) version.push_back(0);	// パッチが無いので足す

				m_version = version[0];
				m_miner1 = version[1];
				m_miner2 = version[2];
				m_revision = version[3];
				m_patch = version[4];
				m_strValue.clear();

				return TRUE;
			}

			virtual int Compare(const ClVersion &clCompare) const
			{
				if(clCompare.IsStatus() == FALSE) return VERSION_FATAL;
				if(!clCompare.StrValue().empty()) return VERSION_UNKNOWN;

				if(clCompare.Version() > m_version) return VERSION_NEWER;
				if(clCompare.Version() < m_version) return VERSION_OLDER;

				if(clCompare.Miner1() > m_miner1) return VERSION_NEWER;
				if(clCompare.Miner1() < m_miner1) return VERSION_OLDER;

				if(clCompare.Miner2() > m_miner2) return VERSION_NEWER;
				if(clCompare.Miner2() < m_miner2) return VERSION_OLDER;

				if(clCompare.Revision() > m_revision) return VERSION_NEWER;
				if(clCompare.Revision() < m_revision) return VERSION_OLDER;

				if(clCompare.Patch() > m_patch) return VERSION_PATCH_NEWER;
				if(clCompare.Patch() < m_patch) return VERSION_PATCH_OLDER;

				return VERSION_IDENTICAL;
			}

			bool IsStatus() const { return m_status; }
			int Version() const { return m_version; }
			int Miner1() const { return m_miner1; }
			int Miner2() const { return m_miner2; }
			int Revision() const { return m_revision; }
			int Patch() const { return m_patch; }
			std::string StrValue() const { return m_strValue; }

		private:
			bool m_status;				// TRUE:FALSE = 情報有り:無し
			int m_version;
			int m_miner1;
			int m_miner2;
			int m_revision;
			int m_patch;
			std::string m_strValue;		// 予想外のバージョン情報格納場所
	};
};

// 文字列
class ClVersion2100P0 : public soga::ClVersion {
public:
	ClVersion2100P0() : soga::ClVersion(VERSION) {}
	virtual ~ClVersion2100P0() {}

	virtual int Compare(const soga::ClVersion &clCompare) const {
		int result = soga::ClVersion::Compare(clCompare);

		if (result == VERSION_UNKNOWN) {
			if (clCompare.StrValue().substr(1, 4) == "2000") {
				result = VERSION_OLDER;
			}
		}

		return result;
	}
};


static void EasyLog(const char* format, ...) {
	char tempPath[MAX_PATH];
	GetTempPath(MAX_PATH, tempPath);
	PathAppend(tempPath, "verchk.log");

	FILE* fp = fopen(tempPath, "a+");
	if (fp) {
		SYSTEMTIME st;
		GetLocalTime(&st);
		fprintf(fp, "[%04d/%02d/%02d %02d:%02d:%02d.%03d] ",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		va_list marker;
		va_start(marker, format);
		vfprintf(fp, format, marker);
		va_end(marker);

		fputs("\n", fp);

		bool renFlag = (_filelength(fileno(fp)) > (1024 * 1024));
		fclose(fp);

		if (renFlag) {
			char newName[MAX_PATH];
			strcat(strcpy(newName, tempPath), "_1");
			DeleteFile(newName);
			MoveFile(tempPath, newName);
		}
	}
}

// プログラムスタート
int WINAPI WinMain(HINSTANCE hInstCurrent, HINSTANCE hinstPrevious, LPSTR lpszCmdLine, int nCmdShow)
{
	EasyLog("----[ '%s' start]------------", __argv[0]);

	const ClVersion2100P0 clMyVersion;
	std::string fileName, section, key, value;

	// 引数チェック
	if(__argc < 4) return VERSION_FATAL;

	fileName = __argv[1];
	if(fileName.empty()) return VERSION_FATAL;

	section = __argv[2];
	if(section.empty()) return VERSION_FATAL;

	key = __argv[3];
	if(key.empty()) return VERSION_FATAL;

	// iniファイル読込み
	value = soga::ClUtil::GetIniKey(section, key, "", fileName);
	if(value.empty()) return VERSION_FATAL;
	soga::ClVersion clNowVersion(value);

	EasyLog("現在のバージョン: [%d.%d.%d.%dP%d] or [%s]", 
		clNowVersion.Version(), clNowVersion.Miner1(), clNowVersion.Miner2(), clNowVersion.Revision(), clNowVersion.Patch(),
		(clNowVersion.StrValue().empty()) ? "(なし)" : clNowVersion.StrValue().c_str());
	EasyLog("未来のバージョン: [%d.%d.%d.%dP%d] or [%s]", 
		clMyVersion.Version(), clMyVersion.Miner1(), clMyVersion.Miner2(), clMyVersion.Revision(), clMyVersion.Patch(),
		(clMyVersion.StrValue().empty()) ? "(なし)" : clMyVersion.StrValue().c_str());

	// 比較
	int result = clMyVersion.Compare(clNowVersion);
	EasyLog("終了コード %d で終了", result);
	return result;
}


