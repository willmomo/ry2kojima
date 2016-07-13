#include <io.h>
#include <stdlib.h>
#include <stdio.h>

void main(void)
{
	int old;
	FILE* new;

	old = _dup(1);

	if (old == -1)
	{
		perror("_dup(1) エラー");
		exit(1);
	}

	write(old, "この行は最初に stdout に出力されます。\r\n", 40);
	if ((new = fopen("data", "w")) == NULL)
	{
		puts("ファイル 'data' を開けません。\n");
		exit(1);
	}

	if (-1 == _dup2(_fileno(new), 1))
	{
		perror("_dup2 関数では stdout できません");
		exit(1);
	}

	puts("この行はファイル 'data' に出力されます。\r\n");

	fflush(stdout);
	fclose(new);

	_dup2(old, 1);
	puts("この行は stdout に出力されます。\r");
	puts("ファイル 'data' の内容は：");

	fflush(stdout);
	system("type data");
}
