#if !defined(__WSS_KADO_H)
#define __WSS_KADO_H

// �ڍ׉ғ��̂��߂ɂƂ肠�����A�w�b�_��������

#include "dat_file.h"
#include "sharedobjs.h"
#include "GetIniFile.h"
#include "../WinSousaCalc/Calc/stt_prt.h"
#include "hscp.h"
#include "../wssasup/minitool.h"

#define ALLTIME	24	/* 24���� */
#define KADOFILE_RECORDSIZE		(ALLTIME * (60 / 5))	/* �@��ʒ莞�ғ��t�@�C���i�ڍׁj�̂P���R�[�h�T�C�Y	*/

#define BackUpInterval	(60 / 10)	/* �P���ɂP�� */
// extern
extern int EigyoZyoutai_Check();


// �ғ����t�@�C���\����
typedef struct {
	unsigned char kadoflg;
	unsigned char dmy[3];
} Kado_Info_t;

typedef std::vector<unsigned long>	ulong_vec;
typedef std::vector<Kado_Info_t> kdinfo_vec;

BOOL ShosaiKadoMain();

// �ғ��ڍ׃f�[�^�e�[�u���Ɏg�p
typedef struct{
	int time;			// ����
	int as_ofs;			// �`�r�̉ғ��f�[�^�I�t�Z�b�g
	int syosai_idx;		// �ғ��ڍׂ̃C���f�b�N�X
	char yobi[2];
} SyosaiIndex_t;

#endif

