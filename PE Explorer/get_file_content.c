#include "header.h"


int get_file_content(FILE* fp, u_char** buf)
{

	int size = 0;

	// ���� ������ ���ϱ�
	// ���� ������ ���ϱ� ���� ���� ��
	if ((size = get_file_size(fp)) == -1)
	{
		printf("���� ����� ���ϴ� �� �����߽��ϴ�.\n");
		return -1;
	}
	// ���� ������ ���ϱ� ���� ���� ��
	else
	{
		// ���� �����ŭ �޸� ���� �Ҵ�
		if ((*buf = (u_char*)malloc(size + 1)) == NULL)
		{
			printf("malloc() error\n");
			return -1;
		}

		// ���� �Ҵ��� heap �޸� ������ 0���� �ʱ�ȭ
		if (memset(*buf, 0x00, size) == NULL)
		{
			printf("memset() error\n");
			return -1;
		}

		// ���� �Ҵ��� heap �޸� ������ NULL�� �ʱ�ȭ ���� �ʾ��� ��
		if (*buf == NULL)
		{
			printf("���� �޸𸮸� �Ҵ��� �� �����ϴ�.\n");
			return -1;
		}

		// �̹� ��� �۾��� ��ģ ���� �����͸� get_file_content�� �Ѱܵ� �����ϵ��� �ϱ� ����
		// get_file_size �Լ����� ������ �۾��� �������� ���ֹǷ� �ʿ� ���� ���ϴ�.
		// rewind(fp);

		// ���� �Ҵ��� �޸𸮿� file ���� ��ü �б�
		if (fread(*buf, 1, size, fp) == 0)
		{
			printf("fread() error\n");
			return -1;
		}

		// ���� ����� ���ؿ԰�, ������ �о��ٸ� �ٽ� ���� �����͸� ó������ ��ġ
		rewind(fp);

		return size;
	}
}