//---------------------------------------------------------------------
// toolbox ���Ŕėp�I�ȏ������܂Ƃ߂�
//---------------------------------------------------------------------


#include "toolbox.h"


//---------------------------------------------------------------------
// �w��t�@�C���̃o�b�N�A�b�v���ꐢ�㕪�c���B
//
// �w�肳�ꂽ�t�@�C���̊g���q�� 000 �ɂ������̂��A�o�b�N�A�b�v�t�@�C���B
//---------------------------------------------------------------------
void BackupLog( const char* filename ) {

	struct _stat buffer;
	if ( _stat( filename, &buffer ) == 0 ) {

		// �t�@�C���̃T�C�Y�� 1M �𒴂�����A�o�b�N�A�b�v�ɂ���
		if ( buffer.st_size > 1 * 1024 * 1024 ) {

			kjm::path_string newname = kjm::path_string( filename ).RenameExtension( ".000" );

			// �Â��o�b�N�A�b�v���폜(�����)
			remove( newname.c_str() );

			// �o�b�N�A�b�v�t�@�C���ɖ��O��ς���
			rename( filename, newname.c_str() );
		}
	}
}


