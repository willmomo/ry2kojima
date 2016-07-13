// rkMemFile.h : �������}�b�v�h�t�@�C���ȒP�Ǘ��N���X

#include <windows.h>
#include "rkException.h"

// rkMemFile �Ŏg�p�����O�N���X

class rkMemFileException : public rkException
{
public:
	enum ErrorCode { errAlreadyUse = 1 };

public:
	rkMemFileException() {};
	rkMemFileException(int code) : rkException(code) {};
	virtual ~rkMemFileException() {};
};

// �������}�b�v�h�t�@�C���̊Ǘ��Ǘ��N���X

class rkMemFile
{
private:
	HANDLE m_hmmf;	// �������}�b�v�h�t�@�C���ւ̃n���h��
	LPVOID m_pt;	// �������}�b�v�h�t�@�C���ւ̃|�C���^

public:

public:
	rkMemFile() : m_hmmf(NULL), m_pt(NULL) {};
	rkMemFile(LPCTSTR lpName, DWORD dwSize) : m_hmmf(NULL), m_pt(NULL)
	{
		rkMemFileException e;
		if (Create(lpName, dwSize, &e) == FALSE)
			throw e;
	};
	rkMemFile(LPCTSTR lpName) : m_hmmf(NULL), m_pt(NULL)
	{
		rkMemFileException e;
		if (Open(lpName, &e) == FALSE)
			throw e;
	};
	virtual ~rkMemFile()
	{
		Close();
	};

public:
	BOOL Create(LPCTSTR lpName, DWORD dwSize, rkMemFileException* e);	// �������}�b�v�h�t�@�C���̍쐬
	BOOL Open(LPCTSTR lpName, rkMemFileException* e);	// �������}�b�v�h�t�@�C�����J��
	VOID Close();	// �������}�b�v�h�t�@�C�������

	LPVOID GetPointer();	// �|�C���^���擾����
	VOID ReleasePointer();	// �|�C���^���J������
};
