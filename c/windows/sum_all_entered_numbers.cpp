#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


int main(void)
{
	//system("mode con cols=60 lines=25"); //cols = 칸/행 (가로)  lines = 줄/열 (세로)
    system("title 모든 수 합해주는 PG");
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 3);
    
	long a,input, sum = 0;
	char input_1;
	//system("cls");
	printf("+-----------------------------------+\n");
	printf("-                                   -\n");
	printf("-          EX) 1+2+3 ... +10 = 55   -\n");
	printf("-                                   -\n");
	printf("- 10개를 더할려면 10을 입력하세요.  -\n");
	printf("+-----------------------------------+\n");
	
	
	printf(" \n\n\n\n총 몇개의 수를 더해줄 건가요 : ");
	scanf("%ld", &input); 
	
	for(a=0; a<=input; a++)
		sum += a;
	
	printf("\n\n\n");
	printf("+=====================================================+\n");
	printf("-                                                     -\n");
	printf("-       %ld의 모든 수를 더한 값은: '%ld'입니다.         -\n",input,sum);
	printf("- (반복하실려면 'b'를 종료하실려면 'q'를 입력하세요.) -\n");
	printf("+=====================================================+\n");
	
	printf("반복 하시겠습니까, 종료 하시겠습니까: ");
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
