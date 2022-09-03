#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define OPSZ 4
#define BUF_SIZE 1024

void error_message(char *message);
int calculate(int num, int opnds[], char oprator);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock, i, result, opnd_cnt, recv_cnt, recv_len;
	char opinfo[BUF_SIZE];
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_adr_size;

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_message("Can't Create server socket...");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_message("Can't bind address...");

	if(listen(serv_sock, 5) == -1)
		error_message("Can't response client's request...");

	clnt_adr_size = sizeof(clnt_addr);

// 소켓과의 통신 성공
// 본격적인 계산 시작

	for(i = 0; i < 5; i++)
	{
		opnd_cnt = 0;

		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_adr_size);
		if(clnt_sock == -1)
			error_message("Connection failed");
		else
			printf("Connection successful");
	//클라이언트로부터 정ㄷ보를 받아옴

		read(clnt_sock, &opnd_cnt, 1);

		recv_len = 0;
		while((opnd_cnt * OPSZ+1) > recv_len) 
    /*opnt_cnt는 클라이언트로부터 정보를 받아오고, OPSZ+1은 4+1 이므로 n*5가 0보다 크다 라는 조건이고,
      클라이언트가 몇 번 반복할 것인지 정보를 안 주면 while문은 실행되지 않는다.*/
		{
			recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE-1); //
			recv_len += recv_cnt; //recv_len = recv_len + recv_cnt
		}

		result = calculate(opnd_cnt, (int *)opinfo, opinfo[recv_len-1]);
		write(clnt_sock, (char*)&result, sizeof(result));
		close(clnt_sock);
	}

	close(serv_sock);
	return 0;
}

int calculate(int num, int opnds[], char op)
{
	int result = opnds[0], i;
	switch(op)
	{
		case '+':
			for(i = 1; i < num; i++) result += opnds[i];
			break;

		case '-':
			for(i = 1; i < num; i++) result -= opnds[i];
			break;

		case '*':
			for(i = 1; i < num; i++) result *= opnds[i];
			break;
	}
	return result;

}

void error_message(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
