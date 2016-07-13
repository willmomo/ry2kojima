//---------------------------------------------------------------------
// �h���C�u�̏���ێ�����N���X
//---------------------------------------------------------------------

#pragma once

#include "kjm_base.h"

namespace kjm {
	class drive {
	public:
		// �f�t�H���g�R���X�g���N�^
		drive() : m_driveNo(-1) {};

		// �R�s�[�R���X�g���N�^
		drive(const drive& src) : m_driveNo(src.m_driveNo) {};

		// �h���C�u�ԍ����w�肷��R���X�g���N�^
		drive(int driveNo) : m_driveNo(driveNo) {};

		// �h���C�u���^�[���w�肷��R���X�g���N�^
		drive(const _tstring& driveLetter);

		// �f�X�g���N�^
		virtual ~drive() {};

		// ������Z�q
		drive& operator=(const drive& rhs);

		// �h���C�u�ԍ����擾
		int get_DriveNo() { return m_driveNo; };

		// �h���C�u���^�[���擾
		_tstring get_DriveLetter() { return _tstring(1, (m_driveNo + 'A')) + _T(":"); };

		// �h���C�u�^�C�v���擾
		UINT get_DriveType() { return GetDriveType(get_DriveLetter().c_str()); };

		// �h���C�u�^�C�v�𕶎���Ŏ擾
		_tstring get_DriveTypeString();

		// �h���C�u�Ƀ��f�B�A���}������Ă��邩�ǂ������ׂ�
		// �ǂ̃h���C�u�^�C�v�ɂ��g���邪�A
		// get_DriveType() == DRIVE_REMOVABLE �̂Ƃ��A�g���Ă��������B
		bool isDiskInDrive();

	private:
		int m_driveNo;	// �h���C�u�ԍ�(-1=���I�� 0=A 1=B...)
	};
};
