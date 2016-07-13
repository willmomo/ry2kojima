/* ���C�����[�v */
int main_loop();

/* CSV�Ǎ��݃f�[�^�Z�b�g�֐� */
void csv_data_set(csv_read *cpd, char *cbuf, int cnt);

/* �d����M�֐� */
int r_data(char *szBuf, size_t bufSize );

/* �d�����M�֐� */
int s_data(char *data);

/* �d����͊֐� */
cr_recv rdata_anal(char *data);

/* CRC�Z�o�֐� */
void crc_cal(char *data, int n, char *crc_a);
void crc_cal_cmp(char *data, int n, char *crc_a);

/* CRC�`�F�b�N�֐� */
int crc_comp(char *data, int n);

/* �d��Ver�`�F�b�N�֐� */
int ver_check(char *data, char *ver);

/* ���[�J�[�R�[�h�`�F�b�N�֐� */
int mcd_check(char *data, char *mcd);

/* �}�g���N�X���� */
void cr_matrix(cr_recv pd, csv_read cpd);

/* �A�C�h���C�x���g�������� */
void none_idle(cr_recv pd, csv_read cpd);		/* �ʏ��� */
void cardin_idle(cr_recv pd, csv_read cpd);		/* �J�[�hIN�� */
void harai_idle(cr_recv pd, csv_read cpd);		/* ���o�� */
void error_idle(cr_recv pd, csv_read cpd);		/* �G���[�� */

/* �m��M���������� */
void none_n(cr_recv pd, csv_read cpd);			/* �ʏ��� */
void cardin_n(cr_recv pd, csv_read cpd);		/* �J�[�h�h�m�� */
void harai_n(cr_recv pd, csv_read cpd);			/* ���o�� */
void error_n(cr_recv pd, csv_read cpd);			/* �G���[�� */

/* �h��M���������� */
void none_i(cr_recv pd, csv_read cpd);			/* �ʏ��� */
void cardin_i(cr_recv pd, csv_read cpd);		/* �J�[�h�h�m�� */
void harai_i(cr_recv pd, csv_read cpd);			/* ���o�� */
void error_i(cr_recv pd, csv_read cpd);			/* �G���[�� */

/* �n��M���������� */
void none_o(cr_recv pd, csv_read cpd);			/* �ʏ��� */
void cardin_o(cr_recv pd, csv_read cpd);		/* �J�[�h�h�m�� */
void harai_o(cr_recv pd, csv_read cpd);			/* ���o�� */
void error_o(cr_recv pd, csv_read cpd);			/* �G���[�� */

/* �q��M���������� */
void none_r(cr_recv pd, csv_read cpd);			/* �ʏ��� */
void cardin_r(cr_recv pd, csv_read cpd);		/* �J�[�h�h�m�� */
void harai_r(cr_recv pd, csv_read cpd);			/* ���o�� */
void error_r(cr_recv pd, csv_read cpd);			/* �G���[�� */

/* ���M�f�[�^�Z�b�g�֐� */
void ndata_set(cr_recv pd, csv_read cpd, cr2tn_n *nd);	/* �m�d���p */
void idata_set(cr_recv pd, csv_read cpd, cr2tn_i *nd);	/* �h�d���p */
void odata_set(cr_recv pd, csv_read cpd, cr2tn_o *nd);	/* �n�d���p */
void rdata_set(cr_recv pd, csv_read cpd, cr2tn_r *nd);	/* �q�d���p */

/* �G���[�����֐� */
void err_hapen();

/* �R�}���h�g������֐� */
char cmd_ex_rand( void );

/* �G���[�f�[�^�Z�b�g�֐� */
void edata_set(csv_read *cpd, char *data);

/* �G���[�f�[�^�Z�b�g�֐� */
void e_ndata_set(csv_read *cpd, char *data);
void e_idata_set(csv_read *cpd, char *data);
void e_odata_set(csv_read *cpd, char *data);
void e_rdata_set(csv_read *cpd, char *data);

/* �������w�肵�����ԕ�������~����֐� */
void sleep( clock_t wait );

/* CRC�̒l��ύX����֐� */
void crc_change_func(char *data);

/* �d����1char��������֐� */
void char_len_chng(char *data);

int retry_err_func(char *data);
