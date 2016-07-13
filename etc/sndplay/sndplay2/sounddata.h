
#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <string>

class CSoundData
{
public:
	// �R���X�g���N�^
	CSoundData(HWND hwnd, const char* soundFile);

	// �f�X�g���N�^
	~CSoundData();

	// �Đ�
	bool Play();

	// ��~
	bool Stop();

	// ����
	bool Close();

private:
	// �f�t�H���g�R���X�g���N�^(��I�u�W�F�N�g�쐬�֎~)
	CSoundData(){};

	// �R�s�[�R���X�g���N�^(�R�s�[�֎~)
	CSoundData(const CSoundData&){};

	// ������Z�q(�R�s�[�֎~)
	operator=(const CSoundData&){};

private:
	// �ʒm���󂯂�E�B���h�E�ւ̃n���h��
	HWND m_hwnd;

	// �Đ�����T�E���h�t�@�C����
	std::string m_soundFile;

	// �f�o�C�XID
	WORD m_deviceID;

	// MCI�֐��Ŕ��������Ō�̃G���[�̕�����
	std::string m_mciLastErrorText;
};
