/* WAIS START */
//----------------------------------------------
//�T�v
//	���L�������֌W�e���`
//
//�쐬��
//	Tokai ��
//
//���l
//	���L�������֌W�e���`��錾����
//----------------------------------------------

#if !defined(_SHAREDOBJS__H)
#define _SHAREDOBJS__H


// �}�b�s���O�I�u�W�F�N�g��
// �v�����`���f�[�^�������t�@�C���i�C���e���`���j
#define MMP_F4006		"f4006.bin"	// �V�X�e���ُ헚��
#define MMP_F4007		"f4007.bin"	// �N���A����
#define MMP_F4008		"f4008.bin"	// ���v�ݒ�
#define MMP_F4009		"f4009.bin"	// ��d����
#define MMP_F4010		"f4010.bin"	// �ݒ藚��
//	2003/11/10	yasui
#define MMP_F4012		"f4012.bin"	// ��ԗ���
#define MMP_F4013		"f4013.bin"	// �L������	2004.03.23 nao
#define MMP_F4015		"f4015.bin"	// �V�X�e������	2004.10.15 hashimoto
#define	MMP_F4021		"f4021.bin"	// �p�l�����t�@�C��
#define	MMP_F4028		"f4028.bin"	// �p�l�����2�t�@�C��

#define	MMP_F4101		"f4101.bin"			// �L�����t�@�C��
#define	MMP_F4102		"f4102.bin"			// ��t�@�C��
#define	MMP_F4103		"f4103.bin"			// �S�X�t�@�C��
#define	MMP_F4112		"f4112.bin"			// �莞���  nakatani
#define	MMP_F4113		"f4113.bin"			// �莞��ʉғ�  nakatani
#define	MMP_F4114		"f4114.bin"			// �莞�S�X�ғ�  nakatani
#define	MMP_F4115		"f4115.bin"			// �@��ʒ莞�ғ� 2003/04/09 nakatani
#define	MMP_F4117		"f4117.bin"			// DSP�f�[�^�t�@�C��	2006.06.29 add hashimoto(T2X002907)
#define	MMP_F4550		"f4550.bin"			// �s�[�N�f�[�^�t�@�C��
#define	MMP_F4601		"f4601.bin"			// �c�Ɨ����t�@�C��
#define	MMP_F4848		"f4848.bin"			// �����䔭�������t�@�C��	2006.05.12 add hashimoto(T2X002883)
#define	MMP_F5201		"f5201.bin"			// �`�r��������f�[�^�t�@�C��	2002.11.06  Add by koketsu (wss1.5)
#define	MMP_F5201_KEN	"f5201_ken.bin"		// �`�r��������f�[�^�����t�@�C��	2002.11.11  Add by koketsu (wss1.5)
#define MMP_RPLY_LOG	"Rply_Log.bin"		// �V�Z����(�݌v�ް�)		2002.11.06  Add by koketsu (wss1.5)
#define MMP_KPLY_LOG	"Kply_Log.bin"		// �V�Z����(�X�ް�)		2002.11.06  Add by koketsu (wss1.5)
#define MMP_KAIIN_LOG	"Kaiin_Log.bin"		// �������					2002.11.06  Add by koketsu (wss1.5)
#define MMP_KAIPLY_LOG	"Kaiply_Log.bin"	// ����V�Z����				2002.11.06  Add by koketsu (wss1.5)
#define MMP_F6001_1		"f6001_1.bin"		// ���[�W�f�f�[�^(��) (AS#1�p)
#define MMP_F6001_2		"f6001_2.bin"		// ���[�W�f�f�[�^(��) (AS#2�p)
#define MMP_F6001_3		"f6001_3.bin"		// ���[�W�f�f�[�^(��) (AS#3�p)
#define MMP_F6001_4		"f6001_4.bin"		// ���[�W�f�f�[�^(��) (AS#4�p)
#define MMP_F6001		"f6001.bin"			// ���[�W�f�f�[�^(��) (AS#1-MAX_AS�p)
#define MMP_F6002_1		"f6002_1.bin"		// ���[�W�f�f�[�^(�S�X) (AS#1�p)
#define MMP_F6002_2		"f6002_2.bin"		// ���[�W�f�f�[�^(�S�X) (AS#2�p)
#define MMP_F6002_3		"f6002_3.bin"		// ���[�W�f�f�[�^(�S�X) (AS#3�p)
#define MMP_F6002_4		"f6002_4.bin"		// ���[�W�f�f�[�^(�S�X) (AS#4�p)
#define MMP_F6002		"f6002.bin"			// ���[�W�f�f�[�^(�S�X) (AS#1-MAX_AS�p)
#define MMP_F6003_1		"f6003_1.bin"		// �e��莞�ғ����t�@�C�� (AS#1�p)
#define MMP_F6003_2		"f6003_2.bin"		// �e��莞�ғ����t�@�C�� (AS#2�p)
#define MMP_F6003_3		"f6003_3.bin"		// �e��莞�ғ����t�@�C�� (AS#3�p)
#define MMP_F6003_4		"f6003_4.bin"		// �e��莞�ғ����t�@�C�� (AS#4�p)
#define MMP_F6003		"f6003.bin"			// �e��莞�ғ����t�@�C�� (AS#1-MAX_AS�p)
#define MMP_F6004		"f6004.bin"			// ���ʏƍ��@�����t�@�C�� (AS#1-MAX_AS�p)
#define MMP_F6004_1		"f6004_1.bin"		// ���ʏƍ��@�����t�@�C�� (AS#1�p)
#define MMP_F6004_2		"f6004_2.bin"		// ���ʏƍ��@�����t�@�C�� (AS#2�p)
#define MMP_F6004_3		"f6004_3.bin"		// ���ʏƍ��@�����t�@�C�� (AS#3�p)
#define MMP_F6004_4		"f6004_4.bin"		// ���ʏƍ��@�����t�@�C�� (AS#4�p)
#define MMP_F6005		"f6005.bin"			// ���ʏƍ��@�����Ǘ��t�@�C�� (AS#1-MAX_AS�p)
#define MMP_F6005_1		"f6005_1.bin"		// ���ʏƍ��@�����Ǘ��t�@�C�� (AS#1�p)
#define MMP_F6005_2		"f6005_2.bin"		// ���ʏƍ��@�����Ǘ��t�@�C�� (AS#2�p)
#define MMP_F6005_3		"f6005_3.bin"		// ���ʏƍ��@�����Ǘ��t�@�C�� (AS#3�p)
#define MMP_F6005_4		"f6005_4.bin"		// ���ʏƍ��@�����Ǘ��t�@�C�� (AS#4�p)
#define	MMP_F6101_1		"f6101_1.bin"		// ���ܗ����L�����t�@�C���P�i�`�r���P�p�j
#define	MMP_F6101_2		"f6101_2.bin"		// ���ܗ����L�����t�@�C���Q�i�`�r���Q�p�j
#define	MMP_F6101_3		"f6101_3.bin"		// ���ܗ����L�����t�@�C���R�i�`�r���R�p�j
#define	MMP_F6101_4		"f6101_4.bin"		// ���ܗ����L�����t�@�C���S�i�`�r���S�p�j
#define	MMP_F6102		"f6102.bin"			// ���ܗ����t�@�C���i�`�r�� 1�`MAX_AS �p�j
#define	MMP_F6102_1		"f6102_1.bin"		// ���ܗ����t�@�C���P�i�`�r���P�p�j
#define	MMP_F6102_2		"f6102_2.bin"		// ���ܗ����t�@�C���Q�i�`�r���Q�p�j
#define	MMP_F6102_3		"f6102_3.bin"		// ���ܗ����t�@�C���R�i�`�r���R�p�j
#define	MMP_F6102_4		"f6102_4.bin"		// ���ܗ����t�@�C���S�i�`�r���S�p�j
//***���@�ǉ��@yasui�@2002/10/09
#define	MMP_F6103		"f6103.bin"			// �X�^�[�g�X�����v�t�@�C���i�`�r�� 1�`MAX_AS �p�j
#define	MMP_F6103_1		"f6103_1.bin"		// �X�^�[�g�X�����v�t�@�C��1�i�`�r���P�p�j
#define	MMP_F6103_2		"f6103_2.bin"		// �X�^�[�g�X�����v�t�@�C��2�i�`�r���Q�p�j
#define	MMP_F6103_3		"f6103_3.bin"		// �X�^�[�g�X�����v�t�@�C��3�i�`�r���R�p�j
#define	MMP_F6103_4		"f6103_4.bin"		// �X�^�[�g�X�����v�t�@�C��4�i�`�r���S�p�j
//***���@�ǉ��@yasui�@2002/10/09
#define	MMP_F6105		"f6105.bin"			// ���ܗ���2�t�@�C���i�`�r�� 1�`MAX_AS �p�j
#define	MMP_F6105_1		"f6105_1.bin"		// ���ܗ���2�t�@�C���P�i�`�r���P�p�j
#define	MMP_F6105_2		"f6105_2.bin"		// ���ܗ���2�t�@�C���Q�i�`�r���Q�p�j
#define	MMP_F6105_3		"f6105_3.bin"		// ���ܗ���2�t�@�C���R�i�`�r���R�p�j
#define	MMP_F6105_4		"f6105_4.bin"		// ���ܗ���2�t�@�C���S�i�`�r���S�p�j
#define	MMP_F6408		"f6408.bin"			// �X��t�@�C���i DAISU ���j
#define	MMP_F6408_1		"f6408_1.bin"		// �X��t�@�C���P�i�`�r���P�p�T�P�Q�䕪�j
#define	MMP_F6408_2		"f6408_2.bin"		// �X��t�@�C���Q�i�`�r���Q�p�T�P�Q�䕪�j
#define	MMP_F6408_3		"f6408_3.bin"		// �X��t�@�C���R�i�`�r���R�p�T�P�Q�䕪�j
#define	MMP_F6408_4		"f6408_4.bin"		// �X��t�@�C���S�i�`�r���S�p�T�P�Q�䕪�j
#define	MMP_F6407		"f6407.bin"			// �݌v��t�@�C���i DAISU ���j			2002.06.12 koketsu Add
#define	MMP_F6407_1		"f6407_1.bin"		// �݌v��t�@�C���P�i�`�r���P�p�T�P�Q�䕪�j2002.06.12 koketsu Add
#define	MMP_F6407_2		"f6407_2.bin"		// �݌v��t�@�C���Q�i�`�r���Q�p�T�P�Q�䕪�j2002.06.12 koketsu Add
#define	MMP_F6407_3		"f6407_3.bin"		// �݌v��t�@�C���R�i�`�r���R�p�T�P�Q�䕪�j2002.06.12 koketsu Add
#define	MMP_F6407_4		"f6407_4.bin"		// �݌v��t�@�C���S�i�`�r���S�p�T�P�Q�䕪�j2002.06.12 koketsu Add
#define	MMP_F6409		"f6409.bin"			// TN�X�����v�t�@�C���i DAISU ���j		2002.11.20 koketsu Add
#define	MMP_F6409_1		"f6409_1.bin"		// TN�X�����v�t�@�C���P�i�`�r���P�p�T�P�Q�䕪�j2002.11.20 koketsu Add
#define	MMP_F6409_2		"f6409_2.bin"		// TN�X�����v�t�@�C���Q�i�`�r���Q�p�T�P�Q�䕪�j2002.11.20 koketsu Add
#define	MMP_F6409_3		"f6409_3.bin"		// TN�X�����v�t�@�C���R�i�`�r���R�p�T�P�Q�䕪�j2002.11.20 koketsu Add
#define	MMP_F6409_4		"f6409_4.bin"		// TN�X�����v�t�@�C���S�i�`�r���S�p�T�P�Q�䕪�j2002.11.20 koketsu Add
#define MMP_F6409_0		"f6409_0.bin"		// �������z���TN�X�����v�t�@�C��

#define	MMP_F6411_1		"f6411_1.bin"		// �S�X�t�@�C���P�i�`�r���P�p�j
#define	MMP_F6411_2		"f6411_2.bin"		// �S�X�t�@�C���Q�i�`�r���Q�p�j
#define	MMP_F6411_3		"f6411_3.bin"		// �S�X�t�@�C���R�i�`�r���R�p�j
#define	MMP_F6411_4		"f6411_4.bin"		// �S�X�t�@�C���S�i�`�r���S�p�j

#define	MMP_F6419		"f6419.bin"			// TN�X�����v�Q�t�@�C���i DAISU ���j
#define	MMP_F6419_1		"f6419_1.bin"		// TN�X�����v�Q�t�@�C���P�i�`�r���P�p�T�P�Q�䕪�j
#define	MMP_F6419_2		"f6419_2.bin"		// TN�X�����v�Q�t�@�C���Q�i�`�r���Q�p�T�P�Q�䕪�j
#define	MMP_F6419_3		"f6419_3.bin"		// TN�X�����v�Q�t�@�C���R�i�`�r���R�p�T�P�Q�䕪�j
#define	MMP_F6419_4		"f6419_4.bin"		// TN�X�����v�Q�t�@�C���S�i�`�r���S�p�T�P�Q�䕪�j

#define	MMP_F9000		"possum_a.bin"		// POS���v�f�[�^�t�@�C���i�`�X�p�j
#define	MMP_F9001		"possum_b.bin"		// POS���v�f�[�^�t�@�C���i�a�X�p�j
#define	MMP_F9010		"tptokei_a.bin"		// TP���v�f�[�^�t�@�C���i�`�X�p�j
#define	MMP_F9011		"tptokei_b.bin"		// TP���v�f�[�^�t�@�C���i�a�X�p�j

#define	MMP_F5120		"f5120.bin"			// ���ʓ���t�@�C��

#define	MMP_KIJUNKI_DAI		"kijunki_dai_syu.bin"	// �V��@ ��W�v�t�@�C��		T2X002549	2005.07.19	nao
#define	MMP_KIJUNKI_KISHU	"kijunki_kis_syu.bin"	// �V��@ �@��W�v�t�@�C��	T2X002549	2005.07.19	nao
#define	MMP_KIJUNKI_SIMA	"kijunki_sim_syu.bin"	// �V��@ ���W�v�t�@�C��		T2X002549	2005.07.19	nao
#define	MMP_KIJUNKI_SBT		"kijunki_sbt_syu.bin"	// �V��@ ��ʏW�v�t�@�C��	T2X002549	2005.07.26	nao

/*//�����g���[���`��MMP�p�~�@2003/12/16�@yasui
// ���f�[�^�������t�@�C���i���g���[���`���j
#define	MMP_MF4101		"mf4101.bin"		// �L�����t�@�C��
#define	MMP_MF4102		"mf4102.bin"		// ��t�@�C��
#define	MMP_MF4103		"mf4103.bin"		// �S�X�t�@�C��
#define	MMP_MF4601		"mf4601.bin"		// �c�Ɨ����t�@�C��
#define	MMP_MF6101_1	"mf6101_1.bin"		// ���ܗ����L�����t�@�C���P�i�`�r���P�p�j
#define	MMP_MF6101_2	"mf6101_2.bin"		// ���ܗ����L�����t�@�C���Q�i�`�r���Q�p�j
#define	MMP_MF6101_3	"mf6101_3.bin"		// ���ܗ����L�����t�@�C���R�i�`�r���R�p�j
#define	MMP_MF6102		"mf6102.bin"		// ���ܗ����t�@�C���i�`�r���P�Q�R�p�j
#define	MMP_MF6102_1	"mf6102_1.bin"		// ���ܗ����t�@�C���P�i�`�r���P�p�j
#define	MMP_MF6102_2	"mf6102_2.bin"		// ���ܗ����t�@�C���Q�i�`�r���Q�p�j
#define	MMP_MF6102_3	"mf6102_3.bin"		// ���ܗ����t�@�C���R�i�`�r���R�p�j
//***���@�ǉ��@yasui�@2002/10/09
#define	MMP_MF6103  	"mf6103.bin"		// �X�^�[�g�X�����v�t�@�C��
#define	MMP_MF6103_1	"mf6103_1.bin"		// �X�^�[�g�X�����v�t�@�C��1�i�`�r���P�p�j
#define	MMP_MF6103_2	"mf6103_2.bin"		// �X�^�[�g�X�����v�t�@�C��2�i�`�r���Q�p�j
#define	MMP_MF6103_3	"mf6103_3.bin"		// �X�^�[�g�X�����v�t�@�C��3�i�`�r���R�p�j
//***���@�ǉ��@yasui�@2002/10/09
#define	MMP_MF6408		"mf6408.bin"		// �X��t�@�C���i�P�T�R�R�䕪�j
#define	MMP_MF6408_1	"mf6408_1.bin"		// �X��t�@�C���P�i�`�r���P�p�T�P�Q�䕪�j
#define	MMP_MF6408_2	"mf6408_2.bin"		// �X��t�@�C���Q�i�`�r���Q�p�T�P�Q�䕪�j
#define	MMP_MF6408_3	"mf6408_3.bin"		// �X��t�@�C���R�i�`�r���R�p�T�P�Q�䕪�j
#define	MMP_MF6407		"mf6407.bin"		// �݌v��t�@�C���i�P�T�R�R�䕪�j			2002.06.12 koketsu Add
#define	MMP_MF6407_1	"mf6407_1.bin"		// �݌v��t�@�C���P�i�`�r���P�p�T�P�Q�䕪�j	2002.06.12 koketsu Add
#define	MMP_MF6407_2	"mf6407_2.bin"		// �݌v��t�@�C���Q�i�`�r���Q�p�T�P�Q�䕪�j	2002.06.12 koketsu Add
#define	MMP_MF6407_3	"mf6407_3.bin"		// �݌v��t�@�C���R�i�`�r���R�p�T�P�Q�䕪�j	2002.06.12 koketsu Add
#define	MMP_MF6409		"mf6409.bin"		// TN�X�����v�t�@�C���i�P�T�R�R�䕪�j		2002.11.20 koketsu Add
#define	MMP_MF6409_1	"mf6409_1.bin"		// TN�X�����v�t�@�C���P�i�`�r���P�p�T�P�Q�䕪�j2002.11.20 koketsu Add
#define	MMP_MF6409_2	"mf6409_2.bin"		// TN�X�����v�t�@�C���Q�i�`�r���Q�p�T�P�Q�䕪�j2002.11.20 koketsu Add
#define	MMP_MF6409_3	"mf6409_3.bin"		// TN�X�����v�t�@�C���R�i�`�r���R�p�T�P�Q�䕪�j2002.11.20 koketsu Add

#define	MMP_MF9000		"possum_a.bin"		// POS���v�f�[�^�t�@�C���i�`�X�p�j
#define	MMP_MF9001		"possum_b.bin"		// POS���v�f�[�^�t�@�C���i�a�X�p�j
#define	MMP_MF9010		"tptokei_a.bin"		// TP���v�f�[�^�t�@�C���i�`�X�p�j
#define	MMP_MF9011		"tptokei_b.bin"		// TP���v�f�[�^�t�@�C���i�a�X�p�j
//�����g���[���`��MMP�p�~�@2003/12/16�@yasui*/

// �O���[�o���e�[�u��
#define	MMP_COMNTBL		"comntbl.bin"		// ���L�e�[�u��
#define	MMP_SYUFILE		"syufile.bin"		// �W�v�f�[�^
#define MMP_KOKODAI		"kokodai.bin"		// �X��f�[�^
#define	MMP_IDXSIMA		"idxsima.bin"		// ���h�c�w�e�[�u��
#define	MMP_IDXKISYU	"idxkisyu.bin"		// �@��h�c�w�e�[�u��
#define	MMP_IDXKINKO	"idxkinko.bin"		// ���ɂh�c�w�e�[�u��
#define	MMP_TEIKI		"teiki.bin"			// ����ʐM�f�[�^�i�e�e�O�P�`���f�[�^�j
//	2002.05.20 �ǉ� By Fujiki(�Q�X��)-->
#define MMP_ZEN_TENPO	"Zen_Tenpo.bin"	// �`�E�a�X�ܑS�X�W�v�f�[�^�i�Q�X�܁j
#define MMP_ZEN_FLOOR	"Zen_Floor.bin"		// �t���A�S�X�W�v�f�[�^�i�Q�X�܁j
//	-->2002.05.20 �ǉ� By Fujiki(�Q�X��)

//�ݒ�f�[�^�������t�@�C��
#define	MMP_F1000		"f1000.bin"			// �V�X�e���ݒ�
#define	MMP_F1001		"f1001.bin"			// ��ڑ��ݒ�
#define	MMP_F1002		"f1002.bin"			// �S�X�ڑ��ݒ�
#define	MMP_F1003		"f1003.bin"			// ���[�����v�ݒ�
#define	MMP_F1004		"f1004.bin"			// �^�C�v�ݒ�
//	2002.05.17 �ǉ� By Fujiki(�Q�X��)-->
#define	MMP_F1006		"f1006.bin"			// �X�ܐڑ��ݒ�
//	-->2002.05.17 �ǉ� By Fujiki(�Q�X��)
#define	MMP_F1007		"f1007.bin"			// ��ʃ|�P�b�g�ݒ� 2002.06.14 �ǉ� kaoru�i�Q�X�܁j
#define	MMP_F1008		"f1008.bin"			// �X�ܥ��ʖ��̐ݒ� ��2002/11/25 (�X�ܐ��Ή�)�ǉ� Fukuyama
#define	MMP_F1010		"f1010.bin"			// ��ڑ��ݒ�2
#define	MMP_F1011		"f1011.bin"			// ���������o�͐ݒ�
#define	MMP_F1012		"f1012.bin"			// ���ʏƍ��@�h�c�ݒ�
#define	MMP_F1050		"f1050.bin"			// ����萔�ݒ�
#define	MMP_F1051		"f1051.bin"			// �@�햼�ݒ�
#define	MMP_F1052		"f1052.bin"			// �^�C�v�ݒ�
#define	MMP_F1053		"f1053.bin"			// �s���X�����v�ݒ�
#define	MMP_F1056		"f1056.bin"			// ���ʏ���l�ݒ� // <-- 2010/09/10 add �V����Ǘ�Step1 kanda
#define	MMP_F1100		"f1100.bin"			// �Ïؔԍ��ݒ�
#define	MMP_F1101		"f1101.bin"			// �J�X�����ݒ�
#define	MMP_F1102		"f1102.bin"			// ���ɓ_���ݒ�
#define	MMP_F1103		"f1103.bin"			// �S�X��~�ݒ�
#define	MMP_F1104		"f1104.bin"			// �@���~�ݒ�
#define	MMP_F1105		"f1105.bin"			// �S�X�^�p�ݒ�
#define	MMP_F1106		"f1106.bin"			// �@��^�p�ݒ�
#define	MMP_F1107		"f1107.bin"			// D2�ݒ�
#define	MMP_F1108		"f1108.bin"			// �X�ܕʕϑ��c�Ɛݒ� ��2002/10/29 (�X�ܕʕϑ��c�ƑΉ�)�ǉ� Fukuyama
#define	MMP_F1109		"f1109.bin"			// ���ܐU���ݒ�		�@��2005/07/19 (�V��@�Ή�)�ǉ� hashimoto
#define	MMP_F1110		"f1110.bin"			// D2�g���ݒ�		�@��2006/02/06 (DU4�g�����͑Ή�)�ǉ� hashimoto
#define	MMP_F1111		"f1111.bin"			// DSP�ݒ�						��2006/06/28 (DSP-100�ڑ��Ή�)�ǉ� hashimoto
#define	MMP_F1112		"f1112.bin"			// DSP�T���h���ɉ�����[�g�ݒ�	��2006/06/28 (DSP-100�ڑ��Ή�)�ǉ� hashimoto
#define	MMP_F1113		"f1113.bin"			// �e��v���@�ݒ�
#define	MMP_F1114		"f1114.bin"			// D2�ݒ�(���)
#define	MMP_F1115		"f1115.bin"			// D2�g���ݒ�(���)
#define	MMP_F1118		"f1118.bin"			// �Z�[�t���[�^�[�ُ팟�m�ݒ�
#define	MMP_F1150		"f1150.bin"			// �s�m�ݒ�
#define	MMP_F1151		"f1151.bin"			// TN�ݒ�Q
#define	MMP_F1152		"f1152.bin"			// ���v���C�萔�����ʐݒ� 2002.06.14 �ǉ� kaoru�i�Q�X�܁j
#define	MMP_F1153		"f1153.bin"			// �s�m���ԕϓ����v���C�萔���ݒ� 2002.06.14 �ǉ� kaoru�i�Q�X�܁j
#define	MMP_F1200		"f1200.bin"			// �|�C���g�S�X�ݒ�
#define	MMP_F1201		"f1201.bin"			// �|�C���g�@��ʐݒ�
//	2002.06.29 �ǉ� By Fujiki-->
#define	MMP_F1202		"f1202.bin"			// ���ԕϓ����X�|�C���g�ݒ�
//	-->2002.06.29 �ǉ� By Fujiki
#define	MMP_F1203		"f1203.bin"			// �ϓ��ғ��|�C���g�ݒ� 2002.06.14 �ǉ� kaoru�i�Q�X�܁j
//	2002.06.29 �ǉ� By Fujiki-->
#define	MMP_F1204		"f1204.bin"			// ���ԕϓ�����|�C���g�ݒ�
//	-->2002.06.29 �ǉ� By Fujiki
#define	MMP_F1401		"f1401.bin"			// �䃉���v�S�X����ݒ�			2005/3/8 (�䃉���v�Ή� T2X002355�Ή�)�ǉ� Fukuyama
#define	MMP_F1402		"f1402.bin"			// �䃉���v�@��ʓ���ݒ�		2005/3/8 (�䃉���v�Ή� T2X002355�Ή�)�ǉ� Fukuyama
#define	MMP_F1403		"f1403.bin"			// �䃉���v�����R���h�c�ݒ�iT2X002355�j2005/03/08 tomida
#define	MMP_F1404		"f1404.bin"			// �䃉���v�����v�p�^�[���ݒ�	2005/3/8 (�䃉���v�Ή� T2X002355�Ή�)�ǉ� Fukuyama
#define	MMP_F1405		"f1405.bin"			// �䃉���v�����v�ݒ�			2005/3/8 (�䃉���v�Ή� T2X002355�Ή�)�ǉ� Fukuyama
#define MMP_F1406		"f1406.bin"			// �䃉���v���t���[�ݒ�			20050629 add tomida �䃉���v�����J���iT2X002500�j
#define MMP_F1407		"f1407.bin"			// �䃉���v���o�t���[�ݒ�		20060106 add hashimoto �f�V�X�e���Ή��iT2X002759�j
#define MMP_F1408		"f1408.bin"			// �䃉���v�����v�p�^�[���ݒ�2	20061208 add hashimoto 80�^�䃉���v�Ή��iT2X003029�j

#define MMP_F1500		"f1500.bin"			// �����䓇�S�̐ݒ�		20060512 add hashimoto ���Ǘ��V�X�e���J���iT2X002883�j
#define MMP_F1501		"f1501.bin"			// �����䓇�ʐݒ�		20060512 add hashimoto ���Ǘ��V�X�e���J���iT2X002883�j

// �����g���[���`��MMP�p�~�ɔ����ړ��@2003/12/16�@yasui
#define MMP_F4104		"f4104.bin"		// ���W�v
#define MMP_F4105		"f4105.bin"		// �@��W�v
#define MMP_F4106		"f4106.bin"		// ��ʏW�v

#define MMP_F4112			"f4112.bin"	// �莞�ғ�
#define MMP_F4113			"f4113.bin"	// �莞��ʉғ�
#define MMP_F4114			"f4114.bin"	// �莞�S�X�ғ�
#define MMP_F4115			"f4115.bin"	// �莞�@��
#define MMP_F4117			"f4117.bin"	// DSP�f�[�^�t�@�C��
#define MMP_F4550			"f4550.bin"	// �s�[�N�f�[�^�t�@�C��
#define MMP_F4005			"f4005.bin"	// �L�����
#define MMP_F4006			"f4006.bin"	// �V�X�e���ُ헚��
#define MMP_F4007			"f4007.bin"	// �N���A����
#define MMP_F4008			"f4008.bin"	// ���v�ݒ�
#define MMP_F4009			"f4009.bin"	// ��d����
#define MMP_F4010			"f4010.bin"	// �ݒ藚��
#define MMP_F4011			"f4011.bin"	// �ݒ�VER���
#define	MMP_F4116			"f4116.bin"	// DSP�f�[�^�t�@�C��		��2006/06/28 (DSP-100�ڑ��Ή�)�ǉ� hashimoto
#define MMP_F4022			"f4022.bin"	// �ُ헚��
#define MMP_F4023			"f4023.bin"	// �Ŏ~����
#define MMP_F4024			"f4024.bin"	// ��������

#define MMP_F4848			"f4848.bin"	// �����䔭������		20060512 add hashimoto ���Ǘ��V�X�e���J���iT2X002883�j

//#define MMP_MF6409			"mf6409.bin"	// �s�m�X�����v2002.11.20 koketsu Del �i��Œ�`�j
#define MMP_F5201			"f5201.bin"	// ������O
#define MMP_F6301			"f6301.bin"	// ������O�Ǘ����

#if	1	/* 2004.06.07(mon) add.AS�����������p by ohnot */

#define MMP_F4001			"f4001.bin"
#define MMP_F6303_1			"f6303_1.bin"	//�s�m���e�[�u���P�i�`�r���P�p�j
#define MMP_F6303_2			"f6303_2.bin"	//�s�m���e�[�u���Q�i�`�r���Q�p�j
#define MMP_F6303_3			"f6303_3.bin"	//�s�m���e�[�u���R�i�`�r���R�p�j
#define MMP_F6303_4			"f6303_4.bin"	//�s�m���e�[�u���S�i�`�r���S�p�j
#define MMP_F6304_1			"f6304_1.bin"	//�s�m�X�����v�L���O�����P�i�`�r���P�p�T�P�Q�䕪�j
#define MMP_F6304_2			"f6304_2.bin"	//�s�m�X�����v�L���O�����Q�i�`�r���Q�p�T�P�Q�䕪�j
#define MMP_F6304_3			"f6304_3.bin"	//�s�m�X�����v�L���O�����R�i�`�r���R�p�T�P�Q�䕪�j
#define MMP_F6304_4			"f6304_4.bin"	//�s�m�X�����v�L���O�����S�i�`�r���S�p�T�P�Q�䕪�j
#define MMP_F6305_1			"f6305_1.bin"	//�s�m�X�����v�L���O�X�����P�i�`�r���P�p�T�P�Q�䕪�j
#define MMP_F6305_2			"f6305_2.bin"	//�s�m�X�����v�L���O�X�����Q�i�`�r���Q�p�T�P�Q�䕪�j
#define MMP_F6305_3			"f6305_3.bin"	//�s�m�X�����v�L���O�X�����R�i�`�r���R�p�T�P�Q�䕪�j
#define MMP_F6305_4			"f6305_4.bin"	//�s�m�X�����v�L���O�X�����S�i�`�r���S�p�T�P�Q�䕪�j

#define MMP_F6410_1			"f6410_1.bin"	// �X�s�m�f�[�^�P�i�`�r���P�p�T�P�Q�䕪�j
#define MMP_F6410_2			"f6410_2.bin"	// �X�s�m�f�[�^�Q�i�`�r���Q�p�T�P�Q�䕪�j
#define MMP_F6410_3			"f6410_3.bin"	// �X�s�m�f�[�^�R�i�`�r���R�p�T�P�Q�䕪�j
#define MMP_F6410_4			"f6410_4.bin"	// �X�s�m�f�[�^�S�i�`�r���S�p�T�P�Q�䕪�j

#define MMP_F6501_1			"f6501_1.bin"	// �䃉���v���J�o���p�o�b�N�A�b�v�f�[�^�P�i�`�r���P�p�T�P�Q�䕪�j
#define MMP_F6501_2			"f6501_2.bin"	// �䃉���v���J�o���p�o�b�N�A�b�v�f�[�^�Q�i�`�r���Q�p�T�P�Q�䕪�j
#define MMP_F6501_3			"f6501_3.bin"	// �䃉���v���J�o���p�o�b�N�A�b�v�f�[�^�R�i�`�r���R�p�T�P�Q�䕪�j
#define MMP_F6501_4			"f6501_4.bin"	// �䃉���v���J�o���p�o�b�N�A�b�v�f�[�^�S�i�`�r���S�p�T�P�Q�䕪�j

#endif	/* 2004.06.07(mon) add.AS�����������p by ohnot */
// �����g���[���`��MMP�p�~�ɔ����ړ��@2003/12/16�@yasui

// �r���A�N�Z�X�p
#define MTX_F4006		"mtx_f4006"			// �V�X�e���ُ헚��
#define MTX_F4007		"mtx_f4007"			// �N���A����
#define MTX_F4008		"mtx_f4008"			// ���v�ݒ�
#define MTX_F4009		"mtx_f4009"			// ��d����
#define MTX_F4010		"mtx_f4010"			// �ݒ藚��
#define MTX_F4012		"mtx_f4012"			// ��ԗ����t�@�C���@2003/11/10yasui
#define MTX_F4013		"mtx_f4013"			// �L������	2004.03.23 nao
#define MTX_F4015		"mtx_f4015"			// �V�X�e������	2004.10.15 hashimoto
#define	MTX_F4021		"mtx_f4021"			// �p�l�����t�@�C��
#define	MTX_F4028		"mtx_f4028"			// �p�l�����2�t�@�C��

#define	MTX_F4101		"mtx_f4101"			// �r���L�����f�[�^
#define	MTX_F4102		"mtx_f4102"			// �r����t�@�C��
#define	MTX_F4103		"mtx_f4103"			// �r���S�X�t�@�C��
#define	MTX_F4112		"mtx_f4112"			// �莞���  nakatani
#define	MTX_F4113		"mtx_f4113"			// �莞��ʉғ�  nakatani
#define	MTX_F4114		"mtx_f4114"			// �莞�S�X�ғ�  nakatani
#define	MTX_F4115		"mtx_f4115"			// �@��ʒ莞�ғ� 2003/04/09 nakatani
#define	MTX_F4117		"mtx_f4117"			// DSP�f�[�^�t�@�C��	2006.06.29 add hashimoto(T2X002907)
#define	MTX_F4550		"mtx_f4550"			// �s�[�N�f�[�^�t�@�C��
#define	MTX_F4601		"mtx_f4601"			// �r���c�Ɨ����t�@�C��
#define	MTX_F4848		"mtx_f4848"			// �r�������䔭�������t�@�C��	2006.05.12 add hashimoto(T2X002883)

#define	MTX_F5201		"mtx_f5201"			// �`�r��������t�@�C���iWin�`���p�j 2002.11.06  Add by koketsu
#define	MTX_MF5201		"mtx_mf5201"		// �`�r��������t�@�C���i��۰׌`���p�j 2002.11.06  Add by koketsu
#define MTX_RPLY_LOG	"mtx_Rply_Log"		// �V�Z����(�݌v�ް�)		2002.11.06  Add by koketsu (wss1.5)
#define MTX_KPLY_LOG	"mtx_Kply_Log"		// �V�Z����(�X�ް�)		2002.11.06  Add by koketsu (wss1.5)
#define MTX_KAIIN_LOG	"mtx_Kaiin_Log"		// �������					2002.11.06  Add by koketsu (wss1.5)
#define MTX_KAIPLY_LOG	"mtx_Kaiply_Log"	// ����V�Z����				2002.11.06  Add by koketsu (wss1.5)

#define	MTX_F5120		"mtx_f5120"			// �j�r���ʓ���t�@�C���iWin�`���p�j 2004.1.15  Add by nao

#define MTX_F6001		"mtx_f6001"			// �r�����[�W�f�f�[�^(��)
#define MTX_F6002		"mtx_f6002"			// �r�����[�W�f�f�[�^(�S�X)
#define MTX_F6003		"mtx_f6003"			// �e��莞�ғ����t�@�C��
#define	MTX_F6004		"mtx_f6004"			// �r�����ʏƍ��@�����t�@�C���i�`�r�� 1�`MAX_AS �S�p�j
#define	MTX_F6004_1		"mtx_f6004_1"		// �r�����ʏƍ��@�����t�@�C���P�i�`�r���P�p�j
#define	MTX_F6004_2		"mtx_f6004_2"		// �r�����ʏƍ��@�����t�@�C���Q�i�`�r���Q�p�j
#define	MTX_F6004_3		"mtx_f6004_3"		// �r�����ʏƍ��@�����t�@�C���R�i�`�r���R�p�j
#define	MTX_F6004_4		"mtx_f6004_4"		// �r�����ʏƍ��@�����t�@�C���S�i�`�r���S�p�j
#define	MTX_F6005		"mtx_f6005"			// �r�����ʏƍ��@�����Ǘ��t�@�C���i�`�r�� 1�`MAX_AS �S�p�j
#define	MTX_F6005_1		"mtx_f6005_1"		// �r�����ʏƍ��@�����Ǘ��t�@�C���P�i�`�r���P�p�j
#define	MTX_F6005_2		"mtx_f6005_2"		// �r�����ʏƍ��@�����Ǘ��t�@�C���Q�i�`�r���Q�p�j
#define	MTX_F6005_3		"mtx_f6005_3"		// �r�����ʏƍ��@�����Ǘ��t�@�C���R�i�`�r���R�p�j
#define	MTX_F6005_4		"mtx_f6005_4"		// �r�����ʏƍ��@�����Ǘ��t�@�C���S�i�`�r���S�p�j

#define	MTX_F6101_1		"mtx_f6101_1"		// �r�����ܗ����L�����t�@�C���P�i�`�r���P�p�j
#define	MTX_F6101_2		"mtx_f6101_2"		// �r�����ܗ����L�����t�@�C���Q�i�`�r���Q�p�j
#define	MTX_F6101_3		"mtx_f6101_3"		// �r�����ܗ����L�����t�@�C���R�i�`�r���R�p�j
#define	MTX_F6101_4		"mtx_f6101_4"		// �r�����ܗ����L�����t�@�C���S�i�`�r���S�p�j
#define	MTX_F6102		"mtx_f6102"			// �r�����ܗ����t�@�C���i�`�r�� 1�`MAX_AS �S�p�j
#define	MTX_F6102_1		"mtx_f6102_1"		// �r�����ܗ����t�@�C���P�i�`�r���P�p�j
#define	MTX_F6102_2		"mtx_f6102_2"		// �r�����ܗ����t�@�C���Q�i�`�r���Q�p�j
#define	MTX_F6102_3		"mtx_f6102_3"		// �r�����ܗ����t�@�C���R�i�`�r���R�p�j
#define	MTX_F6102_4		"mtx_f6102_4"		// �r�����ܗ����t�@�C���S�i�`�r���S�p�j
//***���@�ǉ��@yasui�@2002/10/09
#define	MTX_F6103		"mtx_f6103"			// �X�^�[�g�X�����v�t�@�C���i�`�r�� 1�`MAX_AS �p�j
#define	MTX_F6103_1		"mtx_f6103_1"		// �X�^�[�g�X�����v�t�@�C��1�i�`�r���P�p�j
#define	MTX_F6103_2		"mtx_f6103_2"		// �X�^�[�g�X�����v�t�@�C��2�i�`�r���Q�p�j
#define	MTX_F6103_3		"mtx_f6103_3"		// �X�^�[�g�X�����v�t�@�C��3�i�`�r���R�p�j
#define	MTX_F6103_4		"mtx_f6103_4"		// �X�^�[�g�X�����v�t�@�C��4�i�`�r���S�p�j
//***���@�ǉ��@yasui�@2002/10/09
#define	MTX_F6105		"mtx_f6105"			// �r�����ܗ���2�t�@�C���i�`�r�� 1�`MAX_AS �S�p�j
#define	MTX_F6105_1		"mtx_f6105_1"		// �r�����ܗ���2�t�@�C���P�i�`�r���P�p�j
#define	MTX_F6105_2		"mtx_f6105_2"		// �r�����ܗ���2�t�@�C���Q�i�`�r���Q�p�j
#define	MTX_F6105_3		"mtx_f6105_3"		// �r�����ܗ���2�t�@�C���R�i�`�r���R�p�j
#define	MTX_F6105_4		"mtx_f6105_4"		// �r�����ܗ���2�t�@�C���S�i�`�r���S�p�j
#define	MTX_F6408		"mtx_f6408"			// �r���X��f�[�^
#define	MTX_F6408_1		"mtx_f6408_1"		// �r���X��t�@�C���P�i�`�r���P�p�j
#define	MTX_F6408_2		"mtx_f6408_2"		// �r���X��t�@�C���Q�i�`�r���Q�p�j
#define	MTX_F6408_3		"mtx_f6408_3"		// �r���X��t�@�C���R�i�`�r���R�p�j
#define	MTX_F6408_4		"mtx_f6408_4"		// �r���X��t�@�C���S�i�`�r���S�p�j
#define	MTX_F6407		"mtx_f6407"			// �r���݌v��f�[�^						2002.06.12 koketsu Add
#define	MTX_F6407_1		"mtx_f6407_1"		// �r���݌v��t�@�C���P�i�`�r���P�p�j	2002.06.12 koketsu Add
#define	MTX_F6407_2		"mtx_f6407_2"		// �r���݌v��t�@�C���Q�i�`�r���Q�p�j	2002.06.12 koketsu Add
#define	MTX_F6407_3		"mtx_f6407_3"		// �r���݌v��t�@�C���R�i�`�r���R�p�j	2002.06.12 koketsu Add
#define	MTX_F6407_4		"mtx_f6407_4"		// �r���݌v��t�@�C���S�i�`�r���S�p�j	2002.06.12 koketsu Add
#define	MTX_F6409		"mtx_f6409"			// TN�X�����v�t�@�C���i�T�P�Q * MAX_AS �䕪�j		2002.11.20 koketsu Add
#define	MTX_F6409_1		"mtx_f6409_1"		// TN�X�����v�t�@�C���P�i�`�r���P�p�T�P�Q�䕪�j2002.11.20 koketsu Add
#define	MTX_F6409_2		"mtx_f6409_2"		// TN�X�����v�t�@�C���Q�i�`�r���Q�p�T�P�Q�䕪�j2002.11.20 koketsu Add
#define	MTX_F6409_3		"mtx_f6409_3"		// TN�X�����v�t�@�C���R�i�`�r���R�p�T�P�Q�䕪�j2002.11.20 koketsu Add
#define	MTX_F6409_4		"mtx_f6409_4"		// TN�X�����v�t�@�C���S�i�`�r���S�p�T�P�Q�䕪�j2002.11.20 koketsu Add
#define MTX_F6409_0		"mtx_f6409_0"

#define	MTX_F6411_1		"mtx_f6411_1"		// �r���S�X�t�@�C���P�i�`�r���P�p�j
#define	MTX_F6411_2		"mtx_f6411_2"		// �r���S�X�t�@�C���Q�i�`�r���Q�p�j
#define	MTX_F6411_3		"mtx_f6411_3"		// �r���S�X�t�@�C���R�i�`�r���R�p�j
#define	MTX_F6411_4		"mtx_f6411_4"		// �r���S�X�t�@�C���S�i�`�r���S�p�j

#define	MTX_F6419		"mtx_f6419"			// TN�X�����v�Q�t�@�C���i�T�P�Q * MAX_AS �䕪�j
#define	MTX_F6419_1		"mtx_f6419_1"		// TN�X�����v�Q�t�@�C���P�i�`�r���P�p�T�P�Q�䕪�j
#define	MTX_F6419_2		"mtx_f6419_2"		// TN�X�����v�Q�t�@�C���Q�i�`�r���Q�p�T�P�Q�䕪�j
#define	MTX_F6419_3		"mtx_f6419_3"		// TN�X�����v�Q�t�@�C���R�i�`�r���R�p�T�P�Q�䕪�j
#define	MTX_F6419_4		"mtx_f6419_4"		// TN�X�����v�Q�t�@�C���S�i�`�r���S�p�T�P�Q�䕪�j

#define	MTX_COMNTBL		"mtx_comntbl"		// �r�����L�e�[�u��
#define	MTX_SYUFILE		"mtx_syufile"		// �r���W�v�f�[�^
#define MTX_KOKODAI		"mtx_kokodai"		// �r���X��f�[�^
#define	MTX_IDXSIMA		"mtx_idxsima"		// �r�����h�c�w�e�[�u��
#define	MTX_IDXKISYU	"mtx_idxkisyu"		// �r���@��h�c�w�e�[�u��
#define	MTX_IDXKINKO	"mtx_idxkinko"		// �r�����ɂh�c�w�e�[�u��
#define	MTX_TEIKI		"mtx_teiki"			// �r������ʐM�f�[�^�i�e�e�O�P�`���f�[�^�j
//	2002.05.20 �ǉ� By Fujiki(�Q�X��)-->
#define MTX_ZEN_TENPO	"mtx_Zen_Tenpo"		// �`�E�a�X�ܑS�X�W�v�f�[�^	�i�Q�X�܁j
#define MTX_ZEN_FLOOR	"mtx_Zen_Floor"		// �t���A�S�X�W�v�f�[�^		�i�Q�X�܁j
//	-->2002.05.20 �ǉ� By Fujiki(�Q�X��)

#define MTX_KIJUNKI_ALL		"mtx_Kijunki_all"		// �V��@ �W�v�t�@�C��		2005.07.11	nao

#define	MTX_F1000		"mtx_f1000"			// �r���V�X�e���ݒ�
#define	MTX_F1001		"mtx_f1001"			// �r����ڑ��ݒ�
#define	MTX_F1002		"mtx_f1002"			// �r���S�X�ڑ��ݒ�
#define	MTX_F1003		"mtx_f1003"			// �r�����[�����v�ݒ�
#define	MTX_F1004		"mtx_f1004"			// �r���^�C�v�ݒ�
//	2002.05.20 �ǉ� By Fujiki(�Q�X��)-->
#define	MTX_F1006		"mtx_f1006"			// �r���X�ܐڑ��ݒ�
//	-->2002.05.20 �ǉ� By Fujiki(�Q�X��)
#define	MTX_F1007		"mtx_f1007"			// �r����ʃ|�P�b�g�ݒ� 2002.06.14 �ǉ� kaoru�i�Q�X�܁j
#define	MTX_F1008		"mtx_f1008"			// �r���X�ܥ��ʖ��̐ݒ� ��2002/11/25 (�X�ܐ��Ή�)�ǉ� Fukuyama
#define	MTX_F1010		"mtx_f1010"			// �r����ڑ��ݒ�2
#define	MTX_F1011		"mtx_f1011"			// �r�����������o�͐ݒ�
#define	MTX_F1012		"mtx_f1012"			// ���ʏƍ��@�h�c�ݒ�
#define	MTX_F1050		"mtx_f1050"			// �r������萔�ݒ�
#define	MTX_F1051		"mtx_f1051"			// �r���@�햼�ݒ�
#define	MTX_F1052		"mtx_f1052"			// �r���^�C�v�ݒ�
#define	MTX_F1053		"mtx_f1053"			// �r���s���X�����v�ݒ�
#define	MTX_F1056		"mtx_f1056"			// �r�����ʏ���l�ݒ� // <-- 2010/09/10 add �V����Ǘ�Step1 kanda
#define	MTX_F1100		"mtx_f1100"			// �r���Ïؔԍ��ݒ�
#define	MTX_F1101		"mtx_f1101"			// �r���J�X�����ݒ�
#define	MTX_F1102		"mtx_f1102"			// �r�����ɓ_���ݒ�
#define	MTX_F1103		"mtx_f1103"			// �r���S�X��~�ݒ�
#define	MTX_F1104		"mtx_f1104"			// �r���@���~�ݒ�
#define	MTX_F1105		"mtx_f1105"			// �r���S�X�^�p�ݒ�
#define	MTX_F1106		"mtx_f1106"			// �r���@��^�p�ݒ�
#define	MTX_F1107		"mtx_f1107"			// �r��D2�ݒ�
#define	MTX_F1108		"mtx_f1108"			// �r���X�ܕʕϑ��c�Ɛݒ� ��2002/10/29 (�X�ܕʕϑ��c�ƑΉ�)�ǉ� Fukuyama
#define	MTX_F1109		"mtx_f1109"			// �r�����ܐU���ݒ� �@�@�@��2005/07/19 (�V��@�Ή�)�ǉ� hashimoto
#define	MTX_F1110		"mtx_f1110"			// �r��D2�g���ݒ�		�@��2006/02/06 (DU4�g�����͑Ή�)�ǉ� hashimoto
#define	MTX_F1111		"mtx_f1111"			// DSP�ݒ�						��2006/06/28 (DSP-100�ڑ��Ή�)�ǉ� hashimoto
#define	MTX_F1112		"mtx_f1112"			// DSP�T���h���ɉ�����[�g�ݒ�	��2006/06/28 (DSP-100�ڑ��Ή�)�ǉ� hashimoto
#define	MTX_F1113		"mtx_f1113"			// �r���e��v���@�ݒ�
#define	MTX_F1114		"mtx_f1114"			// �r��D2�ݒ�(���)
#define	MTX_F1115		"mtx_f1115"			// �r��D2�g���ݒ�(���)
#define	MTX_F1118		"mtx_f1118"			// �r���Z�[�t���[�^�[�ُ팟�m�ݒ�
#define	MTX_F1150		"mtx_f1150"			// �r���s�m�ݒ�
#define	MTX_F1151		"mtx_f1151"			// �r��TN�ݒ�Q
#define	MTX_F1152		"mtx_f1152"			// ���v���C�萔�����ʐݒ� 2002.06.14 �ǉ� kaoru�i�Q�X�܁j
#define	MTX_F1153		"mtx_f1153"			// �s�m���ԕϓ����v���C�萔���ݒ� 2002.06.14 �ǉ� kaoru�i�Q�X�܁j
#define	MTX_F1200		"mtx_f1200"			// �r���|�C���g�S�X�ݒ�
#define	MTX_F1201		"mtx_f1201"			// �r���|�C���g�@��ʐݒ�
//	2002.06.29 �ǉ� By Fujiki-->
#define	MTX_F1202		"mtx_f1202"			// �ϓ��ғ��|�C���g�ݒ� 2002.06.14 �ǉ� kaoru�i�Q�X�܁j
//	-->2002.06.29 �ǉ� By Fujiki
#define	MTX_F1203		"mtx_f1203"			// �ϓ��ғ��|�C���g�ݒ� 2002.06.14 �ǉ� kaoru�i�Q�X�܁j
//	2002.06.29 �ǉ� By Fujiki-->
#define	MTX_F1204		"mtx_f1204"			// �ϓ��ғ��|�C���g�ݒ� 2002.06.14 �ǉ� kaoru�i�Q�X�܁j
//	-->2002.06.29 �ǉ� By Fujiki
#define	MTX_F1401		"mtx_f1401"			// �r���䃉���v�S�X����ݒ�			2005/3/8 (�䃉���v�Ή� T2X002355�Ή�)�ǉ� Fukuyama
#define	MTX_F1402		"mtx_f1402"			// �r���䃉���v�@��ʓ���ݒ�		2005/3/8 (�䃉���v�Ή� T2X002355�Ή�)�ǉ� Fukuyama
#define	MTX_F1403		"mtx_f1403"			// �r���䃉���v�����R���h�c�ݒ�iT2X002355�j2005/03/08 tomida
#define	MTX_F1404		"mtx_f1404"			// �r���䃉���v�����v�p�^�[���ݒ�	2005/3/8 (�䃉���v�Ή� T2X002355�Ή�)�ǉ� Fukuyama
#define	MTX_F1405		"mtx_f1405"			// �r���䃉���v�����v�ݒ�			2005/3/8 (�䃉���v�Ή� T2X002355�Ή�)�ǉ� Fukuyama
#define MTX_F1406		"mtx_f1406"			// 20050629 add tomida �䃉���v�����J���iT2X002500�j
#define MTX_F1407		"mtx_f1407"			// 20060106 add hashimoto �f�V�X�e���Ή��iT2X002759�j
#define MTX_F1408		"mtx_f1408"			// 20061208 add hashimoto 80�^�䃉���v�Ή��iT2X003029�j

#define MTX_F1500		"mtx_f1500"			// 20060512 add hashimoto ���Ǘ��V�X�e���J���iT2X002883�j
#define MTX_F1501		"mtx_f1501"			// 20060512 add hashimoto ���Ǘ��V�X�e���J���iT2X002883�j
#define	MTX_F4116		"mtx_f4116"			// DSP�f�[�^�t�@�C��		��2006/06/28 (DSP-100�ڑ��Ή�)�ǉ� hashimoto
#define MTX_F4848		"mtx_f4848"			// 20060512 add hashimoto ���Ǘ��V�X�e���J���iT2X002883�j

#define MTX_F9000		"mtx_f9000"			// POS���v�f�[�^�t�@�C���i�`�X�p�j
#define MTX_F9001		"mtx_f9001"			// POS���v�f�[�^�t�@�C���i�a�X�p�j
#define MTX_F9010		"mtx_f9010"			// TP���v�f�[�^�t�@�C���i�`�X�p�j
#define MTX_F9011		"mtx_f9011"			// TP���v�f�[�^�t�@�C���i�a�X�p�j

/*�����g���[���`��MMP�p�~�@2003/12/16�@yasui
//--------------------------------------------
// �j�o�b�ɕK�v�ȃt�@�C�����i���g���[���`���j
// �����͂v�������삪�g�p����킯�ł͂��肹��
// -------------------------------------------
#define	MMP_MF1000		"mf1000.bin"		// �V�X�e���ݒ�
#define	MMP_MF1001		"mf1001.bin"		// ��ڑ��ݒ�
#define	MMP_MF1002		"mf1002.bin"		// �S�X�ڑ��ݒ�
#define	MMP_MF1003		"mf1003.bin"		// ���[�����v�ݒ�
#define	MMP_MF1004		"mf1004.bin"		// �^�C�v�ݒ�
#define	MMP_MF1005		"mf1005.bin"		// ������ڑ��ݒ�
#define	MMP_MF1006		"mf1006.bin"		// �X�ܐڑ��ݒ�
#define	MMP_MF1007		"mf1007.bin"		// ��ʃ|�P�b�g�ݒ�
#define	MMP_MF1008		"mf1008.bin"		// �X�ܥ��ʖ��̐ݒ�	��2002/11/25 (�X�ܐ��Ή�)�ǉ� Fukuyama
#define	MMP_MF1050		"mf1050.bin"		// ����萔�ݒ�
#define	MMP_MF1051		"mf1051.bin"		// �@�햼�ݒ�
#define	MMP_MF1052		"mf1052.bin"		// �^�C�v�ݒ�
#define	MMP_MF1053		"mf1053.bin"		// �s���X�����v�ݒ�
#define	MMP_MF1100		"mf1100.bin"		// �Ïؔԍ��ݒ�
#define	MMP_MF1101		"mf1101.bin"		// �J�X�����ݒ�
#define	MMP_MF1102		"mf1102.bin"		// ���ɓ_���ݒ�
#define	MMP_MF1103		"mf1103.bin"		// �S�X��~�ݒ�
#define	MMP_MF1104		"mf1104.bin"		// �@���~�ݒ�
#define	MMP_MF1105		"mf1105.bin"		// �S�X�^�p�ݒ�
#define	MMP_MF1106		"mf1106.bin"		// �@��^�p�ݒ�			�� 2002/09/18 �ǉ� Fukuyama
#define	MMP_MF1107		"mf1107.bin"		// D2�ݒ�
#define	MMP_MF1108		"mf1108.bin"		// �X�ܕʕϑ��c�Ɛݒ�   �� 2002/10/29 (�X�ܕʕϑ��c�ƑΉ�)�ǉ� Fukuyama
#define	MMP_MF1150		"mf1150.bin"		// �s�m�ݒ�
#define	MMP_MF1151		"mf1151.bin"		// TN�ݒ�Q
#define	MMP_MF1152		"mf1152.bin"		// ���v���C�萔�����ʐݒ�
#define	MMP_MF1153		"mf1153.bin"		// �s�m���ԕϓ����v���C�萔���ݒ�
#define	MMP_MF1200		"mf1200.bin"		// �|�C���g�S�X�ݒ�
#define	MMP_MF1201		"mf1201.bin"		// �|�C���g�@��ʐݒ�
#define	MMP_MF1202		"mf1202.bin"		// ���ԕϓ����X�|�C���g�ݒ�
#define	MMP_MF1203		"mf1203.bin"		// �ϓ��ғ��|�C���g�ݒ�
#define	MMP_MF1204		"mf1204.bin"		// ���ԕϓ�����|�C���g�ݒ�

#define MMP_MF4104		"mf4104.bin"		// ���W�v
#define MMP_MF4105		"mf4105.bin"		// �@��W�v
#define MMP_MF4106		"mf4106.bin"		// ��ʏW�v


#define MMP_MF4112			"mf4112.bin"	// �莞�ғ�
#define MMP_MF4113			"mf4113.bin"	// �莞��ʉғ�
#define MMP_MF4114			"mf4114.bin"	// �莞�S�X�ғ�
#define MMP_MF4115			"mf4115.bin"	// �莞�@��
#define MMP_MF4005			"mf4005.bin"	// �L�����
#define MMP_MF4006			"mf4006.bin"	// �V�X�e���ُ헚��
#define MMP_MF4007			"mf4007.bin"	// �N���A����
#define MMP_MF4008			"mf4008.bin"	// ���v�ݒ�
#define MMP_MF4009			"mf4009.bin"	// ��d����
#define MMP_MF4010			"mf4010.bin"	// �ݒ藚��
#define MMP_MF4011			"mf4011.bin"	// �ݒ�VER���
#define MMP_MF4022			"mf4022.bin"	// �ُ헚��
#define MMP_MF4023			"mf4023.bin"	// �Ŏ~����
#define MMP_MF4024			"mf4024.bin"	// ��������
//#define MMP_MF6409			"mf6409.bin"	// �s�m�X�����v2002.11.20 koketsu Del �i��Œ�`�j
#define MMP_MF5201			"mf5201.bin"	// ������O
#define MMP_MF6301			"mf6301.bin"	// ������O�Ǘ����
//�����g���[���`��MMP�p�~�@2003/12/12�@yasui*/

// Kpc�o�͗p�t�@�C����
#define KPC_F1051			"f1051_org.bin"	// �@�햼�ݒ�	

#endif
