#if !defined(__SBMEMORY_H)
#define __SBMEMORY_H

/////////////////////////////////////////////////////////////////////
// �������������N���X

class SbMemory
{
public:
	void* m_pv;

public:
	// �R���X�g���N�^/�f�X�g���N�^
	SbMemory() {
		m_pv = NULL;
	};
	SbMemory(int nSize) {
		m_pv = new char [nSize];
	};
	virtual ~SbMemory() {
		if (m_pv)
			delete[] m_pv;
	};

	// ����
	void* operator* () {
		return m_pv;
	};
};

#endif
