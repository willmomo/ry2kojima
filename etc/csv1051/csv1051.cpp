#include <windows.h>
#include <stdio.h>

typedef struct {
	char name[28];	// �@�햼
	char pas;		// ���
	char type;		// ���
	short date;		// ����
} fno1051;

int main(int argc, char** argv)
{
	if (argc == 1) {
		printf("csv1051 <���g���[���t�H�[�}�b�g��1051>\n");
		return -1;
	}

	FILE* fp = fopen(argv[1], "rb");
	if (fp) {
		int line = 1;
		fno1051 buf;
		while (1) {
			size_t ret = fread(&buf, sizeof(buf), 1, fp);
			if (ret != 1) {
				break;
			}

			short dt = ntohs(buf.date);
			printf("%d,%s,%d,%d,%d/%d/%d\n", line++,
				buf.name,
				buf.pas,
				buf.type,
				((dt & 0xfe00) >> 9) + 1910,
				(dt & 0x01e0) >> 5,
				(dt & 0x001f));
		}

		fclose(fp);
	}

	return 0;
}
