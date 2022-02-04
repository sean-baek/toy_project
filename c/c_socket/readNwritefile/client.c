#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 100

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock, str_len, f;
	char message[BUF_SIZE];
	char buf[BUF_SIZE];
	struct sockaddr_in serv_addr;
	
	if(argc != 3)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}
	
	//socket 생성 | Create socket
	sock = socket(PF_INET, SOCK_STREAM, 0);

	//socket 생성에 대한 에러 메세지 출력 | If can't create a socket, print error message
	if(sock == -1)
		error_handling("Not created socket....");

	//주소 정보를 담을 구조체 초기화 | Initialize the structure of the address information
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
	
	//연결 시도 | Connection attempt
	if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("couldn't send request!");

	//서버로부터 접속 성공 메세지 받기 | Receive a connection success message from the server
	str_len = read(sock, message, sizeof(message)-1);
	if(str_len == -1)
		error_handling("Can't read server's message");
	
	//서버로부터 받은 접속 성공 메세지 출력 | Print connection success message from server
	printf("\nMessage from server : %s \n", message);


	//test.txt파일을 읽기 모드로 열기 | Open the test.txt in Read mode
    	f = open("test.txt", O_RDONLY);
	
	//파일 열기 오류 시 에러 메세지 출력 | 	If can't open the file, print error message
    	if(f == -1)
        	error_handling("Couldn't created file");

	//파일의 내용 읽기 | Read the file's data
	if(read(f, buf, sizeof(buf)) == -1)
        	error_handling("Can't read from the server's file");

	printf("File data : %s", buf);
	
	//소켓 닫기 | Close sockets
	close(sock);
	close(f);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

