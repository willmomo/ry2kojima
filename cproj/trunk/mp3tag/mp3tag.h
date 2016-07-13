//---------------------------------------------------------------------
// mp3 の tag にアクセスするクラス類
//---------------------------------------------------------------------

#pragma once

#include "kjmlib.h"

//---------------------------------------------------------------------
// ID3 frame 一つ分を表現するクラス
//---------------------------------------------------------------------
class id3frame {
public:
	// デフォルトコンストラクタ
	id3frame() : m_size(0), m_flags(0) {
		memset(&m_frameID[0], 0, sizeof(m_frameID));
	}

	// コピーコンストラクタ
	id3frame(const id3frame& rhs) : m_size(rhs.m_size), m_flags(rhs.m_flags), m_data(rhs.m_data) {
		memmove(m_frameID, rhs.m_frameID, sizeof(m_frameID));
	}

	// デストラクタ
	virtual ~id3frame() {}

	// 代入演算子
	id3frame& operator=(const id3frame& rhs);

	// データからタグ情報を取り出す
	int parse(const std::vector<BYTE>& data, int offset);

	// COMM のときバッファ全体を解析する処理
	bool parseCOMM(const std::vector<BYTE> data, int& textEncoding, std::vector<BYTE>& language, kjm::_tstring& shortContent, kjm::_tstring& actualText) const;

	// COMM のときのテキスト取得
	_tstring getTextCOMM() const;

	// タグ空文字列を取得
	_tstring getText() const;

	// stream 出力対応
	friend _otstream& operator<<(_otstream& os, const id3frame& obj);

private:
	// PRIVタグを文字列化
	_tstring toStringPRIV() const {
		return kjm::util::toUnicode((char*)&m_data[0]);
	}

private:
	BYTE m_frameID[4];
	DWORD m_size;
	WORD m_flags;
	std::vector<BYTE> m_data;

public:
	// フレームIDの説明を返す
	static _tstring getFrameIdDescription(const std::vector<BYTE>& frameId);

	// エンコード番号の説明を返す
	static _tstring getEncodeDescription(int encode);
};

//---------------------------------------------------------------------
// ID3v2 tag 一つ分を表現するクラス
//---------------------------------------------------------------------
class ID3v2tag {
public:
	// デフォルトコンストラクタ
	ID3v2tag() {}

	// コピーコンストラクタ
	ID3v2tag(const ID3v2tag& rhs);

	// デストラクタ
	virtual ~ID3v2tag() {}

	// 代入演算子
	ID3v2tag& operator=(const ID3v2tag& rhs);

	// ファイルから ID3v2 tag を読み込む
	int load(kjm::cfile& file);

	// ID3 block の数を返す
	int get_frameCount() const {
		return m_frames.size();
	}

	// ID3 block を返す
	id3frame get_frame(int index) const {
		return m_frames.at(index);
	}

	// ID3v2 block の stream 出力対応
	friend _otstream& operator<<(_otstream& os, const ID3v2tag& obj);

private:

private:
	BYTE m_ident[3];	// ID3v2/file identifier
	BYTE m_version[2];	// ID3v2 version
	BYTE m_flags;		// ID3v2 flags
	DWORD m_size;		// ID3v2 size
	std::vector<BYTE> m_data;	// ブロック全体のデータ
	std::vector<id3frame> m_frames;
};

//---------------------------------------------------------------------
// ID3v1 tag を表現するクラス
//---------------------------------------------------------------------
class ID3v1tag {
public:
	// デフォルトコンストラクタ
	ID3v1tag() {}

	// コピーコンストラクタ
	ID3v1tag(const ID3v1tag& rhs) : m_data(rhs.m_data) {}

	// デストラクタ
	virtual ~ID3v1tag() {}

	// 代入演算子
	ID3v1tag& operator=(const ID3v1tag& rhs) {
		if (this == &rhs) return *this;
		m_data = rhs.m_data;
		return *this;
	}

	// ファイルからID3v1タグを読み込む
	int load(kjm::cfile& file);

	// ID3v1.0 or 1.1 の判定
	// 0=1.0 1=1.1 (125バイト目が 0 なら 1.1 それ以外なら 1.0 とみなす)
	int get_Version() const {
		return (m_data[125] == 0 ? 1 : 0);
	}

	// 生データから各種データを抜き出す
	kjm::_tstring get_SongTilte() const {
		std::vector<BYTE> work(m_data.begin() + 3, m_data.begin() + 3 + 30);
		if (1) {
			// SJISの場合
			std::string s(work.begin(), work.end());
			return kjm::util::toUnicode(s);
		} else {
			// Unicodeの場合
			std::wstring s(work.begin(), work.end());
			return s;
		}
	}

	kjm::_tstring get_Artist() const {
		std::vector<BYTE> work(m_data.begin() + 33, m_data.begin() + 33 + 30);
		if (1) {
			// SJISの場合
			std::string s(work.begin(), work.end());
			return kjm::util::toUnicode(s);
		} else {
			// Unicodeの場合
			std::wstring s(work.begin(), work.end());
			return s;
		}
	}

	kjm::_tstring get_Album() const {
		std::vector<BYTE> work(m_data.begin() + 63, m_data.begin() + 63 + 30);
		if (1) {
			// SJISの場合
			std::string s(work.begin(), work.end());
			return kjm::util::toUnicode(s);
		} else {
			// Unicodeの場合
			std::wstring s(work.begin(), work.end());
			return s;
		}
	}

	kjm::_tstring get_Year() const {
		std::vector<BYTE> work(m_data.begin() + 93, m_data.begin() + 93 + 4);
		if (1) {
			// SJISの場合
			std::string s(work.begin(), work.end());
			return kjm::util::toUnicode(s);
		} else {
			// Unicodeの場合
			std::wstring s(work.begin(), work.end());
			return s;
		}
	}

	kjm::_tstring get_Comment() const {
		std::vector<BYTE> work(m_data.begin() + 97, m_data.begin() + 97 + (get_Version() == 0 ? 30 : 28));
		if (1) {
			// SJISの場合
			std::string s(work.begin(), work.end());
			return kjm::util::toUnicode(s);
		} else {
			// Unicodeの場合
			std::wstring s(work.begin(), work.end());
			return s;
		}
	}

	int get_AlbumTrack() const {
		return m_data[126];
	}

	int get_Genre() const {
		return m_data[127];
	}

	// ID3v1 block の stream 出力対応
	friend _otstream& operator<<(_otstream& os, const ID3v1tag& obj);

private:
	std::vector<BYTE> m_data;	// ID3v1の生データ
};

//---------------------------------------------------------------------
// mp3 全体を扱うクラス
//---------------------------------------------------------------------
class mp3tag {
public:
	// デフォルトコンストラクタ
	mp3tag() : m_ID3v1tag(false) {}

	// コピーコンストラクタ
	mp3tag(const mp3tag& rhs) : m_v2tags(rhs.m_v2tags), m_v1tag(rhs.m_v1tag), m_ID3v1tag(rhs.m_ID3v1tag) {}

	// デストラクタ
	virtual ~mp3tag() {}

	// 代入演算子
	mp3tag& operator=(const mp3tag& rhs);

	// ファイルからロードするメソッド
	int load(const _tstring& fname);

	// ID3 tag の数を返す
	int get_tagCount() const {
		return m_v2tags.size();
	}

	// ID3v2 tag を返す
	ID3v2tag get_v2tag(int index) const {
		return m_v2tags.at(index);
	}

	// ID3v1 tag を返す
	ID3v1tag get_v1tag() const {
		return m_v1tag;
	}

	// ID3v1 tag の有無を返す
	bool hasID3v1tag() const {
		return m_ID3v1tag;
	}

private:
	// ID3v2 tag を保存
	std::vector<ID3v2tag> m_v2tags;

	// ID3v1 tag を保存
	ID3v1tag m_v1tag;
	
	// ID3v1 tag が読み込めたとき true
	bool m_ID3v1tag;
};
