#include "header.h"

int get_file_size(FILE* fp)
{
	int size = 0;

	// 파일 포인터를 파일의 끝으로 보낸다.
	// 파일 포인터를 파일의 끝으로 보낼 수 없을 때
	if (fseek(fp, 0, SEEK_END) != 0)
	{
		fprintf(stderr, "파일의 끝으로 이동할 수 없습니다. :%s\n", strerror(errno));
		return -1;
	}

	// 파일 포인터를 이동 후 현재 파일 포인터의 위치를 가져올 수 없을 때
	if ((size = ftell(fp)) == -1)
	{
		fprintf(stderr, "파일에서 현재 위치를 알 수 없습니다. :%s\n", strerror(errno));
		return -1;
	}

	// 파일 포인터를 처음 부분으로 위치시킨다.
	rewind(fp);

	/*
	if (fseek(fp, 0, SEEK_SET) != 0)
	{
		fprintf(stderr, "파일의 처음으로 이동할 수 없습니다. :%s\n", strerror(errno));
		return -1;
	}*/

	return size;
}