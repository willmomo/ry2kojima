//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
#pragma once

#include "./kjmlib-20130318/kjmlib.h"

class patcherIni : public kjm::iniFile {
public:
	patcherIni() {}
	patcherIni(const _tstring& iniFile) : kjm::iniFile(iniFile) {}

	virtual ~patcherIni() {}

public:
	// Startup �Z�N�V�����ɃG���g���[���u�ǉ��v����
	void appendStartupSection(const _tstring& title, const _tstring& commandLine, const _tstring& currentDirectory);

	// �^�C�g������ Startup �Z�N�V������T��
	int findStartupSection(const _tstring& title) const;

	// �ő�� startup �Z�N�V�������擾
	int getMaxStartupSection() const;

private:
	// �R�s�[�Ƒ�����u���b�N
	patcherIni(const patcherIni& src) {}
	patcherIni& operator=(const patcherIni& rhs) {}
};
