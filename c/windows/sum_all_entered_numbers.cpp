#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


int main(void)
{
	//system("mode con cols=60 lines=25"); //cols = ĭ/�� (����)  lines = ��/�� (����)
    system("title ��� �� �����ִ� PG");
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 3);
    
	long a,input, sum = 0;
	char input_1;
	//system("cls");
	printf("+-----------------------------------+\n");
	printf("-                                   -\n");
	printf("-          EX) 1+2+3 ... +10 = 55   -\n");
	printf("-                                   -\n");
	printf("- 10���� ���ҷ��� 10�� �Է��ϼ���.  -\n");
	printf("+-----------------------------------+\n");
	
	
	printf(" \n\n\n\n�� ��� ���� ������ �ǰ��� : ");
	scanf("%ld", &input); 
	
	for(a=0; a<=input; a++)
		sum += a;
	
	printf("\n\n\n");
	printf("+=====================================================+\n");
	printf("-                                                     -\n");
	printf("-       %ld�� ��� ���� ���� ����: '%ld'�Դϴ�.         -\n",input,sum);
	printf("- (�ݺ��ϽǷ��� 'b'�� �����ϽǷ��� 'q'�� �Է��ϼ���.) -\n");
	printf("+=====================================================+\n");
	
	printf("�ݺ� �Ͻðڽ��ϱ�, ���� �Ͻðڽ��ϱ�: ");
	scanf("%s", &input_1);
	
	if(input_1 == 'b')
		{
		return main();
		}
	if(input_1 == 'q')
		{
			return 0;
		}
		
	//return main();
	
	//for(;;);
}
