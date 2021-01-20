#include <stdio.h>

int main(void)
{
	struct stt
	{
		short b;
		int x;
		short s;
		long z;
		char c[5];
		long long a;
		char q;
	}stt;

	stt.b = 32767;
	stt.x = 4294967295;
	stt.s = 32500;
	stt.z = 4294967200;
	stt.c[0] = 'a';
	stt.c[1] = 'b';
	stt.c[2] = 'c';
	stt.c[3] = 'd';
	stt.c[4] = 'e';
	stt.a = 888888888888888888;
	stt.q = 'a';
	return 0;
}
