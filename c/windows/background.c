#include <stdio.h>
#include <windows.h>

void bg();

int main(void)
{
	bg();
	
	return 0;
}

void bg()
{
	while(1)
	{
		SetConsoleTitle("sean");
		system("mode con cols=50 lines=20");
		system("color 0f");
		Sleep(50);
		system("color 1f");
		Sleep(50);
		system("color 2f");
		Sleep(50);
		system("color 3f");
		Sleep(50);
		system("color 4f");
		Sleep(50);
		system("color 5f");
		Sleep(50);
		system("color 6f");
		Sleep(50);
		system("color 7f");
		Sleep(50);
		system("color 8f");	
		Sleep(50);
		system("color 9f");
		Sleep(50);
		system("color af");
		Sleep(50);
		system("color bf");
		Sleep(50);
		system("color cf");
		Sleep(50);
		system("color df");
		Sleep(50);
		system("color ef");
		Sleep(50);
		system("color ff");
	}
}
