#include "kjmsub.h"

// �v���O�����ɗB��̃A�v���P�[�V�����N���X
kjm::app* g_pApp = NULL;

// �G���g���[�|�C���g
int _tmain(int argc, TCHAR** argv) {
	kjm::util::setLocaleJapan();

	return g_pApp->run();
}

// �A�v���P�[�V�����N���X�̃|�C���^��ۑ�����
kjm::app::app() {
	g_pApp = this;
}

// UNICODE�������Shift JIS������ɕϊ�
std::string kjm::util::convertToAscii(const std::wstring& src) {
	std::vector<char> buf;

	DWORD dwSize = WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
	buf.resize(dwSize);

	WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, &buf[0], buf.size(), NULL, NULL);

	return &buf[0];
}

// Shift JIS�������UNICODE������ɕϊ�
std::wstring kjm::util::convertToUnicode(const std::string& src) {
	std::vector<wchar_t> buf;

	DWORD dwSize = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
	buf.resize(dwSize);

	MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, &buf[0], buf.size());

	return &buf[0];
}

// ���[�N�O���[�v�����擾����
kjm::_tstring kjm::lm::getWorkgroupName() {
	kjm::_tstring result;

	NET_API_STATUS status;
	LPBYTE pByte;

	status = NetWkstaGetInfo(NULL, 100, &pByte);
	if (status == NERR_Success) {
		LPWKSTA_INFO_100 p = (LPWKSTA_INFO_100)pByte;
#if !defined(UNICODE)
		result = kjm::util::convertToAscii((wchar_t*)p->wki100_langroup);
#else
		result = p->wki100_langroup;
#endif
		NetApiBufferFree(pByte);
	}

	return result;
}

