#include "header.h"


int get_file_content(FILE* fp, u_char** buf)
{

	int size = 0;

	// 파일 사이즈 구하기
	// 파일 사이즈 구하기 실패 했을 때
	if ((size = get_file_size(fp)) == -1)
	{
		printf("파일 사이즈를 구하는 데 실패했습니다.\n");
		return -1;
	}
	// 파일 사이즈 구하기 성공 했을 때
	else
	{
		// 파일 사이즈만큼 메모리 동적 할당
		if ((*buf = (u_char*)malloc(size + 1)) == NULL)
		{
			printf("malloc() error\n");
			return -1;
		}

		// 동적 할당한 heap 메모리 공간을 0으로 초기화
		if (memset(*buf, 0x00, size) == NULL)
		{
			printf("memset() error\n");
			return -1;
		}

		// 동적 할당한 heap 메모리 공간이 NULL로 초기화 되지 않았을 때
		if (*buf == NULL)
		{
			printf("동적 메모리를 할당할 수 없습니다.\n");
			return -1;
		}

		// 이미 어떠한 작업을 거친 파일 포인터를 get_file_content에 넘겨도 동작하도록 하기 위함
		// get_file_size 함수에서 동일한 작업을 마지막에 해주므로 필요 없을 듯하다.
		// rewind(fp);

		// 동적 할당한 메모리에 file 내용 전체 읽기
		if (fread(*buf, 1, size, fp) == 0)
		{
			printf("fread() error\n");
			return -1;
		}

		// 파일 사이즈를 구해왔고, 내용을 읽었다면 다시 파일 포인터를 처음으로 위치
		rewind(fp);

		return size;
	}
}