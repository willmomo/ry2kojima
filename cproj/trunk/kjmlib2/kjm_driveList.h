//---------------------------------------------------------------------
// �h���C�u�̈ꗗ��ێ�����N���X
//---------------------------------------------------------------------

#pragma once

#include "kjm_drive.h"

namespace kjm {
	class driveList {
	public:
		// �f�t�H���g�R���X�g���N�^
		driveList();

		// �R�s�[�R���X�g���N�^
		driveList(const driveList& src) : m_driveList(src.m_driveList) {};

		// �f�X�g���N�^
		virtual ~driveList() {};

		// ������Z�q
		driveList& operator=(const driveList& rhs);

		// �h���C�u�ꗗ�̐����擾
		int get_Size() { return m_driveList.size(); };

		// �h���C�u�I�u�W�F�N�g���擾
		drive get_Drive(int index) { return m_driveList[index]; };

	private:
		std::vector<drive> m_driveList;	// PC�ɂȂ����Ă���h���C�u�ꗗ
	};
};
