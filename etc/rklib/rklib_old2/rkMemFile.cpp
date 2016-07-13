// rkMemFile.cpp : �������}�b�v�h�t�@�C���ȒP�A�N�Z�X�N���X

#include <assert.h>
#include "rkMemFile.h"

// �������}�b�v�h�t�@�C���̍쐬

BOOL rkMemFile::Create(LPCTSTR lpName, DWORD dwSize, rkMemFileException* e = NULL)
{
	// ���łɃ������}�b�v�h�t�@�C�����Ǘ����̂Ƃ��̓G���[
	if (m_hmmf != NULL) {
		if (e) e->m_error = rkMemFileException::errAlreadyUse | rkMemFileException::customerCodeFlag;
		return FALSE;
	}

	// �������}�b�v�h�t�@�C���̍쐬
	HANDLE hmmf = CreateFileMapping((HANDLE)0xffffffff, NULL, PAGE_READWRITE, 0, dwSize, lpName);
	if (hmmf == NULL) {
		if (e) e->m_error = GetLastError();
		return FALSE;
	}

	// ERROR_ALREADY_EXISTS �̊m�F���K�v�ȂƂ��́A���̃G���[�R�[�h���擾����K�v������B
	if (e) e->m_error = GetLastError();
	m_hmmf = hmmf;

	return TRUE;
}

// �������}�b�v�h�t�@�C�����J��

BOOL rkMemFile::Open(LPCTSTR lpName, rkMemFileException* e = NULL)
{
	// ���łɃ������}�b�v�h�t�@�C�����Ǘ����̂Ƃ��̓G���[
	if (m_hmmf != NULL) {
		if (e) e->m_error = rkMemFileException::errAlreadyUse | rkMemFileException::customerCodeFlag;
		return FALSE;
	}

	// �������}�b�v�h�t�@�C�����J��
	HANDLE hmmf = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, lpName);
	if (hmmf == NULL) {
		if (e) e->m_error = GetLastError();
		return FALSE;
	}

	if (e) e->m_error = GetLastError();	// �ʏ�͐���I���R�[�h
	m_hmmf = hmmf;

	return TRUE;
}

// �������}�b�v�h�t�@�C�������

VOID rkMemFile::Close()
{
	// �������}�b�v�h�t�@�C�����N���[�Y
	if (m_hmmf != NULL) {
		ReleasePointer();	// �G���[�ɂ͂Ȃ�Ȃ�����J��

		CloseHandle(m_hmmf);
		m_hmmf = NULL;
	}
}

// �������}�b�v�h�t�@�C���ւ̃|�C���^�[���擾����

LPVOID rkMemFile::GetPointer()
{
	if (m_hmmf == NULL)	return NULL;	// �������}�b�v�h�t�@�C���������Ă��Ȃ�
	if (m_pt != NULL)	return m_pt;	// �܂��A�����[�X���ĂȂ�

	m_pt = MapViewOfFile(m_hmmf, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	return m_pt;
}

// �������}�b�v�h�t�@�C���ւ̃|�C���^�[���J������

VOID rkMemFile::ReleasePointer()
{
	if (m_hmmf == NULL)	return;			// �������}�b�v�h�t�@�C���������Ă��Ȃ�
	if (m_pt == NULL)	return;			// �|�C���^���擾���Ă��Ȃ�

	UnmapViewOfFile(m_pt);
	m_pt = NULL;
}
