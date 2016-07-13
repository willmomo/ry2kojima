#pragma once

#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


//---------------------------------------------------------------------
// �ƂĂ��ȒP�Ȕz��e���v���[�g
//
// [���l]
// �������̊m�ۂ�remalloc���g���̂ŁAT�͊�{�^�݂̂Ŏg�p���Ă��������B
//---------------------------------------------------------------------
template<class T> class array {
public:
	//-----------------------------------------------------------------
	// �f�t�H���g�R���X�g���N�^
	//-----------------------------------------------------------------
	array() : m_pt(NULL), m_size(0) {};

	//-----------------------------------------------------------------
	// �������m�ەt���R���X�g���N�^
	//-----------------------------------------------------------------
	array(int size) : m_pt(NULL), m_size(0) { resize(size); };

	//-----------------------------------------------------------------
	// �f�X�g���N�^
	//-----------------------------------------------------------------
	~array() { resize(0); };

	//-----------------------------------------------------------------
	// ��������(��)���蓖��
	//
	// [����]
	// size  �V�����v�f��
	//
	// [���l]
	// size��0�ɂ���ƁA��������ɂȂ�܂��B
	//-----------------------------------------------------------------
	bool resize(int size) {
		bool result = true;

		if (size == 0) {
			free(m_pt);
			m_pt = NULL;
			m_size = 0;
		} else {
			m_pt = (T*)realloc(m_pt, sizeof(T) * size);
			m_size = size;
			if (m_pt == NULL) {
				m_size = 0;
				result = false;
			}
		}

		return result;
	};

	//-----------------------------------------------------------------
	// �z��̗v�f����Ԃ�
	//-----------------------------------------------------------------
	int size() const { return m_size; };

	//-----------------------------------------------------------------
	// �z��̐擪�A�h���X���擾
	//-----------------------------------------------------------------
	T* top() { return m_pt; };
	const T* top() const { return m_pt; };
	operator T*() { return m_pt; };
	operator const T* () const { return m_pt; };

private:
	T* m_pt;		// �z��̐擪�A�h���X
	int m_size;		// �v�f��
};


bool remove_folderW(const wchar_t* pszPath);
bool remove_folderA(const char* pszPath);

#if defined(UNICODE)
#define remove_folder	remove_folderW
#else
#define remove_folder	remove_folderA
#endif
