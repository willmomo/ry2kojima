
#include "KjmUtil.h"


// �}���`�o�C�g����������C�h������ɕϊ�
//
std::wstring KjmUtil::StrToWStr(const char* str) {
	// �|�C���^��NULL�̂Ƃ��́A�󕶎����Ԃ�
	if (str == NULL) {
		return std::wstring();
	}

	// �K�v�ȕ��������v�Z
	size_t size = mbstowcs(NULL, str, strlen(str));

	// �o�b�t�@���m��
	std::vector<wchar_t> buf(size);

	// �ϊ�
	mbstowcs(&buf[0], str, strlen(str));

	// �ϊ����ʂ�wstring�ɂ��ĕԂ�
	return std::wstring(buf.begin(), buf.end());
}


// ���C�h��������}���`�o�C�g������ɕϊ�
//
std::string KjmUtil::WStrToStr(const wchar_t* wstr) {
	// �|�C���^��NULL�̂Ƃ��́A�󕶎����Ԃ�
	if (wstr == NULL) {
		return std::string();
	}

	// �K�v�ȕ��������v�Z
	size_t size = wcstombs(NULL, wstr, wcslen(wstr));

	// �o�b�t�@���m��
	std::vector<char> buf(size);

	// �ϊ�
	wcstombs(&buf[0], wstr, wcslen(wstr));

	// �ϊ����ʂ�wstring�ɂ��ĕԂ�
	return std::string(buf.begin(), buf.end());
}
