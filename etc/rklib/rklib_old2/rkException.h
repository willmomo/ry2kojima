#pragma once

// rklib�Ŏg�p�����O�N���X

class rkException
{
public:
	int m_error;	// �G���[�R�[�h

public:
	enum CodeFlag { customerCodeFlag = 0x20000000 };

public:
	rkException() : m_error(0) {};
	rkException(int code) : m_error(code) {};
	virtual ~rkException(){};

};
