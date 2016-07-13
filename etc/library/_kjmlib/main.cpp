// �e�X�g�p���C�����[�`��
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

	// unlock�����ɃX���b�h�I��
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
			std::cout << "����\n";
		} else {
			std::cout << "���s\n";
		}

		lockfile.unlock();

		lf2.lock();
	}

	kjm::mutex mtx;

	// mutex�̍쐬�e�X�g
	std::cout << "mutex�̍쐬�e�X�g\t";
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

	// mutex�̓�d�쐬�e�X�g
	std::cout << "mutex�̓�d�쐬�e�X�g\t";
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

	// ����mutex�쐬�e�X�g
	std::cout << "����mutex�̍쐬�e�X�g\t";
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

	// ���Ă���̍č쐬�e�X�g
	std::cout << "���Ă���̍č쐬�e�X�g\t";
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

	// ��d�N���[�Y�̃e�X�g
	std::cout << "��d�N���[�Y�̃e�X�g\t";
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

	// mtuex�����e�X�g(�R���p�C���G���[�ɂȂ�΂悵)
	// kjm::mutex copy_mtx = same_mtx;
	// copy_mtx = mtx;

	// lock�e�X�g
	std::cout << "lock�e�X�g\t";
	try
	{
		unsigned threadId = 0;
		HANDLE hThread;
		
		hThread = (HANDLE)_beginthreadex(NULL, 0, lock5sec, NULL, 0, &threadId);

		// 1sec �܂��āA�X���b�h�Ƀ��b�N������
		DWORD dw = GetTickCount();
		while ((GetTickCount() - dw) < 1000)
			Sleep(0);

		if (mtx.lock(500) == WAIT_TIMEOUT) {
			std::cout << "[ok]";	// �^�C���A�E�g�ɂȂ�΂悵
		} else {
			assert(0);
		}
		mtx.unlock();

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);

		if (mtx.lock(500) == WAIT_OBJECT_0) {
			std::cout << "[ok]\n";	// ���b�N�ł���΂悵
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
	std::cout << "destructer �e�X�g\t";
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

	// ���݂��Ȃ�mmf���J���e�X�g
	std::cout << "���݂��Ȃ�mmf���J���e�X�g\t";
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

	// memfile�̍쐬�e�X�g
	std::cout << "memfile�̍쐬�e�X�g\t";
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

	// ���݂���mmf���J���e�X�g
	std::cout << "���݂��Ȃ�mmf���J���e�X�g\t";
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

	// memfile�̓�d�쐬�e�X�g
	std::cout << "memfile�̓�d�쐬�e�X�g\t";
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

	// ����memfile�쐬�e�X�g
	std::cout << "����memfile�̍쐬�e�X�g\t";
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

	// ���Ă���̍č쐬�e�X�g
	std::cout << "���Ă���̍č쐬�e�X�g\t";
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

	// ��d�N���[�Y�̃e�X�g
	std::cout << "��d�N���[�Y�̃e�X�g\t";
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

	// mtuex�����e�X�g(�R���p�C���G���[�ɂȂ�΂悵)
	// kjm::memfile<MYDATA> copy_mmf = same_mmf;
	// copy_mmf = mtx;

	// �������݃e�X�g
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

	// �ǂݍ��݃e�X�g
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
