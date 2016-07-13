#if !defined(__SBMEMORY_H)
#define __SBMEMORY_H

/////////////////////////////////////////////////////////////////////
// メモリを扱うクラス

class SbMemory
{
public:
	void* m_pv;

public:
	// コンストラクタ/デストラクタ
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

	// 操作
	void* operator* () {
		return m_pv;
	};
};

#endif
