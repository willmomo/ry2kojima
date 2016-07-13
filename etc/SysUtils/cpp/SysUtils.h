#if !defined(__SYSUTILS_H__)
#define __SYSUTILS_H__

// �K�v�ȃw�b�_���C���N���[�h

#include <string>

// �������@�\���E�B���h�E�ɒǉ�����Ƃ��̒萔

#define WS_EX_LAYERED	0x80000

// �v���g�^�C�v�錾

bool IsLayered(HWND hwnd);
int RemoveSubFolders(const char *strFolder, const char *strExcept);
int SetLayered(HWND hWnd, bool bLayered);
int SetOpacity(HWND hwnd, double dOpacity);
std::string ReplaceStr(const char* pszSrc, const char* pszFind, const char* pszReplaceWith, int nStart = 1, int nCount = -1);
std::wstring StrToWStr(LPCSTR pszSrc);
std::string WStrToStr(LPCWSTR pwszSrc);

#endif
