/*

#include <stdio.h>
#include <unistd.h>

int main()
{
	int passworld;
	
	printf("welcome to ROOT :/n");
	sleep(3);
	
	printf("Loading.... Please wait? /n");
	sleep(4);
	
	printf("Enter you're passworld to connect.../n");
	scanf("%d",&passworld)
	
	if(Passworld == 21720)
	
	else(!=2170)
	printf("Wrong Passworld.... type again");
	
	printf("******");
	printf("*    *");
	printf("*****");
	printf("*  *");
	printf("*   *");
	
	return remain();
	
}
*/

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <process.h>

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X =x;
    coord.Y =y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
    int r, i;
    gotoxy(36, 14);
    printf("Loading...");
    gotoxy(30, 15);
    
    for(r=1; r<=20; r++)
    {
        for(i=0; i<=100000000; i++);
            printf("%c", 26); 
    }
    getch();
    
}

/*
1 =  
2 = 
3 =  
4 = 
5 = 
6 = 
7 = �� �� �� �Ҹ�  
8 = " _ " <- �� ����� ���� �β��� ���·� �����ʿ��� �������� �����̸鼭 ������  
9 = 8���� ����� ������ ���ʿ��� ���������� �̵��ϰ� �����̴� ������ �Ÿ��� �ִ� 
10 = 8���� ����� ������ �Ʒ��� �̵��Ѵ�
11 = 
12 = 
13 = 8���� ����� �������� �ʰ� ������ ���� 
14 = 
15 = 
16 = 
17 = 
18 = 
19 = 
20 = 
21 = 
22 = 
23 = 
24 = 
25 = 
26 = 
27 = 
28 = 
29 = 
30 = 

 */
