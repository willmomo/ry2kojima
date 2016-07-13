#include "../kjmsub/kjmsub.h"

// アプリケーションクラス
class myApp : public kjm::app {
public:
	myApp() {};
	virtual ~myApp() {};

public:
	virtual int run();
};

// 唯一のアプリケーションオブジェクト
myApp theApp;

// アプリケーションの処理
int myApp::run() {
	kjm::_tstring ret = kjm::lm::getWorkgroupName();

	_tcout << _T("ワークグループ名 : ") << ret << _T("\n");

	return 0;
}
