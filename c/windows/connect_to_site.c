#include <stdio.h>
#include <conio.h>
#include <windows.h>

void textcolor(int color_number)
{
 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color_number);
}

int main(void)
{
	system("mode con cols=80 lines=30");
	char input;
	
	textcolor(11);
	printf("\t\t\tsean\n\n");
	
	textcolor(7);
	printf("1. Google: g, 1 \t8. Facebook: f, 8\n2. Naver: n, 2 \t\t9. Twitter: t, 9 \n3. Yahoo: y, 3 \t\t10. instagram: i, A \n4. Yahoo Japen: j, 4 \t11. Wikipedia: w, B \n5. Bing: b, 5 \t\t12. Youtube: y, C\n6. Baidu: u, 6 \n7. Daum: d, 7");
	textcolor(10);
	printf("\nIf you exit this program press '0', or want return press Enter Key"); 
	textcolor(7);
	printf("\n\nWhich site would you like to connect to : ");
	scanf("%s",&input);
	
	if(input != 0)
	{
		switch(input)
		{
			case 'g' : system("start www.google.com"); break;
			case 'n' : system("start www.naver.com"); break;
			case 'Y' : system("start www.yahoo.com"); break;
			case 'j' : system("start www.yahoo.co.jp/"); break;
			case 'b' : system("start www.bing.com"); break;
			case 'u' : system("start www.baidu.com"); break;
			case 'd' : system("start www.daum.net"); break;
			case 'f' : system("start www.facebook.com"); break;
			case 't' : system("start www.twitter.com"); break;
			case 'i' : system("start www.instagram.com"); break;
			case 'w' : system("start www.wikipedia.com"); break;
		
			case '1' : system("start www.google.com"); break;
			case '2' : system("start www.naver.com"); break;
			case '3' : system("start www.yahoo.com"); break;
			case '4' : system("start www.yahoo.co.jp/"); break;
			case '5' : system("start www.bing.com"); break;
			case '6' : system("start www.baidu.com"); break;
			case '7' : system("start www.daum.net"); break;
			case '8' : system("start www.facebook.com"); break;
			case '9' : system("start www.twitter.com"); break;
			case 'A' : system("start www.instagram.com"); break;
			case 'B' : system("start www.wikipedia.com"); break;
			case 'y' : system("start www.youtube.com"); break;
			case 'C' : system("start www.youtube.com"); break;
		
		}
		printf("connecting....");
	}
}
