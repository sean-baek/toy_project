#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock, clnt_sock, f;
	char message[] = "Hi! I'm sean! HAHAHAHA\n\n";
	char message_[] = "Connected to server\n";
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	
	//소켓 생성 | Create socket
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	//파일 소켓 생성 | Create file socket
	f = open("test.txt" , O_CREAT|O_WRONLY|O_TRUNC);
	
	//변수 f의 파일 디스크립터가 -1이면 에러 메세지 출력 | If f's File descriptor is -1, print error message
	if(f == -1)
		error_handling("Can't creat file....");
	
	//파일에 내용 입력 | Write on file
	if(write(f, message, sizeof(message)) == -1)
		error_handling("Can't write to file!");
	
	if(argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	//변수 sock의 파일 디스크립터가 -1이면 에러 메세지 출력 | If sock's File descriptor is -1, print error message
	if(sock == -1)
		error_handling("Not created socket....");
	
	//아이피와 포트번호를 담을 구조체를 초기화 | Initialize the structure to hold the IP and port number
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	//소켓에 아이피와 포트번호 부여 | Give IP and port number to socket
	if(bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
			error_handling("Can't assign Ip and Port number!\n");
	
	//클라이언트의 요청을 받을 수 있도록 대기 상태로 유지 | Wait until client's request comes in
	if(listen(sock, 5) == -1)
		error_handling("Can't listen of the client's request!\n");
	else
		printf("Watting the client....\n");
	
	//클라이언트 수락 | Response client's request
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
	
	//클라이언트를 수락하지 못했을 때 에러 메세지 출력 | printf an error message when the client is not accepted
	if(clnt_sock == -1)
		error_handling("Can't response client's request!");
	
	//클라이언트에게 접속 성공 메세지 보내기 | Send a connection success message to the client
	write(clnt_sock, message_, sizeof(message_));

	//소켓들 닫기 | Close sockets
	close(sock);
	close(clnt_sock);
	close(f);
	
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

