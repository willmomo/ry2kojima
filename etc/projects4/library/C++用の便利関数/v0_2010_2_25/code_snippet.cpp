//---------------------------------------------------------------------
// C++�Ŏg�p�ł���A���[�e�B���e�B�֐�(ver.0.2010.02.25)
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// INI�t�@�C�����炷�ׂẴZ�N�V���������擾����B
//---------------------------------------------------------------------
int GetPrivateProfileSectionNames(std::vector<std::basic_string<TCHAR> >& names, LPCTSTR lpFileName) {
	std::vector<TCHAR> buf(512);
	do {
		if (GetPrivateProfileSectionNames(buf.begin(), buf.size(), lpFileName) != (buf.size() - 2)) {
			break;
		}
		buf.resize(buf.size() * 2);
	} while (1);

	// \0 �ŋ�؂�Ȃ���Anames �ɒǉ����Ă����B
	names.clear();
	for (int i = 0; buf[i] != '\0'; i += (lstrlen(&buf[i]) + 1)) {
		names.push_back(&buf[i]);
	}

	return names.size();
}


//---------------------------------------------------------------------
// INI�t�@�C���̒l���擾����B
// �f�t�H���g�l���̗p�������ǂ����̏����擾�ł���B
//---------------------------------------------------------------------
std::basic_string<TCHAR> GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPCTSTR lpFileName, bool *pfUseDefault=NULL) {
	std::basic_string<TCHAR> pre_default = std::basic_string<TCHAR>(_T("pre_")) + lpDefault;

	std::vector<TCHAR> pre_buf(max(32, pre_default.length() * 2));
	do {
		if (GetPrivateProfileString(lpAppName, lpKeyName, pre_default.c_str(), pre_buf.begin(), pre_buf.size(), lpFileName) != (pre_buf.size() - 1)) {
			break;
		}
		pre_buf.resize(pre_buf.size() * 2);
	} while (1);

	std::vector<TCHAR> buf(pre_buf.size());
	do {
		if (GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, buf.begin(), buf.size(), lpFileName) != (buf.size() - 1)) {
			break;
		}
		buf.resize(buf.size() * 2);
	} while (1);

	if (pfUseDefault) {
		*pfUseDefault = (lstrcmp(pre_buf.begin(), buf.begin()) != 0);
	}

	return buf.begin();
}


/**
	@brief	�V�X�e���W���̃G���[���b�Z�[�W�����
	@param error_code	GetLastError �̖߂�l
	@result	�G���[�������Ԃ��B�G���[���b�Z�[�W���擾�ł��Ȃ��Ƃ��́A�󕶎����Ԃ��B
*/
std::string format_message(DWORD error_code) {
	std::string result;
	char* pBuffer = NULL;

	DWORD ret = ::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		error_code,
		0,
		(LPTSTR)&pBuffer,
		0,
		NULL);

	// �֐��̖߂�l�͌����ɁA�|�C���^�̗L�����݂̂Ŕ��f����B
	if (pBuffer) {
		result = pBuffer;
		::LocalFree(pBuffer);
	}
	return result;
}
</pre>
<hr>
<h1>get_computer_name�֐�</h1>
<pre>
/**
	@brief	�R���s���[�^����std::string�ŕԂ�
	@return	�R���s���[�^����Ԃ��B�֐��Ɏ��s�����Ƃ��́A�󕶎����Ԃ��B
*/

std::string get_computer_name() {
	char buffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD nSize = sizeof(buffer);
	BOOL ret = ::GetComputerName(buffer, &nSize);
	return (ret) ? std::string(buffer) : std::string();
}
