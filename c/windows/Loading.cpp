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
7 = 띠 띠 띠 소리  
8 = " _ " <- 이 모양이 좀더 두꺼운 상태로 오른쪽에서 왼쪽으로 깜빡이면서 움직임  
9 = 8번과 모양은 같지만 왼쪽에서 오른쪽으로 이동하고 깜빡이는 간격의 거리가 멀다 
10 = 8번의 모양이 위에서 아래로 이동한다
11 = 
12 = 
13 = 8번의 모양이 움직이지 않고 가만히 있음 
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
