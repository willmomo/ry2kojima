//---------------------------------------------------------------------
// persistent-routes����ʂɏo�͂���v���O����
//
// HKLM\SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\PersistentRoutes
// �̒l��񋓂���ʂɏo�͂���B
//
// �ۑ������t�@�C�����ȉ���BAT���������邱�ƂŁA�ʊ��Ƀ��[�g��ǉ����邱�Ƃł���
/*
for /f "delims=, tokens=1,2,3" %%i in (persistent-routes.txt) do (
	if "%%i" == "0.0.0.0" (
		echo skip default gateway
	) else (
		route add %%i mask %%j %%k -p
	)
)
*/
//---------------------------------------------------------------------
#include "kjmlib_20130805/kjmlib.h"
#include "resource.h"

//---------------------------------------------------------------------
// ���C�������X�^�[�g
//---------------------------------------------------------------------
void doMain(FILE* fp) {
	kjm::registry reg;

	LONG ret = reg.reg_open_key_ex(HKEY_LOCAL_MACHINE,
		_T("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\PersistentRoutes"),
		KEY_READ);

	if (ret == ERROR_SUCCESS) {
		kjm::_tstring valueName;
		DWORD type = 0;
		std::vector<BYTE> data;
		for (int dwIndex = 0; reg.reg_enum_value(dwIndex, valueName, type, data) == ERROR_SUCCESS; dwIndex++) {
			_ftprintf(fp, _T("%s\n"), valueName.c_str());
		}

		reg.reg_close_key();
	} else {
		_tprintf(_T("kjm::registory::reg_open_key_ex: error %d\n"), ret);
	}
}

//---------------------------------------------------------------------
// �v���O�����X�^�[�g
//---------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	
	FILE* fp = stdout;
	if (argc >= 2) {
		fp = _tfopen(argv[1], _T("w"));
	}

	doMain(fp);

	if (fp != NULL && fp != stdout) {
		fclose(fp);
	}

	return 0;
}
