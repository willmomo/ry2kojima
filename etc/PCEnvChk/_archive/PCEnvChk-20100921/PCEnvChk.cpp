#include "../kjmsub/kjmsub.h"

// �A�v���P�[�V�����N���X
class myApp : public kjm::app {
public:
	myApp() {};
	virtual ~myApp() {};

public:
	virtual int run();
};

// �B��̃A�v���P�[�V�����I�u�W�F�N�g
myApp theApp;

// �A�v���P�[�V�����̏���
int myApp::run() {
	kjm::_tstring ret = kjm::lm::getWorkgroupName();

	_tcout << _T("���[�N�O���[�v�� : ") << ret << _T("\n");

	return 0;
}
