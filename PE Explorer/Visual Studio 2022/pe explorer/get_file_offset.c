#include "header.h"

// size ���� ��ŭ ���� �����͸� �̵��Ͽ� �ش� ��ġ�� offset ���� ��ȯ�Ѵ�.
int get_file_offset(FILE* fp, int offset)
{
	fseek(fp, offset, SEEK_CUR);
	return ftell(fp);
}