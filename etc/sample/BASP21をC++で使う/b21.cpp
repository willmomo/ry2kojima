#include <windows.h>
#include <iostream>
#include <conio.h>


#import "basp21.dll" named_guids


using namespace std;


int main() {

	CoInitialize(NULL);

	{
		BASP21Lib::IFTPPtr pFtp;
		HRESULT hr = pFtp.CreateInstance( BASP21Lib::CLSID_FTP );

		if ( SUCCEEDED( hr ) ) {
			long rc;
			
			rc = pFtp->Connect( "ts00", "kojima", "kojima" );
			if ( rc == 0 ) {

				rc = pFtp->GetFile( "/home/b21.bin", "c:\\work\\b21.bin", 0 );
				if ( rc == 1 ) {
					cout << "ファイル取得完了\n";
				} else if ( rc == 0 ) {
					cout << "該当ファイル無しエラー: " << rc << "\n";
				} else {
					cout << "その他のエラー: " << rc << "\n";
				}

				rc = pFtp->Close();
				cout << "Close の戻り値: " << rc << "\n";

			} else if ( 1 <= rc && rc <= 5 ) {
				cout << "FTP エラー: " << rc << "\n";
				cout << ( const char* )pFtp->GetReply() << "\n";
			} else if ( rc >= 1000 ) {
				cout << "WinSock エラー: " << rc << "\n";
			} else if ( rc == -1 ) {
				cout << "ソケットオープンエラー: " << rc << "\n";
			} else if ( rc == -2 ) {
				cout << "タイムアウト: " << rc << "\n";
			} else {
				cout << "unknown error: " << rc << "\n";
			}
		}
	}

	CoUninitialize();

	_getch();

	return 0;
}
