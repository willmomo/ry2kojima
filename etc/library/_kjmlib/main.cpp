// テスト用メインルーチン
//

#include <iostream>
#include <cassert>
#include <process.h>
#include "kjmlib.h"

typedef struct {
	long a, b, c;
	double d;
} MYDATA;

unsigned __stdcall abandoned_thread(void* p)
{
	kjm::mutex mtx;

	mtx.create("my mutex");
	mtx.lock(500);
	mtx.lock(500);

	// unlockせずにスレッド終了
	return 0;
}

unsigned __stdcall lock5sec(void* p)
{
	DWORD dw;
	kjm::mutex mtx;
	
	mtx.create("my mutex");

	if (mtx.lock(500) != WAIT_OBJECT_0) {
		assert(0);
	}

	dw = GetTickCount();
	while ((GetTickCount() - dw) < 5000)
		Sleep(0);

	mtx.unlock();
	mtx.close();

	return 0;
}

int main(int argc, char** argv)
{
	kjm::lockfile lockfile("D:\\hoge.txt");
	
	if (lockfile.lock()) {
		lockfile.lock();

		kjm::lockfile lf2("D:\\hoge.txt");
		if (lf2.lock()) {
			std::cout << "成功\n";
		} else {
			std::cout << "失敗\n";
		}

		lockfile.unlock();

		lf2.lock();
	}

	kjm::mutex mtx;

	// mutexの作成テスト
	std::cout << "mutexの作成テスト\t";
	try
	{
		if (mtx.create("my mutex")) {
			std::cout << "[OK]\n";
		} else {
			assert(0);
		}
	}
	catch(kjm::mutex_error)
	{
		assert(0);
	}

	// mutexの二重作成テスト
	std::cout << "mutexの二重作成テスト\t";
	try
	{
		if (mtx.create("my mutex")) {
			assert(0);
		} else {
			assert(0);
		}
	}
	catch(kjm::mutex_error e)
	{
		if (e.error == kjm::mutex_error::already_initialized) {
			std::cout << "[OK]\n";
		} else {
			assert(0);
		}
	}

	// 同名mutex作成テスト
	std::cout << "同名mutexの作成テスト\t";
	kjm::mutex same_mtx;
	try
	{
		if (same_mtx.create("my mutex")) {
			if (same_mtx.is_exists()) {
				std::cout << "[ok]\n";
			} else {
				assert(0);
			}
		}
	}
	catch(kjm::mutex_error)
	{
		assert(0);
	}

	// 閉じてからの再作成テスト
	std::cout << "閉じてからの再作成テスト\t";
	try
	{
		same_mtx.close();
		if (same_mtx.is_open()) {
			assert(0);
		}

		if (same_mtx.create("my mutex 2nd")) {
			std::cout << "[ok]\n";
		} else {
			assert(0);
		}
	}
	catch(kjm::mutex_error)
	{
		assert(0);
	}

	// 二重クローズのテスト
	std::cout << "二重クローズのテスト\t";
	try
	{
		same_mtx.close();
		if (same_mtx.is_open()) {
			assert(0);
		}

		same_mtx.close();
		if (same_mtx.is_open()) {
			assert(0);
		}

		std::cout << "[ok]\n";
	}
	catch(kjm::mutex_error)
	{
		assert(0);
	}

	// mtuex複製テスト(コンパイルエラーになればよし)
	// kjm::mutex copy_mtx = same_mtx;
	// copy_mtx = mtx;

	// lockテスト
	std::cout << "lockテスト\t";
	try
	{
		unsigned threadId = 0;
		HANDLE hThread;
		
		hThread = (HANDLE)_beginthreadex(NULL, 0, lock5sec, NULL, 0, &threadId);

		// 1sec まって、スレッドにロックさせる
		DWORD dw = GetTickCount();
		while ((GetTickCount() - dw) < 1000)
			Sleep(0);

		if (mtx.lock(500) == WAIT_TIMEOUT) {
			std::cout << "[ok]";	// タイムアウトになればよし
		} else {
			assert(0);
		}
		mtx.unlock();

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);

		if (mtx.lock(500) == WAIT_OBJECT_0) {
			std::cout << "[ok]\n";	// ロックできればよし
		} else {
			assert(0);
		}
		mtx.unlock();
	}
	catch(kjm::mutex_error)
	{
		assert(0);
	}

	// lock destructer test
	std::cout << "destructer テスト\t";
	try
	{
		unsigned thread_id = 0;
		HANDLE hThread;

		hThread = (HANDLE)_beginthreadex(NULL, 0, abandoned_thread, NULL, 0, &thread_id);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);

		switch (mtx.lock(1000)) {
		case WAIT_OBJECT_0:
			std::cout << "[ok]\n";
			break;
		default:
			assert(0);
		}
	}
	catch(...)
	{
		assert(0);
	}

	kjm::memfile<MYDATA> mmf;

	// 存在しないmmfを開くテスト
	std::cout << "存在しないmmfを開くテスト\t";
	try
	{
		if (mmf.open("mmfx.bin")) {
			assert(0);
		} else {
			std::cout << "[ok]\n";
		}
	}
	catch(kjm::memfile_error)
	{
		assert(0);
	}

	// memfileの作成テスト
	std::cout << "memfileの作成テスト\t";
	try
	{
		if (mmf.create("mmf.bin", 512)) {
			std::cout << "[OK]\n";
		} else {
			assert(0);
		}
	}
	catch(kjm::memfile_error)
	{
		assert(0);
	}

	// 存在するmmfを開くテスト
	std::cout << "存在しないmmfを開くテスト\t";
	try
	{
		kjm::memfile<MYDATA> mmf2;
		if (mmf2.open("mmfx.bin")) {
			assert(0);
		} else {
			std::cout << "[ok]\n";
		}
	}
	catch(kjm::memfile_error)
	{
		assert(0);
	}

	// memfileの二重作成テスト
	std::cout << "memfileの二重作成テスト\t";
	try
	{
		if (mmf.create("mmf.bin", 1024)) {
			assert(0);
		} else {
			assert(0);
		}
	}
	catch(kjm::memfile_error e)
	{
		if (e.error == kjm::memfile_error::already_initialized) {
			std::cout << "[OK]\n";
		} else {
			assert(0);
		}
	}

	// 同名memfile作成テスト
	std::cout << "同名memfileの作成テスト\t";
	kjm::memfile<MYDATA> same_mmf;
	try
	{
		if (same_mmf.create("mmf.bin", 128)) {
			if (same_mmf.is_exists()) {
				std::cout << "[ok]\n";
			} else {
				assert(0);
			}
		}
	}
	catch(kjm::memfile_error)
	{
		assert(0);
	}

	// 閉じてからの再作成テスト
	std::cout << "閉じてからの再作成テスト\t";
	try
	{
		same_mmf.close();
		if (same_mmf.is_open()) {
			assert(0);
		}

		if (same_mmf.create("my memfile 2nd", 512)) {
			std::cout << "[ok]\n";
		} else {
			assert(0);
		}
	}
	catch(kjm::memfile_error)
	{
		assert(0);
	}

	// 二重クローズのテスト
	std::cout << "二重クローズのテスト\t";
	try
	{
		same_mmf.close();
		if (same_mmf.is_open()) {
			assert(0);
		}

		same_mmf.close();
		if (same_mmf.is_open()) {
			assert(0);
		}

		std::cout << "[ok]\n";
	}
	catch(kjm::memfile_error)
	{
		assert(0);
	}

	// mtuex複製テスト(コンパイルエラーになればよし)
	// kjm::memfile<MYDATA> copy_mmf = same_mmf;
	// copy_mmf = mtx;

	// 書き込みテスト
	MYDATA data[3] = {{1, 2, 3, 4.5}, {6, 7, 8, 9.10}, {11, 12, 13, 14.15}};

	try
	{
		mmf.write(data, 3, 3, &mtx, 1000);
	}
	catch(kjm::memfile_error)
	{
	}
	catch(kjm::mutex_error)
	{
	}

	// 読み込みテスト
	try
	{
		MYDATA read[2];

		mmf.read(read, 4, 2, &mtx, 1000);
		if (memcmp(&read[0], &data[1], sizeof(MYDATA)) != 0 || memcmp(&read[1], &data[2], sizeof(MYDATA)) != 0) {
			assert(0);
		}
	}
	catch (kjm::memfile_error)
	{
	}
	catch (kjm::mutex_error)
	{
	}

	{
		kjm::memfile<MYDATA> mmf("mmf.bin");
		kjm::mutex mtx("mtx_mmf");

		if (!mmf.is_open() && !mtx.is_open())	return -1;

		try
		{
			MYDATA data[512];

			for (int i = 0; i < 1024; i++) {
				mmf.read(data, 0, 512, &mtx, 1000);
				Sleep(0);
			}
		}
		catch(kjm::memfile_error& e)
		{
			assert(0);
		}
		catch(kjm::mutex_error& e)
		{
			assert(0);
		}
	}

	return 0;
}
