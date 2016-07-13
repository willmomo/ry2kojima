//---------------------------------------------------------------------
// mp3 �� tag �ɃA�N�Z�X����N���X��
//---------------------------------------------------------------------

#pragma once

#include "kjmlib.h"

//---------------------------------------------------------------------
// ID3 frame �����\������N���X
//---------------------------------------------------------------------
class id3frame {
public:
	// �f�t�H���g�R���X�g���N�^
	id3frame() : m_size(0), m_flags(0) {
		memset(&m_frameID[0], 0, sizeof(m_frameID));
	}

	// �R�s�[�R���X�g���N�^
	id3frame(const id3frame& rhs) : m_size(rhs.m_size), m_flags(rhs.m_flags), m_data(rhs.m_data) {
		memmove(m_frameID, rhs.m_frameID, sizeof(m_frameID));
	}

	// �f�X�g���N�^
	virtual ~id3frame() {}

	// ������Z�q
	id3frame& operator=(const id3frame& rhs);

	// �f�[�^����^�O�������o��
	int parse(const std::vector<BYTE>& data, int offset);

	// COMM �̂Ƃ��o�b�t�@�S�̂���͂��鏈��
	bool parseCOMM(const std::vector<BYTE> data, int& textEncoding, std::vector<BYTE>& language, kjm::_tstring& shortContent, kjm::_tstring& actualText) const;

	// COMM �̂Ƃ��̃e�L�X�g�擾
	_tstring getTextCOMM() const;

	// �^�O�󕶎�����擾
	_tstring getText() const;

	// stream �o�͑Ή�
	friend _otstream& operator<<(_otstream& os, const id3frame& obj);

private:
	// PRIV�^�O�𕶎���
	_tstring toStringPRIV() const {
		return kjm::util::toUnicode((char*)&m_data[0]);
	}

private:
	BYTE m_frameID[4];
	DWORD m_size;
	WORD m_flags;
	std::vector<BYTE> m_data;

public:
	// �t���[��ID�̐�����Ԃ�
	static _tstring getFrameIdDescription(const std::vector<BYTE>& frameId);

	// �G���R�[�h�ԍ��̐�����Ԃ�
	static _tstring getEncodeDescription(int encode);
};

//---------------------------------------------------------------------
// ID3v2 tag �����\������N���X
//---------------------------------------------------------------------
class ID3v2tag {
public:
	// �f�t�H���g�R���X�g���N�^
	ID3v2tag() {}

	// �R�s�[�R���X�g���N�^
	ID3v2tag(const ID3v2tag& rhs);

	// �f�X�g���N�^
	virtual ~ID3v2tag() {}

	// ������Z�q
	ID3v2tag& operator=(const ID3v2tag& rhs);

	// �t�@�C������ ID3v2 tag ��ǂݍ���
	int load(kjm::cfile& file);

	// ID3 block �̐���Ԃ�
	int get_frameCount() const {
		return m_frames.size();
	}

	// ID3 block ��Ԃ�
	id3frame get_frame(int index) const {
		return m_frames.at(index);
	}

	// ID3v2 block �� stream �o�͑Ή�
	friend _otstream& operator<<(_otstream& os, const ID3v2tag& obj);

private:

private:
	BYTE m_ident[3];	// ID3v2/file identifier
	BYTE m_version[2];	// ID3v2 version
	BYTE m_flags;		// ID3v2 flags
	DWORD m_size;		// ID3v2 size
	std::vector<BYTE> m_data;	// �u���b�N�S�̂̃f�[�^
	std::vector<id3frame> m_frames;
};

//---------------------------------------------------------------------
// ID3v1 tag ��\������N���X
//---------------------------------------------------------------------
class ID3v1tag {
public:
	// �f�t�H���g�R���X�g���N�^
	ID3v1tag() {}

	// �R�s�[�R���X�g���N�^
	ID3v1tag(const ID3v1tag& rhs) : m_data(rhs.m_data) {}

	// �f�X�g���N�^
	virtual ~ID3v1tag() {}

	// ������Z�q
	ID3v1tag& operator=(const ID3v1tag& rhs) {
		if (this == &rhs) return *this;
		m_data = rhs.m_data;
		return *this;
	}

	// �t�@�C������ID3v1�^�O��ǂݍ���
	int load(kjm::cfile& file);

	// ID3v1.0 or 1.1 �̔���
	// 0=1.0 1=1.1 (125�o�C�g�ڂ� 0 �Ȃ� 1.1 ����ȊO�Ȃ� 1.0 �Ƃ݂Ȃ�)
	int get_Version() const {
		return (m_data[125] == 0 ? 1 : 0);
	}

	// ���f�[�^����e��f�[�^�𔲂��o��
	kjm::_tstring get_SongTilte() const {
		std::vector<BYTE> work(m_data.begin() + 3, m_data.begin() + 3 + 30);
		if (1) {
			// SJIS�̏ꍇ
			std::string s(work.begin(), work.end());
			return kjm::util::toUnicode(s);
		} else {
			// Unicode�̏ꍇ
			std::wstring s(work.begin(), work.end());
			return s;
		}
	}

	kjm::_tstring get_Artist() const {
		std::vector<BYTE> work(m_data.begin() + 33, m_data.begin() + 33 + 30);
		if (1) {
			// SJIS�̏ꍇ
			std::string s(work.begin(), work.end());
			return kjm::util::toUnicode(s);
		} else {
			// Unicode�̏ꍇ
			std::wstring s(work.begin(), work.end());
			return s;
		}
	}

	kjm::_tstring get_Album() const {
		std::vector<BYTE> work(m_data.begin() + 63, m_data.begin() + 63 + 30);
		if (1) {
			// SJIS�̏ꍇ
			std::string s(work.begin(), work.end());
			return kjm::util::toUnicode(s);
		} else {
			// Unicode�̏ꍇ
			std::wstring s(work.begin(), work.end());
			return s;
		}
	}

	kjm::_tstring get_Year() const {
		std::vector<BYTE> work(m_data.begin() + 93, m_data.begin() + 93 + 4);
		if (1) {
			// SJIS�̏ꍇ
			std::string s(work.begin(), work.end());
			return kjm::util::toUnicode(s);
		} else {
			// Unicode�̏ꍇ
			std::wstring s(work.begin(), work.end());
			return s;
		}
	}

	kjm::_tstring get_Comment() const {
		std::vector<BYTE> work(m_data.begin() + 97, m_data.begin() + 97 + (get_Version() == 0 ? 30 : 28));
		if (1) {
			// SJIS�̏ꍇ
			std::string s(work.begin(), work.end());
			return kjm::util::toUnicode(s);
		} else {
			// Unicode�̏ꍇ
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

	// ID3v1 block �� stream �o�͑Ή�
	friend _otstream& operator<<(_otstream& os, const ID3v1tag& obj);

private:
	std::vector<BYTE> m_data;	// ID3v1�̐��f�[�^
};

//---------------------------------------------------------------------
// mp3 �S�̂������N���X
//---------------------------------------------------------------------
class mp3tag {
public:
	// �f�t�H���g�R���X�g���N�^
	mp3tag() : m_ID3v1tag(false) {}

	// �R�s�[�R���X�g���N�^
	mp3tag(const mp3tag& rhs) : m_v2tags(rhs.m_v2tags), m_v1tag(rhs.m_v1tag), m_ID3v1tag(rhs.m_ID3v1tag) {}

	// �f�X�g���N�^
	virtual ~mp3tag() {}

	// ������Z�q
	mp3tag& operator=(const mp3tag& rhs);

	// �t�@�C�����烍�[�h���郁�\�b�h
	int load(const _tstring& fname);

	// ID3 tag �̐���Ԃ�
	int get_tagCount() const {
		return m_v2tags.size();
	}

	// ID3v2 tag ��Ԃ�
	ID3v2tag get_v2tag(int index) const {
		return m_v2tags.at(index);
	}

	// ID3v1 tag ��Ԃ�
	ID3v1tag get_v1tag() const {
		return m_v1tag;
	}

	// ID3v1 tag �̗L����Ԃ�
	bool hasID3v1tag() const {
		return m_ID3v1tag;
	}

private:
	// ID3v2 tag ��ۑ�
	std::vector<ID3v2tag> m_v2tags;

	// ID3v1 tag ��ۑ�
	ID3v1tag m_v1tag;
	
	// ID3v1 tag ���ǂݍ��߂��Ƃ� true
	bool m_ID3v1tag;
};
