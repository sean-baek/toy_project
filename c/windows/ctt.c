#include <stdio.h>
#include <string.h>
#include <windows.h>
void print_list(void);

int main(void)
{
	int ret;
	char input;
	char help[5] = "help";
	
	puts("!~Use the help command~!");
	printf("Command > ");
	scanf("%s", &input);
	ret = strcmp(help,input);
	if(ret == 0)
	{
		print_list();
	}
}

void print_list(void)
{
	puts("[*] a or A : amazon.com");
	puts("[*] b or B : boannews.com");
	puts("[*] c or C : cgv.co.kr");
	puts("[*] d or D : daum.net");
	puts("[*] f or F : Facebook.com");
	puts("[*] g or G : google.com");
	puts("[*] i or I : instagram.com");
	puts("[*] n or N : naver.com");
	puts("[*] o or O : op.gg");
	puts("[*] q or Q : q-net.or.kr");
	puts("[*] t or T : twitter.com");
	puts("[*] y or Y : youtube.com");	
}
