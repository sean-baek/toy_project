#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void error_message(char *message);

int main(int argc, char *argv[])
{
	int sock, result, opnd_cnt, i;
	char opmsg[BUF_SIZE];
	struct sockaddr_in serv_adr;
	if(argc != 3)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_message("Couldn't create socket!..");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_message("Couldn't connect..");
	else
		puts("Connected......");

//서버 소켓에 접속을 마침
//서버로 보낼 숫자를 입력

	fputs("Operand count : ", stdout);
	scanf("%d", &opnd_cnt);
	opmsg[0] = (char)opnd_cnt;

//몇 번 입력할건지 서버로 보내는데 opmsg배열에 저장

	for(i = 0; i < opnd_cnt; i++)
	{
		printf("Operand %d : ", i+1);
		scanf("%d", (int*)&opmsg[i*OPSZ+1]); //i는 1, OPSZ는 4 | opmsg배열 5번 째 요소의 주소에 저
	}

	fgetc(stdin);
	fputs("Operator : ", stdout);
	scanf("%c", &opmsg[opnd_cnt * OPSZ+1]);
	write(sock, opmsg, opnd_cnt*OPSZ+2);
	read(sock, &result, RLT_SIZE);

	printf("Operation result : %d \n", result);
	close(sock);
	return 0;
}

void error_message(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

