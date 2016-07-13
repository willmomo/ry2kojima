#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <sys/utime.h>


using namespace std;


int main(int argc, char** argv) {

	struct _stat buffer;

	int ret = _stat(argv[1], &buffer);
	if (ret == 0) {
		cout << argv[1] << " - " << ctime(&buffer.st_mtime);
	} else {
		cout << argv[1] << " �̏�񂪎擾�ł��܂���B\n";
	}

	if (argc == 3) {
		if (_stricmp(argv[2], "now") == 0) {	// ���ݎ��Ԃɐݒ肷��
			ret = _utime(argv[1], NULL);
		} else {
		}

		if (ret == 0) {
			int ret = _stat(argv[1], &buffer);
			if (ret == 0) {
				cout << argv[1] << " - " << ctime(&buffer.st_mtime);
			} else {
				cout << argv[1] << " �̏�񂪎擾�ł��܂���B\n";
			}
		} else {
			switch (errno) {
			case EACCES:
				cerr << "�p�X���Ƀf�B���N�g���܂��͓ǂݏo����p�t�@�C�����w�肵���B\n";
				break;
			case EINVAL:
				cerr << "�����������B\n";
				break;
			case EMFILE:
				cerr << "�J���Ă���t�@�C������������ (�t�@�C�����J���Ȃ��ƍX�V������ύX�ł��܂���)�B\n";
				break;
			case ENOENT:
				cerr << "�t�@�C�����܂��̓p�X����������Ȃ��B\n";
				break;
			default:
				cerr << "���m�̃G���[(errno:" << errno << ")\n";
				break;
			}
		}
	}

	return 0;
}
