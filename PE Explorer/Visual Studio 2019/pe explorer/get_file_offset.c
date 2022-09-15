#include "header.h"

// size 인자 만큼 파일 포인터를 이동하여 해당 위치의 offset 값을 반환한다.
int get_file_offset(FILE* fp, int offset)
{
	fseek(fp, offset, SEEK_CUR);
	return ftell(fp);
}