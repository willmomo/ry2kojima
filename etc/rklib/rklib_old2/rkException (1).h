#pragma once

// rklibで使用する例外クラス

class rkException
{
public:
	int m_error;	// エラーコード

public:
	enum CodeFlag { customerCodeFlag = 0x20000000 };

public:
	rkException() : m_error(0) {};
	rkException(int code) : m_error(code) {};
	virtual ~rkException(){};

};
