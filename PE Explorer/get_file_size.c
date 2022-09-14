#include "header.h"

int get_file_size(FILE* fp)
{
	int size = 0;

	// ���� �����͸� ������ ������ ������.
	// ���� �����͸� ������ ������ ���� �� ���� ��
	if (fseek(fp, 0, SEEK_END) != 0)
	{
		fprintf(stderr, "������ ������ �̵��� �� �����ϴ�. :%s\n", strerror(errno));
		return -1;
	}

	// ���� �����͸� �̵� �� ���� ���� �������� ��ġ�� ������ �� ���� ��
	if ((size = ftell(fp)) == -1)
	{
		fprintf(stderr, "���Ͽ��� ���� ��ġ�� �� �� �����ϴ�. :%s\n", strerror(errno));
		return -1;
	}

	// ���� �����͸� ó�� �κ����� ��ġ��Ų��.
	rewind(fp);

	/*
	if (fseek(fp, 0, SEEK_SET) != 0)
	{
		fprintf(stderr, "������ ó������ �̵��� �� �����ϴ�. :%s\n", strerror(errno));
		return -1;
	}*/

	return size;
}