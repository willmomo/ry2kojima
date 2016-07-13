//---------------------------------------------------------------------
// �����N�Ɉȉ��̃t�@�C����ǉ����܂�
// netapi32.lib shlwapi.lib
//---------------------------------------------------------------------

#include <windows.h>
#include <lm.h>
#include <shlwapi.h>
#include <stdio.h>
#include <string>

//---------------------------------------------------------------------
// [�T�v]
// ASCII�������Unicode������ɕϊ�
//
// [����]
// pszSrc	�ϊ���ASCII������ւ̃|�C���^
//
// [�߂�l]
// �ϊ����Unicode������ւ̃|�C���^�B�ϊ��ł��Ȃ��ꍇ�́A�󕶎����Ԃ��B
//---------------------------------------------------------------------

std::wstring StrToWStr(LPCSTR pszSrc)
{
	std::wstring wstr;

	// �ϊ��ɕK�v�ȃo�b�t�@�T�C�Y(������(\0�܂�))���擾
	int nSizeNeeded = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nSizeNeeded == 0) {
		return wstr;
	}

	// �o�b�t�@���m��
	LPWSTR pwsz = (LPWSTR)malloc(sizeof(WCHAR) * nSizeNeeded);
	if (pwsz == NULL) {
		return wstr;
	}

	// �ϊ�
	int ret = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwsz, nSizeNeeded);
	if (ret == 0) {
		free(pwsz);
		return wstr;
	}

	wstr = pwsz;
	free(pwsz);

	return wstr;
}

//---------------------------------------------------------------------
// [�T�v]
// Unicode�������ASCII������ɕϊ�
//
// [����]
// pwszSrc	�ϊ���Unicode������ւ̃|�C���^
//
// [�߂�l]
// �ϊ����ASCII������ւ̃|�C���^�B�ϊ��ł��Ȃ��ꍇ�́A�󕶎����Ԃ��B
//---------------------------------------------------------------------

std::string WStrToStr(LPCWSTR pwszSrc)
{
	std::string str;

	// �ϊ��ɕK�v�ȃo�b�t�@�T�C�Y(������(\0�܂�))���擾
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0) {
		return str;
	}

	// �o�b�t�@���m��
	LPSTR psz = (LPSTR)malloc(sizeof(CHAR) * nSizeNeeded);
	if (psz == NULL) {
		return str;
	}

	// �ϊ�
	int ret = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		free(psz);
		return str;
	}

	str = psz;
	free(psz);

	return str;
}

//---------------------------------------------------------------------
// [�T�v]
// �w��t�H���_���쐬����
//---------------------------------------------------------------------

int CreateFolder(const char* pszFolder)
{
	// ���[�g�t�H���_�́A�K�����݂���(�h���C�u����������������...)
	if (PathIsRoot(pszFolder)) {
		return 0;

	// �w�肳�ꂽ�t�H���_�͑��݂���?
	} else if (PathFileExists(pszFolder)) {
		return 0;

	} else {
		char work[MAX_PATH];
		lstrcpy(work, pszFolder);
		PathAppend(work, "..");

		// �e�t�H���_��(�������)�쐬����
		if (CreateFolder(work) == 0) {
			if (!CreateDirectory(pszFolder, NULL)) {
				return -1;
			}
		} else {
			return -1;
		}
	}

	return 0;
}

//---------------------------------------------------------------------
// [�T�v]
// �w��̋��L�t�H���_�Ƌ��L�p�X�̑��݂�ۏ؂���
//---------------------------------------------------------------------

int GuaranteeSharedFolder(const char* pszSharedFolder, const char* pszSharedPath)
{
	int nRet = 0;

	// ���L�t�H���_����Unicode�ɕϊ�
	std::wstring wstrSharedFolder = StrToWStr(pszSharedFolder);
	if (wstrSharedFolder.empty()) {
		// TODO: ���L�t�H���_�̎w�肪�Ԉ���Ă���Ƃ��̏���

		return -1;
	}

	// ���L�p�X����Unicode�ɕϊ�
	std::wstring wstrSharedPath = StrToWStr(pszSharedPath);
	if (wstrSharedPath.empty()) {
		// TODO: ���L�p�X�̎w�肪�Ԉ���Ă���Ƃ��̏���

		return -1;
	}

	SHARE_INFO_2* pInfo;
	NET_API_STATUS ret;

	// ���[�J���ɗL��A�w��̋��L�t�H���_�����擾
	ret = NetShareGetInfo(NULL, (LPTSTR)wstrSharedFolder.c_str(), 2, (LPBYTE*)&pInfo);
	if (ret == NERR_Success) {
	
		// ���L�t�H���_���w��̋��L�p�X���w���Ă��邩�m�F����
		if (lstrcmpi(WStrToStr((LPCWSTR)pInfo->shi2_path).c_str(), pszSharedPath) != 0) {
			// TODO: ���L�t�H���_�����L�p�X�ƈႤ���Ƃ��x��
		}

		NetApiBufferFree(pInfo);

		nRet = 0;	// ����I��
	} else if (ret == NERR_NetNameNotFound) {
		// ���L�t�H���_�����݂��Ȃ��Ƃ��̏���

		// �w��̋��L�p�X��(�������)�쐬����
		if (CreateFolder(pszSharedPath) < 0) {
			// TODO: ���L�p�X���쐬�ł��Ȃ����̏���

			return -1;
		}

		SHARE_INFO_2 info;
		info.shi2_netname = (LPTSTR)wstrSharedFolder.c_str();
		info.shi2_type = STYPE_DISKTREE;
		info.shi2_remark = (LPTSTR)L"";
		info.shi2_permissions = ACCESS_ALL;
		info.shi2_max_uses = -1;
		info.shi2_current_uses = 0;
		info.shi2_path = (LPTSTR)wstrSharedPath.c_str();
		info.shi2_passwd = (LPTSTR)L"";

		DWORD parm_err = 0;
		ret = NetShareAdd(NULL, 2, (LPBYTE)&info, &parm_err);
		if (ret == NERR_Success) {
			nRet = 0;	// ����I��
		} else {
			// TODO: ���L�t�H���_���쐬�ł��Ȃ������Ƃ��̏���

			nRet = -1;	// �ُ�I��
		}
	} else {
		// TODO: ���L�t�H���_�̏�񂪎擾�ł��Ȃ����̏���
		
		nRet = -1;	// �ُ�I��
	}

	return nRet;
}

int main(int argc, char** argv)
{
	if (argc != 3) {
		printf("GuaranteeSharedFolder <���L�t�H���_> <���L�p�X>\n");
		return -1;
	}

	GuaranteeSharedFolder(argv[1], argv[2]);

	return 0;
}
