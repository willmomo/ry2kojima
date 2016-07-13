//---------------------------------------------------------------------
// mp3 の tag にアクセスするクラス類
//---------------------------------------------------------------------

#include "mp3tag.h"

DWORD SynchsafeToDWORD(const BYTE* p) {
	DWORD result = 0;
	for (int i = 0; i < 4; i++) {
		result = (result << 7) | (p[i] & 0x7f);
	}
	return result;
}

kjm::_tstring toBits(BYTE b) {
	kjm::_tstring result;
	for (int i = 0x80; i != 0; i >>= 1)
		result.push_back((b & i) ? '1' : '0');
	return result;
}

//---------------------------------------------------------------------
// mp3 全体を扱うクラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// 代入演算子
//---------------------------------------------------------------------
mp3tag& mp3tag::operator=(const mp3tag& rhs) {
	if (this == &rhs) return *this;
	m_v2tags = rhs.m_v2tags;
	m_v1tag = rhs.m_v1tag;
	m_ID3v1tag = rhs.m_ID3v1tag;
	return *this;
}

//---------------------------------------------------------------------
// ファイルを読み込むメソッド
//---------------------------------------------------------------------
int mp3tag::load(const _tstring& fname) {
	kjm::cfile file;
	if (file.open(fname, _T("rb"))) {
		ID3v2tag tag;
		while (tag.load(file) == 0) {
			m_v2tags.push_back(tag);
		}

		if (m_v1tag.load(file) != 0) {
			m_ID3v1tag = false;
		} else {
			m_ID3v1tag = true;
		}
	} else {
		return -1;
	}
	return 0;
}

//---------------------------------------------------------------------
// ID3v1 tag を表現するクラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// ファイルからID3v1タグを読み込む
//---------------------------------------------------------------------
int ID3v1tag::load(kjm::cfile& file) {
	std::vector<BYTE> work;
	file.fseek(-128, SEEK_END);
	if (file.read(work, 128) != 128) return -1;
	if ( ! (work[0] == 'T'))	return -1;
	if ( ! (work[1] == 'A'))	return -1;
	if ( ! (work[2] == 'G'))	return -1;

	m_data = work;

	return 0;
}

//---------------------------------------------------------------------
// ID3v1 ブロックを簡単出力
//---------------------------------------------------------------------
_otstream& operator<<(_otstream& os, const ID3v1tag& obj) {

	os << _T("[ID3v1.") << obj.get_Version() << _T("]\n");
	os << _T("Sont Title: [") << obj.get_SongTilte() << _T("]\n");
	os << _T("Artist    : [") << obj.get_Artist() << _T("]\n");
	os << _T("Album     : [") << obj.get_Album() << _T("]\n");
	os << _T("Year      : [") << obj.get_Year() << _T("]\n");
	os << _T("Comment   : [") << obj.get_Comment() << _T("]\n");
	if (obj.get_Version() == 1) {
		os << _T("AlbumTrack: [") << obj.get_AlbumTrack() << _T("]\n");
	}
	os << _T("Genre     : [") << obj.get_Genre() << _T("]\n");

	return os;
}

//---------------------------------------------------------------------
// ID3v2 tag 一つ分を表現するクラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// コピーコンストラクタ
//---------------------------------------------------------------------
ID3v2tag::ID3v2tag(const ID3v2tag& rhs) {
	memmove(m_ident, rhs.m_ident, 3);
	memmove(m_version, rhs.m_version, 2);
	m_flags = rhs.m_flags;
	m_size = rhs.m_size;
	m_data = rhs.m_data;
	m_frames = rhs.m_frames;
}

//---------------------------------------------------------------------
// 代入演算子
//---------------------------------------------------------------------
ID3v2tag& ID3v2tag::operator=(const ID3v2tag& rhs) {
	if (this == &rhs) return *this;
	memmove(m_ident, rhs.m_ident, 3);
	memmove(m_version, rhs.m_version, 2);
	m_flags = rhs.m_flags;
	m_size = rhs.m_size;
	m_data = rhs.m_data;
	m_frames = rhs.m_frames;
	return *this;
}

//---------------------------------------------------------------------
// ファイルから ID3 tag を読み込む
//---------------------------------------------------------------------
int ID3v2tag::load(kjm::cfile& file) {

	// 先頭10バイトを使ってID3 tag の判定を行う
	// An ID3v2 tag can be detected with the following pattern:
	//	$49 44 33 yy yy xx zz zz zz zz
	// Where yy is less than $FF, xx is the 'flags' byte and zz is less than $80.
	
	std::vector<BYTE> work;
	if (file.read(work, 10) != 10) return -1;

	if ( ! (work[0] == 'I'))	return -1;
	if ( ! (work[1] == 'D'))	return -1;
	if ( ! (work[2] == '3'))	return -1;
	if ( ! (work[3] < 0xff))	return -1;
	if ( ! (work[4] < 0xff))	return -1;
	if ( ! (work[6] < 0x80))	return -1;
	if ( ! (work[7] < 0x80))	return -1;
	if ( ! (work[8] < 0x80))	return -1;
	if ( ! (work[9] < 0x80))	return -1;

	for (int i = 0; i < 3; i++) m_ident[i] = work[i];
	for (int i = 0; i < 2; i++) m_version[i] = work[3 + i];
	m_flags = work[5];
	m_size = SynchsafeToDWORD(&work[6]);

	if (m_size > 0) {
		if (file.read(m_data, m_size) != m_size) return -1;

		for (int ofs = 0; ofs < m_data.size() && m_data[ofs] != 0;) {
			id3frame frame;

			ofs = frame.parse(m_data, ofs);

			m_frames.push_back(frame);
		}
	}

	return 0;
}

//---------------------------------------------------------------------
// ID3v2 block を簡単出力
//---------------------------------------------------------------------
_otstream& operator<<(_otstream& os, const ID3v2tag& obj) {
	os << _T("version: ID3v2.") << obj.m_version[0] << _T(".") << obj.m_version[1] << _T("\n");
	os << _T("flags  : %") << toBits(obj.m_flags) << _T(" (0x") << std::hex << std::setw(2) << std::setfill(_T('0')) << obj.m_flags << _T(")\n");
	os << std::dec;
	os << _T("size   : ") << std::dec << obj.m_size;

	return os;
}

//---------------------------------------------------------------------
// フレームを表現するクラス
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// 代入演算子
//---------------------------------------------------------------------
id3frame& id3frame::operator=(const id3frame& rhs) {
	if (this == &rhs) return *this;
	memmove(&m_frameID[0], &rhs.m_frameID[0], 4);
	m_size = rhs.m_size;
	m_flags = rhs.m_flags;
	m_data = rhs.m_data;
	return *this;
}

//---------------------------------------------------------------------
// ID3 block を簡単出力
//---------------------------------------------------------------------
_otstream& operator<<(_otstream& os, const id3frame& obj) {
	if (obj.m_frameID[3] == 0) {
		// ID3v2.0 当時の3文字 frame ID を使っている
		os << _T("Frame ID: ") << static_cast<TCHAR>(obj.m_frameID[0]) << static_cast<TCHAR>(obj.m_frameID[1]) 
			<< static_cast<TCHAR>(obj.m_frameID[2])
			<< _T(" [") << id3frame::getFrameIdDescription(std::vector<BYTE>(&obj.m_frameID[0], &obj.m_frameID[4])) << _T("]")
			<< _T("\n");
	} else {
		os << _T("Frame ID: ") << static_cast<TCHAR>(obj.m_frameID[0]) << static_cast<TCHAR>(obj.m_frameID[1])
			<< static_cast<TCHAR>(obj.m_frameID[2]) << static_cast<TCHAR>(obj.m_frameID[3])
			<< _T(" [") << id3frame::getFrameIdDescription(std::vector<BYTE>(&obj.m_frameID[0], &obj.m_frameID[4])) << _T("]")
			<< _T("\n");
	}
	;
	os << _T("Size    : ") << std::dec << obj.m_size << _T("\n");
	os << _T("Flags   : $") << std::hex << std::setw(4) << std::setfill(_T('0')) << obj.m_flags << std::dec << _T("\n");
	if (obj.m_size > 0) {
		os << _T("RAW data: ") << std::hex;
		for (std::vector<BYTE>::const_iterator i = obj.m_data.begin(); i != obj.m_data.end(); i++) {
			os << std::setw(2) << std::setfill(_T('0')) << *i << _T(" ");
		}
		os << _T("\n");
	}
	if (obj.m_size > 1) {
		if (memcmp(obj.m_frameID, "PRIV", 4) == 0) {
			os << _T("Owner ID: ") << obj.toStringPRIV() << _T("\n");
		} else {
			os << _T("Encoding: $") << std::hex << std::setw(2) << std::setfill(_T('0')) << obj.m_data[0] << _T(" ");
			os << _T("(") << id3frame::getEncodeDescription(obj.m_data[0]) << _T(")\n");
			if (memcmp(obj.m_frameID, "COMM", 4) == 0) {

				int textEncoding = 0;
				std::vector<BYTE> language;
				kjm::_tstring shortContent;
				kjm::_tstring actualText;

				obj.parseCOMM(obj.m_data, textEncoding, language, shortContent, actualText);

				os << _T("Language: ") << (TCHAR)language[0] << (TCHAR)language[1] << (TCHAR)language[2] << _T("\n");
				os << _T("ShortCOM: ") << shortContent << _T("\n");
				os << _T("ActualTx: ") << actualText << _T("\n");
			} else {
				os << _T("Text    : ") <<  obj.getText() << _T("\n");
			}
		}
	}

	return os;
}

//---------------------------------------------------------------------
// フレームIDの説明を返す
//---------------------------------------------------------------------
_tstring id3frame::getFrameIdDescription(const std::vector<BYTE>& frameID) {
	int id = *(int*)&frameID[0];

	if (id == htonl('TALB')) {
		return _T("Album/Movie/Show title");
	} else if (id == htonl('TCON')) {
		return _T("Content type");
	} else if (id == htonl('TIT2')) {
		return _T("Title/songname/content description");
	} else if (id == htonl('TPE1')) {
		return _T("Lead performer(s)/Soloist(s)");
	} else if (id == htonl('TRCK')) {
		return _T("Track number/Position in set");
	} else if (id == htonl('TYER')) {
		return _T("Year");
	}

	return _T("");
}

//---------------------------------------------------------------------
// エンコード番号の説明を返す
//---------------------------------------------------------------------
_tstring id3frame::getEncodeDescription(int encode) {
	switch (encode) {
	case 0:
		return _T("ISO-8859-1");			// ISO-8859-1 [ISO-8859-1]. Terminated with $00.
	case 1:
		return _T("UTF-16 with BOM");		// UTF-16 [UTF-16] encoded Unicode [UNICODE] with BOM.
	case 2:
		return _T("UTF-16BE without BOM");	// UTF-16BE [UTF-16] encoded Unicode [UNICODE] without BOM.
	case 3:
		return _T("UTF-8");					// UTF-8 [UTF-8] encoded Unicode [UNICODE].
		break;
	}
	return _T("unknown encoding");
}

//---------------------------------------------------------------------
// タグ空文字列を取得
//---------------------------------------------------------------------
_tstring id3frame::getText() const {
	std::vector<BYTE> temp;
	_tstring result;

	if (m_size < 2) {
		return _T("");
	}
	int top = 1;
	switch (m_data[0]) {
	case 0:
		// ISO-8859-1
		temp = std::vector<BYTE>(m_data.begin() + top, m_data.end());
		temp.push_back(0);
		result = kjm::util::toUnicode((char*)&temp[0], CP_ACP);
		break;
	case 1:
		// UTF-16 with BOM
		if (m_data[1] == 0xff) {
			temp = std::vector<BYTE>(m_data.begin() + 3, m_data.end());
			temp.push_back(0);
			temp.push_back(0);
			result = _tstring((wchar_t*)&temp[0]);
		} else {
			// エンディアンが処理系と逆
			temp = std::vector<BYTE>(m_data.begin() + 3, m_data.end());
			for (int i = 0; i < temp.size(); i += 2) {
				BYTE c = temp[i];
				temp[i] = temp[i+1];
				temp[i+1] = c;
			}
			temp.push_back(0);
			temp.push_back(0);
			result = _tstring((wchar_t*)&temp[0]);
		}
		//result = kjm::util::toUnicodeString(std::vector<BYTE>(m_data.begin() + 1, m_data.end()));
		break;
	case 2:
		// UTF-16BE without BOM
		break;
	case 3:
		// UTF-8
		result = kjm::util::toUnicode((char*)&m_data[1], CP_UTF8);
		break;
	default:
		// unknown encoding
		break;
	}

	return result;
}

//---------------------------------------------------------------------
// COMM のときのテキスト取得
//---------------------------------------------------------------------
_tstring id3frame::getTextCOMM() const {
	std::vector<BYTE> temp;
	_tstring result;

	int top = 4;
	switch (m_data[0]) {
	case 0:	
		//_tprintf(_T("ISO-8859-1 [ISO-8859-1]. Terminated with $00.\n"));
		temp = std::vector<BYTE>(m_data.begin() + top, m_data.end());
		temp.push_back(0);
		result = kjm::util::toUnicode((char*)&temp[0], CP_ACP);
		//for (n = 0; n < m_data.size(); n++) {
		//	result += kjm::util::sprintf_str(_T("%02X "), m_data[n]);
		//}
		break;
	case 1:
		// _tprintf(_T("UTF-16 [UTF-16] encoded Unicode [UNICODE] with BOM.\n"));
		if (m_data[top] == 0xff) {
			temp = std::vector<BYTE>(m_data.begin() + top + 2, m_data.end());
			temp.push_back(0);
			temp.push_back(0);
			result = _tstring((wchar_t*)&temp[0]);
		} else {
			// エンディアンが処理系と逆
			temp = std::vector<BYTE>(m_data.begin() + top + 2, m_data.end());
			for (int i = 0; i < temp.size(); i += 2) {
				BYTE c = temp[i];
				temp[i] = temp[i+1];
				temp[i+1] = c;
			}
			temp.push_back(0);
			temp.push_back(0);
			result = _tstring((wchar_t*)&temp[0]);
		}
		break;
	case 2: _tprintf(_T("UTF-16BE [UTF-16] encoded Unicode [UNICODE] without BOM."));
		break;
	case 3:
		// _tprintf(_T("UTF-8 [UTF-8] encoded Unicode [UNICODE].\n"));
		result = kjm::util::toUnicode((char*)&m_data[1], CP_UTF8);
		break;
	default:	_tprintf(_T("unknown encoding.\n"));
		break;
	}

	return result;
}

//---------------------------------------------------------------------
// データからタグ情報を取り出す
//---------------------------------------------------------------------
int id3frame::parse(const std::vector<BYTE>& data, int offset) {
	int i = offset;

	m_frameID[0] = data[i++];
	m_frameID[1] = data[i++];
	m_frameID[2] = data[i++];
	m_frameID[3] = data[i++];

	for (int n = 0; n < 4; n++)
		m_size = (m_size << 8) | data[i++];

	for (int n = 0; n < 2; n++)
		m_flags = (m_flags << 8) | data[i++];

	if (m_size > 0) {
		m_data.resize(m_size);
		memcpy(&m_data[0], &data[i], m_size);
		i += m_size;
	}

	return i;
}

kjm::_tstring toUnicode(const std::vector<BYTE>& data) {
	std::vector<BYTE> work = data;

	// 先頭2バイトがBOMかどうか調べる。
	if (work[0] == 0xfe && work[1] == 0xff) {
		work.erase(work.begin(), work.begin() + 2);
		// エンディアン付きで、処理系と逆
		for (int i = 0; i < work.size(); i += 2) {
			BYTE c = work[i];
			work[i] = work[i+1];
			work[i+1] = c;
		}
	} else if (work[0] == 0xff && work[1] == 0xfe) {
		// エンディアンを取り除く
		work.erase(work.begin(), work.begin() + 2);
	}

	// 終端文字列を探す
	std::vector<BYTE>::iterator endPos;
	for (endPos = work.begin(); endPos != work.end(); endPos += 2) {
		if (endPos[0] == 0 && endPos[1] == 0) {
			break;
		}
	}

	std::vector<BYTE> txtBytes(work.begin(), endPos);

	return _T("");
}

//---------------------------------------------------------------------
// COMM のときバッファ全体を解析する処理
//---------------------------------------------------------------------
bool id3frame::parseCOMM(const std::vector<BYTE> data, int& textEncoding, std::vector<BYTE>& language, kjm::_tstring& shortContent, kjm::_tstring& actualText) const {
	std::vector<BYTE> work = data;

	textEncoding = (int)work[0];
	work.erase(work.begin());

	language.assign(work.begin(), work.begin() + 3);
	work.erase(work.begin(), work.begin() + 3);

	if (textEncoding == 0) {
		shortContent = kjm::util::toUnicode(kjm::util::toAnsiString(work));
		actualText = kjm::util::toUnicode(kjm::util::toAnsiString(work));
	} else if (textEncoding == 1) {
		shortContent = kjm::util::toUnicodeString(work);
		actualText = kjm::util::toUnicodeString(work);
	}

	return true;
}

