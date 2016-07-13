#include <windows.h>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <tchar.h>
#include "kjm.h"


/* �C�x���g���O�̓ǂݎ�� */
void ReadLog( LPCTSTR lpSourceName )
{
	DWORD i;

	/* �C�x���g���O�̃I�[�v�� */
	kjm::eventlog eventLog;

	if((eventLog.OpenEventLog(NULL, lpSourceName)) == FALSE) {
		printf("event log can not open.\n");
		return;
	}

	kjm::eventlog_record rec;
	while ( eventLog.ReadEventLog( EVENTLOG_FORWARDS_READ | EVENTLOG_SEQUENTIAL_READ, 0, rec ) ) {

		/* �ǂݎ�����C�x���g�̕\�� */
		printf("���R�[�h�ԍ�: %d\n", rec.RecordNumber());
		printf("��������: %s", rec.TimeGenerated().ctime().c_str() );
		printf("�������ݎ���: %s", rec.TimeWritten().ctime().c_str() );
		printf("�C�x���gID: %08x\n", rec.EventID());

		printf("�C�x���g�̎��: ");
		switch(rec.EventType()) {
			case EVENTLOG_SUCCESS: printf("����\n"); break;
			case EVENTLOG_ERROR_TYPE: printf("�G���[\n"); break;
			case EVENTLOG_WARNING_TYPE: printf("�x��\n"); break;
			case EVENTLOG_INFORMATION_TYPE: printf("���\n"); break;
			case EVENTLOG_AUDIT_SUCCESS: printf("�č�����\n"); break;
			case EVENTLOG_AUDIT_FAILURE: printf("�č����s\n"); break;
			default: printf("�s��\n"); break;
		}

		printf("�\�[�X��: %s\n", rec.SourceName().c_str());
		printf("�R���s���[�^��: %s\n", rec.ComputerName().c_str());

		/* �J�e�S���̕\�� */
		printf("�񎟃J�e�S��: %s", rec.EventCategoryText().c_str() );

		/* ���b�Z�[�W�̕\�� */
		printf("���b�Z�[�W: %s", rec.EventIDText().c_str() );

		/* �ŗL�f�[�^�̕\�� */
		std::vector<BYTE> data = rec.Data();

		if (data.size() > 0) {

			printf("�ŗL�f�[�^: ");
			for ( i = 0; i < data.size(); i++) {
				printf("%02x ", data.at( i ));
			}
			printf("\n");
		}

		printf("\n");

	}

}

int _tmain() {

	ReadLog( "Application" );
	ReadLog( "System" );

	return 0;
}