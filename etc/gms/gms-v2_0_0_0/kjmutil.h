/**
 * �e�탆�[�e�B���e�B
 */

#pragma once

// �x�����x����4�Ɉ����グ��
#pragma warning(push, 4)

// �x�� '�������֐��̖{�̂� 1 �x���Q�Ƃ���܂���B' �𖳌�������
#pragma warning(disable: 4100)

// ���[�e�B���e�B���K�v�Ƃ���w�b�_���C���N���[�h
#include <windows.h>

// namespace kjm �̒�`
namespace kjm {
	
	/**
	 * Rectangle�\����(.NET��)
	 */

	struct Rectangle : public tagRECT
	{
		/**
		 * ����Rectanlge�\���̂̉��[��y���W���擾���܂��B
		 */

		int get_Bottom() {
			return tagRECT::bottom;
		};

		/**
		 * ����Rectangle�\���̂̍������擾�܂��͐ݒ肵�܂��B
		 */

		int get_Height() {
			return tagRECT::bottom - tagRECT::top;
		};
		void set_Height(int value) {
			tagRECT::bottom = value - tagRECT::top;
		};

		/**
		 * ����Rectangle�̂��ׂĂ̐��l�v���p�e�B�ɒl�[�������邩�ǂ����e�X�g���܂��B
		 */

		bool get_IsEmpty() {
			return ::IsRectEmpty(this) != FALSE;
		};

		/**
		 * ����Rectangle�̍\���̂̍��[��x���W���擾���܂��B
		 */

		int get_Left() {
			return tagRECT::left;
		};

		/**
		 * ����Rectangle�\���̂̉E�[��x���W���擾���܂��B
		 */

		int get_Right() {
			return tagRECT::right;
		};

		/**
		 * ����Rectangle�\���̂̏�[��y���W���擾���܂��B
		 */

		int get_Top() {
			return tagRECT::top;
		};

		/**
		 * ����Rectangle�\���̂̕����擾�܂��͐ݒ肵�܂��B
		 */

		int get_Width() {
			return tagRECT::right - tagRECT::left;
		};
		void set_Width(int value) {
			tagRECT::right = value - tagRECT::left;
		};

		/**
		 * ����Rectangle�\���̂̍������x���W���擾�܂��͐ݒ肵�܂��B
		 */

		int get_X() {
			return tagRECT::left;
		};
		void set_X(int value) {
			tagRECT::left = value;
		};

		/**
		 * ����Rectangle�\���̂̍������y���W���擾�܂��͐ݒ肵�܂��B
		 */

		int get_Y() {
			return tagRECT::top;
		};
		void set_Y(int value) {
			tagRECT::top = value;
		};

		/**
		 * ����Rectangle���w��̗ʂ����c��܂��܂��B
		 * @param width ���������̖c����
		 * @param height ���������̖c����
		 */

		void Inflate(int width, int height) {
			::InflateRect(this, width, height);
		};
	};

	// regfunc.cpp
	DWORD RegGetValue(HKEY hkeyRoot, LPCTSTR lpSubKey, LPTSTR lpValueName, DWORD dwDefault);
	BOOL RegSetValue(HKEY hkeyRoot, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwData);
}
