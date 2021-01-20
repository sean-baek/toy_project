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
7 = ¶ì ¶ì ¶ì ¼Ò¸®  
8 = " _ " <- ÀÌ ¸ð¾çÀÌ Á»´õ µÎ²¨¿î »óÅÂ·Î ¿À¸¥ÂÊ¿¡¼­ ¿ÞÂÊÀ¸·Î ±ôºýÀÌ¸é¼­ ¿òÁ÷ÀÓ  
9 = 8¹ø°ú ¸ð¾çÀº °°Áö¸¸ ¿ÞÂÊ¿¡¼­ ¿À¸¥ÂÊÀ¸·Î ÀÌµ¿ÇÏ°í ±ôºýÀÌ´Â °£°ÝÀÇ °Å¸®°¡ ¸Ö´Ù 
10 = 8¹øÀÇ ¸ð¾çÀÌ À§¿¡¼­ ¾Æ·¡·Î ÀÌµ¿ÇÑ´Ù
11 = 
12 = 
13 = 8¹øÀÇ ¸ð¾çÀÌ ¿òÁ÷ÀÌÁö ¾Ê°í °¡¸¸È÷ ÀÖÀ½ 
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
