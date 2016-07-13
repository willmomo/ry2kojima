#include "rklib.h"



//=============================================================================
// �f�B���N�g����T�u�f�B���N�g����ʂ��āA�쐬�A�ړ��A����ї񋓂��邽�߂�
// �ÓI���\�b�h�����J���܂��B
//=============================================================================
//-----------------------------------------------------------------------------
// �����̃t�H���_��V�����t�H���_�ɃR�s�[���܂��B
//-----------------------------------------------------------------------------
void rk::Directory::Copy(const rk::string& sourceDirName, const rk::string& destDirName, bool overwrite/*=false*/) {

	rk::Directory::CreateFolder(destDirName);

	strings files = rk::Directory::GetFiles(sourceDirName);

	for (strings::size_type i = 0; i < files.size(); i++) {
		rk::File::Copy(files[i], rk::Path::Combine(destDirName, rk::Path::GetFileName(files[i])), overwrite);
	}

	strings dirs = rk::Directory::GetDirectories(sourceDirName);

	for (strings::size_type i = 0; i < dirs.size(); i++) {
		rk::Directory::Copy(dirs[i], rk::Path::Combine(destDirName, rk::Path::GetFileName(dirs[i])), overwrite);
	}
}

//-----------------------------------------------------------------------------
// path �Ŏw�肵�����ׂẴf�B���N�g���ƃT�u�f�B���N�g�����쐬���܂��B
// �֐����� CreateDirectory ���� API �Ƃ��Ԃ�̂ŁACreateFolder ��I���B
//-----------------------------------------------------------------------------
rk::DirectoryInfo rk::Directory::CreateFolder(const rk::string& path) {
	if (rk::Directory::Exists(path) == false) {
		rk::Directory::CreateFolder(rk::Path::GetDirectoryName(path));

		::SetLastError(0);
		BOOL bRet = ::CreateDirectory(path.c_str(), NULL);
		if (bRet == FALSE) {
			DWORD dwErr = ::GetLastError();
			switch (dwErr) {
			case ERROR_ACCESS_DENIED:
				throw rk::UnauthorizedAccessException(dwErr, rk::SystemException::mkmsg(dwErr, path));

			default:
				throw rk::SystemException(dwErr, rk::SystemException::mkmsg(dwErr, path));
			}
		}
	}

	return rk::DirectoryInfo(path);
}

//-----------------------------------------------------------------------------
// �w�肵���p�X���f�B�X�N��̊����̃f�B���N�g�����Q�Ƃ��Ă��邩�ǂ������m�F���܂��B
//-----------------------------------------------------------------------------
bool rk::Directory::Exists(const rk::string& path) {
	return (::PathFileExists(path.c_str()) != FALSE) ? true : false;
}

//-----------------------------------------------------------------------------
// �w�肵���f�B���N�g�����̃T�u�f�B���N�g���̖��O���擾���܂��B
//-----------------------------------------------------------------------------
rk::strings rk::Directory::GetDirectories(const rk::string& path) {
	rk::strings result;

	WIN32_FIND_DATA data;
	HANDLE hFind = ::FindFirstFile(rk::Path::Combine(path, _T("*.*")).c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if ((::lstrcmp(data.cFileName, _T(".")) != 0) && (::lstrcmp(data.cFileName, _T("..")) != 0)) {
					result.push_back(rk::Path::Combine(path, data.cFileName));
				}
			}
		} while (::FindNextFile(hFind, &data));

		::FindClose(hFind);
	}

	return result;
}

//-----------------------------------------------------------------------------
// �w�肵���f�B���N�g�����̃t�@�C������Ԃ��܂��B
//-----------------------------------------------------------------------------
rk::strings rk::Directory::GetFiles(const rk::string& path) {
	rk::strings result;

	WIN32_FIND_DATA data;
	HANDLE hFind = ::FindFirstFile(rk::Path::Combine(path, _T("*.*")).c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
				result.push_back(rk::Path::Combine(path, data.cFileName));
			}
		} while (::FindNextFile(hFind, &data));

		::FindClose(hFind);
	}

	return result;
}


//=============================================================================
// �t�@�C���̍쐬�A�R�s�[�A�폜�A�ړ��A�I�[�v���̂��߂̐ÓI���\�b�h��񋟂�
// �܂��B
//=============================================================================
//-----------------------------------------------------------------------------
// �����̃t�@�C����V�����t�@�C���ɃR�s�[���܂��B
//-----------------------------------------------------------------------------
void rk::File::Copy(const rk::string& sourceFileName, const rk::string& destFileName, bool overwrite/*=false*/) {

	::SetLastError(0);
	BOOL bRet = ::CopyFile(sourceFileName.c_str(), destFileName.c_str(), (overwrite) ? FALSE : TRUE);
	DWORD dwErr = ::GetLastError();

	_tout << sourceFileName << _T(" �� ") << destFileName << _T(" : ") << bRet << _T(" : ") << dwErr << _T("\n");

	if (bRet == FALSE) {
		switch (dwErr) {
		case ERROR_ACCESS_DENIED:
			throw rk::UnauthorizedAccessException(dwErr, rk::SystemException::mkmsg(dwErr, destFileName));

		case ERROR_SHARING_VIOLATION:
			throw rk::IOException(dwErr, rk::SystemException::mkmsg(dwErr, sourceFileName));

		case ERROR_FILE_EXISTS:
			throw rk::IOException(dwErr, rk::SystemException::mkmsg(dwErr, destFileName));

		default:
			throw rk::SystemException(dwErr, rk::SystemException::mkmsg(dwErr));
		}
	}
}


//=============================================================================
// �t�@�C���܂��̓f�B���N�g���̃p�X�����i�[���� string �C���X�^���X�ő���
// �����s���܂��B�����̑���́A�v���b�g�t�H�[���ԂŎ��s����܂��B
//=============================================================================
//-----------------------------------------------------------------------------
// 2 �̃p�X��������������܂��B
//-----------------------------------------------------------------------------
rk::string rk::Path::Combine(const rk::string& path1, const rk::string& path2) {
	TCHAR work[MAX_PATH];

	::PathAppend(::lstrcpy(work, path1.c_str()), path2.c_str());

	return work;
}

//-----------------------------------------------------------------------------
// �w�肵���p�X������̃f�B���N�g������Ԃ��܂��B
//-----------------------------------------------------------------------------
rk::string rk::Path::GetDirectoryName(const rk::string& path) {
	TCHAR work[MAX_PATH];

	::PathRemoveFileSpec(::lstrcpy(work, path.c_str()));

	return work;
}

//-----------------------------------------------------------------------------
// �w�肵���p�X������̃t�@�C�����Ɗg���q��Ԃ��܂��B
//-----------------------------------------------------------------------------
rk::string rk::Path::GetFileName(const rk::string& path) {
	return ::PathFindFileName(path.c_str());
}


//=============================================================================
// ��O�̊�{�N���X���`���܂��B
//=============================================================================
//-----------------------------------------------------------------------------
// �G���[�R�[�h�ƃp�����[�^�����ʓI�ȃG���[���b�Z�[�W���쐬����B
//-----------------------------------------------------------------------------
rk::string rk::SystemException::mkmsg(DWORD dwErr, const rk::string& p1/*=_T("")*/) {
	rk::_tostringstream oss;

	switch (dwErr) {
	case ERROR_ACCESS_DENIED:
		oss << _T("�p�X '") << p1 << _T("' �ւ̃A�N�Z�X�����ۂ���܂����B");
		break;

	case ERROR_SHARING_VIOLATION:
		oss << _T("�ʂ̃v���Z�X�Ŏg�p����Ă��邽�߁A�v���Z�X�̓t�@�C�� '") << p1 << _T("' �ɃA�N�Z�X�ł��܂���B");
		break;

	case ERROR_FILE_EXISTS:
		oss << _T("�t�@�C�� '") << p1 << _T("' �͊��ɑ��݂��܂��B");
		break;

	default:
		oss << _T("");
		break;
	}

	return oss.str();
}


int _tmain(int argc, TCHAR** argv) {
	std::locale::global(std::locale("japanese"));

	try {
		rk::Directory::Copy(argv[1], argv[2]);
	} catch (rk::IOException& e) {
		_tout << _T("Code: ") << e.get_Code() << _T("\n");
		_tout << _T("Msg : ") << e.get_Message() << _T("\n");
	} catch (rk::UnauthorizedAccessException& e) {
		_tout << _T("Code: ") << e.get_Code() << _T("\n");
		_tout << _T("Msg : ") << e.get_Message() << _T("\n");
	}

	return 0;
}
